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
Mat BGR_to_HSV;
//Mat RGB_to_HSV;

/* Using ptrs to dictate the filter that will be used on a particular matrix 
    -> Modify Mat address after assignment operator to achieve this         */
Mat *src = &BGR_to_HSV;
Mat erosion_dst;
Mat dilation_dst;





/*-------------*/
/* Blur stuff */
/*-----------*/
int blur_qty = 15;       //Initial size of blurring-kernel (15x15).
int const blur_max = 50; //Maximum size of blurring-kernel.

void doBlur(int, void * );
void BlurWithTrackbar(void);

/*-----------------------*/
/* Erode & Dilate stuff */
/*---------------------*/
// reference here: https://docs.opencv.org/3.4.2/db/df6/tutorial_erosion_dilatation.html
int erosion_elem = 0;
int erosion_size = 1;
int dilation_elem = 0;
int dilation_size = erosion_size;
int const max_elem = 2;
int const max_kernel_size = 21;

void doErode(int, void *);
void doDilate(int, void *);
void Erode_trackbars(void);
void Dilate_trackbars(void);

/*----------------*/
/* inRange stuff */ 
/*--------------*/
//reference here: https://docs.opencv.org/3.4/da/d97/tutorial_threshold_inRange.html
char Source_Feed[16] = "Original frame";
char Blur_Window[16] = "BlurWindow";
char Erode_Window[16] = "ErodeWindow";
char Dilate_Window[16] = "DilateWindow";
char HSV_thresh_window[16] = "HSV Threash";



/*----------------------*/
/* Blur Trackbar stuff */
/*--------------------*/
void doBlur(int sliderBarVal, void * = NULL)
{
    if (sliderBarVal <= 0)
    {
        sliderBarVal = 1;
    }
    /* Slidebar adjusts size of blurring kernel masked over input Mat pixels */
    blur(*src, Blur, Size(sliderBarVal, sliderBarVal));
    imshow(Blur_Window, Blur);
    printf("Blur kernel-size: %d x %d\n\r", sliderBarVal, sliderBarVal);
}
void BlurWithTrackbar()
{
    doBlur(blur_qty);
    imshow(Blur_Window, Blur);
    createTrackbar("Blur value", Blur_Window, &blur_qty, blur_max, doBlur);
}

/*-----------------------*/
/* Erode Trackbar stuff */
/*---------------------*/
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
    imshow(Erode_Window, erosion_dst);
}
void Erode_trackbars()
{
    //doErode(erosion_elem);
    //imshow(Erode_Window, erosion_dst);
    createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", Erode_Window, &erosion_elem, max_elem, doErode);
    createTrackbar("Kernel size:\n 2n +1", Erode_Window, &erosion_size, max_kernel_size, doErode);
}

/*-----------------------------*/
/* Dilate with Trackbar stuff */
/*---------------------------*/
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
    imshow(Dilate_Window, dilation_dst);
}
void Dilate_trackbars()
{
    //doDilate(dilation_elem);
    //imshow(Dilate_Window, dilation_dst);
    createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", Dilate_Window, &dilation_elem, max_elem, doDilate);
    createTrackbar("Kernel size:\n 2n +1", Dilate_Window, &dilation_size, max_kernel_size, doDilate);
}

/*-------------------------------------------------*/
/* Thresholding using inRange() in HSV colorspace */        
/*-----------------------------------------------*/
//reference here: https://docs.opencv.org/3.4/da/d97/tutorial_threshold_inRange.html

/* NOTE: MOST OF THE CODE FOR THIS SECTION IS A DIRECT COPY/PASTE */

// Create trackbars.
// Convert from BGR-to-HSV.
// Detect object based on HSV range values.

const int max_value_H = 360/2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;

Mat frame_HSV, frame_threshold;

static void on_low_H_thresh_trackbar(int low_H, void * = NULL)
{
    low_H = min(high_H-1, low_H);
    setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int high_H, void * = NULL)
{
    high_H = max(high_H, low_H+1);
    setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int low_S, void * = NULL)
{
    low_S = min(high_S-1, low_S);
    setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int high_S, void * = NULL)
{
    high_S = max(high_S, low_S+1);
    setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int low_V, void * = NULL)
{
    low_V = min(high_V-1, low_V);
    setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int high_V, void * = NULL)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("High V", window_detection_name, high_V);
}

