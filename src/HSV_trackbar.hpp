#ifndef __HSV_TRACKBAR__
#define __HSV_TRACKBAR__

#include "find_paper.hpp"

/*-------------------------------------------------*/
/* Thresholding using inRange() in HSV colorspace */        
/*-----------------------------------------------*/
//reference here: https://docs.opencv.org/3.4/da/d97/tutorial_threshold_inRange.html

/* NOTE: MOST OF THE CODE FOR THIS SECTION IS A DIRECT COPY/PASTE */

// Create trackbars.
// Convert from BGR-to-HSV.
// Detect object based on HSV range values.

/*----parameters for setting-up HSV thresholding trackbars----*/
extern const int max_value_H;
extern const int max_value;
extern int low_H, low_S, low_V;
extern int high_H, high_S, high_V;


/*----HSV thresholding trackbar----*/
extern void on_low_H_thresh_trackbar(int, void *);
extern void on_high_H_thresh_trackbar(int, void *);
extern void on_low_S_thresh_trackbar(int, void *);
extern void on_high_S_thresh_trackbar(int, void *);
extern void on_low_V_thresh_trackbar(int, void *);
extern void on_high_V_thresh_trackbar(int, void *);
extern void init_HSV_trackbars();
extern void run_HSV_thresh();
extern void update_HSV_range(int, int, int);
extern void mouseEvent(int, int, int, int, void*) ;

/* Store the x,y location of the user-selected "seed" pixel */
extern int seed_x;
extern int seed_y;

extern void get_xy_pixel_hsv(int,int);

#endif