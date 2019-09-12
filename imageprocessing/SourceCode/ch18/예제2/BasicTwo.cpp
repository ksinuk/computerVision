// BasicTwo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// OpenCV Includes
#include <cv.h>
#include <cxcore.h>
#include <cvaux.h>
#include <highgui.h>

int main(int argc, char* argv[])
{
	char *imgFileName="stuff.jpg";
	char *winNameIn="original image";
	char *winNameOut="processed image";
	IplImage *pImgIpl=0,*pImgIplOut;

	// �̹��� ���� 
	pImgIpl=cvLoadImage(imgFileName);
	// �̹��� ���� 
	pImgIplOut=cvCloneImage(pImgIpl);

	if(!pImgIpl)
		printf("can't load image: %s\n",imgFileName);
	else
	{
		// ������ ���� 
		cvNamedWindow(winNameIn,CV_WINDOW_AUTOSIZE);
		cvNamedWindow(winNameOut,CV_WINDOW_AUTOSIZE);
		// ���� ������ 
		cvFlip(pImgIpl,pImgIplOut);
		// �̹��� ��Ÿ���� 
		cvShowImage(winNameIn,pImgIpl);
		cvShowImage(winNameOut,pImgIplOut);
		// Ű���� �Է��� ������ ��ٸ�
		cvWaitKey(0);  
		// �̹��� ���� 
		cvSaveImage("processed.jpg",pImgIplOut);
		// �̹��� ���� 
		cvReleaseImage(&pImgIpl);
		cvReleaseImage(&pImgIplOut);
		// ������ ����
		cvDestroyWindow(winNameIn);
		cvDestroyWindow(winNameOut);
	}

	return 0;
}

