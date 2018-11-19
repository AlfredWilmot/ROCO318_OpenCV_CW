// testPFC.cpp : This file contains the 'main' function. Program execution begins and ends there.
// the contours2.cpp samplke c++ code is reproduced here
// RUN and adjust slider
// Understand how it works.

//#include "pch.h"
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;


/* Functions defined here */
void TrackbarCallBack_Blur(int, void*);
void doBlur(int);

/* Original camera frame */
Mat frame;

/* Various image processing techniques */
Mat GrayScale;
Mat Blur;
Mat CannyEdit;
Mat BGR_to_HSV;
Mat RGB_to_HSV;

/* Using ptrs to dictate the filter that will be used on a particular matrix 
    -> Modify Mat address after assignment operator to achieve this     */
Mat *src_blur = &GrayScale;


int blur_qty = 15;  //Initial size of blurring-kernel (15x15).
int blur_max = 50;  //Maximum size of blurring-kernel.

char windowName[16] = "BlurWindow";

void TrackbarCallBack_Blur(int sliderBarVal, void *)
{
    doBlur(sliderBarVal);
    printf("Blur kernel-size: %d x %d\n\r", sliderBarVal, sliderBarVal);

}

void doBlur(int sliderBarVal)
{
    if (sliderBarVal <=0 )
    {
        sliderBarVal = 1;
    }
    /* Slidebar adjusts size of blurring kernel masked over input Mat pixels */
    blur(*src_blur, Blur, Size(sliderBarVal, sliderBarVal));
    imshow(&windowName[0], Blur);
}



/* Some reference code: https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp */
/* From here: https://www.youtube.com/watch?v=bSeFrPrqZ2A */
int main(int, char **)
{
    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened()) // check if we succeeded and return -1 if not
        return -1;
    

    while(1)
    {
        cap >> frame; // get a new frame from video capture and store in matrix frame

        cvtColor(frame, GrayScale, COLOR_BGR2GRAY);          //Alters the frame data to gray and stores in matrix GrayScale
        //GaussianBlur(GrayScale, Blur, Size(7, 7), 1.5, 1.5); //Alters the GrayScale data to have guassian blur and stores in matrix Blur
        //Canny(Blur, CannyEdit, 0, 30, 3);                    //Updates the altered Blur data to have canny lines and stores in matrix canny edit
        cvtColor(frame, RGB_to_HSV, COLOR_RGB2HSV);
        //cvtColor(frame, BGR_to_HSV, COLOR_BGR2HSV);

        //imshow("Stdfeed", frame);       //Opens a window called feed, containing the camera frame matrix data
        //imshow("GrayScale", GrayScale); //Opens a window called feed, containing the camera frame matrix data
        //imshow("Blur", Blur);           //Opens a window called feed, containing the camera frame matrix data
        //imshow("CannyEdit", CannyEdit); //Opens a window called feed, containing the camera frame matrix data
        //imshow("RGB_to_HSV", RGB_to_HSV);
        //imshow("BGR_to_HSV", BGR_to_HSV);
        //Polling for a key (probaby in a thread) to close program once pressed (or break the feed loop)
        doBlur(blur_qty);
        imshow(&windowName[0], Blur);
        createTrackbar("Blur value", &windowName[0], &blur_qty, blur_max, TrackbarCallBack_Blur);

        if (waitKey(30) >= 0)
        {
            destroyAllWindows();
            break;
        }   
    }
    
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
