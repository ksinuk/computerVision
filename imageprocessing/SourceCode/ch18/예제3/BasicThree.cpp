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
		
		// ���� ó�� - �� �����ϱ� 
		step=pImgIplOut->widthStep/sizeof(BYTE);
		channels=pImgIplOut->nChannels;
		numrow=pImgIplOut->height;
		numcol=pImgIplOut->width;
		pImgData=(BYTE *)pImgIplOut->imageData;		

		for(r=0;r<numrow;r++)
			for(c=0;c<numcol;c++)
				for(k=0;k<channels;k++)
					pImgData[r*step+c*channels+k]=255-pImgData[r*step+c*channels+k];

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

