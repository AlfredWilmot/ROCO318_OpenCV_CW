
#include "ClickForPixelData.hpp"

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>
#include <stdexcept>


using namespace cv;
using namespace std;


ClickForPixelData::ClickForPixelData(String glugg_nafn)
{
    this->display_window = glugg_nafn;

    namedWindow(this->display_window);

    /* Setup mouse-click event on input frame */
    setMouseCallback(this->display_window, ClickForPixelData::onMouseEvt, this);
}


void ClickForPixelData::FrameToClick(Mat *clk_frm)
{
    this-> _frm_to_clk = clk_frm;
}


int ClickForPixelData::mouseEvent(int evt, int x, int y, int flags) 
{                    

    if (evt == CV_EVENT_LBUTTONDOWN) 
    { 
        this->_mouse_clk = true;      //set flag.
        
        /* Update the new mouse-selected seed pixel coordinates */
        this->_seed_x = x;
        this->_seed_y = y;

    }         
}


/*---- Calculates and stores HSV value of pixel at input coordinate x,y ----*/
int ClickForPixelData::get_seed_pixel_hsv()
{

    // Can only update & display the stored HSV data once per mouse-click.
    if(this->_mouse_clk)
    {   

        if(this->_frm_to_clk->empty())
        {
            printf("Reference frame is empty!\n\r");
            this->_mouse_clk = false; //reset flag, until next mouse click.
            return -1;
        }


        Mat tmp = *this->_frm_to_clk;


        /* decode pixel RGB values */
        Vec3b rgb = tmp.at<Vec3b>(this->_seed_x, this->_seed_y);
        int B=rgb.val[0];
        int G=rgb.val[1];
        int R=rgb.val[2];

        Mat HSV;
        Mat RGB= tmp(Rect(this->_seed_x, this->_seed_y, 1, 1));   //Single-value matrix that is the pixel at point [x,y], RGB encoded by default.
        cvtColor(RGB, HSV,CV_BGR2HSV);

        Vec3b hsv=HSV.at<Vec3b>(0,0);

        this->H = hsv.val[0];
        this->S = hsv.val[1];
        this->V = hsv.val[2];

            printf("[%d, %d] H:%d, S:%d, V:%d\n\r", 
                    this->_seed_x, this->_seed_y, 
                    this->H, this->S, this->V);

        this->_mouse_clk = false; //reset flag, until next mouse click.

        return 0;

    }

    return 1;
}



/* Redirection to get handler working */
void ClickForPixelData::onMouseEvt(int evt, int x, int y, int flags, void* ptr)
{
    ClickForPixelData* tmp = (ClickForPixelData*)(ptr);
    tmp->mouseEvent(evt, x, y, flags);
}
