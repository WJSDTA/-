//��Ϊ��Ҫ���룬ʵ�ֵ�������ҶƬ��ʶ������ȡ������ȡǰ���ֲ��ľ���ҶƬճ�������������
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

	//imshow("����ȥ��", img);
	if (WR == 1)
	{
		Display(img, "", "����ȥ��", WR);
	}
	else {
		Display(img, "F://result//", "����ȥ��", WR);
	}

	//��4��ת��Ϊ�Ҷ�ͼ
	cvtColor(img, img_gray, CV_RGB2GRAY);

	//��5��ʹ��Laplace����
	Laplacian(img_gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);

	//��6���������ֵ���������ת����8λ
	convertScaleAbs(dst, abs_dst);
	////imshow("��ͼ", abs_dst);

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
	if (WR == 1)
	{
		Display(mergeImg, "", "ֱ��ͼ���⻯֮��", WR);
	}
	else {
		Display(mergeImg, "F://result//", "ֱ��ͼ���⻯֮��", WR);
	}
	//imshow("ֱ��ͼ���⻯֮��", mergeImg);

	//��4��ת��Ϊ�Ҷ�ͼ
	cvtColor(mergeImg, img_gray, CV_RGB2GRAY);

	//��5��ʹ��Laplace����
	Laplacian(img_gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
	//��6���������ֵ���������ת����8λ
	convertScaleAbs(dst, abs_dst);
	//imshow("��ͼ", abs_dst);
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
		//imshow("������עͼ", drawing);
		Display(img, "", "src_gray", WR);
	}
	else
	{
		String name = "src_gray";
		//	imwrite("F://result//" +name+ ".jpg", drawing);//д���ļ���
		Display(img, "F://result//", name, WR);
	}
	/// ʹ��Threshold����Ե
	threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);//���ж�ֵ������
	//threshold_output = img;												  /// �ҵ�����
	findContours(threshold_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));//Ѱ������

																										   /// ����αƽ����� + ��ȡ���κ�Բ�α߽��
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));//����㼯����������α߽�
		//minEnclosingCircle(contours_poly[i], center[i], radius[i]);
	}

	//Mat image = imread("F://test1.jpg");;
	/// ����������� + ��Χ�ľ��ο� + Բ�ο�
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3); //����һ����ͬ��СͼƬ
	for (int i = 0; i < contours.size(); i++)
	{
		int j = 0;
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);//���ƾ���
		boundRect[i] = boundingRect(Mat(contours_poly[i]));//�õ�������Ӿ������ݽṹ
		cout << boundRect[i].br() << endl;

		if (boundRect[i].area() > 1000) //����ȡ�õ���������ɸѡ�����˵�һЩ�����С������
		{
			//����һ�����ν�ȡҶƬ
           /*
			cout << i << endl;
			cv::Mat imageROI = image(boundRect[i]);//����������Ӿ�����Ϣ���н�ȡRIO����Ȥ����ͼ��
			std::stringstream ss;//intת��Ϊstring
			std::string str;
			ss << i;
			ss >> str;
			imshow(str, imageROI);   //���и��ͼ����жര�ڵ�չʾ
			*/

			Mat hole(src_gray.size(), CV_8U, Scalar(0)); //����ͼ��  
		
			cv::drawContours(hole, contours_poly, i, Scalar(255), CV_FILLED); //������ͼ���ϣ��ð�ɫ�����������  
		//	namedWindow("My hole");
		//	imshow("My hole", hole);
			
			Mat crop(image.rows, image.cols, CV_8UC3);//��λ�޷�������
			imshow("crop",crop);
			image.copyTo(crop, hole);//��ԭͼ�񿽱�������ͼ��  
			
          	cout << i << endl;		
			cv::Mat imageROI = image(boundRect[i]);//����������Ӿ�����Ϣ���н�ȡRIO����Ȥ����ͼ��
			std::stringstream ss;//intת��Ϊstring
			std::string str;
			ss << i;
			ss >> str;
			if (WR == 1)
			{
				String name = "�ü�";
				//imshow(name + str + ".jpg", crop);   //���и��ͼ����жര�ڵ�չʾ
				Display(crop, "", name + str, WR);
			}
			else
			{
				String name = "�ü�";
				//imwrite("F://result//"+name + str + ".jpg", crop);//д���ļ���
				Display(crop, "F://result//", name + str, WR);
			}
		
			
			//imwrite("F://result//" + str + ".jpg", crop);//д���ļ���
		}
	}

	/// ��ʾ��һ������
	//namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	if (WR == 1) {
		//imshow("������עͼ", drawing);
		Display(drawing, "", "������עͼ", WR);
	}
	else
	{
		String name = "������עͼ";
	//	imwrite("F://result//" +name+ ".jpg", drawing);//д���ļ���
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
	namedWindow("ԭ��");
	Mat ssssssssssss;

	imshow("ԭ��", img);
	FindEdge(img,WR);
	//imshow("testX", img);
	//Mat img2 = imread("F://10086.jpg");
	ImageCut(img,image,WR);//1,չʾ�ļ���2д�ļ�
	waitKey();
	return 0;
}
