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
ContourRectangles::ContourRectangles(Mat *infrm): 
HsvThresholdTrackbar(infrm, "HSV Thresholding")
{
    this->window_name   = this->click_display_window;

    this->masked_input  = Mat(this->_input_frame->size(), CV_8UC3);
}

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
void ContourRectangles::FindRectangles()
{

 /* Clear contours list from last frame */
    this->contours.clear();
    
    /* Ensure that the input/ output matricies are valid before continuing*/
    this->errorHandling();


    /*  Whenever a new pixel is selected use the vanialla input frame instead of the previous mased frame,
        & reset contour comparator */
    if(this->get_seed_pixel_hsv(true) == 0)
    {
         findContours(*this->_input_frame, this->contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    }


    /* New mask for a new input frame */
    Mat mask(this->_input_frame->size(), CV_8UC1, Scalar(0,0,0));

    /* Only start drawing bounding boxes once the seed has been defined by the first mouse-click */
    if(this->_seed_x && this->_seed_y)
    {   
        
        /* Contours from previously masked input guide behaviour of current frame mask */
        if(this->contours.empty())
        {
            findContours(this->masked_input, this->contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); 
        }

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
                circle(this->_frm_to_clk, mc, 4, redDot, -1, 8, 0 );
                /* Update seed location to that of CoM to track ROI*/
                this->_seed_x = int(this->mc.x);
                this->_seed_y = int(this->mc.y);   

                /* Defining bounding box for a given contour */
                minRect[i] = minAreaRect( this->contours[i] );
                
                /* rotated rectangle verticies */
                Point2f rect_points[4];
                minRect[i].points( rect_points );

                /*---------------------------*/
                /* Distance estimation code */
                /*-------------------------*/
                this->distance_estimate = calibration_card_width * focal_length / minRect[i].size.width;

                if(this->avg_count < this->count_limit)
                {
                    this->avg_count++;

                    this->avg_distance += this->distance_estimate;  // running sum.

                }
                else
                {
                    this->avg_count = 0;                            // reset the counter.

                    this->avg_distance/=this->count_limit;          // perform averaging.
                    
                    printf("Estimated distance (averaged): %1.2fcm\n\r",  this->avg_distance);

                    this->avg_distance = 0.0;                       // start averaging anew, using a fresh set of values (mitigates drift).

                }


                /* Enlarged rotated rectangle (for simpler tracking implementation)*/
                Size2f new_size = minRect[i].size;
                new_size.height = new_size.height + 10;
                new_size.width  = new_size.width + 10;
                RotatedRect mask_rect(minRect[i].center, new_size, minRect[i].angle);

                Point vertices[4];  
                Point2f larger_rect_points[4];
                mask_rect.points(larger_rect_points);


                /*  Draw the outline of the ROI RotatedRectangle onto the output image,
                    Convert the vertices into type Point to then fill the ROI for the mask.*/
                for ( int j = 0; j < 4; j++ )
                {   
                    line( this->_frm_to_clk, rect_points[j], rect_points[(j+1)%4], this->ROI_box, 2);
                    vertices[j] = larger_rect_points[j];
                }

                /* Fill mask. */
                fillConvexPoly(mask, vertices, 4, this->ROI_box);

                /* Draw contours inside ROI. */
                drawContours( this->_frm_to_clk, this->contours, (int)i, this->ROI_box );



                /* Mask this ROI onto the Matrix at the input image address, 
                    BEFORE calculating the next ROI:
                    -> generate the first instance of the mask here.
                    -> then use it to generate the mask for the next input frame, ad nauseum.
                */


                /* Perform masking of HSV thresholded image with generated mask */
                bitwise_and(*this->_input_frame, mask, this->masked_input);
                imshow("Masked image Pre", this->masked_input); 
                
                /*  Morphological Opening is performed on pre-mask to mitigate noise around ROI 
                    (ROI needs to be well defined relative to environment for this to work well) */
                this->morph();                                                  
                imshow("Masked image Post", this->masked_input);
            }
            
        }
    }

    /* Display ROI Rectangle & CoM overlay onto target on the interactive image */
    imshow(this->window_name, this->_frm_to_clk);
}

/* Perform morphological operations on the masked input frame */
void ContourRectangles::morph()
{   
    // Erode    = 0
    // Dilate   = 1
    // Opening  = 2
    // Closing  = 3
    // Gradient = 4

    int operation  = 2;
    int morph_size = 2;
    int iterations = 2;
    Mat element = getStructuringElement( 0, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );

    /// Apply the specified morphology operation
    morphologyEx(this->masked_input, this->masked_input, operation, element, Point(-1,-1), iterations);
}

/* Return an error message if the matrix is not a binary image (needed for contour-fitting) */
void ContourRectangles::errorHandling()
{    
    if(_input_frame->empty())
    {
        throw std::invalid_argument( "Input frame is empty!");
    }
    else if(this->_input_frame->channels() != 1)
    {
        printf("input frame has %d channels.\n\r", this->_input_frame->channels());
        throw std::invalid_argument( "Input frame must be single channel.\n\r");
    }

    /* Need to fill the output frame before inserting contours/ rectangles, if it's empty */
    if(this->_frm_to_clk.empty())
    {
        this->_frm_to_clk = Mat::zeros( this->_input_frame->size(), CV_8UC3 );
    }
}
