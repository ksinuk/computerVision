#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;

struct str_mouse //마우스 이벤트 함수에서 전달 받을 변수 
{
	Point start; //드레그 시작점
	Point end; //드레그 종료점
	int event; //마우스 이벤트 종류
	int flags; //마우스 플레그 종류
	int prn; //화면, 히스토그램 출력 여부
} mouse;


void func_mouse(int event,int x,int y,int flags,void *roi) //마우스 이벤트 함수
{
	static Point start(0,0); //드레그 시작점
	Point end(0,0); //드레그 종료점

	mouse.event = event; //이벤트 변수 저장
	mouse.flags = flags;

	if(event==EVENT_LBUTTONDOWN) //오른쪽 버튼 누름
	{
		if(x<0) start.x = 0; //시작점이 0보다 작으면 0으로 변경
		else start.x = x;
		if(y<0) start.y = 0;
		else start.y = y;

		mouse.start = start; //시작점만 반환
		mouse.end = start;
	}
	else if(event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON || event==EVENT_LBUTTONUP) //드레그 하는 중
	{
		if(x<0) end.x = 0; //종료점이 0보다 작으면 0으로 변경
		else end.x = x;
		if(y<0) end.y = 0;
		else end.y = y;

		mouse.start = start; //시작점과 종료점 반환
		mouse.end = end;

		if(event==EVENT_LBUTTONUP) mouse.prn = 4; //드레그가 끝나면 새창에 화면 출력 및 히스토그램 계산
	}
}

int hue_histogram(Mat &input , Mat &hist) //컬러 이미지의 hue값을 추출 후 히스토그램 계산 
{
	Mat img;
	cvtColor(input,img,CV_BGR2HSV); //HSV로 변환

	Mat hsv[3];
	split(img,hsv); //hue값 추출

	int		histSize[] = { 24 };			// 히스토그램 계급개수
	float   range[] = { 0.0, 180.0 };		// 채널 화소값 범위
	int		channels[] = { 0 };				// 채널 목록
	const float* ranges[] = { range };

	calcHist(&hsv[0], 1, channels, Mat(), hist, 1, histSize, ranges); //히스토그램 계산

	return 0;
}

Mat  make_palatte(int rows) // hue 채널에 대한 색상 팔레트 행렬 생성
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

void draw_histo_hue(Mat hist, Mat &hist_img, Size size = Size(256, 200)) //히스토그램 컬러 그래프 만들기
{
	Mat hsv_palatte = make_palatte(hist.rows);

	hist_img = Mat(size, CV_8UC3, Scalar(255, 255, 255));
	float  bin = (float)hist_img.cols / hist.rows;
	normalize(hist, hist, 0, hist_img.rows, NORM_MINMAX);

	for (int i = 0; i<hist.rows; i++)
	{
		float start_x = (i * bin);
		float  end_x = (i + 1) * bin;
		Point2f pt1(start_x, 0); //그래프 막대의 시작점
		Point2f pt2(end_x, hist.at <float>(i)); //그래프 막대의 종료점

		Scalar color = hsv_palatte.at<Vec3b>(i);				// 색상팔레트 색지정
		if (pt2.y>0) rectangle(hist_img, pt1, pt2, color, -1);	// 팔레트 색 그리기	
	}
	flip(hist_img, hist_img, 0);
}

int main()
{
	Mat input = imread("color_space.jpg",IMREAD_COLOR); //원본 이미지
	Mat output = imread("color_space.jpg",IMREAD_COLOR);
	Point end(0,0);

	int height = input.rows; //이미지의 높이
	int width = input.cols; //이미지의 폭

	namedWindow("input");

	setMouseCallback("input",func_mouse,&mouse); //마우스 이벤트 함수 지정
	
	for(;;)
	{
		if(mouse.start.x >= width) mouse.start.x = width -1; //드레그 지점이 이미지에서 벗어나면 이미지 안쪽으로 조정
		if(mouse.start.y>=height) mouse.start.y = height -1;
		if(mouse.end.x >= width) mouse.end.x = width -1;
		if(mouse.end.y>=height) mouse.end.y = height -1;
		
		Rect roi(mouse.start , mouse.end); //드레그의 시작점과 종료점을 이용해서 관심영역 지정

		if(roi.width!=0 && roi.height!=0) //관심영역이 실제로 존재함
		{
			input.copyTo(output); //메인 출력화면

			Mat select(input,roi); //이미지의 관심영역
			Mat select_out(output,roi); //색상반전이 일어나는 영역
			Mat select_copy; //새 창에 출력할 관심 영역
			select.copyTo(select_copy);
			Mat histogram,histo_img; //히스토그램 관련 행렬

			select_out = 255-select_copy; //색상 반전

			if(mouse.prn!=0) //드레그가 종료되면 생 창의 화면과 히스토그램 갱신
			{
				hue_histogram(select_copy,histogram); //컬러 이미지의 hue값의 히스토그램 계산
				draw_histo_hue(histogram, histo_img); //히스토그램 그래프 출력
				imshow("selcet",select_copy); //새창에 출력되는 관심영역
				imshow("histogram",histo_img); //히스토그램 그래프 출력

				mouse.prn=0; //화면 갱신 종료
			}
		}

		imshow("input",output); //색상 반전이 일어나는 메인 출력 이미지
		
		mouse.event=-1; //마우스 이벤트 반환 변수 초기화
		mouse.start = end; //좌표 (0,0)
		mouse.end = end;
		
		if(waitKey(30)==27)return 0; //esc키를 누르면 종료
	}

	waitKey(0);
	return 0;
}