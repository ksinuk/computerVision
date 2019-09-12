// WinTestDoc.cpp : implementation of the CWinTestDoc class
//

#include "stdafx.h"
#include "WinTest.h"

#include "WinTestDoc.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinTestDoc

IMPLEMENT_DYNCREATE(CWinTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CWinTestDoc, CDocument)
	//{{AFX_MSG_MAP(CWinTestDoc)
	ON_COMMAND(IDM_REVERSE_IMG, OnReverseImg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinTestDoc construction/destruction

CWinTestDoc::CWinTestDoc()
{
	// TODO: add one-time construction code here

}

CWinTestDoc::~CWinTestDoc()
{
}

BOOL CWinTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWinTestDoc serialization

void CWinTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar.Write(m_OutImg,256*256); // 처리된 영상배열 m_OutImg를 파일로 저장 
	}
	else
	{
		// TODO: add loading code here
		CFile *infile = ar.GetFile(); // 입력할 화일의 포인트를 가져옴.
		if(infile->GetLength()!=256*256) // 화일 사이즈를 검사함 
		{
			AfxMessageBox("파일 크기가 256x256사이즈가 아닙니다.");
			return;
		}
		ar.Read(m_InImg,infile->GetLength()); // 영상파일을 읽어 m_InImg배열에 저장 
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWinTestDoc diagnostics

#ifdef _DEBUG
void CWinTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWinTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinTestDoc commands

void CWinTestDoc::OnReverseImg() 
{
	// TODO: Add your command handler code here
	for(int i=0; i<256; i++)
	{
		for(int j=0; j<256; j++) m_OutImg[i][j] = 255-m_InImg[i][j];
	}

	UpdateAllViews(NULL);
	
}


void CWinTestDoc::TwoImgLoad()
{
	CFileDialog opendlg1(TRUE); //공통 대화 상자(첫번째 파일 오픈) 
	CFile file;

	if(opendlg1.DoModal()==IDOK) 
	{
		file.Open(opendlg1.GetFileName(), CFile::modeRead);
		file.Read(m_InImg1,sizeof(m_InImg1));
		file.Close();
	}


	CFileDialog opendlg2(TRUE); //공통 대화 상자(두번째 파일 오픈) 

	if(opendlg2.DoModal()==IDOK)
	{
		file.Open(opendlg2.GetFileName(), CFile::modeRead);
		file.Read(m_InImg2,sizeof(m_InImg2));
		file.Close();
	}
}

void CWinTestDoc::m_ImgHisto(int height, int width)
{
	int i,j,vmax,vmin;
	for(i=0; i<256; i++) m_HistoArr[i]=0;

	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			int gv = (int)m_InImg[i][j];
			m_HistoArr[gv]++;
		}
	}

	// 히스토그램 크기 정규화 
	vmin = 1000000; vmax =0;
	for(i=0; i<256; i++) 
	{
		if(m_HistoArr[i]<=vmin) vmin = m_HistoArr[i];
		if(m_HistoArr[i]>=vmax) vmax = m_HistoArr[i];
	}
	if(vmax==vmin) return;

	float vd = (float)(vmax-vmin);
	for(i=0; i<256; i++) 
	{
		m_HistoArr[i] = (int)( ((float)m_HistoArr[i]-vmin)*255.0/vd);
	}

	// 히스토그램의 출력 
	for(i=0; i<height; i++) 
		for(j=0; j<width; j++) m_OutImg[i][j] = 255;

	for(j=0; j<width; j++)
	{
		for(i=0; i<m_HistoArr[j]; i++) m_OutImg[255-i][j] = 0;
	}
}

void CWinTestDoc::m_BinThres(int height, int width, int binThres)
{
	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			if(m_InImg[i][j]>binThres) m_OutImg[i][j] = 255;
			else m_OutImg[i][j] = 0;
		}
	}
}


void CWinTestDoc::m_HistoEqual(int height, int width)
{
	int i,j;
	/// histogram연산을 위해 사용할 배열메모리를 할당 
	unsigned int *histogram = new unsigned int [256];
	unsigned int *sum_hist = new unsigned int [256];

	/// histogram배열을 초기화 
	for(i=0; i<256; i++) histogram[i]=0;

	/// 영상의 histogram을 계산 
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++) histogram[m_InImg[i][j]]++;
	}

	/// histogram의 정규화된 합을 계산 
	int sum=0;
	float scale_factor=255.0f/(float)(height*width);

	for(i=0; i<256; i++)
	{
		sum += histogram[i];
		sum_hist[i] =(int)((sum*scale_factor) + 0.5);
	}

	/// LUT로써 sum_hist배열을 사용하여 영상을 변환 
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++) m_OutImg[i][j]=sum_hist[m_InImg[i][j]];
	}

	// 메모리를 해제 
	delete []histogram; delete []sum_hist;
}

