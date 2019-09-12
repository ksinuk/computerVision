// BlobColoring.cpp: implementation of the CBlobColoring class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinColor.h"
#include "BlobColoring.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
#include "math.h"
#define WIDTHBYTES(bits)    (((bits)+31)/32*4)  //4바이트의 배수여야  
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlobColoring::CBlobColoring(unsigned char *m_InImg, int height, int width)
{
	top =0;
	m_MaxStack = height*width;
	rwsize = WIDTHBYTES(24*width);
	this->height = height;
	this->width = width;
	this->m_InImg = new unsigned char [height*rwsize];
	for(int i=0; i<rwsize*height; i++) this->m_InImg[i]=m_InImg[i];

	passImg=new unsigned char [width*height];
	memset(passImg,0,height*width);
	m_stackx=new int [m_MaxStack];
	m_stacky=new int [m_MaxStack];
	m_Coloring = new int [height*width];
	for(i=0; i<height*width; i++) m_Coloring[i]=0;

	blobData= new BLOBDATA [height*width];
}

CBlobColoring::~CBlobColoring()
{
	delete []m_InImg;
	delete []passImg;
	delete []m_stackx; delete []m_stacky;
	delete []m_Coloring;

	delete []blobData;
}

int CBlobColoring::push(int vx,int vy)
{
	if(top>=m_MaxStack) return(-1);
	top++;
	m_stackx[top]=vx;
	m_stacky[top]=vy;
	return(1);
}

int CBlobColoring::pop(int *vx,int *vy)
{
	if(top==0) return(-1);
	*vx=m_stackx[top];
	*vy=m_stacky[top];
	top--;
	return(1);
}

BOOL CBlobColoring::BoundDist(int rColor, int gColor, int bColor, int rVal, int gVal, int bVal)
{
	if(abs(rColor-rVal) > distanceBound) return FALSE;
	if(abs(gColor-gVal) > distanceBound) return FALSE;
	if(abs(bColor-bVal) > distanceBound) return FALSE;
	return TRUE;
}

void CBlobColoring::fnBlobColoring(int distanceBound)
{
	int i, j, m, n, index, index1, index2, index3, index4;
	int r, c, nColor=1;
	int rVal, gVal, bVal;
	int rColor, gColor, bColor;
	int pixelCount;
	this->distanceBound = distanceBound;

	for(i=0; i<height; i++) 
	{
		index = (height-i-1)*rwsize;
		index3= i*width;
		for(j=0; j<width; j++) 
		{
			if(passImg[index3+j]==0) 
			{
				pixelCount = 1;
				rColor = (int)m_InImg[index+3*j+2];
				gColor = (int)m_InImg[index+3*j+1];
				bColor = (int)m_InImg[index+3*j  ];

				r=i; 
				c=j;
				blobData[nColor].ltx =blobData[nColor].lty =max(height,width);
				blobData[nColor].rbx =blobData[nColor].rby =0;
				blobData[nColor].cx = (float)r; 
				blobData[nColor].cy = (float)c; 
				blobData[nColor].r = (float)rColor;
				blobData[nColor].g = (float)gColor;
				blobData[nColor].b = (float)bColor;
				top=0;

				while(1) 
				{
GRASSFIRE:
					for(m=r-1; m<=r+1; m++)
					{
						index1 = (height-m-1)*rwsize;
						index2 = m*width;
						for(n=c-1; n<=c+1; n++) 
						{
							if((m>=0 && m<height) && (n>=0 && n<width)) 
							{
								index4 = index1+3*n;
								rVal = (int)m_InImg[index4+2];
								gVal = (int)m_InImg[index4+1];
								bVal = (int)m_InImg[index4  ];

								if(BoundDist(rColor,gColor,bColor,rVal,gVal,bVal) 
									&& passImg[index2+n]==0) 
								{
									passImg[index2+n]=1;
									m_Coloring[index2+n]=nColor;
									pixelCount++;

									if( push(m,n)==-1 ) continue;

									blobData[nColor].cx+= (float)m;
									blobData[nColor].cy+= (float)n;
									blobData[nColor].r += (float)rColor;
									blobData[nColor].g += (float)gColor;
									blobData[nColor].b += (float)bColor;

									blobData[nColor].ltx = min(blobData[nColor].ltx,m);
									blobData[nColor].lty = min(blobData[nColor].lty,n);
									blobData[nColor].rbx = max(blobData[nColor].rbx,m);
									blobData[nColor].rby = max(blobData[nColor].rby,n);

									r=m; 
									c=n;

									goto GRASSFIRE;
								}
							}
						}
					}
					if(pop(&r,&c)==-1) break;
				}

				blobData[nColor].n=pixelCount;
				blobData[nColor].cx /=(float)pixelCount;
				blobData[nColor].cy /=(float)pixelCount;
				blobData[nColor].r /= (float)pixelCount;
				blobData[nColor].g /= (float)pixelCount;
				blobData[nColor].b /= (float)pixelCount;

				nColor++;
			}
		}
	}
	nBlob = nColor;
//	m_FindMoment();
	m_FindBoundary();
}


