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

        /* Reset the max and min Hue thresholds */
        ROI_H_max = ROI_HSV[0];
        ROI_H_min = ROI_HSV[0];


        /* Reset ROI perimeter ranges  with each new mouse click */
        seed_offset = 1;
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




int RectDetect1::seed_offset = 1;

/* Flags that indicate when a side encloses the ROI */
bool RectDetect1::left_side_done = false;
bool RectDetect1::top_side_done = false;
bool RectDetect1::right_side_done = false;
bool RectDetect1::bottom_side_done = false;


int RectDetect1::x_left  = 0;
int RectDetect1::x_right = 0;
int RectDetect1::y_up    = 0;
int RectDetect1::y_down  = 0;

/* Max & min Hue values adjusted by generated mask */
int RectDetect1::ROI_H_max = 0;
int RectDetect1::ROI_H_min = 0;

/* Applying mask */
void RectDetect1::get_mask()
{
    
    if(_mouse_clk)
    {

        x_left  = _seed_x - seed_offset;
        x_right = _seed_x + seed_offset;
        y_up    = _seed_y - seed_offset;
        y_down  = _seed_y + seed_offset; 


        // Top left:     x_left,  y_up
        // Top right:    x_right, y_up
        // Bottom left:  x_left, y_down
        // Bottom right: x_right, y_down


        /* Check corner pixels of ROI mask */
        for(int state = 0; state < 4; state++)
        {
            if(state==0)
            {
                update_thresh(x_left, y_up);
            }
            else if(state==1)
            {
                update_thresh(x_right, y_up);
            }
            else if(state==2)
            {
                update_thresh(x_left, y_down);
            }
            else if(state==3)
            {
                update_thresh(x_right, y_down);
            }
            else
            {
                throw invalid_argument( "state in get_mask is an invalid value");
            }
            state ++;
        }


        rectangle(  *_input_frame, 
                    Point(x_left,  y_up ), 
                    Point(x_right, y_down),
                    Scalar(0,0,255),
                    2,
                    8);
    }
}



void RectDetect1::update_thresh(int x_dir, int y_dir)
{
    int thresh = 3;
    int step = 5;

    get_xy_pixel_hsv(x_dir, y_dir);

    /* Check if measured pixel Hue is within threshold */
    if ( H <= (ROI_H_max + thresh) && H >= (ROI_H_min - thresh) )
    {

        /* Update ROI max/min to expand range of acceptable Hue values to improve mask coverage */
        if(H > ROI_H_max)
        {   
            ROI_H_max = H;
        }

        else if (H < ROI_H_min)
        {
            ROI_H_min = H;
        }

        seed_offset += step;

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




