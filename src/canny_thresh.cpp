#include "canny_thresh.hpp"


Mat canny_input_frame; Mat canny_output_frame;
int thresh = 50;
int max_thresh = 255;
RNG rng(12345);
int inside_contour; //used in conjunction w/ pointPolygonTest() to determine if mouse-clicked seed pixel is inside a given contour.

/* Establishing contour CoM */
Moments mu;
Point2f mc;

void adjust_canny_trackbar_pos(int thresh, void * = NULL)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("Canny thresh", post_process_window, thresh);
}

void apply_Contours()
{
    Canny( canny_input_frame, canny_input_frame, thresh, thresh*2, 3 );

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(canny_input_frame, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    
    vector<RotatedRect> minRect( contours.size() );

    canny_output_frame = Mat::zeros( canny_input_frame.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );


        // Draw contours...
        drawContours( canny_output_frame, contours, (int)i, color );

        // Only draw the contour & corresponding bounding-box for contours that contain the seed pixel.
        inside_contour = pointPolygonTest(contours[i], Point2f(seed_x, seed_y), false);
        if (inside_contour == 1)
        {
            /* Calculating CoM */
            mu = moments( contours[i], false );
            mc = Point2f( static_cast<float>(mu.m10/mu.m00) , static_cast<float>(mu.m01/mu.m00) );

            /* Defining bounding box for a given contour */
            minRect[i] = minAreaRect( contours[i] );
             // rotated rectangle
            Point2f rect_points[4];
            minRect[i].points( rect_points );
            for ( int j = 0; j < 4; j++ )
            {
                line( canny_output_frame, rect_points[j], rect_points[(j+1)%4], color );
            }
           printf("seed location: [%d,%d]\n\r", seed_x, seed_y);
        }
        

    }
}

void init_canny_trackbar()
{
  createTrackbar( "Canny thresh", post_process_window, &thresh, max_thresh, adjust_canny_trackbar_pos);
}