void CBlobColoring::m_FindMoment()
{
	int i, j, index, blob;
	float u02,u20, u11;
	for(i=0; i<nBlob; i++) 
		blobData[i].u02 = blobData[i].u20 = blobData[i].u11 =0.0f;
	for(i=0; i<height; i++)
	{
		index = i*width;
		for(j=0; j<width; j++)
		{
			blob = m_Coloring[index+j];
			u11 = (i-blobData[blob].cx)*(j-blobData[blob].cy);
			u02 = (i-blobData[blob].cx)*(i-blobData[blob].cx);
			u20 = (j-blobData[blob].cy)*(j-blobData[blob].cy);
			blobData[blob].u11 += u11; 			
			blobData[blob].u02 += u02; 			
			blobData[blob].u20 += u20; 			
		}
	}
}

void CBlobColoring::m_FindBoundary()
{
	int i,j,k,ii,jj,index,blob;
	float dist,vdist;
	for(i=0; i<nBlob; i++) 
	{
		blobData[i].m = blobData[i].v = 0.0f;
		blobData[i].pc =0;
	}
	POINT a[4] = { {-1,0}, {0,1}, {1,0}, {0,-1}};
	
	// 각 영역 반지름의 합을 구함 
	for(i=1; i<height-1; i++)
	{
		index = i*width;
		for(j=1; j<width-1; j++)
		{
			blob = m_Coloring[index+j];

			for(k=0; k<4; k++)
			{
				ii = i+a[k].x;
				jj = j+a[k].y;
				
				if(blob == m_Coloring[ii*width+jj]) continue;

				dist = (ii-blobData[blob].cx)*(ii-blobData[blob].cx);
				dist+= (jj-blobData[blob].cy)*(jj-blobData[blob].cy);
				dist = (float)sqrt(dist);
				blobData[blob].m += dist;
				blobData[blob].pc++;
			}
		}
	}

	// 각 영역의 평균 반지름을 구함 
	for(i=0; i<nBlob; i++) blobData[i].m /= (float)blobData[i].pc;

	// 각 영역의 분산의 합을 구함 
	for(i=1; i<height-1; i++)
	{
		index = i*width;
		for(j=1; j<width-1; j++)
		{
			blob = m_Coloring[index+j];

			for(k=0; k<4; k++)
			{
				ii = i+a[k].x;
				jj = j+a[k].y;
				
				if(blob == m_Coloring[ii*width+jj]) continue;

				dist = (ii-blobData[blob].cx)*(ii-blobData[blob].cx);
				dist+= (jj-blobData[blob].cy)*(jj-blobData[blob].cy);
				dist = (float)sqrt(dist);
				vdist= (dist-blobData[blob].m)*(dist-blobData[blob].m);

				blobData[blob].v += vdist;
			}
		}
	}

	// 각 영역의 분산을 구함 
	for(i=0; i<nBlob; i++) blobData[i].v /= (float)((blobData[i].pc-1)+0.000001);
}
