#ifndef __CANNYTHRESHOLDTRACKBAR__
#define __CANNYTHRESHOLDTRACKBAR__


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

#endif