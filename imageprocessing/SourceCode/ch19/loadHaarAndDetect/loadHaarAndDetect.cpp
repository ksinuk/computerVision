// loadHaarAndDetect.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//=============================================================================
// OpenCV Includes
//=============================================================================
#include <cv.h>
#include <cxcore.h>
#include <cvaux.h>
#include <highgui.h>

int main(int argc, char* argv[])
{
	int i;
	int iSampleWidth=20,iSampleHeight=20;
	int iNumDetected;
	int iMinNeighbor=5;
	int sr,er,sc,ec;
	double dScaleFactor=1.2;
	IplImage *pImgIplColor,*pImgIplGrey;
	CvHaarClassifierCascade* pCascade;
	CvMemStorage* pMemStorage;
	CvSeq* pObjects;
	CvAvgComp roi;
	
	// ���� 
	pMemStorage=cvCreateMemStorage();
    cvClearMemStorage(pMemStorage);

	// classifier �ε� 
	char *cls_location="./Classifier";
	pCascade=cvLoadHaarClassifierCascade(cls_location,cvSize(iSampleWidth,iSampleHeight));
    if( pCascade == NULL ) 
		printf( "Unable to load classifier");

	printf("number of cascade is %d\n",pCascade->count);

	pImgIplColor=cvLoadImage("c:/temp/test.jpg",1);
	pImgIplGrey=cvCreateImage(cvGetSize(pImgIplColor),IPL_DEPTH_8U,1);

	// cvHaarDetectObjects()�� �°� Į�� -> ������� ��ȯ  
	cvCvtColor(pImgIplColor,pImgIplGrey,CV_BGR2GRAY);
	
	// detect objects using classifier: use edge prunning
	pObjects=cvHaarDetectObjects(pImgIplGrey,pCascade,pMemStorage,dScaleFactor,iMinNeighbor,
								 CV_HAAR_DO_CANNY_PRUNING); 

	iNumDetected=(pObjects ? pObjects->total : 0); 

	for(i=0;i<iNumDetected;i++)
	{
		// ROI �������� 
		roi = *((CvAvgComp*) cvGetSeqElem(pObjects,i));
		sc=roi.rect.x;
		sr=roi.rect.y;
		ec=roi.rect.x+roi.rect.width;
		er=roi.rect.y+roi.rect.height;
		// �̹����� ���� ���� ǥ��: �簢��  
		cvRectangle(pImgIplColor,cvPoint(sc,sr),cvPoint(ec,er),CV_RGB(0,0,255),1);			
		// �̹����� ���� ���� ǥ��: ���� 
		cvCircle(pImgIplColor,cvPoint((sc+ec)/2,(sr+er)/2),2,CV_RGB(0,255,0),1);
	}	

	cvSaveImage("c:/temp/result.jpg",pImgIplColor);

	// ���� 
	cvReleaseImage(&pImgIplColor);
	cvReleaseImage(&pImgIplGrey);
	cvReleaseMemStorage(&pMemStorage);
	cvReleaseHaarClassifierCascade(&pCascade);

	return 0;
}
