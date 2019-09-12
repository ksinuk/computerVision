#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("coin.jpg",IMREAD_COLOR); //�̹��� �Է�
	Mat gray , filter , binary , output;

	cvtColor(img,gray,CV_BGR2GRAY); //��ϵ� ���� ��ȯ

	Size size(5,5);
	double sigmax = 0.3*((size.width-1)*0.5-1)+0.8;
	double sigmay = 0.3*((size.height-1)*0.5-1)+0.8;
	GaussianBlur(gray,filter,size,sigmax,sigmay); //���콺 ����

	threshold(filter,binary,0,255,THRESH_OTSU); //otsu ������ �̿��� �̹��� ����ȭ

	Mat mask(3,3,CV_8U,Scalar(1));
	morphologyEx(binary,output,MORPH_OPEN,mask); //�̹��� ������ ����

	imshow("orignal",img); //���� �̹���
	imshow("output",output); //��� �̹���

	waitKey(0);

	return 0;
}