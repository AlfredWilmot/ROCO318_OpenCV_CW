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

    this->masked_input = Mat(this->input_frame->size(), CV_8UC3);

    //namedWindow(this->window_name);
}


void ContourRectangles::FindRectangles()
{

 /* Clear contours list from last frame */
    this->contours.clear();
    
    /* Ensure that the input/ output matricies are valid before continuing*/
    this->errorHandling();


    /* Whenever a new pixel is selected use the vanialla input frame instead of the previous mased frame */
    if(this->get_seed_pixel_hsv() == 0)
    {
         findContours(*this->input_frame, this->contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    }
 
    //this->get_seed_pixel_hsv();

    /* New mask for a new input frame */
    Mat mask(this->input_frame->size(), CV_8UC1, Scalar(0,0,0));

    /* Only start drawing bounding boxes once the seed has been defined by the first mouse-click */
    if(this->_seed_x && this->_seed_y)
    {   
        
        /* Contours from previously masked input guide behaviour of current frame mask */
        if(this->contours.empty())
        {
            findContours(this->masked_input, this->contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); 
        }

        
        // Mat tmp = this->input_frame->clone();
        // findContours(tmp, this->contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        /* Generate rotated-rectangle ROI, from contours */
        vector<RotatedRect> minRect( this->contours.size() );

        for( size_t i = 0; i< this->contours.size(); i++ )
        {
            /* Draw the bounding box, ie. the ROI, that contains the seed */
            contains_seed = pointPolygonTest(this->contours[i], Point2f(this->_seed_x, this->_seed_y), false); //checks if contour encloses a point.
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
                
                
                /* rotated rectangle verticies */
                Point2f rect_points[4];
                minRect[i].points( rect_points );

                /* Enlarged rotated rectangle (for simpler mask implementation)*/
                Size2f new_size = minRect[i].size;
                new_size.height = new_size.height + 10;
                new_size.width  = new_size.height + 10;
                RotatedRect mask_rect(minRect[i].center, new_size, minRect[i].angle);

                Point vertices[4];  
                Point2f larger_rect_points[4];
                mask_rect.points(larger_rect_points);


                /*  Draw the outline of the ROI RotatedRectangle onto the output image,
                    Convert the vertices into type Point to then fill the ROI for the mask.*/
                for ( int j = 0; j < 4; j++ )
                {   
                    line( *output_frame, rect_points[j], rect_points[(j+1)%4], this->ROI_box, 2);
                    vertices[j] = larger_rect_points[j];
                }

                /* Fill mask */
                fillConvexPoly(mask, vertices, 4, this->ROI_box);

                /* Draw contours inside ROI. */
                drawContours( *output_frame, this->contours, (int)i, this->ROI_box );



                /* Mask this ROI onto the value at the input image address, 
                BEFORE calculating the next ROI:
                    -> generate the first instance of the mask here.
                    -> then apply it to every incoming frame (before generating it's own ROI), once it exists (i.e. it's matrix isn't empty).
                */


               bitwise_and(*this->input_frame, mask, this->masked_input);
               imshow("input_frame", *this->input_frame);
               imshow("mask", mask);
               imshow("Masked image", this->masked_input);
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
