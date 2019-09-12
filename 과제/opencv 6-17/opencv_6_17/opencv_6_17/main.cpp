#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;

struct str_mouse //���콺 �̺�Ʈ �Լ����� ���� ���� ���� 
{
	Point start; //�巹�� ������
	Point end; //�巹�� ������
	int event; //���콺 �̺�Ʈ ����
	int flags; //���콺 �÷��� ����
	int prn; //ȭ��, ������׷� ��� ����
} mouse;


void func_mouse(int event,int x,int y,int flags,void *roi) //���콺 �̺�Ʈ �Լ�
{
	static Point start(0,0); //�巹�� ������
	Point end(0,0); //�巹�� ������

	mouse.event = event; //�̺�Ʈ ���� ����
	mouse.flags = flags;

	if(event==EVENT_LBUTTONDOWN) //������ ��ư ����
	{
		if(x<0) start.x = 0; //�������� 0���� ������ 0���� ����
		else start.x = x;
		if(y<0) start.y = 0;
		else start.y = y;

		mouse.start = start; //�������� ��ȯ
		mouse.end = start;
	}
	else if(event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON || event==EVENT_LBUTTONUP) //�巹�� �ϴ� ��
	{
		if(x<0) end.x = 0; //�������� 0���� ������ 0���� ����
		else end.x = x;
		if(y<0) end.y = 0;
		else end.y = y;

		mouse.start = start; //�������� ������ ��ȯ
		mouse.end = end;

		if(event==EVENT_LBUTTONUP) mouse.prn = 4; //�巹�װ� ������ ��â�� ȭ�� ��� �� ������׷� ���
	}
}

int hue_histogram(Mat &input , Mat &hist) //�÷� �̹����� hue���� ���� �� ������׷� ��� 
{
	Mat img;
	cvtColor(input,img,CV_BGR2HSV); //HSV�� ��ȯ

	Mat hsv[3];
	split(img,hsv); //hue�� ����

	int		histSize[] = { 24 };			// ������׷� ��ް���
	float   range[] = { 0.0, 180.0 };		// ä�� ȭ�Ұ� ����
	int		channels[] = { 0 };				// ä�� ���
	const float* ranges[] = { range };

	calcHist(&hsv[0], 1, channels, Mat(), hist, 1, histSize, ranges); //������׷� ���

	return 0;
}

Mat  make_palatte(int rows) // hue ä�ο� ���� ���� �ȷ�Ʈ ��� ����
{
	Mat hsv(rows, 1, CV_8UC3);
	for (int i = 0; i < rows; i++)
	{
		uchar hue = saturate_cast<uchar>((float)i / rows * 180);
		hsv.at<Vec3b>(i) = Vec3b(hue, 255, 255);
	}
	cvtColor(hsv, hsv, CV_HSV2BGR);
	return hsv;
}

void draw_histo_hue(Mat hist, Mat &hist_img, Size size = Size(256, 200)) //������׷� �÷� �׷��� �����
{
	Mat hsv_palatte = make_palatte(hist.rows);

	hist_img = Mat(size, CV_8UC3, Scalar(255, 255, 255));
	float  bin = (float)hist_img.cols / hist.rows;
	normalize(hist, hist, 0, hist_img.rows, NORM_MINMAX);

	for (int i = 0; i<hist.rows; i++)
	{
		float start_x = (i * bin);
		float  end_x = (i + 1) * bin;
		Point2f pt1(start_x, 0); //�׷��� ������ ������
		Point2f pt2(end_x, hist.at <float>(i)); //�׷��� ������ ������

		Scalar color = hsv_palatte.at<Vec3b>(i);				// �����ȷ�Ʈ ������
		if (pt2.y>0) rectangle(hist_img, pt1, pt2, color, -1);	// �ȷ�Ʈ �� �׸���	
	}
	flip(hist_img, hist_img, 0);
}

int main()
{
	Mat input = imread("color_space.jpg",IMREAD_COLOR); //���� �̹���
	Mat output = imread("color_space.jpg",IMREAD_COLOR);
	Point end(0,0);

	int height = input.rows; //�̹����� ����
	int width = input.cols; //�̹����� ��

	namedWindow("input");

	setMouseCallback("input",func_mouse,&mouse); //���콺 �̺�Ʈ �Լ� ����
	
	for(;;)
	{
		if(mouse.start.x >= width) mouse.start.x = width -1; //�巹�� ������ �̹������� ����� �̹��� �������� ����
		if(mouse.start.y>=height) mouse.start.y = height -1;
		if(mouse.end.x >= width) mouse.end.x = width -1;
		if(mouse.end.y>=height) mouse.end.y = height -1;
		
		Rect roi(mouse.start , mouse.end); //�巹���� �������� �������� �̿��ؼ� ���ɿ��� ����

		if(roi.width!=0 && roi.height!=0) //���ɿ����� ������ ������
		{
			input.copyTo(output); //���� ���ȭ��

			Mat select(input,roi); //�̹����� ���ɿ���
			Mat select_out(output,roi); //��������� �Ͼ�� ����
			Mat select_copy; //�� â�� ����� ���� ����
			select.copyTo(select_copy);
			Mat histogram,histo_img; //������׷� ���� ���

			select_out = 255-select_copy; //���� ����

			if(mouse.prn!=0) //�巹�װ� ����Ǹ� �� â�� ȭ��� ������׷� ����
			{
				hue_histogram(select_copy,histogram); //�÷� �̹����� hue���� ������׷� ���
				draw_histo_hue(histogram, histo_img); //������׷� �׷��� ���
				imshow("selcet",select_copy); //��â�� ��µǴ� ���ɿ���
				imshow("histogram",histo_img); //������׷� �׷��� ���

				mouse.prn=0; //ȭ�� ���� ����
			}
		}

		imshow("input",output); //���� ������ �Ͼ�� ���� ��� �̹���
		
		mouse.event=-1; //���콺 �̺�Ʈ ��ȯ ���� �ʱ�ȭ
		mouse.start = end; //��ǥ (0,0)
		mouse.end = end;
		
		if(waitKey(30)==27)return 0; //escŰ�� ������ ����
	}

	waitKey(0);
	return 0;
}