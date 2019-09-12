#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat img(400,600,CV_8UC3,Scalar(255,255,255)); //��� ���
	Scalar red(0,0,255) , blue(255,0,0);

	ellipse(img,Point(300,200),Size(100,100),0,0,180,blue,-1); //�±� �ϴ�
	ellipse(img,Point(300,200),Size(100,100),0,180,360,red,-1); //�±� ���
	ellipse(img,Point(250,200),Size(50,50),0,0,360,red,-1); //�±� ������ ����
	ellipse(img,Point(350,200),Size(50,50),0,0,360,blue,-1); //�±� ���� ����

	imshow("homework",img);
	waitKey(0);
	return 0;
}