void CWinTestDoc::m_HistoStretch(int height, int width)
{
	int i,j;
	int lowvalue=255, highvalue=0;

	// 밝기값이 가장 작은값 선정 
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++) if(m_InImg[i][j]<lowvalue) lowvalue = m_InImg[i][j];
	}

	// 밝기값이 가장 큰값 선정 
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++) if(m_InImg[i][j]>highvalue) highvalue = m_InImg[i][j];
	}

	// Histogram스트레칭 계산 
	float mult = 255.0f/(float)(highvalue-lowvalue);
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++) 
			m_OutImg[i][j] = (unsigned char)((m_InImg[i][j]-lowvalue)*mult);
	}
}


void CWinTestDoc::m_HistoUpStretch(int height, int width, int lowPercent, int highPercent)
{
	int i, j;
	// histogram연산을 위해 사용할 배열을 할당 
	unsigned int *histogram = new unsigned int [256];

	// histogram배열을 초기화 
	for(i=0; i<256; i++) histogram[i]=0;

	// 영상의 histogram을 계산하라 
	for(i=0; i<height; i++) for(j=0;j<width; j++) histogram[m_InImg[i][j]]++;

	// 
	unsigned int runsum=0;
	int lowthresh=0, highthresh=255;
	for(i=0; i<256; i++) 
	{
		runsum += histogram[i];
		if ( (runsum*100.0/(float)(height*width)) >= lowPercent) 
		{
			lowthresh =i;
			break;
		}
	}

	runsum =0;
	for(i=255; i>=0; i--) 
	{
		runsum += histogram[i];
		if ( (runsum*100.0/(float)(height*width)) >= highPercent) 
		{
			highthresh =i;
			break;
		}
	}

	// 새로운 LUT를 계산함 
	unsigned char *LUT=new unsigned char [256];

	for(i=0; i<lowthresh; i++) LUT[i]=0;
	for(i=255; i>highthresh; i--) LUT[i]=255;

	float scale = 255.0f/(float)(highthresh-lowthresh);
	for(i=lowthresh; i<=highthresh; i++) 
		LUT[i] = (unsigned char)((i-lowthresh) * scale);

	// LUT로써 sum_hist배열을 사용하여 영상을 변환하라 
	for(i=0; i<height; i++) for(j=0; j<width; j++) m_OutImg[i][j]=LUT[m_InImg[i][j]];

	// 메모리를 해제
	delete []histogram; delete []LUT;
}

void CWinTestDoc::m_HistoSpec_FileIn()
{
	CFileDialog opendlg1(TRUE); //공통 대화 상자(히스토그램을 지정할 영상파일 입력) 
	CFile file;

	if(opendlg1.DoModal()==IDOK) 
	{
		file.Open(opendlg1.GetFileName(), CFile::modeRead);
		file.Read(m_InImg1,sizeof(m_InImg1));
		file.Close();
	}
}


