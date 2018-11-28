#include "colorful_contours.hpp"


Mat contour_input_frame; 
Mat contour_output_frame;
char contour_display_window[32] = "Fitted contours";

RNG rng(12345);     //used for the pretty colors.
int inside_contour; //used in conjunction w/ pointPolygonTest() to determine if mouse-clicked seed pixel is inside a given contour.

/* Establishing contour CoM */
Moments mu;
Point2f mc;
Scalar redDot = Scalar(1,0,255);    //BGR code for red.


void apply_Contours()
{
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(contour_input_frame, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    
    vector<RotatedRect> minRect( contours.size() );

    contour_output_frame = Mat::zeros( contour_input_frame.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );


        // Draw contours...
        drawContours( contour_output_frame, contours, (int)i, color );

        // Only draw the contour & corresponding bounding-box for contours that contain the seed pixel (initially clicked by user).
        inside_contour = pointPolygonTest(contours[i], Point2f(seed_x, seed_y), false);
        if (inside_contour == 1)
        {
            /* Calculating CoM */
            mu = moments( contours[i], false );
            mc = Point2f( static_cast<float>(mu.m10/mu.m00) , static_cast<float>(mu.m01/mu.m00) );
            /* Drawing CoM */
            circle( contour_output_frame, mc, 4, redDot, -1, 8, 0 );
            /* Update seed location to that of CoM to track ROI*/
            seed_x = int(mc.x);
            seed_y = int(mc.y);
            /* Update the HSV range of the seed (dynamically adjust) */
            get_xy_pixel_hsv(seed_x, seed_y);

            // Generates a 5x5 recatngular kernel.
            Mat element = getStructuringElement( 0, Size( 5, 5 ));


            /* Defining bounding box for a given contour */
            minRect[i] = minAreaRect( contours[i] );
             // rotated rectangle
            Point2f rect_points[4];
            minRect[i].points( rect_points );
            for ( int j = 0; j < 4; j++ )
            {
                line( contour_output_frame, rect_points[j], rect_points[(j+1)%4], color );
            }
           printf("seed location: [%d,%d]\n\r", seed_x, seed_y);
        }
        

    }
}
