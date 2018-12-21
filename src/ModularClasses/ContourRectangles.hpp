#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>
#include <stdexcept>




/* Generates an image of detected rectangles from an input image, by using contours */
class ContourRectangles
{
private:
    cv::Mat *input_frame;
    cv::Mat *output_frame;
    cv::Mat tmp;
    cv::String window_name;
    cv::Scalar contour_color = cv::Scalar(0,0,255);
    void errorHandling();

public:
    ContourRectangles(cv::Mat *infrm, cv::Mat *outfrm, cv::String glugg_nafn);
    void FindRectangles();

};
