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

        /* Reset ROI perimeter ranges  with each new mouse click */
        left_step   = 1;
        right_step  = 1;
        down_step   = 1;
        up_step     = 1;
    }         
}




    int RectDetect1::H = 0;
    int RectDetect1::S = 0;
    int RectDetect1::V = 0;






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
         S=hsv.val[1];
         V=hsv.val[2];

         printf("[%d, %d] H:%d, S:%d, V:%d\n\r", 
                 x, y, 
                 H, S, V);

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




int RectDetect1::left_step = 20;
int RectDetect1::right_step = 20;
int RectDetect1::down_step = 20;
int RectDetect1::up_step = 20;

/* Used to update average H in ROI_HSV once all ROI perimeter values have been checked */
int RectDetect1::count_H = 0;
int RectDetect1::sum_H = 0;

/* Flags that indicate when a side encloses the ROI */
bool RectDetect1::left_side_done = false;
bool RectDetect1::top_side_done = false;
bool RectDetect1::right_side_done = false;
bool RectDetect1::bottom_side_done = false;


int RectDetect1::x_left  = 0;
int RectDetect1::x_right = 0;
int RectDetect1::y_up    = 0;
int RectDetect1::y_down  = 0;

/* Applying mask */
void RectDetect1::get_mask()
{
    int thresh = 3;

    if(_mouse_clk)
    {

        x_left  = _seed_x - left_step;
        x_right = _seed_x + right_step;
        y_up    = _seed_y - up_step;
        y_down  = _seed_y + down_step; 
        

        get_xy_pixel_hsv(x_left,y_up);
        if(H <= ROI_HSV[0] + thresh || H >= ROI_HSV[0] - thresh)
        {

        }

        rectangle(  *_input_frame, 
                    Point(x_left,  y_up ), 
                    Point(x_right, y_down),
                    Scalar(0,0,255),
                    2,
                    8);
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




