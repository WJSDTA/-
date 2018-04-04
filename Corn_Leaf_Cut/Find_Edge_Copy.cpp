/*���Դ��룬����*/
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
	bilateralFilter(img, bilateralFilter_img, 25, 25 * 2, 25 / 2);//����˫���˲���ȥ�ﱣ��
	Mat element = getStructuringElement(MORPH_RECT, Size(1, 1));
	//������̬ѧ����
	morphologyEx(bilateralFilter_img, bilateralFilter_img, MORPH_CLOSE, element);//������
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

	imshow("��ͼx", img);

	//��4��ת��Ϊ�Ҷ�ͼ
	cvtColor(img, img_gray, CV_RGB2GRAY);

	//��5��ʹ��Laplace����
	Laplacian(img_gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);

	//��6���������ֵ���������ת����8λ
	convertScaleAbs(dst, abs_dst);
	imshow("��ͼ", abs_dst);

	Mat mergeImg;//�ϲ����ͼ��
				 //�����洢��ͨ��ͼƬ������
	vector<Mat> splitBGR(img.channels());
	//�ָ�ͨ�����洢��splitBGR��
	split(img, splitBGR);
	//�Ը���ͨ���ֱ����ֱ��ͼ���⻯
	for (int i = 0; i<img.channels(); i++)
		equalizeHist(splitBGR[i], splitBGR[i]);
	//�ϲ�ͨ��
	merge(splitBGR, mergeImg);

	//namedWindow("equalizeHist");
	imshow("equalizeHist", mergeImg);

	//��4��ת��Ϊ�Ҷ�ͼ
	cvtColor(mergeImg, img_gray, CV_RGB2GRAY);

	//��5��ʹ��Laplace����
	Laplacian(img_gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);

	//��6���������ֵ���������ת����8λ
	convertScaleAbs(dst, abs_dst);
	imshow("��ͼ", abs_dst);
	image = abs_dst;
}

void ImageCut(Mat img, Mat image) {
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat src_gray = img;
	/// ʹ��Threshold����Ե
	threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);//���ж�ֵ������
																	//  	threshold_output = img;												  /// �ҵ�����
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));//Ѱ������

																										   /// ����αƽ����� + ��ȡ���κ�Բ�α߽��
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
	/// ����������� + ��Χ�ľ��ο� + Բ�ο�
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));//�õ�������Ӿ������ݽṹ

		if (boundRect[i].area() > 1000) //����ȡ�õ���������ɸѡ�����˵�һЩ�����С������
		{

			cout << i << endl;
			cv::Mat imageROI = image(boundRect[i]);//����������Ӿ�����Ϣ���н�ȡRIO����Ȥ����ͼ��
			std::stringstream ss;//intת��Ϊstring
			std::string str;
			ss << i;
			ss >> str;
			imshow(str, imageROI);   //���и��ͼ����жര�ڵ�չʾ
		}
	}

	/// ��ʾ��һ������
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
}

int main()
{
Mat img = imread("F://test123.jpg");
Mat image = img;
imshow("ԭ��", img);

FindEdge(img);


imshow("testX", img);
Mat img2 = imread("F://10086.jpg");
ImageCut(img2,image);
//imwrite("10086.jpg", abs_dst);
waitKey();
return 0;
}
*/