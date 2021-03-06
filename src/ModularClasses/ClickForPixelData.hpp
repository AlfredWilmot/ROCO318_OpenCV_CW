/*--------------------------------------------------------------------------------------*/
/*  Class object is used to access pixel information of an input matrix.
    Can be used by other classes to designate a ROI based around a selected seed-pixel. */ 
/*--------------------------------------------------------------------------------------*/
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>
#include <stdexcept>

#ifndef __CLICKFORPIXELDATA__
#define __CLICKFORPIXELDATA__


/*----------------------------------------------------------*/
/* Infering pixel color & (x,y) coordinates at mouse click */
/*--------------------------------------------------------*/
//reference here: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
//Converting from RGB to HSV (reference): oh dear...

/*-------------------------------------------------------------------------------------------------------------*/
/* Trackbar callback methods and redirection to get it working: 
https://stackoverflow.com/questions/28418064/casting-c-class-into-another-one-to-create-opencv-trackbar-handle 

"createTrackbar() requires address of a static function. 
'this' ptr is used as an arg in the actual trackbar instantiation, 
and is later resolved to the class instance."" */
/*-------------------------------------------------------------------------------------------------------------*/

class ClickForPixelData
{
    private:


        /* Trackbar determines if pixel HSV is ignored or not */
        void captureHSV(int val);
        static void onCaptureHSV(int val, void* ptr);

    protected:

        /* Useful for subclasses to see if user wants to update seed HSV */
        int capture_hsv = 1;

        /* Interactive window*/
        cv::String click_display_window;


        /* Frame displayed onto the interactive window*/
        // THIS MUST BE ASSIGNED BEFORE PERFORMING ANY OTHER OPERATION ON INPUT IMAGE 
        cv::Mat _frm_to_clk;
        cv::Mat _frm_to_check;

        /* Last known pixel HSV values */
        int H = 0;
        int S = 0;
        int V = 0;
            
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
        /* Frame whose pixel coordinates will be used when the user clicks on it */
        void FrameToClick(cv::Mat clk_frm);
        /* The coordinates selected by user are used to check the HSV of the corresponding pixel coordinates
            in the frame passed to FrameToCheck (so frame can be processed externally to average HSV values via blurring operation
            prior to pixel HSV inspection, but the user can directly interact with an unadulterated version of the input frame).*/
        void FrameToCheck(cv::Mat chk_frm);
};


#endif
