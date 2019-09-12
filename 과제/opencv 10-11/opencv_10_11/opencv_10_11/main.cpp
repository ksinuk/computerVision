#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("hough_test3.jpg",0); //�̹��� �Է�
	Mat outline , output;
	cvtColor(img,output,CV_GRAY2BGR); //�׸��� ��ȯ

	GaussianBlur(img,outline,Size(5,5),2,2); //����þ� ����
	Canny(outline,outline,100,200); //�ܰ��� ����

	double rho = 1, theta = 2*CV_PI / 180; //�Ÿ����� 1�ȼ� , ���� ���� 2��
	vector<Vec2f> lines;
	HoughLines(outline, lines, rho, theta, 100); //���� ����

	for (int i = 0; i < min((int)lines.size(), 10); i++) //�ִ� 10���� ���� �׸���
	{
		//���� �Ķ���� ����
		float rho = lines[i][0];
		float theta = lines[i][1];
		double a = cos(theta), b = sin(theta);

		double x0 = a*rho, y0 = b*rho; //������ ���� ����� ������ ��ǥ
		Point pt1(cvRound(x0 + 1000*(-b)), cvRound(y0 + 1000*(a)));
		Point pt2(cvRound(x0 - 1000*(-b)) , cvRound(y0 - 1000*(a)));
		line( output, pt1, pt2, Scalar(0,255,0), 2); //�Ķ���͸� �����ϴ� ���� �׸���
	}

	imshow("source", img); //���� �̹��� ���
	imshow("outline", outline); //�ܰ��� ���
	imshow("output", output); //����� ���
	waitKey();

	return 0;
}