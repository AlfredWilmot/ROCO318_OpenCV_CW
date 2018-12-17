#include "RectDetect1.hpp"


/* Initializing static members */
int RectDetect1::_seed_x = 0;
int RectDetect1::_seed_y = 0; 
bool RectDetect1::_mouse_clk = false;

Mat *RectDetect1::_input_frame;
Mat *RectDetect1::_output_frame;



/* Constructor: stores I/O frame ptr addresses */
RectDetect1::RectDetect1(Mat *infrm_ptr, Mat *outfrm_ptr)
{
    _input_frame  = infrm_ptr;
   _output_frame = outfrm_ptr;

    /* Setup the named windows */
    namedWindow(this->window_cam);
    namedWindow(this->window_gauss_name);

    /* Setup the trackbars */
    trackbar_init();

    /* Setup mouse-click event on input frame */
    setMouseCallback(this->window_cam, RectDetect1::onMouseEvt, _input_frame);
}


void RectDetect1::onMouseEvt(int evt, int x, int y, int flags, void* ptr)
{
    RectDetect1* tmp = (RectDetect1*)(ptr);
    tmp->mouseEvent(evt, x, y, flags);
}



int RectDetect1::ROI_HSV[3];

void RectDetect1::mouseEvent(int evt, int x, int y, int flags) 
{                    

    if (evt == CV_EVENT_LBUTTONDOWN) 
    { 
        _mouse_clk = true;      //set flag.
        get_xy_pixel_hsv(x,y);  //get HSV values of selected pixel. 

        // /* Update the new mouse-selected seed pixel coordinates */
        _seed_x = x;
        _seed_y = y;

        /* Store seed-pixel HSV values to serve as starting point of cumulative HSV average. */
        ROI_HSV[0] = H;
        ROI_HSV[1] = S;
        ROI_HSV[2] = V;

        get_mask();

    }         
}




    int RectDetect1::H = 0;
    int RectDetect1::S = 0;
    int RectDetect1::V = 0;
    int *const RectDetect1::H_ptr = &RectDetect1::H;
    int *const RectDetect1::S_ptr = &RectDetect1::S;
    int *const RectDetect1::V_ptr = &RectDetect1::V;





/*---- Calculates and stores HSV value of pixel at input coordinate x,y ----*/
void RectDetect1::get_xy_pixel_hsv(int x, int y)
{

        Mat tmp = *_input_frame;


        /* decode pixel RGB values */
         Vec3b rgb = tmp.at<Vec3b>(y,x);
         int B=rgb.val[0];
         int G=rgb.val[1];
         int R=rgb.val[2];

         Mat HSV;
         Mat RGB= tmp(Rect(x,y,1,1));   //Single-value matrix that is the pixel at point [x,y], RGB encoded by default.
         cvtColor(RGB, HSV,CV_BGR2HSV);

         Vec3b hsv=HSV.at<Vec3b>(0,0);
         H=hsv.val[0];
         *S_ptr=hsv.val[1];
         *V_ptr=hsv.val[2];

         printf("[%d, %d] H:%d, S:%d, V:%d\n\r", 
                 x, y, 
                 H, *S_ptr, *V_ptr);

}







/* 
    createTrackbar() requires address of a static function. 
    "this" ptr is used as an arg in the actual trackbar instantiation, 
    and is later resolved to the class instance.
*/

void RectDetect1::onGausTrack(int val, void* ptr)
{
    RectDetect1* tmp = (RectDetect1*)(ptr);
    tmp->gauss_blur_callback(val);
}


/* Trackbar callbacks */
void RectDetect1::gauss_blur_callback(int val)
{
     if (val % 2 == 0)
     {
       val = val + 1;
     }
     this->_gauss_blur_qty = val;
}

/* Initialzie all the trackbars */
void RectDetect1::trackbar_init()
{
    createTrackbar("Blur", window_gauss_name, &this->_gauss_blur_qty, this->_max_gauss, RectDetect1::onGausTrack, this);
}

/* Processing methods */
void RectDetect1::gauss_blur()
{
    GaussianBlur(*_input_frame, *_input_frame, Size(this->_gauss_blur_qty, this->_gauss_blur_qty),0,0);

}




int RectDetect1::left_step = 2;
int RectDetect1::right_step = 2;
int RectDetect1::down_step = 2;
int RectDetect1::up_step = 2;

/* Used to update average H in ROI_HSV once all ROI perimeter values have been checked */
int RectDetect1::count_H = 0;
int RectDetect1::sum_H = 0;

/* Flags that indicate when a side encloses the ROI */
bool RectDetect1::left_side_done = false;
bool RectDetect1::top_side_done = false;
bool RectDetect1::right_side_done = false;
bool RectDetect1::bottom_side_done = false;


/* Applying mask */
void RectDetect1::get_mask()
{
    if(_mouse_clk)
    {
        /* TEST: Draw rectangle around selected pixel */

        int x_left  = _seed_x - left_step;
        int x_right = _seed_x + right_step;
        int y_up    = _seed_y - up_step;
        int y_down  = _seed_y + down_step;

        int HSV_thresh = 1;



        /*reset flag*/
        left_side_done = false;

        while(!left_side_done)
        {
            // check HSV values along left-side.
            for(int left_y_vals = y_up; left_y_vals <= y_down; left_y_vals++)
            {

                /* Ignore false readings H==0, for a fixed number of iterations */
                // for(int k = 0;k<10;k++)
                // {
                    get_xy_pixel_hsv(x_left, left_y_vals);
                //     if(*H_ptr != 0)
                //     {
                //         break;
                //     }
                // }

                if( ( H >= (ROI_HSV[0] - HSV_thresh) ) && ( H <= (ROI_HSV[0] + HSV_thresh) ) )
                {
                    /* make note of sample if within the Hue threshold of the seed pixel */
                    count_H++;
                    sum_H += H;
                }
                else{
                    
                    /* if the sampled pixel is outside the Hue threshold, then disregard it. */
                }

                printf("x, y: %d, %d\n\r",x_left, left_y_vals);
            
            if(count_H == 0 || x_left == 0)
            {
                /* If all samples are outside the Hue threshold, 
                then this side now encloses the ROI. 
                So stop extending out this side.
                Also considers edge of window.
                */
            left_side_done = true;
            }
            else
            {
                /* Otherwise, further extend this side.*/
                x_left--;
                ROI_HSV[0] = int(float(sum_H)/float(count_H));
            }




            }
        }





        // check HSV values along the bottom-side



        

        //      
        // --||-- bottom-side.
        //      if all not outside HSV range, extend outwards.
        // --||-- right-side.
        //      if all not outside HSV range, extend outwards.
        // --||-- top-side.
        //     if all not outside HSV range, extend outwards.

        // if all within HSV range, add to cumulative average HSV value.
        // else, stop cumulative average.

    }
}














/* Methods for displaying I/O frames in windows */
void RectDetect1::show_input_frames()
{
    imshow(this->window_cam, *_input_frame);
}

void RectDetect1::show_output_frames()
{
    imshow(this->window_gauss_name, *_input_frame);
}