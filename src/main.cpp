
/* OpenCV & Standard libraries */
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

/* user-made libraries */
#include "./ModularClasses/ContourRectangles.hpp"
#include "./ModularClasses/GaussianBlurTrackbar.hpp"
#include "./ModularClasses/CannyThresholdTrackbar.hpp"
#include "./ModularClasses/HsvThresholdTrackbar.hpp"
#include "./ModularClasses/ClickForPixelData.hpp"

using namespace cv;
using namespace std;

/* Stores captured camera frame */
Mat input_frame;

int main(int, char **)
{
    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened()) // check default camera is available, exit if not
    {
        printf("Default camera is unavailable!\n\r");
        return -1;
    }    

    /* Instantiate an object of our user-made class */
    ContourRectangles myRects(&input_frame);

    while(1)
    {
        cap >> input_frame; //Capture image from camera.

        /*  Saves a copy of the input frame so that the user can interact with an unadulterated version of it by clicking 
            on the interaction window */
        myRects.FrameToClick(input_frame);

        /*  Apply HSV thresholding and show processed image with some trackbars for parameter tweaks 
            Also attaches a trackbar for Gaussian blur. */
        myRects.run_HSV_thresh();

        /*  Places an enclosing rectangle around the ROI contour, 
            and tracks it's position by following the contour's CoM.
            The rectangle, enclosing contour, and it's CoM, are all
            drawn onto the aforementioned unadulterated frame that the user can interact with.*/
        myRects.FindRectangles();

        /* Exit loop and safely deallocate memory for display windows if ESC key is pressed */
        if (waitKey(10) == 27)
        {
            destroyAllWindows();
            break;
        }   
    }
    return 0;
}