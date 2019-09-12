#include "tool.h"

label* project(Mat &img) //실제 차량의 위치를 찾는 함수
{
	Rect out_rect(Point(0,150),Point(img.cols-1,410)); //실제 이미지 중에서 차량이 존재 할 확률이 높은 영역 
	Rect sample_rect(Point(0,390),Point(img.cols-1,410)); //이미지 안에서 도로가 있을 확률이 높은 영역
	Mat img_cut = img(out_rect).clone(); //차량을 찾을 이미지
	Mat sample_cut = img(sample_rect).clone(); //도로의 샘플을 추출하는 이미지

	label *red_label = NULL; //결과물을 저장하는 연결 리스트

	Mat hsv[3] , hsv_sample[3]; //각각의 이미지의 hsv값을 저장하는 행렬
	vector<Mat>hsv_vec , hsv_sample_vec; //위의 행렬의 원본

	int mean[3] , stddev[3]; //도로 샘플의 h,s,v값의 평균과 분산

	cvtColor(img_cut,img_cut,COLOR_BGR2HSV); //각각의 이미지에서 h,s,v값을 추출한다.
	split(img_cut,hsv_vec); 
	cvtColor(sample_cut,sample_cut,COLOR_BGR2HSV);
	split(sample_cut,hsv_sample_vec);

	for(int i = 0; i<3; i++)
	{
		medianBlur(hsv_vec[i],hsv[i],3); //추출한 hsv이미지를 중간값 필터로 정리한다.
		medianBlur(hsv_sample_vec[i],hsv_sample[i],3);

		Mat mask(3,3,CV_8U,1);
		if(i!=0) morphologyEx(hsv[i],hsv[i],MORPH_OPEN,mask); //오프닝 연산으로 조명과 채도의 잡음을 정리한다.	
		if(i!=0) morphologyEx(hsv_sample[i],hsv_sample[i],MORPH_OPEN,mask);

		Mat mean_mat,stddev_mat; //각각의 hsv행렬의 평균과 표준편차 값을 구한다.
		meanStdDev(hsv_sample[i],mean_mat,stddev_mat);

		mean[i] = mean_mat.at<double>(0,0); //평균과 표준편차 값을 저장한다.
		stddev[i] = stddev_mat.at<double>(0,0);
	}

	Mat chack_light(out_rect.size(),CV_8U,Scalar(0)); //차량과 차량의 라이트가 겹치는 부분을 추출한 영역

	Mat red(out_rect.size(),CV_8U,Scalar(0)); //이미지에서 각각의 색상을 추출한 행렬
	Mat green(out_rect.size(),CV_8U,Scalar(0));
	Mat yellow(out_rect.size(),CV_8U,Scalar(0));
	Mat sky(out_rect.size(),CV_8U,Scalar(0));
	Mat pink(out_rect.size(),CV_8U,Scalar(0));

	Mat red_plus(out_rect.size(),CV_8U,Scalar(0)); //차량의 라이트를 저장하는 이미지

	for(int y=0;y<img_cut.rows;y++) //각각의 색상을 추출한다.
		for(int x = 0; x<img_cut.cols; x++)
		{
			int hue = hsv[0].at<uchar>(y,x);
			int sat = hsv[1].at<uchar>(y,x);
			int inten = hsv[2].at<uchar>(y,x);
			int ok=0; //흑백이 아닌 컬러 유무를 나타내는 변수

			if(hue<=mean[0]+stddev[0]+1 && hue>=mean[0]-stddev[0]-1 && sat <=mean[1] + stddev[1]+1)
				ok = 0;
			else if(sat<=mean[1])
				ok = 0;
			else
				ok = 255;

			if(ok)
			{
				int h_out = ((hue+15)%180)/30; //색상의 갯수를 6개로 단순화 한다.
				hsv[0].at<uchar>(y,x) = h_out*30;

				//파란색은 제외한 모든 색상을 추출한다.
				if(h_out==0) red.at<uchar>(y,x) = 255;
				else if(h_out==1) yellow.at<uchar>(y,x) = 255;
				else if(h_out==2) green.at<uchar>(y,x) = 255;
				else if(h_out==3) sky.at<uchar>(y,x) = 255;
				else if(h_out==5) pink.at<uchar>(y,x) = 255;
			}
			else hsv[0].at<uchar>(y,x) = 0;
		}

	for(int x = 0; x<yellow.cols; x++) //도로의 노란색 중앙선을 제거한다.
	{
		int y = yellow.rows-1;
		if(yellow.at<uchar>(y,x)==255) delete_grass(yellow,y,x);
	}

	Mat mask7(7,7,CV_8U,1); //모폴로지 연산에 사용하는 마스크 모음
	Mat mask5(5,5,CV_8U,1);
	Mat mask3(3,3,CV_8U,1);

	morphologyEx(red , red ,MORPH_OPEN,mask3); //오프닝 연산으로 잡음을 제거한다.
	morphologyEx(green,green,MORPH_OPEN,mask5);
	morphologyEx(yellow,yellow,MORPH_OPEN,mask5);
	morphologyEx(sky,sky,MORPH_OPEN,mask5);
	morphologyEx(pink,pink,MORPH_OPEN,mask3);

	morphologyEx(red , red ,MORPH_CLOSE,mask3); //클로징 연산으로 틈새를 매꾼다
	morphologyEx(green,green,MORPH_CLOSE,mask5);
	morphologyEx(yellow,yellow,MORPH_CLOSE,mask5);
	morphologyEx(sky,sky,MORPH_CLOSE,mask5);
	morphologyEx(pink,pink,MORPH_CLOSE,mask3);

	bitwise_or(pink , red, red); //빨간색 라이트 값을 구한다.
	dilate(red , red_plus ,mask7);

	bitwise_or(green , yellow, chack_light);  //차량의 색상을 구한다.
	bitwise_or(sky , chack_light, chack_light);

	bitwise_and(red_plus , chack_light, chack_light); //라이트와 차량이 겹치는 부분을 추출한다.

	for(int y=0;y<img_cut.rows;y++)
		for(int x = 0; x<img_cut.cols; x++)
		{
			if(chack_light.at<uchar>(y,x)==255)
			{
				if(yellow.at<uchar>(y,x)==255) //각각의 색상을 가지는 자동차의 위치를 Rect로 구한다.
					red_label=make_label(yellow , Point(x,y) , red_label , red);
				else if(green.at<uchar>(y,x)==255) 
					red_label=make_label(green , Point(x,y) , red_label , red);
				else if(sky.at<uchar>(y,x)==255) 
					red_label=make_label(sky , Point(x,y) , red_label , red);
			}
		}

	return red_label; //결과물 출력
}

