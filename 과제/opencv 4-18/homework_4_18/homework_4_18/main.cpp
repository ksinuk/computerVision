#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	VideoCapture capture(0); //ī�޶� ��ü ���� �� ����
	CV_Assert(capture.isOpened()); //���� ó��

	capture.set(CAP_PROP_FRAME_HEIGHT,480); //�ػ� ����
	capture.set(CAP_PROP_FRAME_WIDTH,640);

	for(;;)
	{
		Mat frame; 
		capture.read(frame); //ī�޶� ������ �б�
		
		//���� ���� ����
		Range r1(100,300),r2(200,300); //���� ������ ��ġ
		Mat control = frame(r1,r2); //���� ���� ����
		control += Scalar(0,50,0); //��� ���� �߰�

		//���� ���� ��ġ �׸���
		Rect range(200,100,100,200); //���� ������ �׵θ�
		rectangle(frame,range,Scalar(0,0,255),3);

		imshow("homework",frame); //����Ϳ� ���
		if(waitKey(30)==27) break; //escŰ�� ������ ����
	}

	return 0;
}


