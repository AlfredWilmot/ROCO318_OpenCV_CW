#ifndef __FIND_PAPER__
#define __FIND_PAPER__


#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>

#include "HSV_trackbar.hpp"


using namespace cv;
using namespace std;


/*----I/O Matrices and pre/post processing viewing windows----*/
extern Mat input_frame;
extern Mat output_frame;
extern char pre_process_window[16];
extern char post_process_window[16];


#endif