/*--------------------------------------------------------------------------------------*/
/*  Attaches HSV thresholding trackbars onto window showing the binarized input image.
    Inherits Gaussian blur trackbar and uses it to perform gaussian blur on input image, 
    prior to HSV thresholding. Inherits ClickForPixelData in order to access selected 
    pixel HSV in blurred image, based on selected pixel coordinate from input image */
/*--------------------------------------------------------------------------------------*/
#ifndef __HSVTHRESHOLDTRACKBAR__
#define __HSVTHRESHOLDTRACKBAR__

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

#include "GaussianBlurTrackbar.hpp"
#include "ClickForPixelData.hpp"

class HsvThresholdTrackbar: public GaussianBlurTrackbar, public ClickForPixelData
{
    private:
        
        /*-------------------------------------------------*/
        /* Thresholding using inRange() in HSV colorspace */        
        /*-----------------------------------------------*/
        //reference here: https://docs.opencv.org/3.4/da/d97/tutorial_threshold_inRange.html


        cv::String window_name;

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

    protected:

        /* This Mat address needs to be shared with the ContourRectangles class */
        cv::Mat *_input_frame;


        /* HSV presets */
        void preset_HSV_callback(int val);                  // Callback fcn
        static void onPreset_track(int val, void *ptr);     // Redirection fcn
        #define RED 0
        #define GRN 1
        #define BLU 2
        int preset_count = 0;
        const int preset_max = BLU;

        int red[6] = {0, max_value_H, 120, max_value, 0, max_value};    // Preset HSV values for color Red 
        int grn[6] = {27, 152, 44, max_value, 0, max_value};            // Preset HSV values for color Green 
        int blu[6] = {80, 130, 52, max_value, 0, max_value};            // Preset HSV values for color Blue  

        void adjust_slideBars(); // Adjust slidebars according to the last known HSV parameters from the ClickForPixelData Base class.

    public:
        HsvThresholdTrackbar(cv::Mat *infrm, const cv::String glugg_nafn);
        void run_HSV_thresh();
};


#endif




