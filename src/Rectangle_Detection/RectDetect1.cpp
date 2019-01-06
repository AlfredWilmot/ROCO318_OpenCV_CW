#include "RectDetect1.hpp"


/* Initializing static members */
int RectDetect1::_seed_x = 0;
int RectDetect1::_seed_y = 0; 
bool RectDetect1::_mouse_clk = false;

/* Declaring static variables to get trackbars to function */
Mat *RectDetect1::_input_frame;
Mat *RectDetect1::_output_frame;
Mat *RectDetect1::gauss_input_frame;
Mat RectDetect1::gauss_output_frame;
Mat *RectDetect1::hsv_thresh_input_frame;
Mat RectDetect1::hsv_thresh_output_frame;
Mat RectDetect1::contour_input_frame;
Mat RectDetect1::contour_output_frame;
Mat RectDetect1::morph_input_frame;
Mat RectDetect1::morph_output_frame;
Mat RectDetect1::canny_input_frame;
Mat RectDetect1::canny_output_frame;



Size RectDetect1::input_frame_size;

/* Constructor: stores I/O frame ptr addresses */
RectDetect1::RectDetect1(Mat *infrm, Mat *outfrm)
{
    /* Reference I/O frames */
    _input_frame  = infrm;
   _output_frame  = outfrm;
    
    /* Setup the named windows */
    namedWindow(this->window_cam);
    namedWindow(this->gauss_display_window);
    namedWindow(this->hsv_display_window);
    //namedWindow(this->contour_display_window);
    //namedWindow(this->morph_display_window);

    /* Setup the trackbars */
    trackbar_init();

    /* Setup mouse-click event on input frame */
    setMouseCallback(this->window_cam, RectDetect1::onMouseEvt, _input_frame);
}


void RectDetect1::onMouseEvt(int evt, int x, int y, int flags, void* ptr)
{
    /* Redirection to get handler working */
    RectDetect1* tmp = (RectDetect1*)(ptr);
    tmp->mouseEvent(evt, x, y, flags);

    /* Get frame size */
    input_frame_size = _input_frame->size(); 

}



int RectDetect1::ROI_HSV[3];

int RectDetect1::mouseEvent(int evt, int x, int y, int flags) 
{                    

    if (evt == CV_EVENT_LBUTTONDOWN) 
    { 
        if (get_xy_pixel_hsv(x,y) == 0)  //get HSV values of selected pixel. 
        {
             _mouse_clk = true;      //set flag.
        }
        else
        {
            printf("Selected pixel has a Hue reading of 0.\n\r");
            return -1;
        }

        // /* Update the new mouse-selected seed pixel coordinates */
        _seed_x = x;
        _seed_y = y;

        /* Store seed-pixel HSV values to serve as starting point of cumulative HSV average. */
        ROI_HSV[0] = H;
        ROI_HSV[1] = S;
        ROI_HSV[2] = V;

        /* Reset the max and min Hue thresholds */
        ROI_H_max = H;
        ROI_H_min = H;
        ROI_S_max = S;
        ROI_S_min = S;
        ROI_V_max = V;
        ROI_V_min = V;


        /* Reset ROI perimeter ranges  with each new mouse click */
        seed_x_offset = 0;
        seed_y_offset = 0;

        /* Within the ROI, Identify the range of Hue values around the selected pixel */
        while(find_Hue_range() == 0);

        return 0;
    }         
}




    int RectDetect1::H = 0;
    int RectDetect1::S = 0;
    int RectDetect1::V = 0;






/*---- Calculates and stores HSV value of pixel at input coordinate x,y ----*/
int RectDetect1::get_xy_pixel_hsv(int x, int y)
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

        if (hsv.val[0] == 0)
        {
            return -1;
        }
        else
        {
            H=hsv.val[0];
            S=hsv.val[1];
            V=hsv.val[2];
        }
         printf("[%d, %d] H:%d, S:%d, V:%d\n\r", 
                 x, y, 
                 H, S, V);

        return 0;

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
    createTrackbar("Blur", gauss_display_window, &this->_gauss_blur_qty, this->_max_gauss, RectDetect1::onGausTrack, this);
}

/* Processing methods */
void RectDetect1::gauss_blur()
{
    gauss_input_frame = _input_frame;
    GaussianBlur(*gauss_input_frame, gauss_output_frame, Size(this->_gauss_blur_qty, this->_gauss_blur_qty),0,0);
    _output_frame = &gauss_output_frame;
    imshow(this->gauss_display_window, gauss_output_frame);
}

void RectDetect1::HSV_binarization()
{
    /* HSV binarization */
    // const int low_S   = 0;
    // const int high_S  = 255;
    // const int low_V   = 0;
    // const int high_V  = 255;
    
    hsv_thresh_input_frame = &gauss_output_frame;
    // Convert from BGR to HSV colorspace
    cvtColor(*hsv_thresh_input_frame, hsv_thresh_output_frame, COLOR_BGR2HSV);
    // Detect the object based on HSV Range Values
    inRange(hsv_thresh_output_frame, Scalar(ROI_H_min, ROI_S_min, ROI_V_min), Scalar(ROI_H_max, ROI_S_max, ROI_V_max), hsv_thresh_output_frame);

    imshow(this->hsv_display_window, hsv_thresh_output_frame);
}










int RectDetect1::seed_x_offset = 0;
int RectDetect1::seed_y_offset = 0;

/* Flags that indicate when the Hue thresholding is calibrated */
bool RectDetect1::up_left_done      = false;
bool RectDetect1::down_left_done    = false;
bool RectDetect1::down_right_done   = false;
bool RectDetect1::up_right_done     = false;


int RectDetect1::x_left  = 0;
int RectDetect1::x_right = 0;
int RectDetect1::y_up    = 0;
int RectDetect1::y_down  = 0;

