
/* OpenCV & Standard libraries */
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

/* user-made libraries */
//#include "RectDetect1.hpp"
#include "./ModularClasses/ContourRectangles.hpp"
#include "./ModularClasses/GaussianBlurTrackbar.hpp"
#include "./ModularClasses/CannyThresholdTrackbar.hpp"
#include "./ModularClasses/HsvThresholdTrackbar.hpp"
using namespace cv;
using namespace std;

Mat input_frame;
Mat output_frame;


char pre_process_window[32]  = "Preprocess";
const String contour_window  = "Contor Window";
const String hsv_window  = "HSV Window";

int main(int, char **)
{
    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened()) // check if we succeeded and return -1 if not
    {
        return -1;
    }    

    /* Instantiate a RectDetect1 obj, pass the I/O frame ptrs*/
    //RectDetect1 myRect(&input_frame, &output_frame);

    namedWindow(pre_process_window);

    //GaussianBlurTrackbar   myGaussObj(&input_frame, &output_frame, hsv_window);
    //CannyThresholdTrackbar myCannyObj(&output_frame, &output_frame, hsv_window);
    HsvThresholdTrackbar   myHsvObj(&input_frame, &output_frame, hsv_window);
    //ContourRectangles myRects(&input_frame, &output_frame, contour_window);
    while(1)
    {
        cap >> input_frame; // get a new frame from video capture and store in matrix frame.

        imshow(pre_process_window, input_frame);
        // myRect.show_input_frames();
        // myRect.gauss_blur();
        // myRect.HSV_binarization();
        
        //myGaussObj.gauss_blur();
        myHsvObj.run_HSV_thresh();
        //myCannyObj.canny_thresh();

        //myRects.FindRectangles();

        if (waitKey(10) == 27)
        {
            printf("%d\n\r", input_frame.channels());
            destroyAllWindows();
            break;
        }   
    }
    
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}