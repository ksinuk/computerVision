//  Color histogram matching method
//  2008. 09, by VISLAB, PNU
//  Programmed for Book: Visual C++ Digital Image Processing  
#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>

#include "FileIO24.h"
#include "ModelConv.h"


using namespace std;

void DrawBox(ColorImg *testImg, int height, int width, int sRow, int sCol, int tH, int tW)
{
	register int i,j;

	for(i=sRow; i<sRow+tH; i++)
	{
		int index = i*width+sCol;
		testImg[index].r= testImg[index].g= testImg[index].b= 0; 
	}
	for(i=sRow; i<sRow+tH; i++) 
	{
		int index = i*width+sCol+tW;
		testImg[index].r= testImg[index].g= testImg[index].b= 0; 
	}
	for(j=sCol; j<sCol+tW; j++) 
	{
		int index = sRow*width+j;
		testImg[index].r= testImg[index].g= testImg[index].b= 0; 
	}
	for(j=sCol; j<sCol+tW; j++) 
	{
		int index = (sRow+tH)*width+j;
		testImg[index].r= testImg[index].g= testImg[index].b= 0; 
	}
}

void BuildHisto(ColorImg *testImg,int width, int sr,int sc, int er, int ec, int vote[8][8][4])
{
	register int i,j,k;
	int r, g, b, index1, index2, v1, v2, v3;

	for(i=0; i<8; i++) for(j=0; j<8; j++) for(k=0; k<4; k++) vote[i][j][k] = 0;

	for(i=sr; i<er; i++)
	{
		index1 = i*width;
		for(j=sc; j<ec; j++)
		{
			index2 = index1+j; 
			r = testImg[index2].r;
			g = testImg[index2].g;
			b = testImg[index2].b;
			
			v1 = (b-g+255) >> 6; // /64; 
			v2 = (g-r+255) >> 6; // /64; 
			v3 = (b+g+r)/192;

			vote[v1][v2][v3]++;
		}
	}
}



void SearchTestImage(ColorImg *orgImg, int height, int width, int m_vote[8][8][4], ColorImg *outImg, int tH, int tW)
{
	register int i,j, k, l, m;
	int vote[8][8][4];

	int sumTest, sumComp, minVal;
	int currRow, currCol;
	float matchVal, currVal=0.0f;

	for(i=0; i<height-tH; i++)
	{
		for(j=0; j<width-tW; j++)
		{
	
			BuildHisto(orgImg,width, i,j,i+tH,j+tW, vote);
				
			sumTest = sumComp = 0;
			for(k=0; k<8; k++)
			{
				for(l=0; l<8; l++)
				{
					for(m=0; m<4; m++)
					{
						sumTest += vote[k][l][m]; 
						if(m_vote[k][l][m]<vote[k][l][m]) minVal = m_vote[k][l][m];
						else							  minVal = vote[k][l][m];

						sumComp += minVal;
					}
				}
			}
			
			matchVal = (float)sumComp / sumTest;

//			if(matchVal>0.7) printf("(%3d, %3d)\n",i,j);

			if(matchVal>=currVal)
			{
				currVal = matchVal;
				currRow = i;
				currCol = j;
			}
		}
	}

	printf("(%3d, %3d)=> %7.3f\n",currRow, currCol, currVal);

	// Draw a box
	DrawBox(outImg, height, width, currRow, currCol, tH, tW);
}



void main()
{
	// Reading test image 
//	CFileIO24 file("woman.bmp");
	CFileIO24 file("t1.bmp");

	int height = file.GetHeight();
	int width  = file.GetWidth();
	ColorImg *orgImg = file.GetOrgImg();
	ColorImg *outImg = file.GetOutImg();

	// Reading model image
	CFileIO24 model("m1.bmp");
	int m_height = model.GetHeight();
	int m_width  = model.GetWidth();
	ColorImg *modImg = model.GetOrgImg();
	int m_vote[8][8][4];


	// Build the model histogram
	BuildHisto(modImg,m_width,0,0,m_height,m_width,m_vote);


	printf("Matching is started...\n");
	clock_t start=clock();

	// Run the color histogram matching for test image
	SearchTestImage(orgImg, height, width, m_vote, outImg, m_height, m_width);
	

	clock_t finish=clock();
	float elapsed_time=(float)(finish-start)/CLOCKS_PER_SEC;
	printf("\n%lf sec elapsed.\n",elapsed_time);


	file.WriteBMP24("outImg.bmp");
}

