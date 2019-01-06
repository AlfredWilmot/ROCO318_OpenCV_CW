/*-------------------------------------------------------------------------------------------------------------------*/
/* Places an enclosing rectangle around the ROI contour, and tracks it's position by following the contour's CoM... */
/*-----------------------------------------------------------------------------------------------------------------*/
/*  1) First mouse click: contours are fitted to the HSV-thresholded input frame, the corresponding mask is generated.
    2) This mask is slightly enlarged (for better object tracking).
    3) The enlarged mask is then masked with the HSV-thresholded input frame (removing distant noise from ROI).
    4) The result of this masking is then morphologically opened in order to remove any noise within the mask that's adjacent to the ROI.
    5) This "final masked and processed image" is used as the basis of the mask for the next HSV-thresholded input frame...

        5a) The contours of the previous "final masked and processed image" 
            are used to generate the mask for the current HSV-thresholded input frame.
        5b) Repeat from step 2).
*/
/*-----------------------------------------------------------------------------------------------------------------*/

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>
#include <stdexcept>

#include "HsvThresholdTrackbar.hpp"


/* Some reference code: https://raw.githubusercontent.com/kylehounslow/opencv-tuts/master/object-tracking-tut/objectTrackingTut.cpp */
/* From here: https://www.youtube.com/watch?v=bSeFrPrqZ2A */

/*--------------------------------------------*/
/* Removing noise from HSV-thresholded image */
/*------------------------------------------*/
//reference here: https://docs.opencv.org/2.4/doc/tutorials/imgproc/opening_closing_hats/opening_closing_hats.html


/* Generates an image of detected rectangles from an binarized input image, by using contours */
class ContourRectangles : public HsvThresholdTrackbar
{
private:

    cv::String window_name;
    
    /* Return an error message if the matrix is not a binary image (needed for contour-fitting) */
    void errorHandling();


    /* Contour ROI stuff */
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    
    /* ROI Center of Mass */
    cv::Moments mu;
    cv::Point2f mc;

    /* ROI visual feedback */
    cv::Scalar const redDot = cv::Scalar(1,0,255);
    cv::Scalar const ROI_box = cv::Scalar(255,0,0);

    /* Used to determine contour that contains the seed pixel */
    int contains_seed = 0;

    /* Used for filtering noie after seed pixel has been selected */
    cv::Mat mask;
    cv::Mat masked_input;

    /* Perform morphological operations on the masked input frame */
    void morph();


    /*  Simple distance estimation based off of focal length calibration:
        https://stackoverflow.com/questions/6714069/finding-distance-from-camera-to-object-of-known-size
     */
    float calibration_card_heigth       = 21;      // cm    
    float calibration_card_width        = 29.7;    // cm
    int   calibration_card_pixel_height = 306;     //reference at 50cm from laptop camera:     306
    int   calibration_card_pixel_width  = 446;     //reference at 50cm from laptop camera:     446
    float calibration_distance          = 50;      // cm

    /*  Using focal-length calculation and similarity of triangles to estimate distance.
        Confounding variables:
        -> rotated rectangle (aliasing will cause edge lengths to cover fewer pixels compared to not being rotated) 
        -> rotation of target perpendicular to camera viewing axis will cause percieved change in aspect-ratio that will be incorrect */
    float focal_length       = float(calibration_card_pixel_width) * calibration_distance / calibration_card_width;

    float distance_estimate = 0.0;//calibration_card_width * focal_length / float(current_card_pixel_width);

    /* for implementing running avg for to filter out sporadic changes */
    float     avg_distance    = 0.0;
    int       avg_count       = 0;    
    const int count_limit     = 20;  // average over predetermined number of frames.


public:

    /* Class constructor */
    ContourRectangles(cv::Mat *infrm);

    /* Fits rectangle onto target contour and does some basic tracking using a ROI. HSV must be calibrated first!*/
    void FindRectangles();
};
