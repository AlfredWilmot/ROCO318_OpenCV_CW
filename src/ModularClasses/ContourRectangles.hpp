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

    cv::Moments mu;
    cv::Point2f mc;

    cv::Scalar const redDot = cv::Scalar(1,0,255);
    cv::Scalar ROI_box = cv::Scalar(255,0,0);

    int contains_seed = 0;
    

public:
    ContourRectangles(cv::Mat *infrm, cv::Mat *outfrm, cv::String glugg_nafn);
    void FindRectangles();

};
