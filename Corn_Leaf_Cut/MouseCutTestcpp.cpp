/*#include "cv.h"  
#include "highgui.h"  
#include <stdio.h>  
#include <stdlib.h>  
#include <iostream>
using namespace std;
IplImage* inpaint_mask = 0;
IplImage* img0 = 0, *img = 0, *inpainted = 0;
CvPoint prev_pt = { -1,-1 };

void on_mouse(int event, int x, int y, int flags, void* zhang)
{
	if (!img)
		return;
	if (event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON))
		prev_pt = cvPoint(-1, -1);//初始化  
	else if (event == CV_EVENT_LBUTTONDOWN)
		prev_pt = cvPoint(x, y);
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))
	{//手一直在绘画  
		CvPoint pt = cvPoint(x, y);
		if (prev_pt.x < 0)
			prev_pt = pt;
		cvLine(inpaint_mask, prev_pt, pt, cvScalarAll(255), 5, 8, 0);
		cvLine(img, prev_pt, pt, cvScalarAll(255), 5, 8, 0);
		prev_pt = pt;
		cvShowImage("image", img);
	}
}

int main(int argc, char** argv)
{
	//char* filename = argc >= 2 ? argv[1] : (char*)"fruits.jpg";
	char *filename = "F://f.jpg";
	if ((img0 = cvLoadImage(filename, -1)) == 0)
		//return 0;
	{
		cout << *filename << endl;
		cvWaitKey(0);
	}
	printf("Hot keys: \n");
    cvNamedWindow("image", 1);
	img = cvCloneImage(img0);
	inpainted = cvCloneImage(img0);
	inpaint_mask = cvCreateImage(cvGetSize(img), 8, 1);
	cvZero(inpaint_mask);
	cvZero(inpainted);
	cvShowImage("image", img);
	cvShowImage("watershed transform", inpainted);
	cvSetMouseCallback("image", on_mouse, 0);
}*/