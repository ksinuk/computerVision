#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("input.png",IMREAD_GRAYSCALE); //이미지 파일 그레이로 읽기
	CV_Assert(img.data); //예외 처리

	imshow("homework",img); //화면 출력
	waitKey(0);

	vector<int> int_jpg,int_png;
	int_jpg.push_back(IMWRITE_JPEG_CHROMA_QUALITY); //JPG 화질 설정
	int_jpg.push_back(100);
	int_png.push_back(IMWRITE_PNG_COMPRESSION); //PNG 화질 설정
	int_png.push_back(9);

	imwrite("test.jpg",img,int_jpg); //JPG 저장
	imwrite("test.png",img,int_png); //PNG 저장

	return 0;
}



