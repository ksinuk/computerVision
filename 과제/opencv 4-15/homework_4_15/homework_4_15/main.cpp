#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("input.png",IMREAD_GRAYSCALE); //�̹��� ���� �׷��̷� �б�
	CV_Assert(img.data); //���� ó��

	imshow("homework",img); //ȭ�� ���
	waitKey(0);

	vector<int> int_jpg,int_png;
	int_jpg.push_back(IMWRITE_JPEG_CHROMA_QUALITY); //JPG ȭ�� ����
	int_jpg.push_back(100);
	int_png.push_back(IMWRITE_PNG_COMPRESSION); //PNG ȭ�� ����
	int_png.push_back(9);

	imwrite("test.jpg",img,int_jpg); //JPG ����
	imwrite("test.png",img,int_png); //PNG ����

	return 0;
}



