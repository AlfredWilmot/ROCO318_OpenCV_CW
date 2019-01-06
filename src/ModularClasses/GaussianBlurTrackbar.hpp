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
        
        cv::Mat *_input_frame;
        cv::String window_name;

        int _gauss_blur_qty = 1, _max_gauss = 100;  // Trackbar parameters

        static void onGausTrack(int val, void *ptr);
        void gauss_blur_callback(int val);

    public:
        GaussianBlurTrackbar(cv::Mat *infrm, const cv::String glugg_nafn);
        void AddGaussTrackbar();
        void gauss_blur(bool show_img = true);
};


#endif