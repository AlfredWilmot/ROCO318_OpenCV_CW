#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;




class GaussianBlurTrackbar
{
private:
    
    Mat *_input_frame;
    Mat *_output_frame;
    String window_name;

    int _gauss_blur_qty = 3, _max_gauss = 100;  // Trackbar parameters

    static void onGausTrack(int val, void *ptr);
    void gauss_blur_callback(int val);

public:
    GaussianBlurTrackbar(Mat *infrm, Mat *outfrm, const String glugg_nafn);
    void AddGaussTrackbar();
    void gauss_blur();
};



/* Gathering I/O frames and display window */
GaussianBlurTrackbar::GaussianBlurTrackbar(Mat *infrm, Mat *outfrm, String glugg_nafn)
{
    this->_input_frame  = infrm;
    this->_output_frame = outfrm;
    this->window_name   = glugg_nafn;
    namedWindow(this->window_name);
    createTrackbar("Gaussian Blur", this->window_name, &this->_gauss_blur_qty, this->_max_gauss, GaussianBlurTrackbar::onGausTrack, this);

}

/* Redirection necessary to setup trackbar */
void GaussianBlurTrackbar::onGausTrack(int val, void* ptr)
{
    GaussianBlurTrackbar* tmp = (GaussianBlurTrackbar*)(ptr);
    tmp->gauss_blur_callback(val);
}



/* Trackbar callback */
void GaussianBlurTrackbar::gauss_blur_callback(int val)
{
     if (val % 2 == 0)
     {
       val = val + 1;
     }
     this->_gauss_blur_qty = val;
}


/* Processing methods */
void GaussianBlurTrackbar::gauss_blur()
{
    GaussianBlur(*this->_input_frame, *this->_output_frame, Size(this->_gauss_blur_qty, this->_gauss_blur_qty),0,0);
    imshow(this->window_name, *this->_output_frame);
}


