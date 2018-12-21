#ifndef __CANNYTHRESHOLDTRACKBAR__
#define __CANNYTHRESHOLDTRACKBAR__


#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>


class CannyThresholdTrackbar
{
private:
    
    cv::Mat *_input_frame;
    cv::Mat *_output_frame;
    cv::String window_name;

    int _thresh = 3, _max_thresh = 200; // Canny Threshold Trackbar parameters

    static void onCannyTrack(int val, void *ptr);
    void canny_thresh_callback(int val);

public:
    CannyThresholdTrackbar(cv::Mat *infrm, cv::Mat *outfrm, const cv::String glugg_nafn);
    void canny_thresh();
};

#endif