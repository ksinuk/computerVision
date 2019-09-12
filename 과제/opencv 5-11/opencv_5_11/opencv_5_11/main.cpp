#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	VideoCapture  capture(0);
	if (!capture.isOpened())
	{
		cout << "카메라가 연결되지 않았습니다." << endl;
		exit(1);
	}

	capture.set(CAP_PROP_FRAME_HEIGHT,240);
	capture.set(CAP_PROP_FRAME_WIDTH,320);
	
	for (;;) 
	{
		Mat frame;
		capture.read(frame); //카메라로 1프레임 촬영

		Mat output(300,400,CV_8UC3,Scalar(0,0,0)); //출력 화면

		Rect roi(30,30,320,240); //관심영역

		Mat out_frame(output,roi); //출력 화면에 관심영역 지정
		
		frame.copyTo(out_frame); //출력화면의 관심영역에 촬영화면 복사

		rectangle(output,Rect(30,30,320,240),Scalar(0,0,255),3); //관심영역 외곽선 그리기

		imshow("homework", output); //하면을 모니터에 출력
		if (waitKey(30) == 27) break; //ecs키를 누르면 종료
	}
	return 0;
}