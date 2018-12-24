#ifndef __HSVTHRESHOLDTRACKBAR__
#define __HSVTHRESHOLDTRACKBAR__

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

#include "GaussianBlurTrackbar.hpp"

class HsvThresholdTrackbar: public GaussianBlurTrackbar
{
private:
    
    //cv::Mat *_input_frame;
    //cv::Mat *_output_frame;
    //cv::String window_name;


    /* Thresholding variables */
    const int max_value_H = 180;
    const int max_value = 255;
    int low_H = 0, low_S = 0, low_V = 0;
    int high_H = this->max_value_H;
    int high_S = this->max_value, high_V = this->max_value;

    /* Redirection methods to get trackbars to function from within class */
    static void onLow_H_track(int val, void *ptr);
    static void onHigh_H_track(int val, void *ptr);
    static void onLow_S_track(int val, void *ptr);
    static void onHigh_S_track(int val, void *ptr);
    static void onLow_V_track(int val, void *ptr);
    static void onHigh_V_track(int val, void *ptr);
    

    /* Callback functions called whenever a trackbar is adjusted */
    void low_H_callback(int val);
    void high_H_callback(int val);
    void low_S_callback(int val);
    void high_S_callback(int val);
    void low_V_callback(int val);
    void high_V_callback(int val);


    /* HSV presets */
    void preset_HSV_callback(int val);                  // Callback fcn
    static void onPreset_track(int val, void *ptr);     // Redirection fcn
    #define RED 0
    #define GRN 1
    #define BLU 2
    int preset_count = 0;
    const int preset_max = BLU;


public:
    HsvThresholdTrackbar(cv::Mat *infrm, cv::Mat *outfrm, const cv::String glugg_nafn);
    void run_HSV_thresh();
};


#endif