void CWinTestDoc::m_HistoSpec(int height, int width)
{
	int i, j;
	// histogram연산을 위해 사용할 배열을 할당 
	unsigned int *histogram = new unsigned int [256];
	unsigned int *sum_hist = new unsigned int [256];
	unsigned int *desired_histogram = new unsigned int [256];
	unsigned int *desired_sum_hist = new unsigned int [256];

	// histogram배열을 초기화 
	for(i=0; i<256; i++) histogram[i]=desired_histogram[i]=0;

	// 영상의 histogram을 계산하라 
	for(i=0; i<height; i++) 
	{
		for(j=0; j<width; j++) 
		{
			histogram[m_InImg[i][j]]++;  // 입력 영상의 히스토그램 
			desired_histogram[m_InImg1[i][j]]++;  // 지정 영상의 히스토그램 
	}}


	// histogram의 정규화된 합을 계산하라 
	int sum=0;
	float scale_factor=255.0f/(float)(height*width);

	for(i=0; i<256; i++)
	{
		sum += histogram[i];
		sum_hist[i] =(int)((sum*scale_factor) + 0.5);
	}

	// desired histogram에 대한 정규화된 합을 계산
	sum=0;
	for(i=0; i<256; i++) 
	{
		sum += desired_histogram[i];
		desired_sum_hist[i] =(int)(sum * scale_factor);
	}

	// 가장 가까운 정규화합 히스토그램값을 주는 index를 찾음   
	int *inv_hist =new int [256];
	int hist_min, hist_index, hist_diff;
	for(i=0; i<256; i++)
	{
		hist_min = 1000;
		for(j=0; j<256; j++)
		{
			hist_diff = abs(sum_hist[i]-desired_sum_hist[j]);
			if(hist_diff < hist_min) 
			{
				hist_min = hist_diff;
				hist_index = j;
			}
		}

		inv_hist[i] = hist_index;
	}

	// 역 히스토그램 변환 
	for(i=0; i<height; i++) for(j=0; j<width; j++) m_OutImg[i][j]=inv_hist[m_InImg[i][j]];

	// 메모리 해제 
	delete []inv_hist; delete []histogram; delete []desired_histogram; 
	delete []sum_hist; delete []desired_sum_hist;
}
void CWinTestDoc::m_SmoothingBox(int height, int width)
{
	int MaskBox[3][3]={{1,1,1},
						{1,1,1},
						{1,1,1}};
	int heightm1=height-1;
	int widthm1=width-1;
	int mr,mc;
	int newValue;
	int i,j;
	
	//결과 이미지 0으로 초기화
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			m_OutImg[i][j]=0;

	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=0; //0으로 초기화
			for(mr=0;mr<3;mr++)
				for(mc=0;mc<3;mc++)
					newValue += (MaskBox[mr][mc]*m_InImg[i+mr-1][j+mc-1]);
			newValue /= 9; //마스크의 합의 크기로 나누기:값의 범위를 0에서 255로 함
			m_OutImg[i][j]=(BYTE)newValue;//BYTE값으로 변환 
		}
	}
}

void CWinTestDoc::m_SmoothingGaussian(int height, int width)
{
	int MaskGaussian[3][3]={{1,2,1},
							{2,4,2},
							{1,2,1}};
	int heightm1=height-1;
	int widthm1=width-1;
	int mr,mc;
	int newValue;
	int i,j;
	//결과 이미지 0으로 초기화
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			m_OutImg[i][j]=0;

	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=0; //0으로 초기화
			for(mr=0;mr<3;mr++)
				for(mc=0;mc<3;mc++)
					newValue += (MaskGaussian[mr][mc]*m_InImg[i+mr-1][j+mc-1]);
			newValue /= 20; //마스크의 합의 크기로 나누기:값의 범위를 0에서 255로 함
			m_OutImg[i][j]=(BYTE)newValue;//BYTE값으로 변환 
		}
	}
}

void CWinTestDoc::m_SharpeningLaplacian(int height, int width)
{
	int MaskBox[3][3]={{-1,-1,-1},
						{-1,8,-1},
						{-1,-1,-1}};
	int heightm1=height-1;
	int widthm1=width-1;
	int mr,mc;
	int newValue;
	int i,j;
	int *pTmpImg;
	int min,max;
	float constVal1,constVal2;

	//정수값을 갖는 이미지 동적 메모리 할당
	pTmpImg=new int[height*width];

	//결과 이미지 0으로 초기화
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
		{
			m_OutImg[i][j]=0;
			pTmpImg[i*width+j]=0;
		}

	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=0; //0으로 초기화
			for(mr=0;mr<3;mr++)
				for(mc=0;mc<3;mc++)
					newValue += (MaskBox[mr][mc]*m_InImg[i+mr-1][j+mc-1]);
			//값을 양수로 변환 
			if(newValue<0)
				newValue=-newValue;
			pTmpImg[i*width+j]=newValue;
		}
	}

	//디스플레이를 위해 0에서 255사이로 값의 범위를 매핑
    //이를 위해 먼저 최대,최소값을 찾은후 이를 이용하여 매핑한다.
	min=(int)10e10;
	max=(int)-10e10;
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=pTmpImg[i*width+j];
			if(newValue<min)
				min=newValue;
			if(newValue>max)
				max=newValue;
		}
	}
	//변환시 상수값을 미리 계산 
	constVal1=(float)(255.0/(max-min));
	constVal2=(float)(-255.0*min/(max-min));	
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			//[min,max]사이의 값을 [0,255]값으로 변환 
			newValue=pTmpImg[i*width+j];
			newValue=constVal1*newValue+constVal2;
			m_OutImg[i][j]=(BYTE)newValue;
		}
	}
	//동적 할당 메모리 해제
	delete [] pTmpImg;
}

