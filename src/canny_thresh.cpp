#include "canny_thresh.hpp"


Mat canny_input_frame; Mat canny_output_frame;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);



void adjust_canny_trackbar_pos(int thresh, void * = NULL)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("Canny thresh", post_process_window, thresh);
}

void apply_Canny()
{
  Mat edge, draw;
  //vector<vector<Point> > contours;
  //vector<Vec4i> hierarchy;
  
  /// Detect edges using canny
  Canny( canny_input_frame, edge, thresh, thresh*2, 3 );
  /// Find contours
  //findContours( edge, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  edge.convertTo(draw, CV_8U);
  /// Draw contours
  // Mat drawing = Mat::zeros(edge.size(), CV_8UC3 );
  // for( int i = 0; i< contours.size(); i++ )
  //    {
  //      Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
  //      drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
  //    }
  
  canny_output_frame = draw;
}

void init_canny_trackbar()
{
  createTrackbar( "Canny thresh", post_process_window, &thresh, max_thresh, adjust_canny_trackbar_pos);
}




