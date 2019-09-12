#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	VideoCapture capture(0); //카메라 객체 선언 및 연결
	CV_Assert(capture.isOpened()); //예외 처리

	capture.set(CAP_PROP_FRAME_HEIGHT,480); //해상도 조절
	capture.set(CAP_PROP_FRAME_WIDTH,640);

	for(;;)
	{
		Mat frame; 
		capture.read(frame); //카메라 프레임 읽기
		
		//관심 영역 편집
		Range r1(100,300),r2(200,300); //관심 영역의 위치
		Mat control = frame(r1,r2); //관심 영역 설정
		control += Scalar(0,50,0); //녹색 성분 추가

		//관심 영역 위치 그리기
		Rect range(200,100,100,200); //관심 영역의 테두리
		rectangle(frame,range,Scalar(0,0,255),3);

		imshow("homework",frame); //모니터에 출력
		if(waitKey(30)==27) break; //esc키를 누르면 종료
	}

	return 0;
}


