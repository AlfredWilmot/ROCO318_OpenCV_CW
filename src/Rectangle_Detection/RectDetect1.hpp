#ifndef __RECTDETECT1__
#define __RECTDETECT1__


#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>
#include <unistd.h>
#include <stdexcept>


using namespace cv;
using namespace std;


 




class RectDetect1
{
private:

    /* Class I/O */
    static Mat *_input_frame;
    static Mat *_output_frame;
    const String window_cam = "Camera Feed";

    static Size input_frame_size;

    /* Gaussian blur*/
    static Mat *gauss_input_frame;
    static Mat gauss_output_frame;
    const String gauss_display_window = "Gaussian Blur";

    int _gauss_blur_qty = 3, _max_gauss = 100;  // Trackbar parameters

    /* HSV thresholding */
    static Mat *hsv_thresh_input_frame;
    static Mat hsv_thresh_output_frame;
    const String hsv_display_window = "HSV Threshold";

    /* Contouring */
    static Mat contour_input_frame;
    static Mat contour_output_frame;
    const String contour_display_window = "Contours";

    /* Morphological operations that use Dilate/ Erode */
    static Mat morph_input_frame;
    static Mat morph_output_frame;
    const String morph_display_window = "Mophological Ops";


    /* Canny edge-detection */
    static Mat canny_input_frame;
    static Mat canny_output_frame;
    const String canny_display_window = "Canny";

    

    /* Mouse-click event stuff */
    static bool _mouse_clk;
    static int _seed_x;
    static int _seed_y; 

    /* ROI Hue rang-finding parameters */
    static int seed_x_offset;
    static int seed_y_offset;

    static int H;
    static int S;
    static int V;


    static int ROI_HSV[3];
    static int ROI_H_max;
    static int ROI_H_min;

    static int x_left;
    static int x_right;
    static int y_up;
    static int y_down;

    static bool up_left_done;
    static bool down_left_done;
    static bool down_right_done;
    static bool up_right_done;


    int find_Hue_range(); // Method is private.


public:

    /* Constructor method */
    RectDetect1(Mat *infrm, Mat *outfrm);

    /*Show frames from camera feed */
    void show_input_frames();

    /* Trackbar initialization */
    void trackbar_init();

    /* Trackbar callback methods and redirection to get it working: https://stackoverflow.com/questions/28418064/casting-c-class-into-another-one-to-create-opencv-trackbar-handle*/
    static void onGausTrack(int val, void *ptr);
    void gauss_blur_callback(int val);


    /* Mouse event handling methods */
    int get_xy_pixel_hsv(int x, int y);
    static void onMouseEvt(int evt, int x, int y, int flags, void* ptr);
    int mouseEvent(int evt, int x, int y, int flags);
    


    /* Processing methods */
    void gauss_blur();
    
    bool update_thresh(int x_dir, int y_dir);

    void HSV_binarization();


};



#endif
