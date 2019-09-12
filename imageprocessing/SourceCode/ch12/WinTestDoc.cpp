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
#include "MainFrm.h"
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
	height = width = 256;
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
	UpdateAllViews(FALSE);
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





void CWinTestDoc::grass_label(int height, int width)
{
	// 라벨링된 영상을 저장하기 위한 배열의 메모리 할당 
	short *coloring = new short [height*width];

	int i, j, curColor=0;
	for(i=0; i<height*width; i++) coloring[i]=0;

	// 영역의 면적을 Text정보로 출력  
	CString tempStr;
	tempStr.Format("\r\n\r\n[ Blob Coloring(1) ]\r\n-----------------------------------------\r\n");
	ResultShowDlgBar(tempStr);

	// 입력 영상의 라벨링 
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			if(m_InImg[i][j]==255 && coloring[i*width+j]==0)
			{
				curColor++;
				m_BlobPixelSum =0;
				grass(coloring,height,width,i,j,curColor);

				tempStr.Format("%d: %d\r\n",curColor,m_BlobPixelSum);
				ResultShowDlgBar(tempStr);
			}
	}}

	float grayGap = 250.0f/(float)curColor; 

	// 라벨링된 데이터를 m_OutImg배열을 이용하여 화면출력 
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			int value = (int)(coloring[i*width+j]*grayGap);
			if(value==0) m_OutImg[i][j] = 255;
			else m_OutImg[i][j] = value;
	}}

	delete []coloring; // 메모리 해제 
}

void CWinTestDoc::grass(short *coloring,int height,int width,int i, int j,int curColor)
{
	int k, l, index;
	for(k=i-1; k<=i+1; k++)
	{
		for(l=j-1; l<=j+1; l++)
		{
			// 영상의 경계를 벗어나면 라벨링하지 않음 
			if(k<0 || k>=height || l<0 || l>=width) continue; 

			index = k*width+l;
			// 아직 방문하지 않은 픽셀이고 값이 255라면 라벨링함 
			if(m_InImg[k][l]==255 && coloring[index]==0) 
			{
				coloring[index] = curColor; 
				m_BlobPixelSum++;
				grass(coloring,height,width,k,l,curColor);
			}
		}
	}
}




int CWinTestDoc::push(short *stackx, short *stacky, short vx, short vy, int *top)
{
	if(*top>=300000) return(-1);
	(*top)++;
	stackx[*top]=vx;
	stacky[*top]=vy;
	return(1);
}

int CWinTestDoc::pop(short *stackx, short *stacky, short *vx, short *vy, int *top)
{
	if(*top==0) return(-1);
	*vx=stackx[*top];
	*vy=stacky[*top];
	(*top)--;
	return(1);
}

void CWinTestDoc::m_BlobColoring(int height, int width)
{
	//
	int i,j,m,n,top, BlobArea[1000];
	short curColor=0, r,c, area;

	// 스택으로 사용할 메모리 할당 
	short* stackx=new short [height*width];
	short* stacky=new short [height*width];

	// 라벨링된 픽셀을 저장하기 위해 메모리 할당 
	short *coloring = new short [height*width];
	for(i=0; i<height*width; i++) coloring[i]=0; //메모리 초기화 

	for(i=0; i<height; i++) 
	{
		for(j=0; j<width; j++) 
		{	
			// 이미 방문한 점이거나 픽셀값이 255가 아니라면 처리 안함 
			if(coloring[i*width+j]!=0 || m_InImg[i][j]!=255) continue;

			r=i; 
			c=j;
			top=0; area=1;
			curColor++;

			while(1) 
			{
GRASSFIRE:
				for(m=r-1; m<=r+1; m++)
				{
					for(n=c-1; n<=c+1; n++) 
					{
						// 관심 픽셀이 영상경계를 벗어나면 처리 안함  
						if(m<0 || m>=height || n<0 || n>=width) continue;

						if((int)m_InImg[m][n]==255 && coloring[m*width+n]==0)
						{
							coloring[m*width+n]=curColor;  // 현재 라벨로 마크 
							if(push(stackx,stacky,(short)m,(short)n,&top)==-1) continue;
							r=m; 
							c=n;
							area++;
							goto GRASSFIRE;
						}
					}
				}
				if(pop(stackx,stacky,&r,&c,&top)==-1) break;
			}
			if(curColor<1000) BlobArea[curColor] = area;
		}
	}

	// 영역의 면적을 Text정보로 출력  
	CString tempStr;
	tempStr.Format("\r\n\r\n[ Blob Coloring(2) ]\r\n-----------------------------------------\r\n");
	ResultShowDlgBar(tempStr);
	for(i=1; i<=curColor; i++) 
	{
		tempStr.Format("%i: %d\r\n",i,BlobArea[i]);
		ResultShowDlgBar(tempStr);
	}

	// 
	float grayGap = 250.0f/(float)curColor; 

	// 라벨링된 데이터를 m_OutImg배열을 이용하여 화면출력 
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			int value = (int)(coloring[i*width+j]*grayGap);
			if(value==0) m_OutImg[i][j] = 255;
			else m_OutImg[i][j] = value;
	}}

	delete []coloring; delete []stackx; delete []stacky;
}


