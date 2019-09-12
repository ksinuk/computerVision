#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	VideoCapture  capture(0);
	if (!capture.isOpened())
	{
		cout << "ī�޶� ������� �ʾҽ��ϴ�." << endl;
		exit(1);
	}

	capture.set(CAP_PROP_FRAME_HEIGHT,240);
	capture.set(CAP_PROP_FRAME_WIDTH,320);
	
	for (;;) 
	{
		Mat frame;
		capture.read(frame); //ī�޶�� 1������ �Կ�

		Mat output(300,400,CV_8UC3,Scalar(0,0,0)); //��� ȭ��

		Rect roi(30,30,320,240); //���ɿ���

		Mat out_frame(output,roi); //��� ȭ�鿡 ���ɿ��� ����
		
		frame.copyTo(out_frame); //���ȭ���� ���ɿ����� �Կ�ȭ�� ����

		rectangle(output,Rect(30,30,320,240),Scalar(0,0,255),3); //���ɿ��� �ܰ��� �׸���

		imshow("homework", output); //�ϸ��� ����Ϳ� ���
		if (waitKey(30) == 27) break; //ecsŰ�� ������ ����
	}
	return 0;
}