void CWinTestDoc::m_EdgePrewitt(int height, int width)
{
	int MaskPrewittX[3][3]={{-1,0,1},
						{-1,0,1},
						{-1,0,1}};
	int MaskPrewittY[3][3]={{1,1,1},
							{0,0,0},
							{-1,-1,-1}};
	int heightm1=height-1;
	int widthm1=width-1;
	int mr,mc;
	int newValue;
	int i,j;
	int *pImgPrewittX,*pImgPrewittY;
	int min,max,where;
	float constVal1,constVal2;

	//정수값을 갖는 이미지 동적 메모리 할당
	pImgPrewittX=new int[height*width];
	pImgPrewittY=new int[height*width];

	//결과 이미지 0으로 초기화
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
		{
			m_OutImg[i][j]=0;
			where=i*width+j;
			pImgPrewittX[where]=0;
			pImgPrewittY[where]=0;
		}

	//X 방향 에지 강도 계산 
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=0; //0으로 초기화
			for(mr=0;mr<3;mr++)
				for(mc=0;mc<3;mc++)
					newValue += (MaskPrewittX[mr][mc]*m_InImg[i+mr-1][j+mc-1]);
			pImgPrewittX[i*width+j]=newValue;
		}
	}
	//Y 방향 에지 강도 계산 
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=0; //0으로 초기화
			for(mr=0;mr<3;mr++)
				for(mc=0;mc<3;mc++)
					newValue += (MaskPrewittY[mr][mc]*m_InImg[i+mr-1][j+mc-1]);
			pImgPrewittY[i*width+j]=newValue;
		}
	}
	//에지 강도 계산 절대값(X)+절대값(Y)후 pImgPrewittX[]에 저장
	for(i=1;i<heightm1;i++)
		for(j=1;j<widthm1;j++)
		{
			where=i*width+j;
			constVal1=pImgPrewittX[where];
			constVal2=pImgPrewittY[where];
			if(constVal1<0)
				constVal1=-constVal1;
			if(constVal2<0)
				constVal2=-constVal2;
			pImgPrewittX[where]=constVal1+constVal2;
		}
	//디스플레이를 위해 0에서 255사이로 값의 범위를 매핑
    //이를 위해 먼저 최대,최소값을 찾은후 이를 이용하여 매핑한다.
	min=(int)10e10;
	max=(int)-10e10;
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=pImgPrewittX[i*width+j];
			if(newValue<min)
				min=newValue;
			if(newValue>max)
				max=newValue;
		}
	}
	//변환시 상수값을 미리 계산 
	constVal1=(float)(255.0/(max-min));
	constVal2=(float)(-255.0*min/(max-min));
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			//[min,max]사이의 값을 [0,255]값으로 변환 
			newValue=pImgPrewittX[i*width+j];
			newValue=constVal1*newValue+constVal2;
			m_OutImg[i][j]=(BYTE)newValue;
		}
	}

	//동적 할당 메모리 해제
	delete [] pImgPrewittX;
	delete [] pImgPrewittY;
}