void CWinTestDoc::m_BorderFollow(int height, int width)
{
	// 영역의 경계정보를 저장하기 위한 구조체 메모리 
	typedef struct tagBORDERINFO{ short *x,*y; short n, dn; } BORDERINFO;
	BORDERINFO stBorderInfo[1000];

	// 영상에 있는 픽셀이 방문된 점인지를 마크하기 위해 영상 메모리 할당 
	unsigned char *visited = new unsigned char [height*width];
	memset(visited,0,height*width*sizeof(char)); // 메모리 초기화 


	// 추적점을 임시로 저장하기 위한 메모리 
	short *xchain = new short [10000];
	short *ychain = new short [10000];
	
	// 관심 픽셀의 시계방향으로 주위점을 나타내기 위한 좌표 설정 
	const POINT nei[8] =           // clockwise neighbours
	{ 
		{1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}
	};

	int x0, y0, x, y, k, n;
	int numberBorder=0,border_count,diagonal_count;
	unsigned char c0,c1;

	for(x=1; x < height; x++)   
	{
		for(y=1; y < width; y++)
		{
			c0=m_InImg[x][y];
			c1=m_InImg[x-1][y];

			if(c0!=c1&&c0==(unsigned char)255 && visited[x*width+y]==0 )  //c0!=c1(경계이고)
			{
				border_count=0;  // 경계점의 갯수를 세기 위한 카운터
				diagonal_count=0;  // 

				x0 = x;
		        y0 = y;
				n = 4;
				do 
				{
					// 관심점 주위에서 같은 칼라를 가진 경계점을 찾기 위함 
					for(k=0; k < 8; k++, n=((n+1) & 7))  // 01234567
					{									 // 12345670
			            short  u = (short)(x + nei[n].x);
			            short  v = (short)(y + nei[n].y);
						if(u<0 || u>=height || v<0 || v>=width) continue;
						if(m_InImg[u][v]==c0) break; // 관심점의 주위를 돌다가 같은 밝기의     
													// 경계를 만나면 다음으로 추적할 점이 된다. 
					}
					if(k == 8) break;    // isolated point occurs
						
					visited[x*width+y]=255;  // 방문한 점으로 마크 
					xchain[border_count]=x;
					ychain[border_count++]=y;

					if(border_count>=10000) break;

					x = x + nei[n].x;
					y = y + nei[n].y;

					if(n%2==1) diagonal_count++;
									 // 01234567 
					n = (n + 5) & 7; // 56701234
				}
				while(!(x == x0  &&  y == y0));

		        if  (k == 8) continue;    // isolated point occurs

		        // 경계정보를 저장 
				if(border_count<10) continue; // 너무작은 영역의 경계이면 무시한다.  

				stBorderInfo[numberBorder].x=new short [border_count];
				stBorderInfo[numberBorder].y=new short [border_count];

				for(k=0; k<border_count; k++) 
				{
					stBorderInfo[numberBorder].x[k]=xchain[k];
					stBorderInfo[numberBorder].y[k]=ychain[k];
				} 
				stBorderInfo[numberBorder].n=border_count;
				stBorderInfo[numberBorder++].dn=diagonal_count;

				if(numberBorder>= 1000) break;
			}
		}
	}


	// 영역의 면적을 Text정보로 출력  
	CString tempStr;
	tempStr.Format("\r\n\r\n[ Border Following ]\r\n-----------------------------------------\r\n");
	ResultShowDlgBar(tempStr);

	// 화면에 경계를 출력하기 위해 m_OutImg배열을 이용하자. 
	memset(m_OutImg,255,height*width*sizeof(char));
	for(k=0; k<numberBorder; k++)
	{
		tempStr.Format("(%d: %d %d, %d)\r\n",k,stBorderInfo[k].n, stBorderInfo[k].dn, (int)(sqrt(2)*stBorderInfo[k].dn)+(stBorderInfo[k].n-stBorderInfo[k].dn));
		ResultShowDlgBar(tempStr);
		for(int i=0; i<stBorderInfo[k].n; i++)
		{
			x = stBorderInfo[k].x[i];
			y = stBorderInfo[k].y[i];
			m_OutImg[x][y] = 0;
	}}

	for(k=0; k<numberBorder; k++) { delete []stBorderInfo[k].x; delete []stBorderInfo[k].y; }
	delete []visited; delete []xchain; delete []ychain;
}

