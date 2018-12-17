#include "RectDetect1.hpp"


/* Initializing static members */
int RectDetect1::_seed_x = 0;
int RectDetect1::_seed_y = 0; 

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

void RectDetect1::mouseEvent(int evt, int x, int y, int flags) 
{                    

    if (evt == CV_EVENT_LBUTTONDOWN) 
    { 
        _mouse_clk = true; //set flag.
        get_xy_pixel_hsv(x,y);
    }         
}

/*---- Pass x,y coordinates of any pixel; updates hsv range accordingly ----*/
void RectDetect1::get_xy_pixel_hsv(int x, int y)
{

        Mat tmp = *_input_frame;


        /* decode pixel RGB values */
         Vec3b rgb = tmp.at<Vec3b>(y,x);
         int B=rgb.val[0];
         int G=rgb.val[1];
         int R=rgb.val[2];

        // /* Update the new mouse-selected seed pixel coordinates */
        _seed_x = x;
        _seed_y = y;

         Mat HSV;
         Mat RGB= tmp(Rect(x,y,1,1));   //Single-value matrix that is the pixel at point [x,y], RGB encoded by default.
         cvtColor(RGB, HSV,CV_BGR2HSV);

         Vec3b hsv=HSV.at<Vec3b>(0,0);
         int H=hsv.val[0];
         int S=hsv.val[1];
         int V=hsv.val[2];

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
    GaussianBlur(*_input_frame, *_output_frame, Size(this->_gauss_blur_qty, this->_gauss_blur_qty),0,0);
}















/* Methods for displaying I/O frames in windows */
void RectDetect1::show_input_frames()
{
    imshow(this->window_cam, *_input_frame);
}

void RectDetect1::show_output_frames()
{
    imshow(this->window_gauss_name, *_output_frame);
}