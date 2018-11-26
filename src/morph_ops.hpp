#ifndef __MORPH_OPS__
#define __MORPH_OPS__

#include "find_paper.hpp"


/*----parameters for setting-up advanced morphological trackbars----*/
extern int morph_elem;
extern int morph_size;
extern int morph_operator;
extern int const max_operator;
extern int const max_elem;
extern int const max_kernel_size;

/*----morphological trackbar functions----*/

// Apply morphological ops.
extern void run_morph();
// Tackbar initializer.
extern void init_morph_ops_trackbars();
// Tackbar callbacks.
extern void adjust_morph_operation(int, void *);
extern void adjust_kernel_type(int, void *);
extern void adjust_kernel_size(int, void *);



#endif