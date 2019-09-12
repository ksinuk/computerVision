// BasicOne.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// OpenCV Includes
#include <cv.h>
#include <cxcore.h>
#include <cvaux.h>
#include <highgui.h>

int main(int argc, char* argv[])
{
	IplImage *pImgIpl=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);

	// ������ ���� 
	cvNamedWindow("win1",CV_WINDOW_AUTOSIZE);
	cvMoveWindow("win1",100,100);
	
	// �׸��� �Լ� 
	// ���簢��
	cvRectangle(pImgIpl,cvPoint(100,100),cvPoint(200,200),cvScalar(255,0,0),1);  
	// ��
	cvCircle(pImgIpl,cvPoint(300,300),100,cvScalar(0,255,0),1);  
	// Ÿ��
	cvEllipse(pImgIpl,cvPoint(300,300),cvSize(200,100),45,0,360,cvScalar(0,0,255),1,6);  
	// ����
	cvLine(pImgIpl,cvPoint(100,100),cvPoint(300,300),cvScalar(0,255,0),1);  

	CvPoint curve1[]={100,100,  200,100,  100,200,  0,100};
	CvPoint curve2[]={200,200,  400,200,  400,400,  500,500,  300,300};
	CvPoint* curveArr[]={curve1,curve2};
	int nCurvePts[2]={sizeof(curve1)/sizeof(CvPoint),sizeof(curve2)/sizeof(CvPoint)};
	int nCurves=2;
	int isCurveClosed=1;
	int lineWidth=3;

	// �ٰ��� �׸���
	cvPolyLine(pImgIpl,curveArr,nCurvePts,nCurves,isCurveClosed,cvScalar(0,255,255),lineWidth);  

	CvPoint poly1[]={50,50, 100,50, 100,100};
	CvPoint poly2[]={200,200, 250,200, 250,250};
	CvPoint* polyArr[]={poly1,poly2};
	int nPolyPts[]={sizeof(poly1)/sizeof(CvPoint),sizeof(poly2)/sizeof(CvPoint)};
	int nPoly=2;
	
	// ä���� �ٰ��� �׸���
	cvFillPoly(pImgIpl,polyArr,nPolyPts,nPoly,cvScalar(0,255,0));  

	// ���� ��Ÿ���� 
	CvFont font;
	double hScale=1.0;
	double vScale=2.0;
	
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC,hScale,vScale,1.0,lineWidth);
	cvPutText(pImgIpl,"Hello OpenCV!",cvPoint(320,240),&font,cvScalar(255,100,100));

	// �����쿡 �̹��� ��Ÿ���� 
	cvShowImage("win1",pImgIpl);

	// Ű���� �Է��� ������ ��ٸ�
	cvWaitKey(0);  

	// �̹��� ���� 
	cvReleaseImage(&pImgIpl);

	// ������ ����
	cvDestroyWindow("win1");

	return 0;
}

