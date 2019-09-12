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
		
	// �ʱ�ȭ - ī�޶� ����  
	pCamera=cvCaptureFromCAM(0);
	// ������ ���� 
	cvNamedWindow(winNameIn,CV_WINDOW_AUTOSIZE);
	// ���콺 ó�� �Լ� ��� 
	cvSetMouseCallback(winNameIn,mouseHandlerFunc,&param);

	for(;;)
	{
		if(gCameraFlag)
		{
			pImgIpl=cvQueryFrame(pCamera);
			cvShowImage(winNameIn,pImgIpl);
		}

		c = cvWaitKey(10);
        if( (char)c == 27 ) // 27 <-> ESC Ű 
            break;
    }
	// ī�޶� ���� ����
	cvReleaseCapture(&pCamera);
	// ������ ���� 
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