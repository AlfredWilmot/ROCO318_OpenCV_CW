#ifndef __GAUSSIANBLURTRACKBAR__
#define __GAUSSIANBLURTRACKBAR__

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;




class GaussianBlurTrackbar
{
private:
    
    Mat *_input_frame;
    Mat *_output_frame;
    String window_name;

    int _gauss_blur_qty = 3, _max_gauss = 100;  // Trackbar parameters

    static void onGausTrack(int val, void *ptr);
    void gauss_blur_callback(int val);

public:
    GaussianBlurTrackbar(Mat *infrm, Mat *outfrm, const String glugg_nafn);
    void AddGaussTrackbar();
    void gauss_blur();
};


#endif