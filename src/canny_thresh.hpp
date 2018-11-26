#ifndef __CANNY_THRESH__
#define __CANNY_THRESH__

#include "find_paper.hpp"

/*----parameters for setting-up Canny trackbars----*/
extern int thresh;
extern int max_thresh;

/*----Canny trackbar functions----*/
extern void adjust_canny_trackbar_pos(int, void* );
extern void apply_Canny();
extern void init_canny_trackbar();

#endif