#include "opencv2/imgproc.hpp"  
#include "opencv2/highgui.hpp"  
#include <iostream>  
using namespace cv;
using namespace std;
void main() {
	Mat original = imread("F:\\test123.jpg");
	namedWindow("My original");
	imshow("My original", original);
	Mat gray = original;
	cv::cvtColor(gray, gray, CV_RGB2GRAY);//灰度化  

	int thresh_size = (100 / 4) * 2 + 1; //自适应二值化阈值  
	adaptiveThreshold(gray, gray, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, thresh_size, thresh_size / 3);
	morphologyEx(gray, gray, MORPH_OPEN, Mat());//形态学开运算去噪点  

	vector<vector<Point> > contours;
	cv::findContours(gray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //找轮廓  
	vector<vector<Point>> contours1;
	for (int i = 0; i < contours.size(); ++i)
	{
		if (contours[i].size() > 200)//将比较小的轮廓剔除掉  
		{
			contours1.push_back(contours[i]);
		}
	}
	/*可以加入以下功能*/
	//vector<vector<Point>> contours2;  
	//vector<Point> approx_poly;//存放顶点  
	//for (int i = 0; i < contours1.size(); ++i)  
	//{  
	//double eps = contours1[i].size()*0.08;//精度  
	//approxPolyDP(contours1[i], approx_poly, eps, true);//用另一条顶点较少的曲线来逼近一条曲线或者一个多边形  
	//if ((approx_poly.size() != 4) || (!isContourConvex(approx_poly)))//提取只有4个顶点的轮廓和凸多边形  
	//{  
	//  continue;  
	//}  
	//else {  
	//contours2.push_back(contours1[i]);  
	//}  
	//}  
	Mat hole(gray.size(), CV_8U, Scalar(0)); //遮罩图层  
	cv::drawContours(hole, contours1, -1, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓  
	namedWindow("My hole");
	imshow("My hole", hole);
	Mat crop(original.rows, original.cols, CV_8UC3);
	original.copyTo(crop, hole);//将原图像拷贝进遮罩图层  
	namedWindow("My warpPerspective");
	imshow("My warpPerspective", crop);
	waitKey(0);
}