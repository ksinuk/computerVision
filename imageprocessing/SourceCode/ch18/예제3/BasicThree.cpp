// BasicThree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// OpenCV Includes
#include <cv.h>
#include <cxcore.h>
#include <cvaux.h>
#include <highgui.h>

int main(int argc, char* argv[])
{
	BYTE *pImgData;
	char *imgFileName="stuff.jpg";
	char *winNameIn="original image";
	char *winNameOut="processed image";
	int channels;
	int step;
	int numrow,numcol;
	int r,c,k;
	IplImage *pImgIpl=0,*pImgIplOut;

	// 이미지 생성 
	pImgIpl=cvLoadImage(imgFileName);
	// 이미지 복사 
	pImgIplOut=cvCloneImage(pImgIpl);

	if(!pImgIpl)
		printf("can't load image: %s\n",imgFileName);
	else
	{
		// 윈도우 생성 
		cvNamedWindow(winNameIn,CV_WINDOW_AUTOSIZE);
		cvNamedWindow(winNameOut,CV_WINDOW_AUTOSIZE);
		
		// 영상 처리 - 값 반전하기 
		step=pImgIplOut->widthStep/sizeof(BYTE);
		channels=pImgIplOut->nChannels;
		numrow=pImgIplOut->height;
		numcol=pImgIplOut->width;
		pImgData=(BYTE *)pImgIplOut->imageData;		

		for(r=0;r<numrow;r++)
			for(c=0;c<numcol;c++)
				for(k=0;k<channels;k++)
					pImgData[r*step+c*channels+k]=255-pImgData[r*step+c*channels+k];

		// 이미지 나타내기 
		cvShowImage(winNameIn,pImgIpl);

		cvShowImage(winNameOut,pImgIplOut);
		// 키보드 입력을 무한정 기다림
		cvWaitKey(0);  
		// 이미지 저장 
		cvSaveImage("processed.jpg",pImgIplOut);
		// 이미지 해제 
		cvReleaseImage(&pImgIpl);
		cvReleaseImage(&pImgIplOut);
		// 윈도우 종료
		cvDestroyWindow(winNameIn);
		cvDestroyWindow(winNameOut);
	}

	return 0;
}

