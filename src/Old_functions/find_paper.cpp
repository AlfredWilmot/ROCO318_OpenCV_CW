#include "find_paper.hpp"

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat input_frame;
Mat output_frame;


char pre_process_window[32]  = "Preprocess";
char post_process_window[32] = "Postprocess";


/* Some reference code: https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp */
/* From here: https://www.youtube.com/watch?v=bSeFrPrqZ2A */
int main(int, char **)
{
    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened()) // check if we succeeded and return -1 if not
    {
        return -1;
    }    

/* Initializing Main I/O viewing windows */   
    namedWindow(pre_process_window);
    namedWindow(post_process_window);
    

/*-- Initialize new windows & image processing trackbars, where applicable (reuse old windows using strcpy(src,dst)) --*/

/*HSV*/
    //namedWindow(hsv_display_window);
    strcpy(hsv_display_window, post_process_window);
    init_HSV_trackbars(); 
/*MORPH*/
    //namedWindow(morph_display_window);
    strcpy(morph_display_window, post_process_window);
    init_morph_ops_trackbars();
/*CANNY*/
    namedWindow(canny_display_window);
    init_canny_trackbar();
/*CONTOURS*/
    //namedWindow(contour_display_window);
    strcpy(contour_display_window, post_process_window);







    while(1)
    {
        cap >> input_frame; // get a new frame from video capture and store in matrix frame.

    /*show the captured frame.*/
        imshow(pre_process_window, input_frame); 

    
    /*Apply HSV thresholding.*/
        hsv_thresh_input_frame = input_frame;
        run_HSV_thresh();
        output_frame = hsv_thresh_output_frame;

    /*Apply morphological operations.*/
        morph_input_frame = output_frame;
        run_morph();
        output_frame = morph_output_frame;
    
    /*Apply Canny edge-detection.*/
        canny_input_frame = input_frame;
        apply_Canny();
        imshow(canny_display_window, canny_output_frame);

    /*Apply Contour-fitting.*/
        //contour_input_frame = output_frame;
        //apply_Contours();
        //output_frame = contour_output_frame;
    
    /* Display processed frames in available windows.*/
        imshow(morph_display_window, morph_output_frame);
        imshow(post_process_window, output_frame);



        if (waitKey(30) >= 0)
        {
            destroyAllWindows();
            break;
        }   
    }
    
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
