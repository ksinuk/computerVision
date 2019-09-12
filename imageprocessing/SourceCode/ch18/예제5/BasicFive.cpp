// BasicFive.cpp : Defines the entry point for the console application.
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
	int fps=25;
	int isColor=1;
	IplImage *pImgIpl=0,*pImgIplOut;
	CvCapture *pCamera;
	CvVideoWriter *videoWriter;
		
	// �ʱ�ȭ - ī�޶� ����  
	pCamera=cvCaptureFromCAM(0);
	pImgIpl=cvQueryFrame(pCamera);
	pImgIplOut=cvCloneImage(pImgIpl);
	// ������ ���� 
	cvNamedWindow(winNameIn,CV_WINDOW_AUTOSIZE);
	// ���콺 ó�� �Լ� ��� 
	cvSetMouseCallback(winNameIn,mouseHandlerFunc,&param);
	// ������ ����� ���� �ʱ�ȭ 
	videoWriter=cvCreateVideoWriter("result.avi",0,fps,cvGetSize(pImgIpl),isColor);

	for(;;)
	{
		if(gCameraFlag)
		{
			pImgIpl=cvQueryFrame(pCamera);
			cvShowImage(winNameIn,pImgIpl);
			// ���� ó�� - smoothing ���� 
			cvSmooth(pImgIpl,pImgIplOut);
			// ������ ��� 
			cvWriteFrame(videoWriter,pImgIplOut);
			cvWaitKey(10);
		}

		c = cvWaitKey(10);
        if( (char)c == 27 ) // 27 <-> ESC Ű 
            break;
    }
	// ī�޶� ���� ����
	cvReleaseCapture(&pCamera);
	// ������ ���� ����
	cvReleaseVideoWriter(&videoWriter);
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

