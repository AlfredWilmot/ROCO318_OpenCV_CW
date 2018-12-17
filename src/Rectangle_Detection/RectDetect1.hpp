#ifndef __RECTDETECT1__
#define __RECTDETECT1__


#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;


 




class RectDetect1
{
private:

    /* Class I/O */
    Mat *_input_frame;
    Mat *_output_frame;

    /* Gaussian blur params */
    int _gauss_blur_qty = 3, _max_gauss = 100;

    /* Output window names */
    const String window_gauss_name = "Gaussian Blur";

public:

    /* Constructor method */
    RectDetect1(Mat *infrm_ptr, Mat *outfrm_ptr);

    /* Show processed frames */
    void show_output_frames();

    /* Trackbar initialization */
    void trackbar_init();

    /* Trackbar callback methods and redirection to get it working: https://stackoverflow.com/questions/28418064/casting-c-class-into-another-one-to-create-opencv-trackbar-handle*/
    static void onGausTrack(int val, void *ptr);
    void gauss_blur_callback(int val);



    /* Processing methods */
    void gauss_blur();
};





/* Constructor: stores I/O frame ptr addresses */
RectDetect1::RectDetect1(Mat *infrm_ptr, Mat *outfrm_ptr)
{
    this->_input_frame  = infrm_ptr;
    this->_output_frame = outfrm_ptr;

    /* Setup the named windows */
    namedWindow(window_gauss_name);

    /* Setup the trackbars */
    trackbar_init();
}

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
    GaussianBlur(*this->_input_frame, *this->_output_frame, Size(this->_gauss_blur_qty, this->_gauss_blur_qty),0,0);
}



/* Show output frames */
void RectDetect1::show_output_frames()
{
    imshow(this->window_gauss_name, *this->_output_frame);
}

#endif
