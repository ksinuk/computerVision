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
	
	//��� �̹��� 0���� �ʱ�ȭ
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			m_OutImg[i][j]=0;

	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=0; //0���� �ʱ�ȭ
			for(mr=0;mr<3;mr++)
				for(mc=0;mc<3;mc++)
					newValue += (MaskBox[mr][mc]*m_InImg[i+mr-1][j+mc-1]);
			newValue /= 9; //����ũ�� ���� ũ��� ������:���� ������ 0���� 255�� ��
			m_OutImg[i][j]=(BYTE)newValue;//BYTE������ ��ȯ 
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
	//��� �̹��� 0���� �ʱ�ȭ
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			m_OutImg[i][j]=0;

	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=0; //0���� �ʱ�ȭ
			for(mr=0;mr<3;mr++)
				for(mc=0;mc<3;mc++)
					newValue += (MaskGaussian[mr][mc]*m_InImg[i+mr-1][j+mc-1]);
			newValue /= 20; //����ũ�� ���� ũ��� ������:���� ������ 0���� 255�� ��
			m_OutImg[i][j]=(BYTE)newValue;//BYTE������ ��ȯ 
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

	//�������� ���� �̹��� ���� �޸� �Ҵ�
	pTmpImg=new int[height*width];

	//��� �̹��� 0���� �ʱ�ȭ
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
			newValue=0; //0���� �ʱ�ȭ
			for(mr=0;mr<3;mr++)
				for(mc=0;mc<3;mc++)
					newValue += (MaskBox[mr][mc]*m_InImg[i+mr-1][j+mc-1]);
			//���� ����� ��ȯ 
			if(newValue<0)
				newValue=-newValue;
			pTmpImg[i*width+j]=newValue;
		}
	}

	//���÷��̸� ���� 0���� 255���̷� ���� ������ ����
    //�̸� ���� ���� �ִ�,�ּҰ��� ã���� �̸� �̿��Ͽ� �����Ѵ�.
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
	//��ȯ�� ������� �̸� ��� 
	constVal1=(float)(255.0/(max-min));
	constVal2=(float)(-255.0*min/(max-min));	
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			//[min,max]������ ���� [0,255]������ ��ȯ 
			newValue=pTmpImg[i*width+j];
			newValue=constVal1*newValue+constVal2;
			m_OutImg[i][j]=(BYTE)newValue;
		}
	}
	//���� �Ҵ� �޸� ����
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

	//�������� ���� �̹��� ���� �޸� �Ҵ�
	pImgPrewittX=new int[height*width];
	pImgPrewittY=new int[height*width];

	//��� �̹��� 0���� �ʱ�ȭ
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
		{
			m_OutImg[i][j]=0;
			where=i*width+j;
			pImgPrewittX[where]=0;
			pImgPrewittY[where]=0;
		}

	//X ���� ���� ���� ��� 
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=0; //0���� �ʱ�ȭ
			for(mr=0;mr<3;mr++)
				for(mc=0;mc<3;mc++)
					newValue += (MaskPrewittX[mr][mc]*m_InImg[i+mr-1][j+mc-1]);
			pImgPrewittX[i*width+j]=newValue;
		}
	}
	//Y ���� ���� ���� ��� 
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=0; //0���� �ʱ�ȭ
			for(mr=0;mr<3;mr++)
				for(mc=0;mc<3;mc++)
					newValue += (MaskPrewittY[mr][mc]*m_InImg[i+mr-1][j+mc-1]);
			pImgPrewittY[i*width+j]=newValue;
		}
	}
	//���� ���� ��� ���밪(X)+���밪(Y)�� pImgPrewittX[]�� ����
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
	//���÷��̸� ���� 0���� 255���̷� ���� ������ ����
    //�̸� ���� ���� �ִ�,�ּҰ��� ã���� �̸� �̿��Ͽ� �����Ѵ�.
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
	//��ȯ�� ������� �̸� ��� 
	constVal1=(float)(255.0/(max-min));
	constVal2=(float)(-255.0*min/(max-min));
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			//[min,max]������ ���� [0,255]������ ��ȯ 
			newValue=pImgPrewittX[i*width+j];
			newValue=constVal1*newValue+constVal2;
			m_OutImg[i][j]=(BYTE)newValue;
		}
	}

	//���� �Ҵ� �޸� ����
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

	//�������� ���� �̹��� ���� �޸� �Ҵ�
	pImgSobelX=new int[height*width];
	pImgSobelY=new int[height*width];

	//��� �̹��� 0���� �ʱ�ȭ
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
		{
			m_OutImg[i][j]=0;
			where=i*width+j;
			pImgSobelX[where]=0;
			pImgSobelY[where]=0;
		}

	//X ���� ���� ���� ��� 
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=0; //0���� �ʱ�ȭ
			for(mr=0;mr<3;mr++)
				for(mc=0;mc<3;mc++)
					newValue += (MaskSobelX[mr][mc]*m_InImg[i+mr-1][j+mc-1]);
			pImgSobelX[i*width+j]=newValue;
		}
	}
	//Y ���� ���� ���� ��� 
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			newValue=0; //0���� �ʱ�ȭ
			for(mr=0;mr<3;mr++)
				for(mc=0;mc<3;mc++)
					newValue += (MaskSobelY[mr][mc]*m_InImg[i+mr-1][j+mc-1]);
			pImgSobelY[i*width+j]=newValue;
		}
	}
	//���� ���� ��� ���밪(X)+���밪(Y)�� pImgSobelX[]�� ����
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
	//���÷��̸� ���� 0���� 255���̷� ���� ������ ����
    //�̸� ���� ���� �ִ�,�ּҰ��� ã���� �̸� �̿��Ͽ� �����Ѵ�.
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
	//��ȯ�� ������� �̸� ��� 
	constVal1=(float)(255.0/(max-min));
	constVal2=(float)(-255.0*min/(max-min));
	for(i=1; i<heightm1; i++)
	{
		for(j=1; j<widthm1; j++)
		{
			//[min,max]������ ���� [0,255]������ ��ȯ 
			newValue=pImgSobelX[i*width+j];
			newValue=constVal1*newValue+constVal2;
			m_OutImg[i][j]=(BYTE)newValue;
		}
	}

	//���� �Ҵ� �޸� ����
	delete [] pImgSobelX;
	delete [] pImgSobelY;
}
void CWinTestDoc::m_ZoomIn(int height, int width, float zoominfactor)
{
	BYTE *pZoomImg;
	BYTE newValue;
	int new_height=(int)(height*zoominfactor);//���ο� �̹����� ���� ��� 
	int new_width=(int)(width*zoominfactor);//���ο� �̹����� �� ��� 
	int heightm1=height-1;
	int widthm1=width-1;
	int where,org_where;
	int r,c;//Ÿ�� �̹��� ��ǥ 
	float r_orgr,r_orgc;//�� �̹��� ���� �ش� ��ǥ (�Ǽ���)
	int i_orgr,i_orgc;//�� �̹��� ���� �ش� ��ǥ (������)
	float sr,sc;// �� 1.24-1=0.24
	float I1,I2,I3,I4;


	//Zoom Image�� ���� ���� �޸� �Ҵ�
	pZoomImg=new BYTE[new_height*new_width];

	for(r=0;r<new_height;r++)
		for(c=0;c<new_width;c++)
		{
			r_orgr=r/zoominfactor;
			r_orgc=c/zoominfactor;
			i_orgr=floor(r_orgr);//��: floor(2.8)=2.0
			i_orgc=floor(r_orgc);
			sr=r_orgr-i_orgr;
			sc=r_orgc-i_orgc;
			//���� ���� 
			//���̹����� ������ ����� ��� 0�� �Ҵ� 
			if(i_orgr<0 || i_orgr>heightm1 || i_orgc<0 || i_orgc>widthm1)
			{
				where=r*new_width+c;
				pZoomImg[where]=0;
			}
			//�� �̹����� ���� ���� ���� ���� ���� ���� ���� 
			else
			{
				I1=(float)m_InImg[i_orgr][i_orgc];//(org_r,org_c)
				I2=(float)m_InImg[i_orgr][i_orgc+1];//(org_r,org_c+1)
				I3=(float)m_InImg[i_orgr+1][i_orgc+1];//(org_r+1,org_c+1)
				I4=(float)m_InImg[i_orgr+1][i_orgc];//(org_r+1,org_c)

				//���� ���� ������ ���� ���ο� ��Ⱚ ���
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
	//���� �Ҵ� �޸� ���� 
	delete [] pZoomImg;
}

void CWinTestDoc::m_ZoomOut(int height, int width, float zoomoutfactor)
{
	BYTE *pZoomImg;
	BYTE newValue;
	int new_height=(int)(height*zoomoutfactor);//���ο� �̹����� ���� ��� 
	int new_width=(int)(width*zoomoutfactor);//���ο� �̹����� �� ��� 
	int heightm1=height-1;
	int widthm1=width-1;
	int where,org_where;
	int r,c;//Ÿ�� �̹��� ��ǥ 
	float r_orgr,r_orgc;//�� �̹��� ���� �ش� ��ǥ (�Ǽ���)
	int i_orgr,i_orgc;//�� �̹��� ���� �ش� ��ǥ (������)
	float sr,sc;// �� 1.24-1=0.24
	float I1,I2,I3,I4;


	//Zoom Image�� ���� ���� �޸� �Ҵ�
	pZoomImg=new BYTE[new_height*new_width];

	for(r=0;r<new_height;r++)
		for(c=0;c<new_width;c++)
		{
			r_orgr=r/zoomoutfactor;
			r_orgc=c/zoomoutfactor;
			i_orgr=floor(r_orgr);//��: floor(2.8)=2.0
			i_orgc=floor(r_orgc);
			sr=r_orgr-i_orgr;
			sc=r_orgc-i_orgc;
			//���� ���� 
			//���̹����� ������ ����� ��� 0�� �Ҵ� 
			if(i_orgr<0 || i_orgr>heightm1 || i_orgc<0 || i_orgc>widthm1)
			{
				where=r*new_width+c;
				pZoomImg[where]=0;
			}
			//�� �̹����� ���� ���� ���� ���� ���� ���� ���� 
			else
			{
				I1=(float)m_InImg[i_orgr][i_orgc];//(org_r,org_c)
				I2=(float)m_InImg[i_orgr][i_orgc+1];//(org_r,org_c+1)
				I3=(float)m_InImg[i_orgr+1][i_orgc+1];//(org_r+1,org_c+1)
				I4=(float)m_InImg[i_orgr+1][i_orgc];//(org_r+1,org_c)

				//���� ���� ������ ���� ���ο� ��Ⱚ ���
				newValue=(BYTE)(I1*(1-sc)*(1-sr)+I2*sc*(1-sr)+I3*sc*sr+I4*(1-sc)*sr);
				where=r*new_width+c;
				pZoomImg[where]=newValue;
			}
		}
	//�ϴ� ������ �Ϻθ� �����ϴ� ������ ��.
	for(r=0;r<new_height;r++)
		for(c=0;c<new_width;c++)
		{
			m_OutImg[r][c]=pZoomImg[r*new_width+c];
		}
	//���� �Ҵ� �޸� ���� 
	delete [] pZoomImg;
}
//center_r: ȸ�� �߽��� y ��ǥ 
//center_c: ȸ�� �߽��� x ��ǥ 
void CWinTestDoc::m_Rotation(int height, int width,int center_r,int center_c, float rotationAngle)
{
	BYTE *pRotationImg;
	BYTE newValue;
	int heightm1=height-1;//����� �ߺ��� ���ϱ� ���� ���
	int widthm1=width-1;//����� �ߺ��� ���ϱ� ���� ���
	int where,org_where;
	int r,c;//Ÿ�� �̹��� ��ǥ 
	float r_orgr,r_orgc;//�� �̹��� ���� �ش� ��ǥ (�Ǽ���)
	int i_orgr,i_orgc;//�� �̹��� ���� �ش� ��ǥ (������)
	float sr,sc;// �� 1.24-1=0.24
	float I1,I2,I3,I4;
	float cosAngle,sinAngle;
	float rotationAngleRad=(float)(rotationAngle*3.14159265/180);//angle->radian���� ��ȯ

	//Rotation Image�� ���� ���� �޸� �Ҵ�
	pRotationImg=new BYTE[height*width];

	for(r=0;r<height;r++)
		for(c=0;c<width;c++)
		{
			cosAngle=(float)cos(rotationAngleRad);
			sinAngle=(float)sin(rotationAngleRad);
			//ȸ������ �� �̹������� ��ǥ ���� 
			r_orgr=-sinAngle*(c-center_c)+cosAngle*(r-center_r)+center_r;
			r_orgc=cosAngle*(c-center_c)+sinAngle*(r-center_r)+center_c;
			i_orgr=floor(r_orgr);//��: floor(2.8)=2.0
			i_orgc=floor(r_orgc);
			sr=r_orgr-i_orgr;
			sc=r_orgc-i_orgc;
			//���� ���� 
			//���̹����� ������ ����� ��� 0�� �Ҵ� 
			if(i_orgr<0 || i_orgr>heightm1 || i_orgc<0 || i_orgc>widthm1)
			{
				where=r*width+c;
				pRotationImg[where]=0;
			}
			//�� �̹����� ���� ���� ���� ���� ���� ���� ���� 
			else
			{
				I1=(float)m_InImg[i_orgr][i_orgc];//(org_r,org_c)
				I2=(float)m_InImg[i_orgr][i_orgc+1];//(org_r,org_c+1)
				I3=(float)m_InImg[i_orgr+1][i_orgc+1];//(org_r+1,org_c+1)
				I4=(float)m_InImg[i_orgr+1][i_orgc];//(org_r+1,org_c)

				//���� ���� ������ ���� ���ο� ��Ⱚ ���
				newValue=(BYTE)(I1*(1-sc)*(1-sr)+I2*sc*(1-sr)+I3*sc*sr+I4*(1-sc)*sr);
				where=r*width+c;
				pRotationImg[where]=newValue;
			}
		}
	//��� �̹����� ���� ���� 
	for(r=0;r<height;r++)
		for(c=0;c<width;c++)
		{
			m_OutImg[r][c]=pRotationImg[r*width+c];
		}
	//���� �Ҵ� �޸� ���� 
	delete [] pRotationImg;
}
//height�� width�� ���� ���� ������ �ϸ� 2�� �������̾�� �Ѵ� 
//Foward: 1�ΰ�� Discrete Fourier Transform (Forward)
//        0�� ��� Inverse Discrete Fourier Transform (Backward) 
void CWinTestDoc::m_FFT2D(int height, int width, int Forward)
{
	int r,c,where;
	float *pTwoDRealImage; //FFT������ �Ǽ��ο� �ش� 
	float *pTwoDImImage; //FFT������ ����ο� �ش� 
	float *pOneDRealImage; //���� �Ǵ� �ѿ��� ���� FFT�� �Ǽ��ο� �ش� 
	float *pOneDImImage;//���� �Ǵ� �ѿ��� ���� FFT�� ����ο� �ش�

	float magnitude,phase,real,imag;//���� ��ǥ����� ������ Polar Coordinate���� ���� 
	float max,Const,BVal;

	//���� �޸� �Ҵ�
	pOneDRealImage=new float[height];
	pOneDImImage=new float[height];
	pTwoDRealImage=new float[height*width];
	pTwoDImImage=new float[height*width];

	//�� �̹����κ��� FFT������ ���� �ʱⰪ ����
	for(r=0;r<height;r++)
		for(c=0;c<width;c++)
		{
			where=r*width+c;
			pTwoDRealImage[where]=m_InImg[r][c];
			pTwoDImImage[where]=0;
		}
	//->�������� 1���� FFT ���� 
	for(r=0;r<height;r++)
	{
		//�ش��ϴ� ���� ������ ���� 
		for(c=0;c<width;c++)
		{
			where=r*width+c;
			pOneDRealImage[c]=pTwoDRealImage[where];
			pOneDImImage[c]=pTwoDImImage[where];
		}
		//1���� FFT ���� 
		m_FFT1D(height,pOneDRealImage,pOneDImImage,Forward);
		//1���� FFT ���� ��� ����
		for(c=0;c<width;c++)
		{
			where=r*width+c;
			pTwoDRealImage[where]=pOneDRealImage[c];
			pTwoDImImage[where]=pOneDImImage[c];
		}

	}
	//���� �������� 1���� FFT ���� 
	for(c=0;c<width;c++)
	{
		//�ش��ϴ� ���� ������ ���� 
		for(r=0;r<height;r++)
		{
			where=r*width+c;
			pOneDRealImage[r]=pTwoDRealImage[where];
			pOneDImImage[r]=pTwoDImImage[where];
		}
		//1���� FFT ���� 
		m_FFT1D(height,pOneDRealImage,pOneDImImage,Forward);
		//1���� FFT ���� ��� ����
		for(r=0;r<width;r++)
		{
			where=r*width+c;
			pTwoDRealImage[where]=pOneDRealImage[r];
			pTwoDImImage[where]=pOneDImImage[r];
		}

	}
	//���� ��ǥ����� ������ Polar Coordinate���� ���� 
	for(r=0;r<height;r++)
		for(c=0;c<width;c++)
		{
			where=r*width+c;
			real=pTwoDRealImage[where];
			imag=pTwoDImImage[where];
			magnitude=(float)sqrt((real*real+imag*imag));
			pTwoDRealImage[where]=magnitude;
		}

	//���� ������ [0~255]�� ���߱� ���� �ִ밪 ã�� 
	max=-1;
	for(r=0;r<height;r++)
		for(c=0;c<width;c++)
		{
			magnitude=pTwoDRealImage[r*width+c];
			if(magnitude>max)
				max=magnitude;
		}
	Const=(float)(255.0/log10((1+max)));//���� ������ [0~255]�� ��ȯ�ϱ� ���� ��� 

	//��ȯ ������� �̿��Ͽ� ��ȯ 
	for(r=0;r<height;r++)
		for(c=0;c<width;c++)
		{
			magnitude=pTwoDRealImage[r*width+c];
			BVal=Const*log10((1+magnitude));
			m_OutImg[r][c]=(BYTE)BVal;
		}

	//���ļ��� ���ġ: ������ ������ ������ �߾Ӻο� ������ ��ġ
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

	//���� �޸� �Ҵ� ���� 
	delete [] pOneDRealImage;
	delete [] pOneDImImage;
	delete [] pTwoDRealImage;
	delete [] pTwoDImImage;
}
//NumData 2�� �������̾�� �Ѵ� 
//Foward: 1�� ��� Discrete Fourier Transform (Forward)
//        0�� ��� Inverse Discrete Fourier Transform (Backward) 
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

	//scramble ���� ���� 
	//�Է� �������� ������ �ٲ� 
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
	//butterfly ���� ���� 
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