/*-------------------------------------------------------------------------------------*/
/*  Attaches Gaussian-blur trackbar onto a window that shows the blurred input Matrix */
/*-----------------------------------------------------------------------------------*/
#ifndef __GAUSSIANBLURTRACKBAR__
#define __GAUSSIANBLURTRACKBAR__

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>


class GaussianBlurTrackbar
{
    private:
        
        /* Input Matrix address and display window window */
        cv::Mat *_input_frame;
        cv::String window_name;

        int _gauss_blur_qty = 1, _max_gauss = 100;  // Trackbar parameters

        /*  Gaussian-blur trackbar callback method 
            and corresponding static redirection method 
            needed to get trackbar working from within a class.
        */
        static void onGausTrack(int val, void *ptr);
        void gauss_blur_callback(int val);

    public:

        /* Class constructor */
        GaussianBlurTrackbar(cv::Mat *infrm, const cv::String glugg_nafn);

        /*  Method called from main loop so that blurring 
            (of kernel-size according to trackbar value) 
            is performed on every captured camera frame. 
        */
        void gauss_blur(bool show_img = true);  
};


#endif