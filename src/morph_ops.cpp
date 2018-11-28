#include "morph_ops.hpp"

/*--------------------------------------------*/
/* Removing noise from HSV-thresholded image */
/*------------------------------------------*/
//reference here: https://docs.opencv.org/2.4/doc/tutorials/imgproc/opening_closing_hats/opening_closing_hats.html

Mat morph_input_frame;
Mat morph_output_frame;
char morph_display_window[32] = "morph";

int morph_elem = 0;     //Rectangle kernel type by default.
int morph_size = 5;     //default Kernel size = 2n+1, n=5. 
int morph_operator = 0; //"Opening" operation by default. (Erosion folowed by dilation)

/*NOTE:
    -> most appropriate kernel size depends on the physical size of the card (ROI), and it's distance from the camera.
    -> Opening operator seems to mitigate noise most effectively, and get the highest fidelity bounding rectangle.


*/


int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;




/*---- Initialzie the various morphological trackbars ----*/
void init_morph_ops_trackbars()
{
    /// Create Trackbar to select Morphology operation
    createTrackbar("Operator:\n 0: Opening - 1: Closing \n 2: Gradient - 3: Top Hat \n 4: Black Hat", 
                    morph_display_window, 
                    &morph_operator, max_operator, 
                    adjust_morph_operation );

    /// Create Trackbar to select kernel type
    createTrackbar( "Element:\n 0: Rect - 1: Cross - 2: Ellipse", 
                    morph_display_window,
                    &morph_elem, max_elem,
                    adjust_kernel_type );

    /// Create Trackbar to choose kernel size
    createTrackbar( "Kernel size:\n 2n +1", 
                    morph_display_window,
                    &morph_size, max_kernel_size,
                    adjust_kernel_size );
}

/*---- Callbacks for updating trackbar positions and dependent variable ----*/
void adjust_morph_operation(int morph_operator, void * = NULL)
{
    setTrackbarPos("Operator:\n 0: Opening - 1: Closing \n 2: Gradient - 3: Top Hat \n 4: Black Hat", morph_display_window, morph_operator);
}
void adjust_kernel_type(int morph_elem, void * = NULL)
{
    setTrackbarPos("Element:\n 0: Rect - 1: Cross - 2: Ellipse", morph_display_window, morph_elem);
}
void adjust_kernel_size(int morph_size, void * = NULL)
{
    setTrackbarPos("Kernel size:\n 2n +1", morph_display_window, morph_size);
}

/*---- Apply morphological operations as defined by the trackbar values ----*/
void run_morph()
{
  // Since MORPH_X : 2,3,4,5 and 6
  int operation = morph_operator + 2;

  Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );

  /// Apply the specified morphology operation
  morphologyEx( morph_input_frame, morph_output_frame, operation, element );
  }
