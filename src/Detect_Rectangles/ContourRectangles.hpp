#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;




/* Generates an image of detected rectangles from an input image, by using contours */
class ContourRectangles
{
private:
    Mat *input_frame;
    Mat *output_frame;
    Mat tmp;
    const String contour_window = "Contour fit rectangles";
    Scalar contour_color = Scalar(255,255,255);

public:
    ContourRectangles(Mat *infrm, Mat *outfrm);
    void FindRectangles();

};

/* Class constructor */
ContourRectangles::ContourRectangles(Mat *infrm, Mat *outfrm)
{
    input_frame   = infrm;
    output_frame  = outfrm;

    namedWindow(contour_window);
}


void ContourRectangles::FindRectangles()
{
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    cvtColor( *input_frame, tmp, CV_BGR2GRAY );
    blur( tmp, tmp, Size(3,3) );
    Canny( tmp, tmp, 100, 100*2, 3 );

    findContours(*output_frame, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    vector<RotatedRect> minRect( contours.size() );

    *output_frame = Mat::zeros(tmp.size(), CV_8UC3 );


    for( size_t i = 0; i< contours.size(); i++ )
    {
        /* Defining bounding box for a given contour */
        minRect[i] = minAreaRect( contours[i] );
        // rotated rectangle
        Point2f rect_points[4];
        minRect[i].points( rect_points );
        for ( int j = 0; j < 4; j++ )
        {
            line( *output_frame, rect_points[j], rect_points[(j+1)%4], contour_color );
        }
        // Draw contours...
        drawContours( *output_frame, contours, (int)i, contour_color );
    }

    /* Display processed image */
    imshow(contour_window, *output_frame);
}

