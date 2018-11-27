#include "canny_thresh.hpp"


Mat canny_input_frame; Mat canny_output_frame;
int thresh = 50;
int max_thresh = 255;
RNG rng(12345);



void adjust_canny_trackbar_pos(int thresh, void * = NULL)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("Canny thresh", post_process_window, thresh);
}

void apply_Canny()
{
    vector<vector<Point> > contours;
    findContours( canny_input_frame, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<RotatedRect> minRect( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ )
    {
        minRect[i] = minAreaRect( contours[i] );
    }
    canny_output_frame = Mat::zeros( canny_input_frame.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        // contour
        drawContours( canny_output_frame, contours, (int)i, color );
        // rotated rectangle
        Point2f rect_points[4];
        minRect[i].points( rect_points );
        for ( int j = 0; j < 4; j++ )
        {
            line( canny_output_frame, rect_points[j], rect_points[(j+1)%4], color );
        }
    }
}

void init_canny_trackbar()
{
  createTrackbar( "Canny thresh", post_process_window, &thresh, max_thresh, adjust_canny_trackbar_pos);
}




