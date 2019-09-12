// BasicFour.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// OpenCV Includes
#include <cv.h>
#include <cxcore.h>
#include <cvaux.h>
#include <highgui.h>

int gCameraFlag=0;

void mouseHandlerFunc(int event,int x,int y,int flags,void *param);

int main(int argc, char* argv[])
{
	char *winNameIn="original image";
	int c,param=3;
	IplImage *pImgIpl=0;
	CvCapture *pCamera;
		
	// 초기화 - 카메라 연결  
	pCamera=cvCaptureFromCAM(0);
	// 윈도우 생성 
	cvNamedWindow(winNameIn,CV_WINDOW_AUTOSIZE);
	// 마우스 처리 함수 등록 
	cvSetMouseCallback(winNameIn,mouseHandlerFunc,&param);

	for(;;)
	{
		if(gCameraFlag)
		{
			pImgIpl=cvQueryFrame(pCamera);
			cvShowImage(winNameIn,pImgIpl);
		}

		c = cvWaitKey(10);
        if( (char)c == 27 ) // 27 <-> ESC 키 
            break;
    }
	// 카메라 연결 해제
	cvReleaseCapture(&pCamera);
	// 윈도우 해제 
    cvDestroyWindow(winNameIn);

	return 0;
}
void mouseHandlerFunc(int event,int x,int y,int flags,void *param)
{
	switch(event)
	{
	case CV_EVENT_LBUTTONDOWN:
		gCameraFlag=1;
		break;
	case CV_EVENT_RBUTTONDOWN:
		gCameraFlag=0;
		break;
	}
}