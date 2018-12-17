#ifndef __RECTDETECT1__
#define __RECTDETECT1__


#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>
#include <unistd.h>



using namespace cv;
using namespace std;


 




class RectDetect1
{
private:

    /* Class I/O */
    static Mat *_input_frame;
    static Mat *_output_frame;

    /* Gaussian blur params */
    int _gauss_blur_qty = 3, _max_gauss = 100;

    /* Window names */
    const String window_cam = "Camera Feed";
    const String window_gauss_name = "Gaussian Blur";

    /* Mouse-click event stuff */
    static bool _mouse_clk;
    static int _seed_x;
    static int _seed_y; 

    /* Mask parameters */
    static int left_step;
    static int right_step;
    static int down_step;
    static int up_step;

    static int H;
    static int S;
    static int V;


    static int ROI_HSV[3];
    static int count_H;
    static int sum_H;


    static int x_left;
    static int x_right;
    static int y_up;
    static int y_down;

    static bool left_side_done;
    static bool top_side_done;
    static bool right_side_done;
    static bool bottom_side_done;

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
    void get_mask();
};



#endif
