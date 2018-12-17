#ifndef __RECTDETECT1__
#define __RECTDETECT1__


#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;


 




class RectDetect1
{
private:

    /* Class I/O */
    static Mat *_input_frame;
    Mat *_output_frame;

    /* Gaussian blur params */
    int _gauss_blur_qty = 3, _max_gauss = 100;

    /* Window names */
    const String window_cam = "Camera Feed";
    const String window_gauss_name = "Gaussian Blur";

    /* Mouse-click event stuff */
    bool _mouse_clk = false;
    static int _seed_x;
    static int _seed_y; 
 


public:

    /* Constructor method */
    RectDetect1(Mat *infrm_ptr, Mat *outfrm_ptr);

    /*Show frames from camera feed */
    void show_input_frames();
    /* Show processed frames */
    void show_output_frames();

    /* Trackbar initialization */
    void trackbar_init();

    /* Trackbar callback methods and redirection to get it working: https://stackoverflow.com/questions/28418064/casting-c-class-into-another-one-to-create-opencv-trackbar-handle*/
    static void onGausTrack(int val, void *ptr);
    void gauss_blur_callback(int val);


    /* Mouse event handling methods */
    void get_xy_pixel_hsv(int x, int y);
    static void onMouseEvt(int evt, int x, int y, int flags, void* ptr);
    void mouseEvent(int evt, int x, int y, int flags);

    /* Processing methods */
    void gauss_blur();
};



#endif
