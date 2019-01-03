#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>
#include <stdexcept>


#include "ClickForPixelData.hpp"

/* Generates an image of detected rectangles from an input image, by using contours */
class ContourRectangles : public ClickForPixelData
{
private:
    cv::Mat *input_frame;
    cv::Mat *output_frame;
    cv::String window_name;

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

    //int current_card_pixel_width  = 1;
    //int current_card_pixel_height = 1;

    float distance_estimate = 0.0;//calibration_card_width * focal_length / float(current_card_pixel_width);

    /* for implementing running avg for to filter out sporadic changes */
    float     avg_distance    = 0.0;
    int       avg_count       = 0;    
    const int count_limit     = 20;  // average over predetermined number of frames.


public:
    ContourRectangles(cv::Mat *infrm, cv::Mat *outfrm, cv::String glugg_nafn);
    void FindRectangles();

};
