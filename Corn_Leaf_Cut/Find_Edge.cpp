//此为主要代码，实现单株玉米叶片的识别与提取，在提取前需对植株的茎和叶片粘连部分作出标记
#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include <vector>
#include<assert.h>
using namespace cv;
using namespace std;
RNG rng(12345);
int thresh = 100;
string source = "F://";
void Display(Mat image,String source,String name,int WR=0) {
	if (WR == 1)
	{
		imshow(name, image);
	}
	if(WR==2) {
		imwrite(source+name+".jpg",image);
		imshow(name, image);
	}

}
void FindEdge(Mat& image,int WR=0) {

	if (image.empty())
	{
		cout << "The image of FindEdge is empty !" << endl;
	}
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
				//if (((img.at<Vec3b>(row, col)[1]) * 2 - (img.at<Vec3b>(row, col)[0]) - (img.at<Vec3b>(row, col)[2]))>45)

				//if(img.at<Vec3b>(row, col)[0]==0&& img.at<Vec3b>(row, col)[2]==0)
			{
				//img.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
			}
			else {
				//img.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
				img.at<Vec3b>(row, col) = Vec3b(255, 255, 255);
			}
		}
	}

	//imshow("背景去除", img);
	if (WR == 1)
	{
		Display(img, "", "背景去除", WR);
	}
	else {
		Display(img, "F://result//", "背景去除", WR);
	}

	//【4】转换为灰度图
	cvtColor(img, img_gray, CV_RGB2GRAY);

	//【5】使用Laplace函数
	Laplacian(img_gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);

	//【6】计算绝对值，并将结果转换成8位
	convertScaleAbs(dst, abs_dst);
	////imshow("新图", abs_dst);

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
	if (WR == 1)
	{
		Display(mergeImg, "", "直方图均衡化之后", WR);
	}
	else {
		Display(mergeImg, "F://result//", "直方图均衡化之后", WR);
	}
	//imshow("直方图均衡化之后", mergeImg);

	//【4】转换为灰度图
	cvtColor(mergeImg, img_gray, CV_RGB2GRAY);

	//【5】使用Laplace函数
	Laplacian(img_gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
	//【6】计算绝对值，并将结果转换成8位
	convertScaleAbs(dst, abs_dst);
	//imshow("新图", abs_dst);
	image = abs_dst;
}

void ImageCut(Mat img,Mat image,int WR=0) {
	if (img.empty()) {
		cout << "The img of ImageCut is empty !" << endl;
	}
	if (image.empty()) {
		cout << "The image of ImageCut is empty !" << endl;
	}
	if (WR == 0) {
		cout << "The WR of ImageCut is wrong !" << endl;
	}

	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat src_gray = img;
	imshow("src_gray", img);
	if (WR == 1) {
		//imshow("轮廓标注图", drawing);
		Display(img, "", "src_gray", WR);
	}
	else
	{
		String name = "src_gray";
		//	imwrite("F://result//" +name+ ".jpg", drawing);//写入文件夹
		Display(img, "F://result//", name, WR);
	}
	/// 使用Threshold检测边缘
	threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);//进行二值化处理
	//threshold_output = img;												  /// 找到轮廓
	findContours(threshold_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));//寻找轮廓

																										   /// 多边形逼近轮廓 + 获取矩形和圆形边界框
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));//计算点集的最外面矩形边界
		//minEnclosingCircle(contours_poly[i], center[i], radius[i]);
	}

	//Mat image = imread("F://test1.jpg");;
	/// 画多边形轮廓 + 包围的矩形框 + 圆形框
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3); //创建一个相同大小图片
	for (int i = 0; i < contours.size(); i++)
	{
		int j = 0;
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);//绘制矩形
		boundRect[i] = boundingRect(Mat(contours_poly[i]));//得到轮廓外接矩形数据结构
		cout << boundRect[i].br() << endl;

		if (boundRect[i].area() > 1000) //对所取得的轮廓进行筛选，过滤掉一些面积过小的区域
		{
			//方案一：矩形截取叶片
           /*
			cout << i << endl;
			cv::Mat imageROI = image(boundRect[i]);//根据轮廓外接矩形信息进行截取RIO感兴趣部分图像
			std::stringstream ss;//int转换为string
			std::string str;
			ss << i;
			ss >> str;
			imshow(str, imageROI);   //对切割的图像进行多窗口的展示
			*/

			Mat hole(src_gray.size(), CV_8U, Scalar(0)); //遮罩图层  
		
			cv::drawContours(hole, contours_poly, i, Scalar(255), CV_FILLED); //在遮罩图层上，用白色像素填充轮廓  
		//	namedWindow("My hole");
		//	imshow("My hole", hole);
			
			Mat crop(image.rows, image.cols, CV_8UC3);//八位无符号整形
			imshow("crop",crop);
			image.copyTo(crop, hole);//将原图像拷贝进遮罩图层  
			
          	cout << i << endl;		
			cv::Mat imageROI = image(boundRect[i]);//根据轮廓外接矩形信息进行截取RIO感兴趣部分图像
			std::stringstream ss;//int转换为string
			std::string str;
			ss << i;
			ss >> str;
			if (WR == 1)
			{
				String name = "裁剪";
				//imshow(name + str + ".jpg", crop);   //对切割的图像进行多窗口的展示
				Display(crop, "", name + str, WR);
			}
			else
			{
				String name = "裁剪";
				//imwrite("F://result//"+name + str + ".jpg", crop);//写入文件夹
				Display(crop, "F://result//", name + str, WR);
			}
		
			
			//imwrite("F://result//" + str + ".jpg", crop);//写入文件夹
		}
	}

	/// 显示在一个窗口
	//namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	if (WR == 1) {
		//imshow("轮廓标注图", drawing);
		Display(drawing, "", "轮廓标注图", WR);
	}
	else
	{
		String name = "轮廓标注图";
	//	imwrite("F://result//" +name+ ".jpg", drawing);//写入文件夹
		Display(drawing, "F://result//", name, WR);
	}
	
	
}


int main()
{
	Mat img = imread(source+"2017051103.jpg");
	//Mat img = imread(source + "zzz.jpg");
	int WR = 1;
	//Mat img = imread("F://2017051703.jpg");
	if (img.empty())
	{
		assert(img.empty());
	}
	Mat image = img;
	namedWindow("原画");
	Mat ssssssssssss;

	imshow("原画", img);
	FindEdge(img,WR);
	//imshow("testX", img);
	//Mat img2 = imread("F://10086.jpg");
	ImageCut(img,image,WR);//1,展示文件，2写文件
	waitKey();
	return 0;
}
