#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("coin.jpg",IMREAD_COLOR); //이미지 입력
	Mat gray , filter , binary , output;

	cvtColor(img,gray,CV_BGR2GRAY); //명암도 영상 변환

	Size size(5,5);
	double sigmax = 0.3*((size.width-1)*0.5-1)+0.8;
	double sigmay = 0.3*((size.height-1)*0.5-1)+0.8;
	GaussianBlur(gray,filter,size,sigmax,sigmay); //가우스 블러링

	threshold(filter,binary,0,255,THRESH_OTSU); //otsu 연산을 이용한 이미지 이진화

	Mat mask(3,3,CV_8U,Scalar(1));
	morphologyEx(binary,output,MORPH_OPEN,mask); //이미지 오프닝 연산

	imshow("orignal",img); //원본 이미지
	imshow("output",output); //결과 이미지

	waitKey(0);

	return 0;
}