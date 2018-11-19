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

/* Original camera frame */
Mat frame;

/* Various image processing techniques */
Mat GrayScale;
Mat Blur;
//Mat CannyEdit;
//Mat BGR_to_HSV;
//Mat RGB_to_HSV;

/* Using ptrs to dictate the filter that will be used on a particular matrix 
    -> Modify Mat address after assignment operator to achieve this         */
Mat *src = &GrayScale;
Mat erosion_dst;
Mat dilation_dst;

/* Blur stuff */
int blur_qty = 15;       //Initial size of blurring-kernel (15x15).
int const blur_max = 50; //Maximum size of blurring-kernel.

void doBlur(int, void * );
void BlurWithTrackbar(void);


/* Erode & Dilate stuff */
// reference here: https://docs.opencv.org/3.4.2/db/df6/tutorial_erosion_dilatation.html
int erosion_elem = 0;
int erosion_size = 1;
int dilation_elem = 0;
int dilation_size = erosion_size;
int const max_elem = 2;
int const max_kernel_size = 21;

void doErode(int, void *);
void doDilate(int, void *);
void ErodeWithTrackbars(void);
void DilateWithTrackbars(void);

char windowName0_[16] = "BlurWindow";
char windowName1_[16] = "ErodeWindow";
char windowName2_[16] = "DilateWindow";

/* Blur Trackbar stuff */
void doBlur(int sliderBarVal, void * = NULL)
{
    if (sliderBarVal <= 0)
    {
        sliderBarVal = 1;
    }
    /* Slidebar adjusts size of blurring kernel masked over input Mat pixels */
    blur(*src, Blur, Size(sliderBarVal, sliderBarVal));
    imshow(&windowName0_[0], Blur);
    printf("Blur kernel-size: %d x %d\n\r", sliderBarVal, sliderBarVal);
}

void BlurWithTrackbar()
{
    doBlur(blur_qty);
    imshow(&windowName0_[0], Blur);
    createTrackbar("Blur value", &windowName0_[0], &blur_qty, blur_max, doBlur);
}

/* Erode Trackbar stuff */
void doErode(int erosion_elem, void * = NULL)
{
    int erosion_type = 0;
    //int erosion_size = 1; /*TEST*/
    if (erosion_elem == 0)
    {
        erosion_type = MORPH_RECT;
    }
    else if (erosion_elem == 1)
    {
        erosion_type = MORPH_CROSS;
    }
    else if (erosion_elem == 2)
    {
        erosion_type = MORPH_ELLIPSE;
    }
    Mat element = getStructuringElement(erosion_type,
                                        Size(2 * erosion_size + 1, 2 * erosion_size + 1),
                                        Point(erosion_size, erosion_size));
    erode(*src, erosion_dst, element);
    imshow(&windowName1_[0], erosion_dst);
}


void ErodeWithTrackbars()
{
    doErode(erosion_elem);
    imshow(&windowName1_[0], erosion_dst);
    createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", &windowName1_[0], &erosion_elem, max_elem, doErode);
    createTrackbar("Kernel size:\n 2n +1", &windowName1_[0], &erosion_size, max_kernel_size, doErode);
}

/* Dilate with Trackbar stuff */
void doDilate(int dilation_elem, void * = NULL)
{
    int dilation_type = 0;
    //int dilation_size = 1;  /*TEST*/
    if (dilation_elem == 0)
    {
        dilation_type = MORPH_RECT;
    }
    else if (dilation_elem == 1)
    {
        dilation_type = MORPH_CROSS;
    }
    else if (dilation_elem == 2)
    {
        dilation_type = MORPH_ELLIPSE;
    }
    Mat element = getStructuringElement(dilation_type,
                                        Size(2 * dilation_size + 1, 2 * dilation_size + 1),
                                        Point(dilation_size, dilation_size));
    dilate(*src, dilation_dst, element);
    imshow(&windowName2_[0], dilation_dst);
}

void DilateWithTrackbars()
{
    doDilate(dilation_elem);
    imshow(&windowName2_[0], dilation_dst);
    createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", &windowName2_[0], &dilation_elem, max_elem, doDilate);
    createTrackbar("Kernel size:\n 2n +1", &windowName2_[0], &dilation_size, max_kernel_size, doDilate);
}

/* Some reference code: https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp */
/* From here: https://www.youtube.com/watch?v=bSeFrPrqZ2A */
int main(int, char **)
{
    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened()) // check if we succeeded and return -1 if not
        return -1;

    while (1)
    {
        cap >> frame; // get a new frame from video capture and store in matrix frame

        cvtColor(frame, GrayScale, COLOR_BGR2GRAY); //Alters the frame data to gray and stores in matrix GrayScale
        //GaussianBlur(GrayScale, Blur, Size(7, 7), 1.5, 1.5); //Alters the GrayScale data to have guassian blur and stores in matrix Blur
        //Canny(Blur, CannyEdit, 0, 30, 3);                    //Updates the altered Blur data to have canny lines and stores in matrix canny edit
        //cvtColor(frame, RGB_to_HSV, COLOR_RGB2HSV);
        //cvtColor(frame, BGR_to_HSV, COLOR_BGR2HSV);

        //imshow("Stdfeed", frame);       //Opens a window called feed, containing the camera frame matrix data
        //imshow("GrayScale", GrayScale); //Opens a window called feed, containing the camera frame matrix data
        //imshow("Blur", Blur);           //Opens a window called feed, containing the camera frame matrix data
        //imshow("CannyEdit", CannyEdit); //Opens a window called feed, containing the camera frame matrix data
        //imshow("RGB_to_HSV", RGB_to_HSV);
        //imshow("BGR_to_HSV", BGR_to_HSV);

        //BlurWithTrackbar();
        ErodeWithTrackbars();
        DilateWithTrackbars();

        if (waitKey(30) >= 0)
        {
            destroyAllWindows();
            break;
        }
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
