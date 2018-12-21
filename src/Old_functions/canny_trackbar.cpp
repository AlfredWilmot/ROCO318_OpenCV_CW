#include "canny_trackbar.hpp"

int thresh = 50;
int max_thresh = 255;

/* Canny Edge-detection */
Mat canny_input_frame;
Mat canny_output_frame;
char canny_display_window[32] = "Canny";

/* Callback for the canny trackbar */
void adjust_canny_trackbar_pos(int thresh, void * = NULL)
{
    setTrackbarPos("Canny thresh", canny_display_window, thresh);
}


/* Setup the trackbar for adjusting the Canny threshold value */
void init_canny_trackbar()
{
  createTrackbar( "Canny thresh", canny_display_window, &thresh, max_thresh, adjust_canny_trackbar_pos);
}
/* Perform canny edge-detection on the canny_input_frame */
void apply_Canny()
{
    //Convert to gray-scale
    cvtColor( canny_input_frame, canny_input_frame, CV_BGR2GRAY );
    //Apply Gaussian-blur
    GaussianBlur(canny_input_frame, canny_input_frame, Size(7, 7), 1.5, 1.5);
    //Apply Canny edge-detection
    Canny( canny_input_frame, canny_output_frame, thresh, thresh*2, 3 );

}