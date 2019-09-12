// WinTestDoc.cpp : implementation of the CWinTestDoc class
//

#include "stdafx.h"
#include "WinTest.h"

#include "WinTestDoc.h"

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
