#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#include <iostream>
using namespace std;

int n_boards = 0; 
const int board_dt = 40; // 40 frame 마다 영상을 획득
int board_w;             // 가로방향의 교차점의 수  
int board_h;              // 세로방향의 교차점의 수

int main()
{
	cout << "board width : ";  
	cin >> board_w;                // 가로 방향의 교차점의 수를 입력 받는다.
	cout << "board height : ";
	cin >> board_h;                // 세로 방향의 교차점의 수를 입력 받는다.
	cout << "board number : ";
	cin >> n_boards;               // 추후 획득할 영상의 수를 입력받는다.

	int board_n = board_w * board_h; // 총교차점의 수를 계산한다.

	CvSize board_sz = cvSize( board_w, board_h );
	CvCapture* capture = cvCreateCameraCapture( 0 ); // index0번의 카메라의 핸들을 획득한다.
	assert( capture );
	cvNamedWindow("Calibration"); // 영상을 디스플레이할 "Calibration"이란 창을 생성한다.

	CvMat* image_points = cvCreateMat(n_boards*board_n,2,CV_32FC1);
	// 추후 각 frame마다의 영상에서 획득한 교차점의 포인트가 저장될 메트릭스 
	CvMat* object_points = cvCreateMat(n_boards*board_n,3,CV_32FC1);
	// 추후 각 frame마다의 실제 교차점의 관계가 저장될 메트릭스
	CvMat* point_counts = cvCreateMat(n_boards,1,CV_32SC1);
	
	CvMat* intrinsic_matrix = cvCreateMat(3,3,CV_32FC1);
	// 추후 내부 파라미터가 저장될 메트릭스 
	CvMat* distortion_coeffs = cvCreateMat(4,1,CV_32FC1);
	// 추후 distortion 파라미터가 저장될 메트릭스 


	CvMat* rotation_matrices = cvCreateMat(n_boards,9,CV_32FC1);
	// 추후 각 frame과 카메라의 회전관계가 저장될 메트릭스 
	CvMat* translation_vectors = cvCreateMat(n_boards,3,CV_32FC1);
	// 추후 각 frame과 카메라의 위치관계가 저장될 메트릭스
	CvPoint2D32f* corners = new CvPoint2D32f[ board_n ];
	// 추후 교차점들의 좌표가 기록될 배열

	int corner_count; 		// 획득한 교차점의 수
	int successes = 0;		// 올바른 이미지 획득 횟수
	int step, frame = 0;		
	
	IplImage *image = cvQueryFrame( capture );  // 영상을 획득
	IplImage *gray_image = cvCreateImage(cvGetSize(image),8,1); 
	// 획득 영상과 같은 크기의 흑백 영상의 생성 

	while(successes < n_boards) // 올바른 이미지의 획득 횟수가 입력 받은 목표가 될 때까지의 루프
	{
		if(frame++ % board_dt == 0) // 40 frame마다 실행되는 분기문  
		{
			int found = cvFindChessboardCorners(
				image, board_sz, corners, &corner_count,
				CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS
				); 
			/* 
			   교차점을 찾는 함수 corners에는 교차점들의 영상에서의 좌표들이 
                           corner_count 교차점들의 수가 함수에 의해서 기록되어지며 
			   리턴 값으로 탐색의 성공 및 실패 여부를 알려준다..
			*/

			cvCvtColor(image, gray_image, CV_BGR2GRAY);
			/*
			  cvFindCornerSubPix함수를 이용하여 SubPixel을 찾기 위해서 
			  cvFindCornerSubPix함수가 흑백영상 기반이므로 획득영상을 흑백영상으로
			  변환 후 gray_image에 복사한다. 	
			*/

			cvFindCornerSubPix(gray_image, corners, corner_count,
				cvSize(11,11),cvSize(-1,-1), cvTermCriteria(
				CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
			/*
			  SubPixel을 찾기 위한 함수로써 cvSize(11,11)는 탐색 윈도우의 넓이/2 +1,
			  탐색 윈도우의 높이/2 +1이란 의미로 탐색윈도우의 높이및 넓이가 20이라는 
			  의미이다.    	
			*/
			

			cvDrawChessboardCorners(image, board_sz, corners,corner_count, found);
			// 원본이미지에 탐색된 좌표를 표시한다. 성공여부에 따라 표시되는 방법이 다르다.
			cvShowImage("Calibration", image );
			// 영상을 "Calibration"이란 이름을 가진 윈도우에 디스플레이한다.

			if( corner_count == board_n && found)
			// 설정한 조건에 맞는 획득 결과를 가질 때에   
			{
				step = successes*board_n; // 데이터 삽입 인덱스 계산
				for( int i=step, j=0; j<board_n; ++i,++j ) {

					CV_MAT_ELEM(*image_points, float,i,0) = corners[j].x;
					CV_MAT_ELEM(*image_points, float,i,1) = corners[j].y;

					// image_points 메트릭스에 영상 내의 좌표를 설정

					CV_MAT_ELEM(*object_points,float,i,0) = j/board_w;
					CV_MAT_ELEM(*object_points,float,i,1) = j%board_w;
					CV_MAT_ELEM(*object_points,float,i,2) = 0.0f;

					/* 
   					  object_points 메트릭스는 각각의 frame의 교차점 간의 관계를
					  정의한다. 교차점들은 이웃간에 1이란 동일한 간격을 가지고 
					  있으며 한평면에 존재하고 있기 때문에 깊이는 0이다.
					*/  
				}
				CV_MAT_ELEM(*point_counts, int,successes,0) = board_n;
				printf("success %d/%d\n\n",successes,n_boards);
				successes++; 		// 올바른 영상의 획득을 카운트한다. 
			}
		} 
		image = cvQueryFrame( capture ); 	// 영상을 획득한다.
		cvWaitKey(15);						
	} 

	CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0f;
	CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1 ) = 1.0f;
	// intrinsic_matrix의 초기화 

	cvCalibrateCamera2(
		object_points, image_points,point_counts, cvGetSize( image ),intrinsic_matrix,
		 distortion_coeffs, rotation_matrices, translation_vectors,0 );
		
		 /*  cvCalibrateCamera2 함수를 이용하여 칼리브레이션을 한다.
		 intrinsic_matrix에는 카메라 내부 파라미터가 distortion_coeffs에는 distortion값이
		 rotation_matrices, translation_vectors에는 체스보드와 카메라간의 회전 및 위치
		 관계에 대한 값이 함수에 의해서 저장되게 된다.	*/

	cvSave("Intrinsics.xml",intrinsic_matrix);
	cvSave("Distortion.xml",distortion_coeffs);
	cvSave("rotation_matrices.xml",rotation_matrices);
	cvSave("translation_vectors.xml",translation_vectors);
	
	// cvCalibrateCamera2함수를 이용하여 얻게 된 결과를 XML파일로 저장한다.

	IplImage* mapx = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );
	IplImage* mapy = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );

	cvInitUndistortMap(intrinsic_matrix, distortion_coeffs, mapx, mapy);
	/* 
	   cvCalibrateCamera2함수를 이용하여 얻게 된 내부 파라미터와 distortion값을 이용하여 
	   영상을 보정할 때에 사용될 가상 렌즈를 생성한다. 
	*/
	cvNamedWindow("Undistort");	
	// 보정된 영상을 디스플레이할 "Undistort"라는 이름을 가진 윈도우를 생성한다.

	while(image) {
		IplImage *t = cvCloneImage(image);  // 획득한 영상을 복제한다. 
		cvShowImage("Calibration", image ); // 획득한 영상을 "Calibration"윈도우에 디스플레이
		cvRemap( t, image, mapx, mapy ); // 이미지를 보정한다.
		cvReleaseImage(&t); // 복제한 영상을 삭제한다.
		cvShowImage("Undistort", image); // 보정한 영상을 "Undistort"윈도우에 디스플레이

		int c = cvWaitKey(15); // 키보드 입력 값을 획득

		if(c =='p') // 'p'키를 누르면 일시정지
		{
			c = 0;
			while(c !='p'&& c != 27) // 'p'키가 다시 눌려질 때까지 무한 루프
			{
				c = cvWaitKey(250);
			}
		}

		if(c == 27) // Esc키를 누르면 종료
		{
			break;
		}

		image = cvQueryFrame( capture );
	}
	return 0;
}