void CWinTestDoc::ResultShowDlgBar(CString str)
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CRect rect; 
	pFrame->GetWindowRect(&rect);

	if (!pFrame->m_ResultShowBar.IsWindowVisible())
	{
		pFrame->DockControlBar(&pFrame->m_ResultShowBar);
		pFrame->m_ResultShowBar.ShowWindow(SW_SHOW);
		pFrame->FloatControlBar(&pFrame->m_ResultShowBar,CPoint(rect.right-324,rect.bottom-125));
	}

	CEdit *pEdit = (CEdit *)pFrame->m_ResultShowBar.GetDlgItem(IDC_RESULTSHOW);
	int nLength = pEdit->GetWindowTextLength();
	
	if(nLength<10000) pEdit->SetSel(nLength, nLength);
	else pEdit->SetSel(nLength-10000, nLength);
	pEdit->ReplaceSel(str);
	pFrame->RecalcLayout();
}


POINT CWinTestDoc::m_TemplateMatch(int height, int width, unsigned char *m_TempImg, int tHeight, int tWidth)
{
	float ST=0.0f,temp;
	float SumT=0.0f;
	int NoT =0;

	register int i,j,m,n,index;

	for(i=0;i<tHeight;i++)
	{
		index = i*tWidth;
		for(j=0;j<tWidth ;j++) 
		{
			temp =(float)m_TempImg[index+j];
			SumT  += temp;
			ST += temp * temp ;
			NoT++;
	}}

	// ProgressBar표시를 위한 부분 
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->m_pProgressBar.SetRange(0, height-tHeight);
	pFrame->m_pProgressBar.SetStep(1);
    CRect rc;
    pFrame->pStatusBar->GetItemRect (0, rc); 
	pFrame->m_pProgressBar.MoveWindow(&rc);


	float MaxCorr=0.0f, CurCorr, a1, a2, DeNomi, Nomi;
	float SumI, SI, ScT, tpi;
	POINT OptPos;
	CString tempStr;
	tempStr.Format("\r\n[ Template Matching Started ]\r\n----------------------------\r\n");
	ResultShowDlgBar(tempStr);
	for(m=0; m<height-tHeight; m++) 
	{
		for(n=0; n<width-tWidth; n++) 
		{
			SumI = SI= ScT= 0.0f;
			for(i=0;i<tHeight;i++) 
			{
				index = i*tWidth; 
				for(j=0; j<tWidth ;j++) 
				{
					temp  = (float)m_InImg[m+i][n+j];
					tpi   = (float)m_TempImg[index+j];
					SumI += temp;
					SI   += temp * temp ;
					ScT  += tpi*temp;
				}
			}

			a1 =NoT*SI-SumI*SumI;
			a2 =NoT*ST-SumT*SumT;
			DeNomi = (float)( a1*a2 );
			Nomi = ( NoT*ScT - SumI*SumT);
			
			if (DeNomi < 0.0001 ) CurCorr = 0;
			else                  CurCorr = Nomi*Nomi/DeNomi;

			if (CurCorr>MaxCorr) 
			{
				MaxCorr=CurCorr;
				OptPos.y=m;
				OptPos.x=n;
			}
		}

		pFrame->m_pProgressBar.StepIt();
	}
	tempStr.Format("Optimal Position: (%d, %d)\r\n Maximum NGC Value: %7.4f\r\n",OptPos.y,OptPos.x,sqrt(MaxCorr));
	ResultShowDlgBar(tempStr);

	return OptPos;
}


