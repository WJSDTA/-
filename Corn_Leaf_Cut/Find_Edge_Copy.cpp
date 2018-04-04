/*测试代码，无用*/
/*
#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include <vector>
using namespace cv;
using namespace std;
RNG rng(12345);
int thresh = 100;

void FindEdge(Mat& image) {

	Mat img = image;
	Mat bilateralFilter_img, img_gray, dst, abs_dst;
	bilateralFilter(img, bilateralFilter_img, 25, 25 * 2, 25 / 2);//进行双边滤波，去燥保边
	Mat element = getStructuringElement(MORPH_RECT, Size(1, 1));
	//进行形态学操作
	morphologyEx(bilateralFilter_img, bilateralFilter_img, MORPH_CLOSE, element);//闭运算
	img = bilateralFilter_img;

	for (int row = 0; row < img.rows; row++)
	{
		for (int col = 0; col < img.cols; col++)
		{
			if (((img.at<Vec3b>(row, col)[1] - img.at<Vec3b>(row, col)[0] > 8))
				&& ((img.at<Vec3b>(row, col)[1] - img.at<Vec3b>(row, col)[2] >8)))
				//if(img.at<Vec3b>(row, col)[0]==0&& img.at<Vec3b>(row, col)[2]==0)
			{
				img.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
			}
			else {
				//img.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
				img.at<Vec3b>(row, col) = Vec3b(255, 255, 255);
			}
		}
	}

	imshow("新图x", img);

	//【4】转换为灰度图
	cvtColor(img, img_gray, CV_RGB2GRAY);

	//【5】使用Laplace函数
	Laplacian(img_gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);

	//【6】计算绝对值，并将结果转换成8位
	convertScaleAbs(dst, abs_dst);
	imshow("新图", abs_dst);

	Mat mergeImg;//合并后的图像
				 //用来存储各通道图片的向量
	vector<Mat> splitBGR(img.channels());
	//分割通道，存储到splitBGR中
	split(img, splitBGR);
	//对各个通道分别进行直方图均衡化
	for (int i = 0; i<img.channels(); i++)
		equalizeHist(splitBGR[i], splitBGR[i]);
	//合并通道
	merge(splitBGR, mergeImg);

	//namedWindow("equalizeHist");
	imshow("equalizeHist", mergeImg);

	//【4】转换为灰度图
	cvtColor(mergeImg, img_gray, CV_RGB2GRAY);

	//【5】使用Laplace函数
	Laplacian(img_gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);

	//【6】计算绝对值，并将结果转换成8位
	convertScaleAbs(dst, abs_dst);
	imshow("新图", abs_dst);
	image = abs_dst;
}

void ImageCut(Mat img, Mat image) {
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat src_gray = img;
	/// 使用Threshold检测边缘
	threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);//进行二值化处理
																	//  	threshold_output = img;												  /// 找到轮廓
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));//寻找轮廓

																										   /// 多边形逼近轮廓 + 获取矩形和圆形边界框
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		//minEnclosingCircle(contours_poly[i], center[i], radius[i]);
	}

	//Mat image = imread("F://test1.jpg");;
	/// 画多边形轮廓 + 包围的矩形框 + 圆形框
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));//得到轮廓外接矩形数据结构

		if (boundRect[i].area() > 1000) //对所取得的轮廓进行筛选，过滤掉一些面积过小的区域
		{

			cout << i << endl;
			cv::Mat imageROI = image(boundRect[i]);//根据轮廓外接矩形信息进行截取RIO感兴趣部分图像
			std::stringstream ss;//int转换为string
			std::string str;
			ss << i;
			ss >> str;
			imshow(str, imageROI);   //对切割的图像进行多窗口的展示
		}
	}

	/// 显示在一个窗口
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
}

int main()
{
Mat img = imread("F://test123.jpg");
Mat image = img;
imshow("原画", img);

FindEdge(img);


imshow("testX", img);
Mat img2 = imread("F://10086.jpg");
ImageCut(img2,image);
//imwrite("10086.jpg", abs_dst);
waitKey();
return 0;
}
*/