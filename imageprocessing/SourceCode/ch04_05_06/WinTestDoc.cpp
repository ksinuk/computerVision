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
		ar.Write(m_OutImg,256*256); // ó���� ����迭 m_OutImg�� ���Ϸ� ���� 
	}
	else
	{
		// TODO: add loading code here
		CFile *infile = ar.GetFile(); // �Է��� ȭ���� ����Ʈ�� ������.
		if(infile->GetLength()!=256*256) // ȭ�� ����� �˻��� 
		{
			AfxMessageBox("���� ũ�Ⱑ 256x256����� �ƴմϴ�.");
			return;
		}
		ar.Read(m_InImg,infile->GetLength()); // ���������� �о� m_InImg�迭�� ���� 
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
	CFileDialog opendlg1(TRUE); //���� ��ȭ ����(ù��° ���� ����) 
	CFile file;

	if(opendlg1.DoModal()==IDOK) 
	{
		file.Open(opendlg1.GetFileName(), CFile::modeRead);
		file.Read(m_InImg1,sizeof(m_InImg1));
		file.Close();
	}


	CFileDialog opendlg2(TRUE); //���� ��ȭ ����(�ι�° ���� ����) 

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

	// ������׷� ũ�� ����ȭ 
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

	// ������׷��� ��� 
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
	/// histogram������ ���� ����� �迭�޸𸮸� �Ҵ� 
	unsigned int *histogram = new unsigned int [256];
	unsigned int *sum_hist = new unsigned int [256];

	/// histogram�迭�� �ʱ�ȭ 
	for(i=0; i<256; i++) histogram[i]=0;

	/// ������ histogram�� ��� 
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++) histogram[m_InImg[i][j]]++;
	}

	/// histogram�� ����ȭ�� ���� ��� 
	int sum=0;
	float scale_factor=255.0f/(float)(height*width);

	for(i=0; i<256; i++)
	{
		sum += histogram[i];
		sum_hist[i] =(int)((sum*scale_factor) + 0.5);
	}

	/// LUT�ν� sum_hist�迭�� ����Ͽ� ������ ��ȯ 
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++) m_OutImg[i][j]=sum_hist[m_InImg[i][j]];
	}

	// �޸𸮸� ���� 
	delete []histogram; delete []sum_hist;
}

void CWinTestDoc::m_HistoStretch(int height, int width)
{
	int i,j;
	int lowvalue=255, highvalue=0;

	// ��Ⱚ�� ���� ������ ���� 
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++) if(m_InImg[i][j]<lowvalue) lowvalue = m_InImg[i][j];
	}

	// ��Ⱚ�� ���� ū�� ���� 
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++) if(m_InImg[i][j]>highvalue) highvalue = m_InImg[i][j];
	}

	// Histogram��Ʈ��Ī ��� 
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
	// histogram������ ���� ����� �迭�� �Ҵ� 
	unsigned int *histogram = new unsigned int [256];

	// histogram�迭�� �ʱ�ȭ 
	for(i=0; i<256; i++) histogram[i]=0;

	// ������ histogram�� ����϶� 
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

	// ���ο� LUT�� ����� 
	unsigned char *LUT=new unsigned char [256];

	for(i=0; i<lowthresh; i++) LUT[i]=0;
	for(i=255; i>highthresh; i--) LUT[i]=255;

	float scale = 255.0f/(float)(highthresh-lowthresh);
	for(i=lowthresh; i<=highthresh; i++) 
		LUT[i] = (unsigned char)((i-lowthresh) * scale);

	// LUT�ν� sum_hist�迭�� ����Ͽ� ������ ��ȯ�϶� 
	for(i=0; i<height; i++) for(j=0; j<width; j++) m_OutImg[i][j]=LUT[m_InImg[i][j]];

	// �޸𸮸� ����
	delete []histogram; delete []LUT;
}

void CWinTestDoc::m_HistoSpec_FileIn()
{
	CFileDialog opendlg1(TRUE); //���� ��ȭ ����(������׷��� ������ �������� �Է�) 
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
	// histogram������ ���� ����� �迭�� �Ҵ� 
	unsigned int *histogram = new unsigned int [256];
	unsigned int *sum_hist = new unsigned int [256];
	unsigned int *desired_histogram = new unsigned int [256];
	unsigned int *desired_sum_hist = new unsigned int [256];

	// histogram�迭�� �ʱ�ȭ 
	for(i=0; i<256; i++) histogram[i]=desired_histogram[i]=0;

	// ������ histogram�� ����϶� 
	for(i=0; i<height; i++) 
	{
		for(j=0; j<width; j++) 
		{
			histogram[m_InImg[i][j]]++;  // �Է� ������ ������׷� 
			desired_histogram[m_InImg1[i][j]]++;  // ���� ������ ������׷� 
	}}


	// histogram�� ����ȭ�� ���� ����϶� 
	int sum=0;
	float scale_factor=255.0f/(float)(height*width);

	for(i=0; i<256; i++)
	{
		sum += histogram[i];
		sum_hist[i] =(int)((sum*scale_factor) + 0.5);
	}

	// desired histogram�� ���� ����ȭ�� ���� ���
	sum=0;
	for(i=0; i<256; i++) 
	{
		sum += desired_histogram[i];
		desired_sum_hist[i] =(int)(sum * scale_factor);
	}

	// ���� ����� ����ȭ�� ������׷����� �ִ� index�� ã��   
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

	// �� ������׷� ��ȯ 
	for(i=0; i<height; i++) for(j=0; j<width; j++) m_OutImg[i][j]=inv_hist[m_InImg[i][j]];

	// �޸� ���� 
	delete []inv_hist; delete []histogram; delete []desired_histogram; 
	delete []sum_hist; delete []desired_sum_hist;
}
