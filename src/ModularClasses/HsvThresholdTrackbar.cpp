#include "HsvThresholdTrackbar.hpp"

using namespace cv;
using namespace std;


/* Gathering I/O frames and display window */
HsvThresholdTrackbar::HsvThresholdTrackbar(Mat *infrm, String glugg_nafn):
GaussianBlurTrackbar(infrm, glugg_nafn)
{
    this->_input_frame  = infrm;
    this->window_name   = glugg_nafn;

    // Need to setup window so trackbars can be attached to it.
    namedWindow(this->window_name);

    // Trackbars to set thresholds for HSV values
    createTrackbar("Low H",  this->window_name, &low_H,     max_value_H,    onLow_H_track,  this);
    createTrackbar("High H", this->window_name, &high_H,    max_value_H,    onHigh_H_track, this);
    createTrackbar("Low S",  this->window_name, &low_S,     max_value,      onLow_S_track,  this);
    createTrackbar("High S", this->window_name, &high_S,    max_value,      onHigh_S_track, this);
    createTrackbar("Low V",  this->window_name, &low_V,     max_value,      onLow_V_track,  this);
    createTrackbar("High V", this->window_name, &high_V,    max_value,      onHigh_V_track, this);

    // HSV presets
    createTrackbar("RED: 0, GRN: 1, BLU:2", this->window_name, &preset_count, preset_max, onPreset_track, this);

}


void HsvThresholdTrackbar::run_HSV_thresh()
{

    /*SHOULD APPLY GAUSSIAN BLUR FIRST!*/
    this->gauss_blur();

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
    inRange(frame_HSV, Scalar(this->low_H, this->low_S, this->low_V), Scalar(this->high_H, this->high_S, this->high_V), *this->_input_frame);

    imshow(this->window_name, *this->_input_frame);
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






/* HSV preset functions */
void HsvThresholdTrackbar::onPreset_track(int val, void *ptr)
{
    HsvThresholdTrackbar* tmp = (HsvThresholdTrackbar*)(ptr);
    tmp->preset_HSV_callback(val);
}
void HsvThresholdTrackbar::preset_HSV_callback(int val)
{
    
    switch (val)
    {
        case RED: 

            this->high_H  = max_value_H;
            this->low_H   = 0;
            this->high_S  = max_value;
            this->low_S   = 120;
            this->high_V  = max_value;
            this->low_V   = 0;

            break;

        case BLU:

            this->high_H  = 130;
            this->low_H   = 80;
            this->high_S  = max_value;
            this->low_S   = 52;
            this->high_V  = max_value;
            this->low_V   = 0;

            break;

        case GRN:

            this->high_H  = 152;
            this->low_H   = 27;
            this->high_S  = max_value;
            this->low_S   = 44;
            this->high_V  = max_value;
            this->low_V   = 0;

            break;
    
        default:
            break;
    }

    setTrackbarPos("High H",    this->window_name, this->high_H);
    setTrackbarPos("Low H",     this->window_name, this->low_H);
    setTrackbarPos("High S",    this->window_name, this->high_S);
    setTrackbarPos("Low S",     this->window_name, this->low_S);
    setTrackbarPos("High V",    this->window_name, this->high_V);
    setTrackbarPos("Low V",     this->window_name, this->low_V);
}