POINT CWinTestDoc::m_MatchMAD(int height, int width, unsigned char *m_TempImg, int tHeight, int tWidth)
{
	register int i,j,m,n,index;
	float SumD, MinCorr=255.0f*255.0f*tHeight*tWidth; 
	POINT OptPos;

	// ProgressBar표시를 위한 부분 
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->m_pProgressBar.SetRange(0, height-tHeight);
	pFrame->m_pProgressBar.SetStep(1);
    CRect rc;
    pFrame->pStatusBar->GetItemRect (0, rc); 
	pFrame->m_pProgressBar.MoveWindow(&rc);
	
	CString tempStr;
	tempStr.Format("\r\n[ MAD Matching Started ]\r\n----------------------------\r\n");
	ResultShowDlgBar(tempStr);

	for(m=0; m<height-tHeight; m++) 
	{
		for(n=0; n<width-tWidth; n++) 
		{
			SumD = 0.0f;
			for(i=0;i<tHeight;i++) 
			{
				index = i*tWidth; 
				for(j=0; j<tWidth ;j++) SumD += (float)fabs((int)m_InImg[m+i][n+j]-(int)m_TempImg[index+j]);
			}

			if (SumD<MinCorr) 
			{
				MinCorr=SumD;
				OptPos.y=m;
				OptPos.x=n;
			}
		}
		pFrame->m_pProgressBar.StepIt();
	}
	MinCorr /= (float)(tHeight*tWidth);
	tempStr.Format("Optimal Position: (%d, %d)\r\n Minimum MAD Value: %7.3f\r\n",OptPos.y,OptPos.x,MinCorr);
	ResultShowDlgBar(tempStr);

	return OptPos;
}


POINT CWinTestDoc::m_MatchMSE(int height, int width, unsigned char *m_TempImg, int tHeight, int tWidth)
{
	register int i,j,m,n,index;
	int diff;
	float SumD, MinCorr=255.0f*255.0f*tHeight*tWidth; 
	POINT OptPos;

	// ProgressBar표시를 위한 부분 
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->m_pProgressBar.SetRange(0, height-tHeight);
	pFrame->m_pProgressBar.SetStep(1);
    CRect rc;
    pFrame->pStatusBar->GetItemRect (0, rc); 
	pFrame->m_pProgressBar.MoveWindow(&rc);

	CString tempStr;
	tempStr.Format("\r\n[ MSE Matching Started ]\r\n----------------------------\r\n");
	ResultShowDlgBar(tempStr);

	for(m=0; m<height-tHeight; m++) 
	{
		for(n=0; n<width-tWidth; n++) 
		{
			SumD = 0.0f; 
			for(i=0;i<tHeight;i++) 
			{
				index = i*tWidth; 
				for(j=0; j<tWidth ;j++) 
				{
					diff = m_InImg[m+i][n+j]-m_TempImg[index+j];
					SumD += (float)(diff*diff);
				}
			}

			if(SumD<MinCorr) 
			{
				MinCorr=SumD;
				OptPos.y=m;
				OptPos.x=n;
			}
		}
		pFrame->m_pProgressBar.StepIt();
	}

	MinCorr /= (float)(tHeight*tWidth);
	tempStr.Format("Optimal Position: (%d, %d)\r\n Minimum MSE Value: %7.3f\r\n",OptPos.y,OptPos.x,MinCorr);
	ResultShowDlgBar(tempStr);

	return OptPos;
}
