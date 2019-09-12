// loadHaarAndSave.cpp : Defines the entry point for the console application.
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
	char imgFileName[100];
	int iSampleWidth=20,iSampleHeight=20;
	int scale=3;
	int left,top,right,bottom;
	int i,j;
	IplImage *pImgIpl;
	CvHaarStageClassifier* ptr;
	CvHaarClassifierCascade* pCascade;
	
	// classifier �ε� 
	char *cls_location="./Classifier";
	pCascade=cvLoadHaarClassifierCascade(cls_location,cvSize(iSampleWidth,iSampleHeight));
    if( pCascade == NULL )
        printf( "Unable to load classifier");

	printf("number of cascade is %d\n",pCascade->count);

	// �̹��� ���� 
	pImgIpl=cvCreateImage(cvSize(scale*iSampleWidth,scale*iSampleHeight),IPL_DEPTH_8U,1);

	for(i=0;i<pCascade->count;i++) // cascade�� �� 
	{
		ptr=&pCascade->stage_classifier[i];

		for(j=0;j<ptr->count;j++) // �ϳ��� �������������� �з����� ��
		{
			cvSet(pImgIpl,cvScalar(255,255,255));

			// ù ��° �簢��: ���̳ʽ� ����ġ 
			left=ptr->classifier[j].haar_feature->rect[0].r.x;
			top=ptr->classifier[j].haar_feature->rect[0].r.y;
			right=left+ptr->classifier[j].haar_feature->rect[0].r.width;
			bottom=top+ptr->classifier[j].haar_feature->rect[0].r.height;
			cvRectangle(pImgIpl,cvPoint(scale*left,scale*top),cvPoint(scale*right,scale*bottom),CV_RGB(0,0,0),1);

			// �� ��° �簢�� 
			left=ptr->classifier[j].haar_feature->rect[1].r.x;
			top=ptr->classifier[j].haar_feature->rect[1].r.y;
			right=left+ptr->classifier[j].haar_feature->rect[1].r.width;
			bottom=top+ptr->classifier[j].haar_feature->rect[1].r.height;
			cvRectangle(pImgIpl,cvPoint(scale*left,scale*top),cvPoint(scale*right,scale*bottom),CV_RGB(0,0,0),-1);

			// �� ��° �簢�� 
			if(ptr->classifier[j].haar_feature->rect[2].weight!=0)
			{
				left=ptr->classifier[j].haar_feature->rect[2].r.x;
				top=ptr->classifier[j].haar_feature->rect[2].r.y;
				right=left+ptr->classifier[j].haar_feature->rect[2].r.width;
				bottom=top+ptr->classifier[j].haar_feature->rect[2].r.height;
				cvRectangle(pImgIpl,cvPoint(scale*left,scale*top),cvPoint(scale*right,scale*bottom),CV_RGB(0,0,0),-1);
			}	

			// �ܰ� ��� ǥ�� �簢�� 
			cvRectangle(pImgIpl,cvPoint(0,0),cvPoint(scale*iSampleWidth-1,scale*iSampleHeight-1),CV_RGB(0,0,0),1);

			// �̹��� ���� 
			sprintf(imgFileName,"./img/cls_%d_%d.jpg",i,j);
			cvSaveImage(LPCTSTR(imgFileName),pImgIpl);
		}
	}

	// ���� 
	cvReleaseImage(&pImgIpl);
	cvReleaseHaarClassifierCascade(&pCascade);

	return 0;
}

