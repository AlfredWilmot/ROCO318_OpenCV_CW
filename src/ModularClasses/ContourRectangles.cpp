#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>
#include <stdexcept>


using namespace cv;
using namespace std;

#include "ContourRectangles.hpp"

/* Class constructor */
ContourRectangles::ContourRectangles(Mat *infrm, Mat *outfrm, String glugg_nafn)
{

    this->input_frame   = infrm;
    this->output_frame  = outfrm;
    this->window_name   = glugg_nafn;
    namedWindow(this->window_name);
}


void ContourRectangles::FindRectangles()
{
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    
    /* Return an error message if the matrix is not a binary image (needed for contour-fitting)*/

    if(input_frame->empty())
    {
        throw std::invalid_argument( "Input frame is empty!");
    }
    
    /* Ensure that the input matrix is valid before continuing code execution */
    this->errorHandling();

    findContours(*this->input_frame, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    vector<RotatedRect> minRect( contours.size() );

    this->tmp = Mat::zeros(input_frame->size(), CV_8UC3 );


    for( size_t i = 0; i< contours.size(); i++ )
    {
        /* Defining bounding box for a given contour */
        minRect[i] = minAreaRect( contours[i] );
        // rotated rectangle
        Point2f rect_points[4];
        minRect[i].points( rect_points );
        for ( int j = 0; j < 4; j++ )
        {   /* Traces-out the lines of each rectangle */
            line( *output_frame, rect_points[j], rect_points[(j+1)%4], contour_color, 2);
        }
        // Draw contours...
        //drawContours( *output_frame, contours, (int)i, contour_color );
    }

    /* Display processed image */
    imshow(this->window_name, *output_frame);
}


void ContourRectangles::errorHandling()
{
    if(input_frame->empty())
    {
        throw std::invalid_argument( "Input frame is empty!");
    }
    else if(input_frame->channels() != 1)
    {
        printf("input frame has %d channels.\n\r", input_frame->channels());
        throw std::invalid_argument( "Input frame must be single channel.\n\r");
    }
}