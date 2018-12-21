#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;


#include "CannyThresholdTrackbar.hpp"


/* Gathering I/O frames and display window */
CannyThresholdTrackbar::CannyThresholdTrackbar(Mat *infrm, Mat *outfrm, String glugg_nafn)
{
    this->_input_frame  = infrm;
    this->_output_frame = outfrm;
    this->window_name   = glugg_nafn;
    namedWindow(this->window_name);
    createTrackbar("Canny Threshold", this->window_name, &this->_thresh, this->_max_thresh, CannyThresholdTrackbar::onCannyTrack, this);
}



/* Canny & Sobel Trackbar callbacks */
void CannyThresholdTrackbar::canny_thresh_callback(int val)
{
     this->_thresh = val;
}


/* Processing methods */
void CannyThresholdTrackbar::canny_thresh()
{

    // make sure input frame isn't empty.
    this->errorHandling();

    /* Convert an unbinarized image into grayscale */
    if(this->_input_frame->channels() > 1)
    {
        //Convert from BGR to HSV colorspace
        cvtColor( *this->_input_frame, *this->_output_frame, CV_BGR2GRAY );
    }
    else
    {
        *this->_output_frame = *this->_input_frame;
    }
    
    blur( *this->_output_frame, *this->_output_frame, Size(3,3) );
    Canny( *this->_output_frame, *this->_output_frame, this->_thresh, this->_thresh*2, 3);

    imshow(this->window_name, *this->_output_frame);
}










/* Redirection necessary to setup trackbars within a class */

void CannyThresholdTrackbar::onCannyTrack(int val, void* ptr)
{
    CannyThresholdTrackbar* tmp = (CannyThresholdTrackbar*)(ptr);
    tmp->canny_thresh_callback(val);
}


void CannyThresholdTrackbar::errorHandling()
{
    if(this->_input_frame->empty())
    {
        throw std::invalid_argument( "Input frame for CannyThresholdTrackbar object is empty!");
    }
}