#ifndef __CANNY_TRACKBAR__
#define __CANNY_TRACKBAR__

#include "find_paper.hpp"

/*----parameters for setting-up Canny trackbars----*/
extern int thresh;
extern int max_thresh;


/*----Canny trackbar function----*/
extern void adjust_canny_trackbar_pos(int, void* );
extern void init_canny_trackbar();
extern void apply_Canny();

#endif