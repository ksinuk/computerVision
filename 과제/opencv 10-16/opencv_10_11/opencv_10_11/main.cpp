#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Mat image, corner2;

Mat draw_coner(Mat corner, Mat image, int thresh) //�ڳ� �׸���
{
	normalize(corner, corner, 0, 100, NORM_MINMAX, CV_32FC1, Mat()); //�ڷ����� float�� ��ȯ

	for (int i = 1; i < corner.rows - 1; i++) 
		for (int j = 1; j < corner.cols - 1; j++)
		{
			float cur = (int)corner.at<float>(i, j); //�ڳ� ���䰪 ����
			if (cur > thresh) //���䰪�� ������ �Ѿ
			{
				int ok=1;
				for(int y = -1; y<=1; y++) //8���� �˻�
					for(int x = -1; x<=1; x++)
					{
						if(y==0 && x==0)continue; //�ڱ� �ڽ��� �˻� �� ��
						if(cur <= corner.at<float>(i+y, j+x)) //��ǥ ������ �ִ밪�� ������
						{
							ok=0; //���� ��ǥ ���
							x = 100; y = 100; //���� Ż��
							break;
						}
					}

				//�ֿ� ��ǥ �߿��� �ڳ� ���䰪�� �ִ��̸� ȭ�鿡 �׸���
				if (ok) circle(image, Point(j, i), 2, Scalar(255, 0, 0), -1); 
			}
		}

	return image;
}

void cornerHarris_demo(int  thresh, void*) //Ʈ���� �̺�Ʈ ó�� �Լ�
{
	Mat img2 = draw_coner(corner2, image.clone(), thresh); //�ڳ� �׸���
	imshow("harris", img2); //����� ���
}

int main()
{
	image = imread("harris_test.jpg", 1); // �÷� �����Է�
	CV_Assert(image.data); //���� ó��

	int blockSize = 4;
	int apertureSize = 3;
	double k = 0.04;
	int  thresh = 20;
	Mat gray;

	cvtColor(image, gray, CV_BGR2GRAY);
	cornerHarris(gray, corner2, blockSize, apertureSize, k); //�ڳ� ���䰪 ���

	cornerHarris_demo(0, 0);
	createTrackbar("Threshold: ", "harris", &thresh, 100, cornerHarris_demo); //Ʈ���� �̺�Ʈ �Լ�
	waitKey();
}