void init_HSV_trackbars()
{
    // Trackbars to set thresholds for HSV values
    createTrackbar("Low H", HSV_thresh_window, &low_H, max_value_H, on_low_H_thresh_trackbar);
    createTrackbar("High H", HSV_thresh_window, &high_H, max_value_H, on_high_H_thresh_trackbar);
    createTrackbar("Low S", HSV_thresh_window, &low_S, max_value, on_low_S_thresh_trackbar);
    createTrackbar("High S", HSV_thresh_window, &high_S, max_value, on_high_S_thresh_trackbar);
    createTrackbar("Low V", HSV_thresh_window, &low_V, max_value, on_low_V_thresh_trackbar);
    createTrackbar("High V", HSV_thresh_window, &high_V, max_value, on_high_V_thresh_trackbar);
}
void run_HSV_thresh()
{
    // Convert from BGR to HSV colorspace
    cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
    // Detect the object based on HSV Range Values
    inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
    // Show the frames
    imshow(Source_Feed, frame);
    imshow(HSV_thresh_window, frame_threshold);
}

/*----------------------------------------------------------*/
/* Infering pixel color & (x,y) coordinates at mouse click */
/*--------------------------------------------------------*/
//reference here: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
//Converting from RGB to HSV (reference):

void update_HSV_range(int H, int S, int V)
{

    low_H = H-30;
    low_S = S-100;
    low_V = V-30;

    high_H = H+30;
    high_S = 255;
    high_V = 255;

    on_high_V_thresh_trackbar(high_V);
    on_low_V_thresh_trackbar(low_V);
    on_high_S_thresh_trackbar(high_S);
    on_low_S_thresh_trackbar(low_S);
    on_high_H_thresh_trackbar(high_H);
    on_low_H_thresh_trackbar(low_H);

    //run_HSV_thresh();

}
void mouseEvent(int evt, int x, int y, int flags, void* ) 
{                    

    if (evt == CV_EVENT_LBUTTONDOWN) 
    { 
        Vec3b rgb=frame.at<Vec3b>(y,x);
        int B=rgb.val[0];
        int G=rgb.val[1];
        int R=rgb.val[2];

        Mat HSV;
        Mat RGB=frame(Rect(x,y,1,1));   //Single-value matrix that is the pixel at point [x,y], RGB encoded by default.
        cvtColor(RGB, HSV,CV_BGR2HSV);

        Vec3b hsv=HSV.at<Vec3b>(0,0);
        int H=hsv.val[0];
        int S=hsv.val[1];
        int V=hsv.val[2];

        printf("[%d, %d] H:%d, S:%d, V:%d\n\r", 
                x, y, 
                H, S, V);


        /* update slider positions (with hysteresis) on mouse-click */
        update_HSV_range(H,S,V);
    }         
}



/* Some reference code: https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp */
/* From here: https://www.youtube.com/watch?v=bSeFrPrqZ2A */
int main(int, char **)
{
    VideoCapture cap(0); // open the default camera
    if (!cap.isOpened()) // check if we succeeded and return -1 if not
        return -1;

    /*------------------------------------------*/
    /* SETUP: Initialize trackbars and windows */
    /*----------------------------------------*/

    //namedWindow(Erode_Window);  
    //namedWindow(Dilate_Window);  
    namedWindow(Source_Feed);
    namedWindow(HSV_thresh_window);

    //cvtColor(frame, GrayScale, COLOR_BGR2GRAY); //Alters the frame data to gray and stores in matrix GrayScale
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

    Erode_trackbars();
    Dilate_trackbars();
    init_HSV_trackbars();

    //set the callback function for any mouse event
    setMouseCallback(Source_Feed, mouseEvent, &frame);

    while (1)
    {   

        cap >> frame; // get a new frame from video capture and store in matrix frame.

        cvtColor(frame, BGR_to_HSV, COLOR_BGR2HSV);
        //doErode(erosion_elem);
        //doDilate(dilation_elem);
        run_HSV_thresh();


        if (waitKey(30) >= 0)
        {
            destroyAllWindows();
            break;
        }
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
