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
	cv::cvtColor(gray, gray, CV_RGB2GRAY);//�ҶȻ�  

	int thresh_size = (100 / 4) * 2 + 1; //����Ӧ��ֵ����ֵ  
	adaptiveThreshold(gray, gray, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, thresh_size, thresh_size / 3);
	morphologyEx(gray, gray, MORPH_OPEN, Mat());//��̬ѧ������ȥ���  

	vector<vector<Point> > contours;
	cv::findContours(gray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //������  
	vector<vector<Point>> contours1;
	for (int i = 0; i < contours.size(); ++i)
	{
		if (contours[i].size() > 200)//���Ƚ�С�������޳���  
		{
			contours1.push_back(contours[i]);
		}
	}
	/*���Լ������¹���*/
	//vector<vector<Point>> contours2;  
	//vector<Point> approx_poly;//��Ŷ���  
	//for (int i = 0; i < contours1.size(); ++i)  
	//{  
	//double eps = contours1[i].size()*0.08;//����  
	//approxPolyDP(contours1[i], approx_poly, eps, true);//����һ��������ٵ��������ƽ�һ�����߻���һ�������  
	//if ((approx_poly.size() != 4) || (!isContourConvex(approx_poly)))//��ȡֻ��4�������������͹�����  
	//{  
	//  continue;  
	//}  
	//else {  
	//contours2.push_back(contours1[i]);  
	//}  
	//}  
	Mat hole(gray.size(), CV_8U, Scalar(0)); //����ͼ��  
	cv::drawContours(hole, contours1, -1, Scalar(255), CV_FILLED); //������ͼ���ϣ��ð�ɫ�����������  
	namedWindow("My hole");
	imshow("My hole", hole);
	Mat crop(original.rows, original.cols, CV_8UC3);
	original.copyTo(crop, hole);//��ԭͼ�񿽱�������ͼ��  
	namedWindow("My warpPerspective");
	imshow("My warpPerspective", crop);
	waitKey(0);
}