#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#include <iostream>
using namespace std;

int n_boards = 0; 
const int board_dt = 40; // 40 frame ���� ������ ȹ��
int board_w;             // ���ι����� �������� ��  
int board_h;              // ���ι����� �������� ��

int main()
{
	cout << "board width : ";  
	cin >> board_w;                // ���� ������ �������� ���� �Է� �޴´�.
	cout << "board height : ";
	cin >> board_h;                // ���� ������ �������� ���� �Է� �޴´�.
	cout << "board number : ";
	cin >> n_boards;               // ���� ȹ���� ������ ���� �Է¹޴´�.

	int board_n = board_w * board_h; // �ѱ������� ���� ����Ѵ�.

	CvSize board_sz = cvSize( board_w, board_h );
	CvCapture* capture = cvCreateCameraCapture( 0 ); // index0���� ī�޶��� �ڵ��� ȹ���Ѵ�.
	assert( capture );
	cvNamedWindow("Calibration"); // ������ ���÷����� "Calibration"�̶� â�� �����Ѵ�.

	CvMat* image_points = cvCreateMat(n_boards*board_n,2,CV_32FC1);
	// ���� �� frame������ ���󿡼� ȹ���� �������� ����Ʈ�� ����� ��Ʈ���� 
	CvMat* object_points = cvCreateMat(n_boards*board_n,3,CV_32FC1);
	// ���� �� frame������ ���� �������� ���谡 ����� ��Ʈ����
	CvMat* point_counts = cvCreateMat(n_boards,1,CV_32SC1);
	
	CvMat* intrinsic_matrix = cvCreateMat(3,3,CV_32FC1);
	// ���� ���� �Ķ���Ͱ� ����� ��Ʈ���� 
	CvMat* distortion_coeffs = cvCreateMat(4,1,CV_32FC1);
	// ���� distortion �Ķ���Ͱ� ����� ��Ʈ���� 


	CvMat* rotation_matrices = cvCreateMat(n_boards,9,CV_32FC1);
	// ���� �� frame�� ī�޶��� ȸ�����谡 ����� ��Ʈ���� 
	CvMat* translation_vectors = cvCreateMat(n_boards,3,CV_32FC1);
	// ���� �� frame�� ī�޶��� ��ġ���谡 ����� ��Ʈ����
	CvPoint2D32f* corners = new CvPoint2D32f[ board_n ];
	// ���� ���������� ��ǥ�� ��ϵ� �迭

	int corner_count; 		// ȹ���� �������� ��
	int successes = 0;		// �ùٸ� �̹��� ȹ�� Ƚ��
	int step, frame = 0;		
	
	IplImage *image = cvQueryFrame( capture );  // ������ ȹ��
	IplImage *gray_image = cvCreateImage(cvGetSize(image),8,1); 
	// ȹ�� ����� ���� ũ���� ��� ������ ���� 

	while(successes < n_boards) // �ùٸ� �̹����� ȹ�� Ƚ���� �Է� ���� ��ǥ�� �� �������� ����
	{
		if(frame++ % board_dt == 0) // 40 frame���� ����Ǵ� �б⹮  
		{
			int found = cvFindChessboardCorners(
				image, board_sz, corners, &corner_count,
				CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS
				); 
			/* 
			   �������� ã�� �Լ� corners���� ���������� ���󿡼��� ��ǥ���� 
                           corner_count ���������� ���� �Լ��� ���ؼ� ��ϵǾ����� 
			   ���� ������ Ž���� ���� �� ���� ���θ� �˷��ش�..
			*/

			cvCvtColor(image, gray_image, CV_BGR2GRAY);
			/*
			  cvFindCornerSubPix�Լ��� �̿��Ͽ� SubPixel�� ã�� ���ؼ� 
			  cvFindCornerSubPix�Լ��� ��鿵�� ����̹Ƿ� ȹ�濵���� ��鿵������
			  ��ȯ �� gray_image�� �����Ѵ�. 	
			*/

			cvFindCornerSubPix(gray_image, corners, corner_count,
				cvSize(11,11),cvSize(-1,-1), cvTermCriteria(
				CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
			/*
			  SubPixel�� ã�� ���� �Լ��ν� cvSize(11,11)�� Ž�� �������� ����/2 +1,
			  Ž�� �������� ����/2 +1�̶� �ǹ̷� Ž���������� ���̹� ���̰� 20�̶�� 
			  �ǹ��̴�.    	
			*/
			

			cvDrawChessboardCorners(image, board_sz, corners,corner_count, found);
			// �����̹����� Ž���� ��ǥ�� ǥ���Ѵ�. �������ο� ���� ǥ�õǴ� ����� �ٸ���.
			cvShowImage("Calibration", image );
			// ������ "Calibration"�̶� �̸��� ���� �����쿡 ���÷����Ѵ�.

			if( corner_count == board_n && found)
			// ������ ���ǿ� �´� ȹ�� ����� ���� ����   
			{
				step = successes*board_n; // ������ ���� �ε��� ���
				for( int i=step, j=0; j<board_n; ++i,++j ) {

					CV_MAT_ELEM(*image_points, float,i,0) = corners[j].x;
					CV_MAT_ELEM(*image_points, float,i,1) = corners[j].y;

					// image_points ��Ʈ������ ���� ���� ��ǥ�� ����

					CV_MAT_ELEM(*object_points,float,i,0) = j/board_w;
					CV_MAT_ELEM(*object_points,float,i,1) = j%board_w;
					CV_MAT_ELEM(*object_points,float,i,2) = 0.0f;

					/* 
   					  object_points ��Ʈ������ ������ frame�� ������ ���� ���踦
					  �����Ѵ�. ���������� �̿����� 1�̶� ������ ������ ������ 
					  ������ ����鿡 �����ϰ� �ֱ� ������ ���̴� 0�̴�.
					*/  
				}
				CV_MAT_ELEM(*point_counts, int,successes,0) = board_n;
				printf("success %d/%d\n\n",successes,n_boards);
				successes++; 		// �ùٸ� ������ ȹ���� ī��Ʈ�Ѵ�. 
			}
		} 
		image = cvQueryFrame( capture ); 	// ������ ȹ���Ѵ�.
		cvWaitKey(15);						
	} 

	CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0f;
	CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1 ) = 1.0f;
	// intrinsic_matrix�� �ʱ�ȭ 

	cvCalibrateCamera2(
		object_points, image_points,point_counts, cvGetSize( image ),intrinsic_matrix,
		 distortion_coeffs, rotation_matrices, translation_vectors,0 );
		
		 /*  cvCalibrateCamera2 �Լ��� �̿��Ͽ� Į���극�̼��� �Ѵ�.
		 intrinsic_matrix���� ī�޶� ���� �Ķ���Ͱ� distortion_coeffs���� distortion����
		 rotation_matrices, translation_vectors���� ü������� ī�޶��� ȸ�� �� ��ġ
		 ���迡 ���� ���� �Լ��� ���ؼ� ����ǰ� �ȴ�.	*/

	cvSave("Intrinsics.xml",intrinsic_matrix);
	cvSave("Distortion.xml",distortion_coeffs);
	cvSave("rotation_matrices.xml",rotation_matrices);
	cvSave("translation_vectors.xml",translation_vectors);
	
	// cvCalibrateCamera2�Լ��� �̿��Ͽ� ��� �� ����� XML���Ϸ� �����Ѵ�.

	IplImage* mapx = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );
	IplImage* mapy = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );

	cvInitUndistortMap(intrinsic_matrix, distortion_coeffs, mapx, mapy);
	/* 
	   cvCalibrateCamera2�Լ��� �̿��Ͽ� ��� �� ���� �Ķ���Ϳ� distortion���� �̿��Ͽ� 
	   ������ ������ ���� ���� ���� ��� �����Ѵ�. 
	*/
	cvNamedWindow("Undistort");	
	// ������ ������ ���÷����� "Undistort"��� �̸��� ���� �����츦 �����Ѵ�.

	while(image) {
		IplImage *t = cvCloneImage(image);  // ȹ���� ������ �����Ѵ�. 
		cvShowImage("Calibration", image ); // ȹ���� ������ "Calibration"�����쿡 ���÷���
		cvRemap( t, image, mapx, mapy ); // �̹����� �����Ѵ�.
		cvReleaseImage(&t); // ������ ������ �����Ѵ�.
		cvShowImage("Undistort", image); // ������ ������ "Undistort"�����쿡 ���÷���

		int c = cvWaitKey(15); // Ű���� �Է� ���� ȹ��

		if(c =='p') // 'p'Ű�� ������ �Ͻ�����
		{
			c = 0;
			while(c !='p'&& c != 27) // 'p'Ű�� �ٽ� ������ ������ ���� ����
			{
				c = cvWaitKey(250);
			}
		}

		if(c == 27) // EscŰ�� ������ ����
		{
			break;
		}

		image = cvQueryFrame( capture );
	}
	return 0;
}