/* Max & min Hue values adjusted by generated mask */
int RectDetect1::ROI_H_max = H;
int RectDetect1::ROI_H_min = H;

/* Get a Hue range for the selcted pixel */
int  RectDetect1::find_Hue_range()
{
    
    if(_mouse_clk)
    {

        /* Check corner pixels of ROI mask */
        for(int state = 0; state < 4; state++)
        {
            if(state==0 && !up_left_done)
            {
                x_left  = _seed_x - seed_x_offset;
                y_up    = _seed_y - seed_y_offset;

                up_left_done = update_thresh(x_left, y_up);         //check top-left corner pixel.
            }
            else if(state==1 && !down_left_done)
            {
                x_left  = _seed_x - seed_x_offset;
                y_down  = _seed_y + seed_y_offset; 

                down_left_done = update_thresh(x_left, y_down);      //check bottom-left corner pixel.
            }
            else if(state==2 && !down_right_done)
            {

                x_right = _seed_x + seed_x_offset;
                y_down  = _seed_y + seed_y_offset;

                down_right_done = update_thresh(x_right, y_down);    //check bottom-right corner pixel.
            }
            else if(state==3 && !up_right_done)
            {
                x_right = _seed_x + seed_x_offset;
                y_up    = _seed_y - seed_y_offset;

                up_right_done = update_thresh(x_right, y_up);           //check top-right corner pixel.
            }
        }

        /* Stop calibration once maximum ROI Hue threshold ranging is done */
        if(up_left_done && down_left_done && down_right_done && up_right_done)
        {
            /* Reset flags, wait for next calibration trigger */
            _mouse_clk      = false; 
            up_left_done    = false;
            down_left_done  = false;
            down_right_done = false;
            up_right_done   = false;

            printf("measured ROI Hue range: [%d, %d]\n\r", ROI_H_max, ROI_H_min);
            printf("measured ROI Saturation range: [%d, %d]\n\r", ROI_S_max, ROI_S_min);
            printf("measured ROI Value range: [%d, %d]\n\r", ROI_V_max, ROI_V_min);

            return 1;//indicate done
        }

    /* Shows rectangle bounded by corners used to calibrate Hue range for ROI */
    rectangle(  *_input_frame, 
                Point(x_left,  y_up ), 
                Point(x_right, y_down),
                Scalar(0,0,255),
                2,
                8);
    }
    return 0;
}


int RectDetect1::ROI_S_max = S;
int RectDetect1::ROI_S_min = S;
int RectDetect1::ROI_V_max = V;
int RectDetect1::ROI_V_min = V;


/*BUG: TOO DEPENDENT ON INITIAL SEED PIXEL HSV VALUE*/
bool RectDetect1::update_thresh(int x_dir, int y_dir)
{
    int thresh = 1;
    int step = 2;


    get_xy_pixel_hsv(x_dir, y_dir);

    /* Check if Hue is either in the upper threshold or the lower threshold */
    if  ( 
            ( 
                (   H <= (ROI_H_max + thresh) ) 
            &&
                ( (ROI_H_max + thresh) < 180  ) 
            )
        ||
            (
                (   H >= (ROI_H_min - thresh) )
            &&
                ( (ROI_H_min - thresh) > 0    )
            )
        )
    {
        /* Extend Hue max */
        if  (
                ( H > ROI_H_max )
            && 
                ( H <= (ROI_H_max + thresh) )
            )
        {   
            ROI_H_max = H;
        }
        /* Extend Hue Hue min */
        else if(
                    ( H < ROI_H_min)
                &&
                    ( H >= (ROI_H_min - thresh) )
                )
        {
            ROI_H_min = H;
        }

        /*  Update the saturation & value ranges based on HSV data from sampled pixel:
            This helps filter out some noise.
        */
        // ROI_S_max = S ? S > ROI_S_max : ROI_S_max = ROI_S_max;
        // ROI_S_min = S ? S < ROI_S_min : ROI_S_min = ROI_S_min;
        // ROI_V_max = V ? V > ROI_V_max : ROI_V_max = ROI_V_max;
        // ROI_V_min = V ? V < ROI_V_min : ROI_V_min = ROI_V_min;

        if(S > ROI_S_max)
        {
            ROI_S_max = S;
        }
        if(S < ROI_S_min)
        {
            ROI_S_min = S;
        }

        if(V > ROI_V_max)
        {
             ROI_V_max = V;
        }
        if(V < ROI_V_min)
        {
            ROI_V_min = V;
        }



        /* Only move the ROI corners outwards if no risk of exceeding window boundaries */
        if
        (
            ((x_dir + step) < (input_frame_size.width-10)) 
            && 
            ((x_dir - step) > 10)
        )
        {
            seed_x_offset += step;
        }
        else
        {
            /* STOP THRESHOLD FROM EXPANDING */   
            up_left_done    = true;
            down_left_done  = true;
            down_right_done = true;
            up_right_done   = true;
            return true; //Stop moving corner if it has hit an x-axis boundary.
        }

        if
        (
            ((y_dir + step) < (input_frame_size.height-10))
            && 
            ((y_dir - step) > 10)
        )
        {
            seed_y_offset += step;
            return false;
        }
        else
        {         
            /* STOP THRESHOLD FROM EXPANDING */   
            up_left_done    = true;
            down_left_done  = true;
            down_right_done = true;
            up_right_done   = true;
            return true; //Stop moving corner if it has hit a y-axis boundary.
        }
    }
    else
    {
        return true;   /* Stop corner creep if it's outside the Hue threshold */ 
    }  
}















/* Methods for displaying I/O frames in windows */
void RectDetect1::show_input_frames()
{
    imshow(this->window_cam, *_input_frame);
}
