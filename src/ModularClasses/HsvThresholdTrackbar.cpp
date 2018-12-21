#include "HsvThresholdTrackbar.hpp"

using namespace cv;
using namespace std;


/* Gathering I/O frames and display window */
HsvThresholdTrackbar::HsvThresholdTrackbar(Mat *infrm, Mat *outfrm, String glugg_nafn)
{
    this->_input_frame  = infrm;
    this->_output_frame = outfrm;
    this->window_name   = glugg_nafn;
    namedWindow(this->window_name);

    // Trackbars to set thresholds for HSV values
    createTrackbar("Low H",  this->window_name, &low_H,     max_value_H,    onLow_H_track,  this);
    createTrackbar("High H", this->window_name, &high_H,    max_value_H,    onHigh_H_track, this);
    createTrackbar("Low S",  this->window_name, &low_S,     max_value,      onLow_S_track,  this);
    createTrackbar("High S", this->window_name, &high_S,    max_value,      onHigh_S_track, this);
    createTrackbar("Low V",  this->window_name, &low_V,     max_value,      onLow_V_track,  this);
    createTrackbar("High V", this->window_name, &high_V,    max_value,      onHigh_V_track, this);

}


void HsvThresholdTrackbar::run_HSV_thresh()
{

    /*SHOULD APPLY GAUSSIAN BLUR FIRST!*/

 // Apply blur to remove random noise.
    //GaussianBlur(*this->_input_frame, *this->_output_frame, Size(7, 7), 1.5, 1.5);
    
    Mat frame_HSV;

    /* Convert an unbinarized image into grayscale */
    if(this->_input_frame->channels() > 1)
    {
        //Convert from BGR to HSV colorspace
        cvtColor(*this->_input_frame, frame_HSV, COLOR_BGR2HSV);
    }
    else
    {
        frame_HSV = *this->_input_frame;
    }

    // Detect the object based on HSV Range Values
    inRange(frame_HSV, Scalar(this->low_H, this->low_S, this->low_V), Scalar(this->high_H, this->high_S, this->high_V), *this->_output_frame);

    imshow(this->window_name, *this->_output_frame);
}



/*---- Slidbar callback functions ----*/
void HsvThresholdTrackbar::low_H_callback(int val)
{
    this->low_H = min(this->high_H-1, val);
    setTrackbarPos("Low H", this->window_name, this->low_H);
}
void HsvThresholdTrackbar::high_H_callback(int val)
{
    this->high_H = max(val, this->low_H+1);
    setTrackbarPos("High H", this->window_name, this->high_H);
}
void HsvThresholdTrackbar::low_S_callback(int val)
{
    this->low_S = min(this->high_S-1, val);
    setTrackbarPos("Low S", this->window_name, this->low_S);
}
void HsvThresholdTrackbar::high_S_callback(int val)
{
    this->high_S = max(val, this->low_S+1);
    setTrackbarPos("High S", this->window_name, this->high_S);
}
void HsvThresholdTrackbar::low_V_callback(int val)
{
    this->low_V = min(this->high_V-1, val);
    setTrackbarPos("Low V", this->window_name, this->low_V);
}
void HsvThresholdTrackbar::high_V_callback(int val)
{
    this->high_V = max(val, this->low_V+1);
    setTrackbarPos("High V", this->window_name, this->high_V);
}


/* Redirection necessary to setup trackbar */
void HsvThresholdTrackbar::onLow_H_track(int val, void* ptr)
{
    HsvThresholdTrackbar* tmp = (HsvThresholdTrackbar*)(ptr);
    tmp->low_H_callback(val);
}
void HsvThresholdTrackbar::onHigh_H_track(int val, void* ptr)
{
    HsvThresholdTrackbar* tmp = (HsvThresholdTrackbar*)(ptr);
    tmp->high_H_callback(val);
}
void HsvThresholdTrackbar::onLow_S_track(int val, void* ptr)
{
    HsvThresholdTrackbar* tmp = (HsvThresholdTrackbar*)(ptr);
    tmp->low_S_callback(val);
}
void HsvThresholdTrackbar::onHigh_S_track(int val, void* ptr)
{
    HsvThresholdTrackbar* tmp = (HsvThresholdTrackbar*)(ptr);
    tmp->high_S_callback(val);
}
void HsvThresholdTrackbar::onLow_V_track(int val, void* ptr)
{
    HsvThresholdTrackbar* tmp = (HsvThresholdTrackbar*)(ptr);
    tmp->low_V_callback(val);
}
void HsvThresholdTrackbar::onHigh_V_track(int val, void* ptr)
{
    HsvThresholdTrackbar* tmp = (HsvThresholdTrackbar*)(ptr);
    tmp->high_V_callback(val);
}
