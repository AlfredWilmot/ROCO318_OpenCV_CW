#ifndef __COLORFUL_CONTOURS__
#define __COLORFUL_CONTOURS__

#include "find_paper.hpp"

extern int inside_contour; //used in conjunction w/ pointPolygonTest() to determine if mouse-clicked seed pixel is inside a given contour.

/*---- Contour fitting functions ----*/
extern void apply_Contours(void);

#endif