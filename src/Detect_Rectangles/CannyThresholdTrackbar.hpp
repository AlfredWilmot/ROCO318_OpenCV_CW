#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;




class CannyThresholdTrackbar
{
private:
    
    Mat *_input_frame;
    Mat *_output_frame;
    String window_name;

    int _thresh = 3, _max_thresh = 100; // Canny Threshold Trackbar parameters

    static void onCannyTrack(int val, void *ptr);
    void canny_thresh_callback(int val);

public:
    CannyThresholdTrackbar(Mat *infrm, Mat *outfrm, const String glugg_nafn);
    void canny_thresh();
};



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


/* Processing method */
void CannyThresholdTrackbar::canny_thresh()
{
    cvtColor( *this->_input_frame, *this->_output_frame, CV_BGR2GRAY );
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