void CWinTestDoc::m_EdgeSobel(int height, int width)
{
	int MaskSobelX[3][3]={{-1,0,1},
						{-2,0,2},
						{-1,0,1}};
	int MaskSobelY[3][3]={{1,2,1},
							{0,0,0},
							{-1,-2,-1}};
	int heightm1=height-1;
	int widthm1=width-1;
	int mr,mc;
	int newValue;
	int i,j;
	int *pImgSobelX,*pImgSobelY;
	int min,max,where;
	float constVal1,constVal2;

	//정수값을 갖는 이미지 동적 메모리 할당
	pImgSobelX=new int[height*width];
	pImgSobelY=new int[height*width];

	//결과 이미지 0으로 초기화
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
		{
			m_OutImg[i][j]=0;
			where=i*width+j;
			pImgSobelX[where]=0;
			pImgSobelY[where]=0;
		}

	//X 방향 에지 강도 계산 
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=0; //0으로 초기화
			for(mr=0;mr<3;mr++)
				for(mc=0;mc<3;mc++)
					newValue += (MaskSobelX[mr][mc]*m_InImg[i+mr-1][j+mc-1]);
			pImgSobelX[i*width+j]=newValue;
		}
	}
	//Y 방향 에지 강도 계산 
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=0; //0으로 초기화
			for(mr=0;mr<3;mr++)
				for(mc=0;mc<3;mc++)
					newValue += (MaskSobelY[mr][mc]*m_InImg[i+mr-1][j+mc-1]);
			pImgSobelY[i*width+j]=newValue;
		}
	}
	//에지 강도 계산 절대값(X)+절대값(Y)후 pImgSobelX[]에 저장
	for(i=1;i<heightm1;i++)
		for(j=1;j<widthm1;j++)
		{
			where=i*width+j;
			constVal1=pImgSobelX[where];
			constVal2=pImgSobelY[where];
			if(constVal1<0)
				constVal1=-constVal1;
			if(constVal2<0)
				constVal2=-constVal2;
			pImgSobelX[where]=constVal1+constVal2;
		}
	//디스플레이를 위해 0에서 255사이로 값의 범위를 매핑
    //이를 위해 먼저 최대,최소값을 찾은후 이를 이용하여 매핑한다.
	min=(int)10e10;
	max=(int)-10e10;
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=pImgSobelX[i*width+j];
			if(newValue<min)
				min=newValue;
			if(newValue>max)
				max=newValue;
		}
	}
	//변환시 상수값을 미리 계산 
	constVal1=(float)(255.0/(max-min));
	constVal2=(float)(-255.0*min/(max-min));
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			//[min,max]사이의 값을 [0,255]값으로 변환 
			newValue=pImgSobelX[i*width+j];
			newValue=constVal1*newValue+constVal2;
			m_OutImg[i][j]=(BYTE)newValue;
		}
	}

	//동적 할당 메모리 해제
	delete [] pImgSobelX;
	delete [] pImgSobelY;
}
void CWinTestDoc::m_ZoomIn(int height, int width, float zoominfactor)
{
	BYTE *pZoomImg;
	BYTE newValue;
	int new_height=(int)(height*zoominfactor);//새로운 이미지의 높이 계산 
	int new_width=(int)(width*zoominfactor);//새로운 이미지의 폭 계산 
	int heightm1=height-1;
	int widthm1=width-1;
	int where,org_where;
	int r,c;//타겟 이미지 좌표 
	float r_orgr,r_orgc;//원 이미지 상의 해당 좌표 (실수값)
	int i_orgr,i_orgc;//원 이미지 상의 해당 좌표 (정수값)
	float sr,sc;// 예 1.24-1=0.24
	float I1,I2,I3,I4;


	//Zoom Image를 위한 동적 메모리 할당
	pZoomImg=new BYTE[new_height*new_width];

	for(r=0;r<new_height;r++)
		for(c=0;c<new_width;c++)
		{
			r_orgr=r/zoominfactor;
			r_orgc=c/zoominfactor;
			i_orgr=floor(r_orgr);//예: floor(2.8)=2.0
			i_orgc=floor(r_orgc);
			sr=r_orgr-i_orgr;
			sc=r_orgc-i_orgc;
			//범위 조사 
			//원이미지의 범위를 벗어나는 경우 0값 할당 
			if(i_orgr<0 || i_orgr>heightm1 || i_orgc<0 || i_orgc>widthm1)
			{
				where=r*new_width+c;
				pZoomImg[where]=0;
			}
			//원 이미지의 범위 내에 존재 이중 선형 보간 수행 
			else
			{
				I1=(float)m_InImg[i_orgr][i_orgc];//(org_r,org_c)
				I2=(float)m_InImg[i_orgr][i_orgc+1];//(org_r,org_c+1)
				I3=(float)m_InImg[i_orgr+1][i_orgc+1];//(org_r+1,org_c+1)
				I4=(float)m_InImg[i_orgr+1][i_orgc];//(org_r+1,org_c)

				//이중 선형 보간을 통한 새로운 밝기값 계산
				newValue=(BYTE)(I1*(1-sc)*(1-sr)+I2*sc*(1-sr)+I3*sc*sr+I4*(1-sc)*sr);
				where=r*new_width+c;
				pZoomImg[where]=newValue;
			}
		}
	
	for(r=0;r<height;r++)
		for(c=0;c<width;c++)
		{
			m_OutImg[r][c]=pZoomImg[r*new_width+c];
		}
	//동적 할당 메모리 해제 
	delete [] pZoomImg;
}

