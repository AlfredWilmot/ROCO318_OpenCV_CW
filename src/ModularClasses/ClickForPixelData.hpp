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
    protected:
        /* Last known pixel HSV values */
        int H;
        int S;
        int V;

        /*  Trained HSV range: user selects pixels manually to iteratively improve tracking of color being trained to
            within a given environment */
            
        /* Thresholding variables */
        const int max_value_H = 180;
        const int max_value = 255;
        int low_H = 0, low_S = 0, low_V = 0;
        int high_H = this->max_value_H;
        int high_S = this->max_value, high_V = this->max_value;
        // Add hysteresis (ignore outlier values)


        /* Mouse-click event stuff */
        bool _mouse_clk;
        int _seed_x = 0;
        int _seed_y = 0; 

        /* Mouse event handling methods */
        static void onMouseEvt(int evt, int x, int y, int flags, void* ptr);
        int mouseEvent(int evt, int x, int y, int flags);

        int get_seed_pixel_hsv();


    public:
        ClickForPixelData(cv::String glugg_nafn);
        /* Frame whose pixel HSV values will be checked when the user clicks on it */
        void FrameToClick(cv::Mat clk_frm);
};


#endif
