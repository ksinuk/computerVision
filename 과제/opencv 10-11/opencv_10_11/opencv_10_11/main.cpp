#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("hough_test3.jpg",0); //이미지 입력
	Mat outline , output;
	cvtColor(img,output,CV_GRAY2BGR); //그리드 변환

	GaussianBlur(img,outline,Size(5,5),2,2); //가우시안 블러링
	Canny(outline,outline,100,200); //외곽선 추출

	double rho = 1, theta = 2*CV_PI / 180; //거리간격 1픽셀 , 각도 간격 2도
	vector<Vec2f> lines;
	HoughLines(outline, lines, rho, theta, 100); //직선 추출

	for (int i = 0; i < min((int)lines.size(), 10); i++) //최대 10개의 직선 그리기
	{
		//직선 파라미터 복원
		float rho = lines[i][0];
		float theta = lines[i][1];
		double a = cos(theta), b = sin(theta);

		double x0 = a*rho, y0 = b*rho; //원점과 가장 가까운 직선의 좌표
		Point pt1(cvRound(x0 + 1000*(-b)), cvRound(y0 + 1000*(a)));
		Point pt2(cvRound(x0 - 1000*(-b)) , cvRound(y0 - 1000*(a)));
		line( output, pt1, pt2, Scalar(0,255,0), 2); //파라미터를 만족하는 직선 그리기
	}

	imshow("source", img); //원본 이미지 출력
	imshow("outline", outline); //외곽선 출력
	imshow("output", output); //결과물 출력
	waitKey();

	return 0;
}