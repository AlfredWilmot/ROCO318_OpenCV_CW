#include "HsvThresholdTrackbar.hpp"

using namespace cv;
using namespace std;


/* Gathering I/O frames and display window */
HsvThresholdTrackbar::HsvThresholdTrackbar(Mat *infrm, String glugg_nafn):
GaussianBlurTrackbar(infrm, glugg_nafn),
ClickForPixelData("Click me!")
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


/*---- Use inherited get_pixel_HSV to adjust slidebars  ----*/
void HsvThresholdTrackbar::adjust_slideBars()
{
    if(this->get_seed_pixel_hsv(false) == 0 && this->capture_hsv == 1) 
    {
        
        /* hard-coded HSV ranges around the seed-pixel HSV values */
        int H_step = 20;
        int S_step = 50;
        int V_step = 50;

        /* Use the HSV values to adjust the low/ high HSV trackbar values */
        this->low_H  = ( (this->H - H_step) > 0) ? this->H - H_step : 0;
        this->low_S  = ( (this->S - S_step) > 0) ? this->S - S_step : 0;
        this->low_V  = ( (this->V - V_step) > 0) ? this->V - V_step : 0;

        this->high_H = ( (this->H + H_step) < this->max_value_H) ? this->H + H_step : this->max_value_H; 
        this->high_S = ( (this->S + S_step) < this->max_value)   ? this->S + S_step : this->max_value; 
        this->high_V = ( (this->V + V_step) < this->max_value)   ? this->V + V_step : this->max_value; 


        /* Consider that color RED is at either extreme of the Hue-spectrum */
        if(this->low_H == 0 || this->high_H == this->max_value_H)
        {
          this->low_H  = 0;
          this->high_H = this->max_value_H;
        }

        /* Update trackbar values */
        setTrackbarPos("Low H",  this->window_name, this->low_H);
        setTrackbarPos("High H", this->window_name, this->high_H);
        setTrackbarPos("Low S",  this->window_name, this->low_S);
        setTrackbarPos("High S", this->window_name, this->high_S);
        setTrackbarPos("Low V",  this->window_name, this->low_V);
        setTrackbarPos("High V", this->window_name, this->high_V);
    }
}

/*---- Apply HSV thresholding to input image ----*/
void HsvThresholdTrackbar::run_HSV_thresh()
{

    /*SHOULD APPLY GAUSSIAN BLUR FIRST!*/
    this->gauss_blur(false);

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


    /* Update the slidebars if mouse has been recently clicked */
    adjust_slideBars();

}



/*---- Slidebar callback functions ----*/
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




/*---- Redirections necessary to setup Thresholding trackbars ----*/
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






/*---- HSV preset functions ----*/
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

            this->low_H   = this->red[0];
            this->high_H  = this->red[1];
            this->low_S   = this->red[2];
            this->high_S  = this->red[3];
            this->low_V   = this->red[4];
            this->high_V  = this->red[5];
            

            break;

        case GRN:

            this->low_H   = this->grn[0];
            this->high_H  = this->grn[1];
            this->low_S   = this->grn[2];
            this->high_S  = this->grn[3];
            this->low_V   = this->grn[4];
            this->high_V  = this->grn[5];
            

            break;

        case BLU:

            this->low_H   = this->blu[0];
            this->high_H  = this->blu[1];
            this->low_S   = this->blu[2];
            this->high_S  = this->blu[3];
            this->low_V   = this->blu[4];
            this->high_V  = this->blu[5];
            

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