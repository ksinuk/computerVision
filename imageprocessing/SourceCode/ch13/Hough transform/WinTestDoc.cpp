// WinTestDoc.cpp : implementation of the CWinTestDoc class
//

#include "stdafx.h"
#include "WinTest.h"
#include "math.h"

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
	ON_COMMAND(IDM_BINAR_OTZU, OnBinarOtzu)
	ON_COMMAND(IDM_BINAR_ADAP, OnBinarAdap)
	ON_COMMAND(IDM_Hough_Line, OnHoughLine)
	ON_COMMAND(IDM_Hough_Circle, OnHoughCircle)
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

void CWinTestDoc::OnBinarOtzu() 
{
	// TODO: Add your command handler code here
	int height = 256, width = 256;
	register int i,j;

	unsigned char* orgImg = new unsigned char [height*width];
	unsigned char* outImg = new unsigned char [height*width];

	for(i=0; i<height; i++) for(j=0; j<width; j++) orgImg[i*width+j] = m_InImg[i][j];

	Otzu_Threshold(orgImg, outImg, height, width);

	for(i=0; i<height; i++) for(j=0; j<width; j++) m_OutImg[i][j] = outImg[i*width+j];

	delete []orgImg; 
	delete []outImg;

	UpdateAllViews(NULL);	
}

void CWinTestDoc::Otzu_Threshold(unsigned char *orgImg, unsigned char *outImg, int height, int width)
{
	register int i,t;

	// Histogram set
	int		hist[256];
	float   prob[256];
	for(i=0; i<256; i++) { hist[i]=0; prob[i] = 0.0f; }
	for(i=0; i<height*width; i++) hist[(int)orgImg[i]]++; 
	for(i=0; i<256; i++) prob[i] = (float)hist[i]/(float)(height*width);


	float wsv_min = 1000000.0f;
	float wsv_u1, wsv_u2, wsv_s1, wsv_s2;
	int wsv_t;

	for(t=0; t<256; t++)
	{
		// q1, q2 ��� 
		float q1 = 0.0f, q2 = 0.0f;

		for(i=0; i<t; i++)   q1 += prob[i]; 
		for(i=t; i<256; i++) q2 += prob[i];

		if(q1==0 || q2==0) continue;

		// u1, u2 ��� 
		float u1=0.0f, u2=0.0f;
		for(i=0; i<t; i++)   u1 += i*prob[i]; u1 /= q1;
		for(i=t; i<256; i++) u2 += i*prob[i]; u2 /= q2;

		// s1, s2 ��� 
		float s1=0.0f, s2=0.0f;
		for(i=0; i<t; i++)   s1 += (i-u1)*(i-u1)*prob[i]; s1 /= q1;
		for(i=t; i<256; i++) s2 += (i-u2)*(i-u2)*prob[i]; s2 /= q2;

		float wsv = q1*s1+q2*s2;

		if(wsv < wsv_min) 
		{ 
			wsv_min = wsv; wsv_t = t; 
			wsv_u1 = u1; wsv_u2 = u2;
			wsv_s1 = s1; wsv_s2 = s2;
		}
	}
	
	// Display the result values
	CString strTemp;
	strTemp.Format("Optimal Threshold: %3d\nMean: %7.3f, %7.3f\nVariance: %7.3f, %7.3f",wsv_t,wsv_u1,wsv_u2,wsv_s1,wsv_s2);
	AfxMessageBox(strTemp);

	// thresholding
	for(i=0; i<height*width; i++) if(orgImg[i]<wsv_t) outImg[i]=0; else outImg[i]=255;
}



void CWinTestDoc::OnBinarAdap() 
{
	// TODO: Add your command handler code here
	int height = 256, width = 256;
	register int i,j;

	unsigned char* orgImg = new unsigned char [height*width];
	unsigned char* outImg = new unsigned char [height*width];

	for(i=0; i<height; i++) for(j=0; j<width; j++) orgImg[i*width+j] = m_InImg[i][j];


	AdaptiveBinarization(orgImg, outImg, height, width);


	for(i=0; i<height; i++) for(j=0; j<width; j++) m_OutImg[i][j] = outImg[i*width+j];

	delete []orgImg; 
	delete []outImg;

	UpdateAllViews(NULL);		
}

void CWinTestDoc::AdaptiveBinarization(unsigned char *orgImg, unsigned char *outImg, int height, int width)
{
	register int i,j, k, l;
	int gval, index1, index2;
	float mean, vari, thres;
	int W = 20;

	for(i=0; i<height*width; i++) outImg[i] = 255;

	for(i=0; i<height; i++)
	{
		index2 = i*width;
		for(j=0; j<width; j++)
		{
			float gsum  = 0.0f;
			float ssum  = 0.0f;	
			int   count = 0;

			for(k=i-W; k<=i+W; k++)
			{
				index1  = k*width;
				if(k<0 || k >= height) continue;

				for(l=j-W; l<=j+W; l++)
				{
					if(l<0 || l >= width) continue;

					gval  = orgImg[index1+l];
					gsum += gval;
					ssum += gval*gval;
					count++;
				}
			}

			mean	= gsum/(float)count; 
			vari	= ssum/(float)count-mean*mean;

			if(vari<0) vari = 0.0f;

			thres	= mean*(1.0f-0.02f*(1-(float)sqrt(vari)/128));

			if(orgImg[index2+j]<thres) outImg[index2+j] = 0; 
		}
	}
}

