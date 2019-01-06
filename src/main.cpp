
/* OpenCV & Standard libraries */
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

/* user-made libraries */
//#include "RectDetect1.hpp"
//#include "./ModularClasses/ContourRectangles.hpp"
#include "./ModularClasses/GaussianBlurTrackbar.hpp"
#include "./ModularClasses/CannyThresholdTrackbar.hpp"
#include "./ModularClasses/HsvThresholdTrackbar.hpp"
#include "./ModularClasses/ClickForPixelData.hpp"

using namespace cv;
using namespace std;

Mat input_frame;

int main(int, char **)
{
    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened()) // check if we succeeded and return -1 if not
    {
        return -1;
    }    

    //ContourRectangles       myRects(&input_frame, "Contor Window");
    //ClickForPixelData       test("Click me!");
    HsvThresholdTrackbar    test(&input_frame, "HSV Thresholding!");
    while(1)
    {
        cap >> input_frame; //Capture image from camera.

        /* Store original camera frame so rectangle can be shown over it */
        /* Also used to designate the frame for mouse-click pixel selection */
        test.FrameToClick(input_frame);

        /* Apply HSV thresholding and show processed image with some trackbars for parameter tweaks */
        test.run_HSV_thresh();

        /*  Detect largest contour enclosing seed-pixel, 
            infer CoM of resulting contour (assign as new "seed_pixel"),
            Draw rectangle around contour and use seed_pixel as it's center point
            ... TBC */
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