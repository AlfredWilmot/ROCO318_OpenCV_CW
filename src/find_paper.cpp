// testPFC.cpp : This file contains the 'main' function. Program execution begins and ends there.
// the contours2.cpp samplke c++ code is reproduced here
// RUN and adjust slider
// Understand how it works.

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
char pre_process_window[16]  = "Preprocess";
char post_process_window[16] = "Postprocess";


/* Some reference code: https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp */
/* From here: https://www.youtube.com/watch?v=bSeFrPrqZ2A */
int main(int, char **)
{
    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened()) // check if we succeeded and return -1 if not
    {
        return -1;
    }    

    /* Initializing viewing windows */   
    namedWindow(pre_process_window);
    namedWindow(post_process_window);

    /* Initialize image processing trackbars */
    init_HSV_trackbars(); 
    //init_canny_trackbar();
    init_morph_ops_trackbars();


    while(1)
    {
        cap >> input_frame; // get a new frame from video capture and store in matrix frame.

        
        imshow(pre_process_window, input_frame); //show the captured frame in the relevant display window.

        //Apply Gaussian blur.
        GaussianBlur(input_frame, hsv_thresh_input_frame, Size(7, 7), 1.5, 1.5);

        //Apply HSV thresholding.
        run_HSV_thresh();
        
        //Apply morphological operations.
        morph_input_frame = hsv_thresh_output_frame;
        run_morph();
        
        //Apply Canny Contour detection.
        //Canny( morph_output_frame, canny_output_frame, 50, 150, 3 );
        

        output_frame = morph_output_frame;;
        
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
