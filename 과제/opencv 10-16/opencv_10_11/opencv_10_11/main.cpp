#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Mat image, corner2;

Mat draw_coner(Mat corner, Mat image, int thresh) //코너 그리기
{
	normalize(corner, corner, 0, 100, NORM_MINMAX, CV_32FC1, Mat()); //자료형을 float로 변환

	for (int i = 1; i < corner.rows - 1; i++) 
		for (int j = 1; j < corner.cols - 1; j++)
		{
			float cur = (int)corner.at<float>(i, j); //코너 응답값 추출
			if (cur > thresh) //응답값이 기준을 넘어섬
			{
				int ok=1;
				for(int y = -1; y<=1; y++) //8방향 검사
					for(int x = -1; x<=1; x++)
					{
						if(y==0 && x==0)continue; //자기 자신을 검사 안 함
						if(cur <= corner.at<float>(i+y, j+x)) //좌표 주위에 최대값이 존재함
						{
							ok=0; //다음 좌표 계산
							x = 100; y = 100; //루프 탈출
							break;
						}
					}

				//주외 좌표 중에서 코너 응답값이 최대이면 화면에 그리기
				if (ok) circle(image, Point(j, i), 2, Scalar(255, 0, 0), -1); 
			}
		}

	return image;
}

void cornerHarris_demo(int  thresh, void*) //트렉바 이벤트 처리 함수
{
	Mat img2 = draw_coner(corner2, image.clone(), thresh); //코너 그리기
	imshow("harris", img2); //결과물 출력
}

int main()
{
	image = imread("harris_test.jpg", 1); // 컬러 영상입력
	CV_Assert(image.data); //예외 처리

	int blockSize = 4;
	int apertureSize = 3;
	double k = 0.04;
	int  thresh = 20;
	Mat gray;

	cvtColor(image, gray, CV_BGR2GRAY);
	cornerHarris(gray, corner2, blockSize, apertureSize, k); //코너 응답값 계산

	cornerHarris_demo(0, 0);
	createTrackbar("Threshold: ", "harris", &thresh, 100, cornerHarris_demo); //트렉바 이벤트 함수
	waitKey();
}