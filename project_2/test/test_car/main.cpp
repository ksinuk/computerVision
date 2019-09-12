#include "main_header.h"



int main()
{
	string address = "C:/Users/ghttt/Desktop/컴퓨터비전/project_2/training/";
	const int kDatasetNumber = 40;

	for(int number = 0; number<kDatasetNumber; number++)
	{
		Mat img = imread(address + "image/" + to_string(number) + ".bmp",1);
		Mat print_img(img.rows,img.cols*2,CV_8UC3,Scalar(0,0,0));

		Rect img_rect(0,0,img.cols,img.rows);
		Rect out_rect(img.cols,0,img.cols,img.rows);

		Mat orginal = print_img(img_rect);
		Mat output = print_img(out_rect);
		img.copyTo(orginal);

		project(img,output,number);
		cout << number << endl;

		imwrite(address  + "img_test/" + to_string(number) + ".png",print_img);
	}



	return 0;
}

int project(Mat &img,Mat &output , int number)
{
	Rect out_rect(Point(0,150),Point(img.cols-1,410));
	Rect sample_rect(Point(0,390),Point(img.cols-1,410));
	Mat img_cut = img(out_rect).clone();
	Mat sample_cut = img(sample_rect).clone();
	Mat prn_cut = output(out_rect);

	label *red_label = NULL;

	Mat hsv[3] , hsv_sample[3];
	vector<Mat>hsv_vec , hsv_sample_vec;

	int cut[3] , mean[3] , stddev[3];

	cvtColor(img_cut,img_cut,COLOR_BGR2HSV);
	split(img_cut,hsv_vec);
	cvtColor(sample_cut,sample_cut,COLOR_BGR2HSV);
	split(sample_cut,hsv_sample_vec);

	for(int i = 0; i<3; i++)
	{
		medianBlur(hsv_vec[i],hsv[i],3);
		medianBlur(hsv_sample_vec[i],hsv_sample[i],3);

		Mat mask(3,3,CV_8U,1);
		if(i!=0) morphologyEx(hsv[i],hsv[i],MORPH_OPEN,mask);		
		if(i!=0) morphologyEx(hsv_sample[i],hsv_sample[i],MORPH_OPEN,mask);

		Mat mean_mat,stddev_mat;
		meanStdDev(hsv_sample[i],mean_mat,stddev_mat);

		mean[i] = mean_mat.at<double>(0,0);
		stddev[i] = stddev_mat.at<double>(0,0);
	}

	Mat size_mat(out_rect.size(),CV_8U,Scalar(0));
	Mat chack_light(out_rect.size(),CV_8U,Scalar(0));

	Mat red(out_rect.size(),CV_8U,Scalar(0));
	Mat green(out_rect.size(),CV_8U,Scalar(0));
	Mat yellow(out_rect.size(),CV_8U,Scalar(0));
	Mat sky(out_rect.size(),CV_8U,Scalar(0));
	Mat pink(out_rect.size(),CV_8U,Scalar(0));

	Mat red_plus(out_rect.size(),CV_8U,Scalar(0));

	for(int y=0;y<img_cut.rows;y++)
		for(int x = 0; x<img_cut.cols; x++)
		{
			int hue = hsv[0].at<uchar>(y,x);
			int sat = hsv[1].at<uchar>(y,x);
			int inten = hsv[2].at<uchar>(y,x);
			int ok=0;


			if(hue<=mean[0]+stddev[0]+1 && hue>=mean[0]-stddev[0]-1 && sat <=mean[1] + stddev[1]+1)
				ok = 0;
			else if(sat<=mean[1])
				ok = 0;
			else
				ok = 255;

			if(ok)
			{
				int h_out = ((hue+15)%180)/30;
				hsv[0].at<uchar>(y,x) = h_out*30;

				if(h_out==0) red.at<uchar>(y,x) = 255;
				else if(h_out==1) yellow.at<uchar>(y,x) = 255;
				else if(h_out==2) green.at<uchar>(y,x) = 255;
				else if(h_out==3) sky.at<uchar>(y,x) = 255;
				else if(h_out==5) pink.at<uchar>(y,x) = 255;
			}
			else hsv[0].at<uchar>(y,x) = 0;
		}

	for(int x = 0; x<yellow.cols; x++)
	{
		int y = yellow.rows-1;
		if(yellow.at<uchar>(y,x)==255) delete_grass(yellow,y,x);
	}

	Mat mask7(7,7,CV_8U,1);
	Mat mask5(5,5,CV_8U,1);
	Mat mask3(3,3,CV_8U,1);
	morphologyEx(red , red ,MORPH_OPEN,mask3);
	morphologyEx(green,green,MORPH_OPEN,mask5);
	morphologyEx(yellow,yellow,MORPH_OPEN,mask5);
	morphologyEx(sky,sky,MORPH_OPEN,mask5);
	morphologyEx(pink,pink,MORPH_OPEN,mask3);

	morphologyEx(red , red ,MORPH_CLOSE,mask3);
	morphologyEx(green,green,MORPH_CLOSE,mask5);
	morphologyEx(yellow,yellow,MORPH_CLOSE,mask5);
	morphologyEx(sky,sky,MORPH_CLOSE,mask5);
	morphologyEx(pink,pink,MORPH_CLOSE,mask3);

	bitwise_or(red , yellow, size_mat); 
	bitwise_or(green , size_mat, size_mat); 
	bitwise_or(sky , size_mat, size_mat); 
	bitwise_or(pink , size_mat, size_mat); 

	bitwise_or(pink , red, red);
	dilate(red , red_plus ,mask7);
	bitwise_or(green , yellow, chack_light); 
	bitwise_or(sky , chack_light, chack_light);
	bitwise_and(red_plus , chack_light, chack_light); 

	for(int y=0;y<img_cut.rows;y++)
		for(int x = 0; x<img_cut.cols; x++)
		{
			if(chack_light.at<uchar>(y,x)==255)
			{
				if(yellow.at<uchar>(y,x)==255) red_label=make_label(yellow , Point(x,y) , red_label , red);
				else if(green.at<uchar>(y,x)==255) red_label=make_label(green , Point(x,y) , red_label , red);
				else if(sky.at<uchar>(y,x)==255) red_label=make_label(sky , Point(x,y) , red_label , red);
			}
		}

	for(int y=0;y<img_cut.rows;y++)
		for(int x = 0; x<img_cut.cols; x++)
		{
			if( size_mat.at<uchar>(y,x)==0)
			{
				hsv[1].at<uchar>(y,x) = 0;
				hsv[2].at<uchar>(y,x) = 0;
			}
			else
			{
				hsv[1].at<uchar>(y,x) = 255;
				hsv[2].at<uchar>(y,x) = 180;
			}
		}

	merge(hsv,3,prn_cut);
	cvtColor(prn_cut,prn_cut,COLOR_HSV2BGR);
	paint_label(prn_cut , red_label);

	delete_label_list(red_label);
	return 0;
}