void CWinTestDoc::m_ZoomOut(int height, int width, float zoomoutfactor)
{
	BYTE *pZoomImg;
	BYTE newValue;
	int new_height=(int)(height*zoomoutfactor);//새로운 이미지의 높이 계산 
	int new_width=(int)(width*zoomoutfactor);//새로운 이미지의 폭 계산 
	int heightm1=height-1;
	int widthm1=width-1;
	int where,org_where;
	int r,c;//타겟 이미지 좌표 
	float r_orgr,r_orgc;//원 이미지 상의 해당 좌표 (실수값)
	int i_orgr,i_orgc;//원 이미지 상의 해당 좌표 (정수값)
	float sr,sc;// 예 1.24-1=0.24
	float I1,I2,I3,I4;


	//Zoom Image를 위한 동적 메모리 할당
	pZoomImg=new BYTE[new_height*new_width];

	for(r=0;r<new_height;r++)
		for(c=0;c<new_width;c++)
		{
			r_orgr=r/zoomoutfactor;
			r_orgc=c/zoomoutfactor;
			i_orgr=floor(r_orgr);//예: floor(2.8)=2.0
			i_orgc=floor(r_orgc);
			sr=r_orgr-i_orgr;
			sc=r_orgc-i_orgc;
			//범위 조사 
			//원이미지의 범위를 벗어나는 경우 0값 할당 
			if(i_orgr<0 || i_orgr>heightm1 || i_orgc<0 || i_orgc>widthm1)
			{
				where=r*new_width+c;
				pZoomImg[where]=0;
			}
			//원 이미지의 범위 내에 존재 이중 선형 보간 수행 
			else
			{
				I1=(float)m_InImg[i_orgr][i_orgc];//(org_r,org_c)
				I2=(float)m_InImg[i_orgr][i_orgc+1];//(org_r,org_c+1)
				I3=(float)m_InImg[i_orgr+1][i_orgc+1];//(org_r+1,org_c+1)
				I4=(float)m_InImg[i_orgr+1][i_orgc];//(org_r+1,org_c)

				//이중 선형 보간을 통한 새로운 밝기값 계산
				newValue=(BYTE)(I1*(1-sc)*(1-sr)+I2*sc*(1-sr)+I3*sc*sr+I4*(1-sc)*sr);
				where=r*new_width+c;
				pZoomImg[where]=newValue;
			}
		}
	//일단 영상의 일부만 복사하는 것으로 함.
	for(r=0;r<new_height;r++)
		for(c=0;c<new_width;c++)
		{
			m_OutImg[r][c]=pZoomImg[r*new_width+c];
		}
	//동적 할당 메모리 해제 
	delete [] pZoomImg;
}
//center_r: 회전 중심의 y 좌표 
//center_c: 회전 중심의 x 좌표 
void CWinTestDoc::m_Rotation(int height, int width,int center_r,int center_c, float rotationAngle)
{
	BYTE *pRotationImg;
	BYTE newValue;
	int heightm1=height-1;//계산의 중복을 피하기 위해 사용
	int widthm1=width-1;//계산의 중복을 피하기 위해 사용
	int where,org_where;
	int r,c;//타겟 이미지 좌표 
	float r_orgr,r_orgc;//원 이미지 상의 해당 좌표 (실수값)
	int i_orgr,i_orgc;//원 이미지 상의 해당 좌표 (정수값)
	float sr,sc;// 예 1.24-1=0.24
	float I1,I2,I3,I4;
	float cosAngle,sinAngle;
	float rotationAngleRad=(float)(rotationAngle*3.14159265/180);//angle->radian으로 변환

	//Rotation Image를 위한 동적 메모리 할당
	pRotationImg=new BYTE[height*width];

	for(r=0;r<height;r++)
		for(c=0;c<width;c++)
		{
			cosAngle=(float)cos(rotationAngleRad);
			sinAngle=(float)sin(rotationAngleRad);
			//회전전의 원 이미지상의 좌표 구함 
			r_orgr=-sinAngle*(c-center_c)+cosAngle*(r-center_r)+center_r;
			r_orgc=cosAngle*(c-center_c)+sinAngle*(r-center_r)+center_c;
			i_orgr=floor(r_orgr);//예: floor(2.8)=2.0
			i_orgc=floor(r_orgc);
			sr=r_orgr-i_orgr;
			sc=r_orgc-i_orgc;
			//범위 조사 
			//원이미지의 범위를 벗어나는 경우 0값 할당 
			if(i_orgr<0 || i_orgr>heightm1 || i_orgc<0 || i_orgc>widthm1)
			{
				where=r*width+c;
				pRotationImg[where]=0;
			}
			//원 이미지의 범위 내에 존재 이중 선형 보간 수행 
			else
			{
				I1=(float)m_InImg[i_orgr][i_orgc];//(org_r,org_c)
				I2=(float)m_InImg[i_orgr][i_orgc+1];//(org_r,org_c+1)
				I3=(float)m_InImg[i_orgr+1][i_orgc+1];//(org_r+1,org_c+1)
				I4=(float)m_InImg[i_orgr+1][i_orgc];//(org_r+1,org_c)

				//이중 선형 보간을 통한 새로운 밝기값 계산
				newValue=(BYTE)(I1*(1-sc)*(1-sr)+I2*sc*(1-sr)+I3*sc*sr+I4*(1-sc)*sr);
				where=r*width+c;
				pRotationImg[where]=newValue;
			}
		}
	//결과 이미지로 영상 복사 
	for(r=0;r<height;r++)
		for(c=0;c<width;c++)
		{
			m_OutImg[r][c]=pRotationImg[r*width+c];
		}
	//동적 할당 메모리 해제 
	delete [] pRotationImg;
}
//height와 width는 동일 값을 가져야 하며 2의 지수승이어야 한다 
//Foward: 1인경우 Discrete Fourier Transform (Forward)
//        0인 경우 Inverse Discrete Fourier Transform (Backward) 
void CWinTestDoc::m_FFT2D(int height, int width, int Forward)
{
	int r,c,where;
	float *pTwoDRealImage; //FFT연산의 실수부에 해당 
	float *pTwoDImImage; //FFT연산의 허수부에 해당 
	float *pOneDRealImage; //한행 또는 한열에 대한 FFT의 실수부에 해당 
	float *pOneDImImage;//한행 또는 한열에 대한 FFT의 허수부에 해당

	float magnitude,phase,real,imag;//직교 좌표계상의 정보를 Polar Coordinate으로 변경 
	float max,Const,BVal;

	//동적 메모리 할당
	pOneDRealImage=new float[height];
	pOneDImImage=new float[height];
	pTwoDRealImage=new float[height*width];
	pTwoDImImage=new float[height*width];

	//원 이미지로부터 FFT연산을 위한 초기값 설정
	for(r=0;r<height;r++)
		for(c=0;c<width;c++)
		{
			where=r*width+c;
			pTwoDRealImage[where]=m_InImg[r][c];
			pTwoDImImage[where]=0;
		}
	//->방향으로 1차의 FFT 수행 
	for(r=0;r<height;r++)
	{
		//해당하는 행의 데이터 복사 
		for(c=0;c<width;c++)
		{
			where=r*width+c;
			pOneDRealImage[c]=pTwoDRealImage[where];
			pOneDImImage[c]=pTwoDImImage[where];
		}
		//1차의 FFT 수행 
		m_FFT1D(height,pOneDRealImage,pOneDImImage,Forward);
		//1차의 FFT 수행 결과 저장
		for(c=0;c<width;c++)
		{
			where=r*width+c;
			pTwoDRealImage[where]=pOneDRealImage[c];
			pTwoDImImage[where]=pOneDImImage[c];
		}

	}
	//수직 방향으로 1차의 FFT 수행 
	for(c=0;c<width;c++)
	{
		//해당하는 행의 데이터 복사 
		for(r=0;r<height;r++)
		{
			where=r*width+c;
			pOneDRealImage[r]=pTwoDRealImage[where];
			pOneDImImage[r]=pTwoDImImage[where];
		}
		//1차의 FFT 수행 
		m_FFT1D(height,pOneDRealImage,pOneDImImage,Forward);
		//1차의 FFT 수행 결과 저장
		for(r=0;r<width;r++)
		{
			where=r*width+c;
			pTwoDRealImage[where]=pOneDRealImage[r];
			pTwoDImImage[where]=pOneDImImage[r];
		}

	}
	//직교 좌표계상의 정보를 Polar Coordinate으로 변경 
	for(r=0;r<height;r++)
		for(c=0;c<width;c++)
		{
			where=r*width+c;
			real=pTwoDRealImage[where];
			imag=pTwoDImImage[where];
			magnitude=(float)sqrt((real*real+imag*imag));
			pTwoDRealImage[where]=magnitude;
		}

	//값의 범위를 [0~255]로 맞추기 위해 최대값 찾음 
	max=-1;
	for(r=0;r<height;r++)
		for(c=0;c<width;c++)
		{
			magnitude=pTwoDRealImage[r*width+c];
			if(magnitude>max)
				max=magnitude;
		}
	Const=(float)(255.0/log10((1+max)));//값의 범위를 [0~255]로 변환하기 위한 상수 

	//변환 상수값을 이용하여 변환 
	for(r=0;r<height;r++)
		for(c=0;c<width;c++)
		{
			magnitude=pTwoDRealImage[r*width+c];
			BVal=Const*log10((1+magnitude));
			m_OutImg[r][c]=(BYTE)BVal;
		}

	//주파수의 재배치: 저주파 성분을 영상의 중앙부에 오도록 배치
	int half_r=height/2,half_c=width/2;
	int half_r_m1=half_r-1,half_c_m1=half_c-1;
	int rr,cc;
	for(r=0;r<height;r+=half_r)
		for(c=0;c<width;c+=half_c)
		{
			for(rr=0;rr<half_r;rr++)
				for(cc=0;cc<half_c;cc++)
					pTwoDRealImage[(half_r_m1-rr+r)*width+(half_c_m1-cc+c)]=
						m_OutImg[r+rr][c+cc];
		}
	for(r=0;r<height;r++)
		for(c=0;c<width;c++)
			m_OutImg[r][c]=(BYTE)pTwoDRealImage[r*width+c];

	//동적 메모리 할당 해제 
	delete [] pOneDRealImage;
	delete [] pOneDImImage;
	delete [] pTwoDRealImage;
	delete [] pTwoDImImage;
}
//NumData 2의 지수승이어야 한다 
//Foward: 1인 경우 Discrete Fourier Transform (Forward)
//        0인 경우 Inverse Discrete Fourier Transform (Backward) 
void CWinTestDoc::m_FFT1D(int NumData, float *pOneDRealImage, float *pOneDImImage, int Forward)
{
	int log2NumData;
	int HalfNumData;
	int i,j,k,mm;
	int step;
	int ButterFly,ButterFlyHalf;
	float RealValue,ImValue;
	double AngleRadian;
	float CosineValue,SineValue;
	float ArcRe,ArcIm,ReBuf,ImBuf,ArcBuf;

	//scramble 과정 수행 
	//입력 데이터의 순서를 바꿈 
	log2NumData=0;
	while(NumData != (1<<log2NumData))
		log2NumData++;
	HalfNumData=NumData>>1;
	j=1;
	for(i=1;i<NumData;i++)
	{
		if(i<j)
		{
			RealValue=pOneDRealImage[j-1];
			ImValue=pOneDImImage[j-1];
			pOneDRealImage[j-1]=pOneDRealImage[i-1];
			pOneDImImage[j-1]=pOneDImImage[i-1];
			pOneDRealImage[i-1]=RealValue;
			pOneDImImage[i-1]=ImValue;
		}
		k=HalfNumData;
		while(k<j)
		{
			j -= k;
			k=k>>1;
		}
		j += k;
	}
	//butterfly 과정 수행 
	for(step=1;step<=log2NumData;step++)
	{
		ButterFly=1<<step;
		ButterFlyHalf=ButterFly>>1;
		ArcRe=1;
		ArcIm=0;
		AngleRadian=(double)(3.141592/ButterFlyHalf);
		CosineValue=(float)cos(AngleRadian);
		SineValue=(float)sin(AngleRadian);
		if(Forward) //Foward
			SineValue=-SineValue;
		for(j=1;j<=ButterFlyHalf;j++)
		{
			i=j;
			while(i<=NumData)
			{
				mm=i+ButterFlyHalf;
				ReBuf=pOneDRealImage[mm-1]*ArcRe-pOneDImImage[mm-1]*ArcIm;
				ImBuf=pOneDRealImage[mm-1]*ArcIm+pOneDImImage[mm-1]*ArcRe;
				pOneDRealImage[mm-1]=pOneDRealImage[i-1]-ReBuf;
				pOneDImImage[mm-1]=pOneDImImage[i-1]-ImBuf;
				pOneDRealImage[i-1]=pOneDRealImage[i-1]+ReBuf;
				pOneDImImage[i-1]=pOneDImImage[i-1]+ImBuf;
				i += ButterFly;
			}
			ArcBuf=ArcRe;
			ArcRe=ArcRe*CosineValue-ArcIm*SineValue;
			ArcIm=ArcBuf*SineValue+ArcIm*CosineValue;
		}
	}
	if(Forward) //Forward
	{
		for(j=0;j<NumData;j++)
		{
			pOneDRealImage[j] /= NumData;
			pOneDImImage[j] /= NumData;
		}
	}
}