void CWinTestDoc::OnHoughLine() 
{
	// TODO: Add your command handler code here
	int height = 256, width = 256;
	register int i,j;

	unsigned char* orgImg = new unsigned char [height*width];
	unsigned char* outImg = new unsigned char [height*width];

	for(i=0; i<height; i++) for(j=0; j<width; j++) orgImg[i*width+j] = m_InImg[i][j];

	HT_Line(orgImg, outImg, height, width);

	for(i=0; i<height; i++) for(j=0; j<width; j++) m_OutImg[i][j] = outImg[i*width+j];

	delete []orgImg; 
	delete []outImg;

	UpdateAllViews(NULL);		
}

void CWinTestDoc::HT_Line(unsigned char *orgImg, unsigned char *outImg, int height, int width)
{
	register int i,j,k;
	int d, index;
	float p2d = 3.141592654f/180.0f;

	int H[360][362]={0};
	int thres = 60;

	float *LUT_COS = new float [360];
	float *LUT_SIN = new float [360];
	
	for(i=0; i<360; i++) 
	{
		LUT_COS[i] = (float)cos(i*p2d);
		LUT_SIN[i] = (float)sin(i*p2d);
	}

	for(i=0; i<height*width; i++) outImg[i] = orgImg[i];

	// For voting
	for(i=0; i<height; i++)
	{
		index = i*width;
		for(j=0; j<width; j++)
		{
			if(orgImg[index+j] == 255)
			{
				for(k=0; k<360; k++)
				{
					d = (int)( i*LUT_COS[k] + j*LUT_SIN[k] );
					if(d >=4 && d<= 360) H[k][d]++;
				}
			}
		}
	}

	// For display
	for(d=4; d<=360; d++)
	{
		for(k=0; k<360; k++)
		{
			if(H[k][d] > thres) 
			{
				i = j = 2;

				for(j=2; j<height; j++) // vertical pixel
				{
					i = (int)( (d-j*LUT_SIN[k])/LUT_COS[k] );
					if( i<height && i>0) outImg[i*width+j] = 255;
				}

				for(i=2; i<width; i++) // horizontal pixel
				{
					j = (int)( (d-i*LUT_COS[k])/LUT_SIN[k] );
					if( j<height && j>0) outImg[i*width+j] = 255;
				}
			}
		}
	}

	delete []LUT_COS; 
	delete []LUT_SIN;
}

void CWinTestDoc::OnHoughCircle() 
{
	// TODO: Add your command handler code here
	int height = 256, width = 256;
	register int i,j;

	unsigned char* orgImg = new unsigned char [height*width];
	unsigned char* outImg = new unsigned char [height*width];

	for(i=0; i<height; i++) for(j=0; j<width; j++) orgImg[i*width+j] = m_InImg[i][j];

	HT_Circle(orgImg, outImg, height, width);

	for(i=0; i<height; i++) for(j=0; j<width; j++) m_OutImg[i][j] = outImg[i*width+j];

	delete []orgImg; 
	delete []outImg;

	UpdateAllViews(NULL);			
}

void CWinTestDoc::HT_Circle(unsigned char *orgImg, unsigned char *outImg, int height, int width)
{
	register int i,j,k,ang;
	int index, rr, cc;
	int r_min = 30, r_max = 100;
	float p2d = 3.141592654f/180.0f;

	unsigned int thres = 90;

	// Memory allocation
	unsigned int ***H = new unsigned int** [height];
	for(i=0; i<height; i++)
	{
		H[i] = new unsigned int* [width];
		for(j=0; j<width; j++)
		{
			H[i][j] = new unsigned int [r_max];
			for(k=0; k<r_max; k++) H[i][j][k] = 0;
		}
	}

	float *LUT_COS = new float [360];
	float *LUT_SIN = new float [360];
	
	for(i=0; i<360; i++) 
	{
		LUT_COS[i] = (float)cos(i*p2d);
		LUT_SIN[i] = (float)sin(i*p2d);
	}

	for(i=0; i<height*width; i++) outImg[i] = orgImg[i];


	// For vote
	for(i=0; i<height; i++)
	{
		index = i*width;
		for(j=0; j<width; j++)
		{
			if(orgImg[index+j] == 255)
			{
				for(k=r_min; k<r_max; k++)
				{
					for(ang=0; ang<360; ang++)
					{
						rr = (int)( i-k*LUT_COS[ang] );
						cc = (int)( j-k*LUT_SIN[ang] );

						if(rr<height && rr>0 && cc<width && cc>0) H[rr][cc][k]++;
					}
				}
			}
		}
	}

	// For display
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			for(k=r_min; k<r_max; k++)
			{
				if( H[i][j][k] > thres )
				{
					for(ang=0; ang<360; ang++)
					{
						rr = (int)( i+k*LUT_COS[ang] );
						cc = (int)( j+k*LUT_SIN[ang] );

						if(rr>0 && rr<height && cc>0 && cc<width) outImg[rr*width+cc] = 255;
					}
				}
			}
		}
	}


	// Delete arrays
	for(i=0; i<height; i++) 
	{
		for(j=0; j<width; j++) delete [] H[i][j];
		delete [] H[i];
	}
	delete []H;

	delete []LUT_COS; 
	delete []LUT_SIN;
}