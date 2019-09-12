#include "tool.h"

label* project(Mat &img) //���� ������ ��ġ�� ã�� �Լ�
{
	Rect out_rect(Point(0,150),Point(img.cols-1,410)); //���� �̹��� �߿��� ������ ���� �� Ȯ���� ���� ���� 
	Rect sample_rect(Point(0,390),Point(img.cols-1,410)); //�̹��� �ȿ��� ���ΰ� ���� Ȯ���� ���� ����
	Mat img_cut = img(out_rect).clone(); //������ ã�� �̹���
	Mat sample_cut = img(sample_rect).clone(); //������ ������ �����ϴ� �̹���

	label *red_label = NULL; //������� �����ϴ� ���� ����Ʈ

	Mat hsv[3] , hsv_sample[3]; //������ �̹����� hsv���� �����ϴ� ���
	vector<Mat>hsv_vec , hsv_sample_vec; //���� ����� ����

	int mean[3] , stddev[3]; //���� ������ h,s,v���� ��հ� �л�

	cvtColor(img_cut,img_cut,COLOR_BGR2HSV); //������ �̹������� h,s,v���� �����Ѵ�.
	split(img_cut,hsv_vec); 
	cvtColor(sample_cut,sample_cut,COLOR_BGR2HSV);
	split(sample_cut,hsv_sample_vec);

	for(int i = 0; i<3; i++)
	{
		medianBlur(hsv_vec[i],hsv[i],3); //������ hsv�̹����� �߰��� ���ͷ� �����Ѵ�.
		medianBlur(hsv_sample_vec[i],hsv_sample[i],3);

		Mat mask(3,3,CV_8U,1);
		if(i!=0) morphologyEx(hsv[i],hsv[i],MORPH_OPEN,mask); //������ �������� ����� ä���� ������ �����Ѵ�.	
		if(i!=0) morphologyEx(hsv_sample[i],hsv_sample[i],MORPH_OPEN,mask);

		Mat mean_mat,stddev_mat; //������ hsv����� ��հ� ǥ������ ���� ���Ѵ�.
		meanStdDev(hsv_sample[i],mean_mat,stddev_mat);

		mean[i] = mean_mat.at<double>(0,0); //��հ� ǥ������ ���� �����Ѵ�.
		stddev[i] = stddev_mat.at<double>(0,0);
	}

	Mat chack_light(out_rect.size(),CV_8U,Scalar(0)); //������ ������ ����Ʈ�� ��ġ�� �κ��� ������ ����

	Mat red(out_rect.size(),CV_8U,Scalar(0)); //�̹������� ������ ������ ������ ���
	Mat green(out_rect.size(),CV_8U,Scalar(0));
	Mat yellow(out_rect.size(),CV_8U,Scalar(0));
	Mat sky(out_rect.size(),CV_8U,Scalar(0));
	Mat pink(out_rect.size(),CV_8U,Scalar(0));

	Mat red_plus(out_rect.size(),CV_8U,Scalar(0)); //������ ����Ʈ�� �����ϴ� �̹���

	for(int y=0;y<img_cut.rows;y++) //������ ������ �����Ѵ�.
		for(int x = 0; x<img_cut.cols; x++)
		{
			int hue = hsv[0].at<uchar>(y,x);
			int sat = hsv[1].at<uchar>(y,x);
			int inten = hsv[2].at<uchar>(y,x);
			int ok=0; //����� �ƴ� �÷� ������ ��Ÿ���� ����

			if(hue<=mean[0]+stddev[0]+1 && hue>=mean[0]-stddev[0]-1 && sat <=mean[1] + stddev[1]+1)
				ok = 0;
			else if(sat<=mean[1])
				ok = 0;
			else
				ok = 255;

			if(ok)
			{
				int h_out = ((hue+15)%180)/30; //������ ������ 6���� �ܼ�ȭ �Ѵ�.
				hsv[0].at<uchar>(y,x) = h_out*30;

				//�Ķ����� ������ ��� ������ �����Ѵ�.
				if(h_out==0) red.at<uchar>(y,x) = 255;
				else if(h_out==1) yellow.at<uchar>(y,x) = 255;
				else if(h_out==2) green.at<uchar>(y,x) = 255;
				else if(h_out==3) sky.at<uchar>(y,x) = 255;
				else if(h_out==5) pink.at<uchar>(y,x) = 255;
			}
			else hsv[0].at<uchar>(y,x) = 0;
		}

	for(int x = 0; x<yellow.cols; x++) //������ ����� �߾Ӽ��� �����Ѵ�.
	{
		int y = yellow.rows-1;
		if(yellow.at<uchar>(y,x)==255) delete_grass(yellow,y,x);
	}

	Mat mask7(7,7,CV_8U,1); //�������� ���꿡 ����ϴ� ����ũ ����
	Mat mask5(5,5,CV_8U,1);
	Mat mask3(3,3,CV_8U,1);

	morphologyEx(red , red ,MORPH_OPEN,mask3); //������ �������� ������ �����Ѵ�.
	morphologyEx(green,green,MORPH_OPEN,mask5);
	morphologyEx(yellow,yellow,MORPH_OPEN,mask5);
	morphologyEx(sky,sky,MORPH_OPEN,mask5);
	morphologyEx(pink,pink,MORPH_OPEN,mask3);

	morphologyEx(red , red ,MORPH_CLOSE,mask3); //Ŭ��¡ �������� ƴ���� �Ų۴�
	morphologyEx(green,green,MORPH_CLOSE,mask5);
	morphologyEx(yellow,yellow,MORPH_CLOSE,mask5);
	morphologyEx(sky,sky,MORPH_CLOSE,mask5);
	morphologyEx(pink,pink,MORPH_CLOSE,mask3);

	bitwise_or(pink , red, red); //������ ����Ʈ ���� ���Ѵ�.
	dilate(red , red_plus ,mask7);

	bitwise_or(green , yellow, chack_light);  //������ ������ ���Ѵ�.
	bitwise_or(sky , chack_light, chack_light);

	bitwise_and(red_plus , chack_light, chack_light); //����Ʈ�� ������ ��ġ�� �κ��� �����Ѵ�.

	for(int y=0;y<img_cut.rows;y++)
		for(int x = 0; x<img_cut.cols; x++)
		{
			if(chack_light.at<uchar>(y,x)==255)
			{
				if(yellow.at<uchar>(y,x)==255) //������ ������ ������ �ڵ����� ��ġ�� Rect�� ���Ѵ�.
					red_label=make_label(yellow , Point(x,y) , red_label , red);
				else if(green.at<uchar>(y,x)==255) 
					red_label=make_label(green , Point(x,y) , red_label , red);
				else if(sky.at<uchar>(y,x)==255) 
					red_label=make_label(sky , Point(x,y) , red_label , red);
			}
		}

	return red_label; //����� ���
}

