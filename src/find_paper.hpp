#ifndef __FIND_PAPER__
#define __FIND_PAPER__


#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

#include "HSV_trackbar.hpp"
#include "apply_contour.hpp"
#include "morph_ops.hpp"

using namespace cv;
using namespace std;


/*----I/O Matrices and pre/post processing viewing windows----*/
extern Mat input_frame;
extern Mat output_frame;

/* HSV thresholding */
extern Mat hsv_thresh_input_frame;
extern Mat hsv_thresh_output_frame;
extern char hsv_display_window[32];

/* Canny edge-detection */
extern Mat canny_input_frame;
extern Mat canny_output_frame;
extern char canny_display_window[32];

/* Morphological operations that use Dilate/ Erode */
extern Mat morph_input_frame;
extern Mat morph_output_frame;
extern char morph_display_window[32];

extern char pre_process_window[32];
extern char post_process_window[32];


#endif