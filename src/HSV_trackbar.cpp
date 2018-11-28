/*---- Function definitions for implementing the HSV threashold trackbar ----*/


#include "HSV_trackbar.hpp"

/* Initialize trackbar parameter values */
const int max_value_H = 360/2;
const int max_value = 255;
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;

/* Initialize seed */
int seed_x = 0;
int seed_y = 0;

/* Define I/O frames & display window */
Mat hsv_thresh_input_frame;
Mat hsv_thresh_output_frame;
char hsv_display_window[32] = "HSV thresh";


/*---- Slidbar callback functions ----*/
void on_low_H_thresh_trackbar(int low_H, void * = NULL)
{
    low_H = min(high_H-1, low_H);
    setTrackbarPos("Low H", hsv_display_window, low_H);
}
void on_high_H_thresh_trackbar(int high_H, void * = NULL)
{
    high_H = max(high_H, low_H+1);
    setTrackbarPos("High H", hsv_display_window, high_H);
}
void on_low_S_thresh_trackbar(int low_S, void * = NULL)
{
    low_S = min(high_S-1, low_S);
    setTrackbarPos("Low S", hsv_display_window, low_S);
}
void on_high_S_thresh_trackbar(int high_S, void * = NULL)
{
    high_S = max(high_S, low_S+1);
    setTrackbarPos("High S", hsv_display_window, high_S);
}
void on_low_V_thresh_trackbar(int low_V, void * = NULL)
{
    low_V = min(high_V-1, low_V);
    setTrackbarPos("Low V", hsv_display_window, low_V);
}
void on_high_V_thresh_trackbar(int high_V, void * = NULL)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("High V", hsv_display_window, high_V);
}


/*---- initialize HSV thresholding trackbars, and set the mouse-click callback for sniffing pixel info from the camera-frame ----*/
void init_HSV_trackbars()
{
    // Trackbars to set thresholds for HSV values
    createTrackbar("Low H", hsv_display_window, &low_H, max_value_H, on_low_H_thresh_trackbar);
    createTrackbar("High H", hsv_display_window, &high_H, max_value_H, on_high_H_thresh_trackbar);
    createTrackbar("Low S", hsv_display_window, &low_S, max_value, on_low_S_thresh_trackbar);
    createTrackbar("High S", hsv_display_window, &high_S, max_value, on_high_S_thresh_trackbar);
    createTrackbar("Low V", hsv_display_window, &low_V, max_value, on_low_V_thresh_trackbar);
    createTrackbar("High V", hsv_display_window, &high_V, max_value, on_high_V_thresh_trackbar);

    setMouseCallback(pre_process_window, mouseEvent, &hsv_thresh_input_frame); 
}
/*---- Applies HSV thresholding to the input frame, using slider parameters ----*/
void run_HSV_thresh()
{
    Mat frame_HSV;
    // Convert from BGR to HSV colorspace
    cvtColor(hsv_thresh_input_frame, frame_HSV, COLOR_BGR2HSV);
    // Detect the object based on HSV Range Values
    inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), hsv_thresh_output_frame);
}

/*----------------------------------------------------------*/
/* Infering pixel color & (x,y) coordinates at mouse click */
/*--------------------------------------------------------*/
//reference here: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
//Converting from RGB to HSV (reference): oh dear...

int tol_H   = 5;
int tol_S   = 60;
int tol_V   = 60;

int old_Hue = 0;
int new_Hue = 0;
int delta = 0;
int delta_tol = tol_H*4;

int update_HSV_range(int H, int S, int V)
{
    if(!mouse_clk)
    {
        //do not change trackbar Hue value, if it changes too dramatically from last known value.
        delta = old_Hue - new_Hue;
        if( delta <= -1*delta_tol || delta >= delta_tol)
        {
            //ignore change, and don't adjust HSV range.
            return 0;
        }

    }
    else
    {

        low_H = H-tol_H;//H-30;
        low_S = S-tol_S;//S-100;
        low_V = V-tol_V;

        high_H = H+tol_H;//H+30;
        high_S = S+tol_S;
        high_V = V+tol_V;



        on_high_V_thresh_trackbar(high_V);
        on_low_V_thresh_trackbar(low_V);
        on_high_S_thresh_trackbar(high_S);
        on_low_S_thresh_trackbar(low_S);
        on_high_H_thresh_trackbar(high_H);
        on_low_H_thresh_trackbar(low_H);

    
        mouse_clk = false; //reset flag.

        run_HSV_thresh();
    }

    return 1;

}

bool mouse_clk = true;

/*---- User selects pixel in camera-feed window, and some processing is done using the rgb and x,y data of that pixel ----*/
void mouseEvent(int evt, int x, int y, int flags, void* ) 
{                    

    if (evt == CV_EVENT_LBUTTONDOWN) 
    { 
        mouse_clk = true; //set flag.
        get_xy_pixel_hsv(x,y);
    }         
}

/*---- Pass x,y coordinates of any pixel; updates hsv range accordingly ----*/
void get_xy_pixel_hsv(int x, int y)
{

        /* Store old Hue value */
        old_Hue = new_Hue;

        /* decode pixel RGB values */
        Vec3b rgb=hsv_thresh_input_frame.at<Vec3b>(y,x);
        int B=rgb.val[0];
        int G=rgb.val[1];
        int R=rgb.val[2];

        /* Update the new mouse-selected seed pixel coordinates */
        seed_x = x;
        seed_y = y;

        Mat HSV;
        Mat RGB=hsv_thresh_input_frame(Rect(x,y,1,1));   //Single-value matrix that is the pixel at point [x,y], RGB encoded by default.
        cvtColor(RGB, HSV,CV_BGR2HSV);

        Vec3b hsv=HSV.at<Vec3b>(0,0);
        int H=hsv.val[0];
        int S=hsv.val[1];
        int V=hsv.val[2];

        printf("[%d, %d] H:%d, S:%d, V:%d\n\r", 
                x, y, 
                H, S, V);

        /* Update new Hue value */
        new_Hue = H;


        /* update slider positions (with hysteresis) on mouse-click */
        update_HSV_range(H,S,V);

}