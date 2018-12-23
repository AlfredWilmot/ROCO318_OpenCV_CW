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
ContourRectangles::ContourRectangles(Mat *infrm, Mat *outfrm, String glugg_nafn): ClickForPixelData(infrm, outfrm, glugg_nafn) 
{

    this->input_frame   = infrm;
    this->output_frame  = outfrm;
    this->window_name   = glugg_nafn;
    //namedWindow(this->window_name);
}


void ContourRectangles::FindRectangles()
{
    /* Clear contours list from last frame */
    this->contours.clear();
    
    /* Ensure that the input/ output matricies are valid before continuing*/
    this->errorHandling();

    findContours(*this->input_frame, this->contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    vector<RotatedRect> minRect( this->contours.size() );


    this->get_seed_pixel_hsv();


    for( size_t i = 0; i< this->contours.size(); i++ )
    {
        /* Only draw bounding boxes once the seed has been defined by ther first mouse-click */
        if(this->_seed_x && this->_seed_y)
        {
            /* Draw the bounding box, ie. the ROI, that contains the seed */
            contains_seed = pointPolygonTest(this->contours[i], Point2f(this->_seed_x, this->_seed_y), false);
            if(contains_seed == 1)
            {
                /* Calculating ROI CoM */
                mu = moments( this->contours[i], false );
                mc = Point2f( static_cast<float>(mu.m10/mu.m00) , static_cast<float>(mu.m01/mu.m00) );
               
                /* Drawing CoM */
                circle(*this->output_frame, mc, 4, redDot, -1, 8, 0 );
                /* Update seed location to that of CoM to track ROI*/
                this->_seed_x = int(this->mc.x);
                this->_seed_y = int(this->mc.y);   

                /* Defining bounding box for a given contour */
                minRect[i] = minAreaRect( this->contours[i] );
                
                // rotated rectangle
                Point2f rect_points[4];

                minRect[i].points( rect_points );
                for ( int j = 0; j < 4; j++ )
                {   /* Traces-out the lines of each rectangle */
                    line( *output_frame, rect_points[j], rect_points[(j+1)%4], this->ROI_box, 2);
                }
                //Draw contours...
                drawContours( *output_frame, this->contours, (int)i, this->ROI_box );



                /* Mask this ROI onto the value at the input image address, 
                BEFORE calculating the next ROI:
                    -> generate the first instance of the mask here.
                    -> then apply it to every incoming frame (before generating it's own ROI), once it exists (i.e. it's matrix isn't empty).
                */


            }
        }
    }

    /* Display processed image */
    imshow(this->window_name, *output_frame);
}



/* Return an error message if the matrix is not a binary image (needed for contour-fitting)*/
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

    /* Need to fill the output frame before inserting contours/ rectangles, if it's empty */
    if(this->output_frame->empty())
    {
        *this->output_frame = Mat::zeros( _input_frame->size(), CV_8UC3 );
    }
}
