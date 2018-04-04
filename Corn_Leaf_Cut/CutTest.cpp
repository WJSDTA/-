/*测试代码，无用*/
/*
#include "opencv2/opencv.hpp"
#include "iostream"

using namespace cv;
using namespace std;

void imgROI(Mat &img, Mat &mask, Mat &roi)
{
	int left = img.cols - 1, right = 0, top = 0, down = img.rows - 1;
	for (int i = 0; i<img.rows; i++)   //找上下左右邊界
	{
		uchar *ptr_mask = mask.ptr<uchar>(i);
		for (int j = 0; j<img.cols; j++)
		{
			if (ptr_mask[j] == 255)
			{
				if (i>top) top = i;
				if (i<down) down = i;
				if (j<left) left = j;
				if (j>right) right = j;
			}
		}
	}
	int roi_height = top - down;    //ROI圖像的尺寸
	int roi_widgh = right - left;
	//    cout<<img.rows<<","<<img.cols<<","<<roi_height<<","<<roi_widgh<<endl;
	roi = Mat::zeros(roi_height, roi_widgh, CV_8UC3);
	for (int i = 0; i<roi.rows; i++)
	{
		Vec3b *ptr_img = img.ptr<Vec3b>(i + down);
		Vec3b *ptr_roi = roi.ptr<Vec3b>(i);
		for (int j = 0; j<roi.cols; j++)
		{
			ptr_roi[j] = ptr_img[j + left];    //ROI.x = 原圖.x+left
		}                                    //ROI.y = 原圖.y+down
	}
}
int main()
{
	Mat img = imread("F://f.jpg");
	Mat gray, binary, roi;
	cvtColor(img, gray, CV_BGR2GRAY);
	threshold(gray, binary, 145, 255, THRESH_BINARY);
	imgROI(img, binary, roi);
	imshow("img", img);
	imshow("binary", binary);
	imshow("roi", roi);
	waitKey();
	return 0;
}
*/