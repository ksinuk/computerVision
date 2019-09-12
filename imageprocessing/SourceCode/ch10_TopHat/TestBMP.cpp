#include <stdio.h>
#include "FileIO.h"

uchar* GrayErosion(uchar* orgImg,int height,int width,int r)
{
	r = (int)(r/2.0f);
	register int i,j,k,l;
	uchar* outImg = new uchar [height*width];
	for(i=0; i<height*width; i++) outImg[i] = 0;

	for(i=0; i<height; i++)
	{
		int index2 = i*width;
		for(j=0; j<width; j++)
		{
			int minVal = 100000;
			for(k=-r; k<=r; k++)
			{
				if(i+k<0 || i+k>=height) continue;

				int index1 = (i+k)*width;
				for(l=-r; l<=r; l++)
				{
					if(j+l<0 || j+l >=width) continue;
					
					uchar imVal = orgImg[index1+j+l];
					if(imVal<minVal) minVal = imVal;
				}
			}
			outImg[index2+j] = (uchar)minVal;
		}
	}
	return outImg;
}

uchar* GrayDilation(uchar* orgImg,int height,int width,int r)
{
	r = (int)(r/2.0f);
	register int i,j,k,l;
	uchar* outImg = new uchar [height*width];
	for(i=0; i<height*width; i++) outImg[i] = 0;

	for(i=0; i<height; i++)
	{
		int index2 = i*width;
		for(j=0; j<width; j++)
		{
			int maxVal = 0;
			for(k=-r; k<=r; k++)
			{
				if(i+k<0 || i+k>=height) continue;

				int index1 = (i+k)*width;
				for(l=-r; l<=r; l++)
				{
					if(j+l<0 || j+l >=width) continue;
					
					uchar imVal = orgImg[index1+j+l];
					if(imVal>maxVal) maxVal = imVal;
				}
			}
			outImg[index2+j] = (uchar)maxVal;
		}
	}
	return outImg;
}

void ImgDiff(uchar* orgImg,uchar* maxImg,int height,int width,uchar* outImg)
{
	for(int i=0; i<height*width; i++) 
	{
		int diff = orgImg[i]-maxImg[i];
		outImg[i] = diff<0?(uchar)0:(uchar)diff;
	}
}

void TopHat()
{
	char mInStr[40];

	/// 흑백 영상 데이타를 읽는다 
	printf("Input File (for example type 'rice.bmp') : "); 
	scanf("%s",mInStr);

	// CFileIO 객체를 하나 생성하고 파일을 읽음  
	CFileIO pFile(mInStr);

	uchar* orgImg = pFile.GetOrgImg();
	uchar* outImg = pFile.GetOutImg();
	int	height			   = pFile.GetHeight();
	int width			   = pFile.GetWidth();	

	// TopHat연산 
	int radius = 30;

	uchar* minImg = GrayErosion(orgImg,height,width,radius);
	uchar* maxImg = GrayDilation(minImg,height,width,radius);  


	ImgDiff(orgImg,maxImg,height,width,outImg);
	
	delete []minImg; // 메모리 제거 
	delete []maxImg;   


	/// 출력할 파일 이름을 받아 들임 
	char outName[40];
	sprintf(outName,"out%s",mInStr);

	/// 처리한 결과를 저장한다
	pFile.WriteBMP8(outName);
}


void main()
{
	TopHat();
}