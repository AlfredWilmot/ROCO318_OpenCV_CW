
/* OpenCV & Standard libraries */
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

/* user-made libraries */
#include "RectDetect1.hpp"

using namespace cv;
using namespace std;

Mat input_frame;
Mat output_frame;


char pre_process_window[32]  = "Preprocess";
char post_process_window[32] = "Postprocess";



int main(int, char **)
{
    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened()) // check if we succeeded and return -1 if not
    {
        return -1;
    }    

    /* Instantiate a RectDetect1 obj, pass the I/O frame ptrs*/
    RectDetect1 myRect(&input_frame, &output_frame);

    while(1)
    {
        cap >> input_frame; // get a new frame from video capture and store in matrix frame.

        myRect.show_input_frames();
        myRect.gauss_blur();
        myRect.HSV_binarization();
        //myRect.show_output_frames();

        if (waitKey(10) == 27)
        {
            destroyAllWindows();
            break;
        }   
    }
    
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}