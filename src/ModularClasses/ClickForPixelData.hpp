/*  Class object is used to access pixel information of an input matrix.
    Can be used by other classes to designate a ROI based around a selected seed-pixel.
*/ 
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>
#include <stdexcept>

#ifndef __CLICKFORPIXELDATA__
#define __CLICKFORPIXELDATA__




class ClickForPixelData
{
    private:

        /* I/O frame and window */
        cv::Mat _frm_to_clk;
        cv::String display_window;


        /* Trackbar determines if pixel HSV is ignored or not */
        void captureHSV(int val);
        static void onCaptureHSV(int val, void* ptr);
        int capture_hsv = 0;

    protected:
        /* Last known pixel HSV values */
        int H = 0;
        int S = 0;
        int V = 0;

        /*  Trained HSV range: user selects pixels manually to iteratively improve tracking of color being trained to
            within a given environment */
            
        /* Thresholding variables */
        const int max_value_H = 180;
        const int max_value = 255;
        int low_H = 0, low_S = 0, low_V = 0;
        int high_H = this->max_value_H;
        int high_S = this->max_value, high_V = this->max_value;

        /* Indicate if next pixel selections are used to train based off an initially selected value */
        bool train_target_HSV = false;


        /* Mouse-click event stuff */
        bool _mouse_clk = false;                // Want flag to persist until all events related to a mouse click have transpired
        int _seed_x = 0;
        int _seed_y = 0; 

        /* Update H, S, V variables with those corresponding to the pixel selected (if the mouse has been clicked recently).*/
        int get_seed_pixel_hsv(bool clear_mouse_clk = true);


        /* Mouse event handling methods */
        static void onMouseEvt(int evt, int x, int y, int flags, void* ptr);
        int mouseEvent(int evt, int x, int y, int flags);


    public:
        ClickForPixelData(cv::String glugg_nafn);
        /* Frame whose pixel HSV values will be checked when the user clicks on it */
        void FrameToClick(cv::Mat clk_frm);
};


#endif
