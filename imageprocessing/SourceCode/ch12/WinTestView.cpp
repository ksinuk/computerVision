// WinTestView.cpp : implementation of the CWinTestView class
//

#include "stdafx.h"
#include "WinTest.h"

#include "WinTestDoc.h"
#include "WinTestView.h"

#include "BinCntrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
#include "MainFrm.h"
/////////////////////////////////////////////////////////////////////////////
// CWinTestView

IMPLEMENT_DYNCREATE(CWinTestView, CView)

BEGIN_MESSAGE_MAP(CWinTestView, CView)
	//{{AFX_MSG_MAP(CWinTestView)
	ON_COMMAND(IDM_CONST_ADD, OnConstAdd)
	ON_COMMAND(IDM_CONST_SUB, OnConstSub)
	ON_COMMAND(IDM_CONST_MUL, OnConstMul)
	ON_COMMAND(IDM_CONST_DIV, OnConstDiv)
	ON_COMMAND(IDM_FRM_ADD, OnFrmAdd)
	ON_COMMAND(IDM_FRM_SUB, OnFrmSub)
	ON_COMMAND(IDM_FRM_MUL, OnFrmMul)
	ON_COMMAND(IDM_LUT_MUL, OnLutMul)
	ON_COMMAND(IDM_BINARIZATION, OnBinarization)
	ON_COMMAND(IDM_IMGHISTO, OnImghisto)
	ON_COMMAND(IDM_BIN_DYNAMIC, OnBinDynamic)
	ON_COMMAND(IDM_HISTO_EQUAL, OnHistoEqual)
	ON_COMMAND(IDM_HISTO_STRETCH, OnHistoStretch)
	ON_COMMAND(IDM_HISTO_UPSTRETCH, OnHistoUpstretch)
	ON_COMMAND(IDM_HISTO_SPEC_FILEIN, OnHistoSpecFilein)
	ON_COMMAND(IDM_HISTO_SPEC_CAL, OnHistoSpecCal)
	ON_COMMAND(IDM_BIN_LABELING, OnBinLabeling)
	ON_COMMAND(IDM_BIN_LABELING_B, OnBinLabelingB)
	ON_COMMAND(IDM_BORDER_FOLLOW, OnBorderFollow)
	ON_COMMAND(IDM_OUT2IN, OnOut2in)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(IDM_TEMPMATCH_NGC, OnTempmatchNgc)
	ON_COMMAND(IDM_BLOCKMATCH_MAD, OnBlockmatchMad)
	ON_COMMAND(IDM_BLOCKMATCH_MSE, OnBlockmatchMse)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinTestView construction/destruction

CWinTestView::CWinTestView()
{
	// TODO: add construction code here
	height = width = 256;
	int rwsize = (((8*width)+31)/32*4);  // 4바이트의 배수여야 함
	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO)+256*sizeof(RGBQUAD));

	BmInfo->bmiHeader.biBitCount=8; 
	BmInfo->bmiHeader.biClrImportant=256;
	BmInfo->bmiHeader.biClrUsed=256;
	BmInfo->bmiHeader.biCompression=0;
	BmInfo->bmiHeader.biHeight = height; 
	BmInfo->bmiHeader.biPlanes=1;
	BmInfo->bmiHeader.biSize=40;
	BmInfo->bmiHeader.biSizeImage=rwsize*height; 
	BmInfo->bmiHeader.biWidth =width; 
	BmInfo->bmiHeader.biXPelsPerMeter=0;
	BmInfo->bmiHeader.biYPelsPerMeter=0;
	for(int i=0; i<256; i++) // Palette number is 256
	{
		BmInfo->bmiColors[i].rgbRed= BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbBlue = i; 
		BmInfo->bmiColors[i].rgbReserved = 0;
	}

	// 템플레이트 정합을 위한 설정부 
	BufBmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO)+256*sizeof(RGBQUAD));
	memcpy(BufBmInfo,BmInfo,sizeof(BITMAPINFO));
	memcpy(BufBmInfo->bmiColors,BmInfo->bmiColors,256*sizeof(RGBQUAD));
	m_flagMouse = FALSE;
	m_MatchPos.right = m_MatchPos.bottom =0;
}


CWinTestView::~CWinTestView()
{
	free(BmInfo);
	free(BufBmInfo);
}


BOOL CWinTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWinTestView drawing

void CWinTestView::OnDraw(CDC* pDC)
{
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	// 입력영상(m_InImg)을 화면출력하기 위한 부분 
	for(int i=0; i<height; i++)
		for(int j=0; j<width; j++) m_RevImg[i][j]=pDoc->m_InImg[height-i-1][j];

	SetDIBitsToDevice(pDC->GetSafeHdc(),0,0,width,height,
						0,0,0,height,m_RevImg,BmInfo, DIB_RGB_COLORS);

	// 처리결과영상(m_OutImg)을 화면출력하기 위한 부분 
	for(i=0; i<height; i++)
		for(int j=0; j<width; j++) m_RevImg[i][j]=pDoc->m_OutImg[height-i-1][j];

	SetDIBitsToDevice(pDC->GetSafeHdc(),300,0,width,height,
						0,0,0,height,m_RevImg,BmInfo, DIB_RGB_COLORS);

	// 템플레이트 정합을 위한 부분 
	if(m_flagMouse==TRUE) pDC->DrawEdge(&m_RectTrack,EDGE_ETCHED,BF_RECT); 
	CMainFrame *pFrame= (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame); 
	if(pFrame->m_flagTemplate==TRUE) // template가 설정되어 있는 경우 
	{
		pDC->DrawEdge(&m_RectTrack,EDGE_ETCHED,BF_RECT); 	
		m_DibDraw(pDC,300,0,pFrame->tHeight,pFrame->tWidth,pFrame->m_TempImg);
	}
	if(!(m_MatchPos.right==0 && m_MatchPos.bottom==0)) 
		pDC->DrawEdge(&m_MatchPos,EDGE_BUMP,BF_RECT); 
}

/////////////////////////////////////////////////////////////////////////////
// CWinTestView printing

BOOL CWinTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWinTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWinTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWinTestView diagnostics

#ifdef _DEBUG
void CWinTestView::AssertValid() const
{
	CView::AssertValid();
}

void CWinTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinTestDoc* CWinTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinTestDoc)));
	return (CWinTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinTestView message handlers



void CWinTestView::OnConstAdd() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int tempVal = pDoc->m_InImg[i][j]+60;
			tempVal = tempVal > 255 ? 255: tempVal;
			tempVal = tempVal <   0 ?   0: tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}

	Invalidate(FALSE); //화면 갱신 
}

void CWinTestView::OnConstSub() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int tempVal = pDoc->m_InImg[i][j]-60;
			tempVal = tempVal > 255 ? 255: tempVal;
			tempVal = tempVal <   0 ?   0: tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}

	Invalidate(FALSE); //화면 갱신 
}

void CWinTestView::OnConstMul() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int tempVal = (int)(pDoc->m_InImg[i][j]*1.4);
			tempVal = tempVal > 255 ? 255: tempVal;
			tempVal = tempVal <   0 ?   0: tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}

	Invalidate(FALSE); //화면 갱신 
}

void CWinTestView::OnConstDiv() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int tempVal = (int)(pDoc->m_InImg[i][j]/1.4);
			tempVal = tempVal > 255 ? 255: tempVal;
			tempVal = tempVal <   0 ?   0: tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}

	Invalidate(FALSE); //화면 갱신 
}

void CWinTestView::OnFrmAdd() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->TwoImgLoad();					// 빼기 연산을 할 두 영상을 입력한다.
	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int tempVal = pDoc->m_InImg1[i][j]+pDoc->m_InImg2[i][j];
			tempVal = tempVal > 255 ? 255: tempVal;
			tempVal = tempVal <   0 ?   0: tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}

	Invalidate(FALSE); //화면 갱신 
}

void CWinTestView::OnFrmSub() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->TwoImgLoad();					// 빼기 연산을 할 두 영상을 입력한다.
	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int tempVal = pDoc->m_InImg1[i][j]-pDoc->m_InImg2[i][j];
			tempVal = tempVal > 255 ? 255: tempVal;
			tempVal = tempVal <   0 ?   0: tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}

	Invalidate(FALSE); //화면 갱신 
}

void CWinTestView::OnFrmMul() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->TwoImgLoad();					// 빼기 연산을 할 두 영상을 입력한다.
	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int tempVal = pDoc->m_InImg1[i][j] & pDoc->m_InImg2[i][j];
			tempVal = tempVal > 255 ? 255: tempVal;
			tempVal = tempVal <   0 ?   0: tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}

	Invalidate(FALSE); //화면 갱신 
}

void CWinTestView::OnLutMul() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	unsigned char LUT[256]; // LUT로 사용할 메모리를 선언 

	// LUT값을 계산한다 
	for(int i=0; i<256; i++) 
	{
		int temp=(int)((float)i*1.4);
		LUT[i] = temp > 255 ? 255 : temp;
	}

	// LUT를 통하여 영상을 처리한다
	for(i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			pDoc->m_OutImg[i][j]=LUT[pDoc->m_InImg[i][j]];
		}
	}

	Invalidate(FALSE); //화면 갱신 
}


void CWinTestView::OnImghisto() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	pDoc->m_ImgHisto(256,256);		// 히스토그램 계산 함수 호출 
	Invalidate(FALSE); //화면 갱신 	
}

void CWinTestView::OnBinarization() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			if(pDoc->m_InImg[i][j]>100) pDoc->m_OutImg[i][j]=255;
			else pDoc->m_OutImg[i][j]=0;
		}
	}

	Invalidate(FALSE); //화면 갱신 	
}



void CWinTestView::OnBinDynamic() 
{
	// TODO: Add your command handler code here
	CBinCntrlDlg pbinCtrlDlg;
	pbinCtrlDlg.DoModal();	
}

void CWinTestView::OnHistoEqual() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	// 도큐멘트 클래스에 있는 히스토그램 평활화 함수 호출
	pDoc->m_HistoEqual(256,256);  

	Invalidate(FALSE); //화면 갱신 	
}

void CWinTestView::OnHistoStretch() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	// 도큐멘트 클래스에 있는 히스토그램 평활화 함수 호출
	pDoc->m_HistoStretch(256,256);  

	Invalidate(FALSE); //화면 갱신 		
}

void CWinTestView::OnHistoUpstretch() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	// 도큐멘트 클래스에 있는 개선된 히스토그램 스트레칭함수 호출
	pDoc->m_HistoUpStretch(256,256,20,20);

	Invalidate(FALSE); //화면 갱신 			
}

void CWinTestView::OnHistoSpecFilein() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	// 도큐멘트 클래스에 있는 히스토그램지정을 위한 영상입력함수 호출
	pDoc->m_HistoSpec_FileIn();  
}

void CWinTestView::OnHistoSpecCal() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	// 도큐멘트 클래스에 있는 히스토그램지정 연산함수 호출
	pDoc->m_HistoSpec(256, 256);  

	Invalidate(FALSE); //화면 갱신 			
}

void CWinTestView::OnBinLabeling() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	// 도큐멘트 클래스에 있는 재귀라벨링 함수 호출
	pDoc->grass_label(256, 256);  

	Invalidate(FALSE); //화면 갱신 				
}

void CWinTestView::OnBinLabelingB() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	// 도큐멘트 클래스에 있는 히스토그램지정 연산함수 호출
	pDoc->m_BlobColoring(256, 256);  

	Invalidate(FALSE); //화면 갱신 					
}

void CWinTestView::OnBorderFollow() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	// 도큐멘트 클래스에 있는 영역경계 추적 연산함수 호출
	pDoc->m_BorderFollow(256, 256);  

	Invalidate(FALSE); //화면 갱신 						
}

void CWinTestView::OnOut2in() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	// 도큐멘트 클래스에 있는 영역경계 추적 연산함수 호출
	memcpy(pDoc->m_InImg,pDoc->m_OutImg,256*256*sizeof(char));
	Invalidate(FALSE); //화면 갱신 							
}

void CWinTestView::m_DibDraw(CDC *pDC, int px, int py, int height, int width, BYTE *BufImg)
{
	// TODO: add construction code here
	int rwsize = (((8*width)+31)/32*4);  // 4바이트의 배수여야 함
	BufBmInfo->bmiHeader.biHeight = height; 
	BufBmInfo->bmiHeader.biSizeImage=rwsize*height; 
	BufBmInfo->bmiHeader.biWidth =width; 

	unsigned char *BufRevImg = new unsigned char [height*rwsize];
	int index1,index2,i,j;
	for(i=0; i<height; i++)
	{
		index1 = i*rwsize;
		index2 = (height-i-1)*width;
		for(j=0; j<width; j++) BufRevImg[index1+j]=BufImg[index2+j];
	}

	SetDIBitsToDevice(pDC->GetSafeHdc(),px,py,width,height,
						0,0,0,height,BufRevImg,BufBmInfo, DIB_RGB_COLORS);
	delete []BufRevImg;
}

void CWinTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(point.y<0 || point.y>=height) return;
	if(point.x<0 || point.x>=width) return;

	m_RectTrack.left = m_RectTrack.right = point.x;
	m_RectTrack.top = m_RectTrack.bottom = point.y;
	
	m_flagMouse = TRUE;
	CMainFrame *pFrame= (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	pFrame->m_flagTemplate = FALSE;
	
	CView::OnLButtonDown(nFlags, point);
}

void CWinTestView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_flagMouse==TRUE)
	{
		if(point.y<0 || point.y>=height) { Invalidate(FALSE); return; }
		if(point.x<0 || point.x>=width) { Invalidate(FALSE); return; }

		m_RectTrack.right = point.x;
		m_RectTrack.bottom = point.y;
		Invalidate(FALSE);
	}
	
	CView::OnMouseMove(nFlags, point);
}

void CWinTestView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_flagMouse==TRUE)
	{
		CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
		ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 
		CMainFrame *pFrame= (CMainFrame*)AfxGetMainWnd();
		ASSERT(pFrame);

		if(point.y<0 || point.y>=height) { m_flagMouse=pFrame->m_flagTemplate = FALSE; Invalidate(FALSE); return; }
		if(point.x<0 || point.x>=width) { m_flagMouse=pFrame->m_flagTemplate = FALSE; Invalidate(FALSE); return; }
		if(m_RectTrack.top>=m_RectTrack.bottom || m_RectTrack.left>=m_RectTrack.right) 
			{ m_flagMouse=FALSE; pFrame->m_flagTemplate=TRUE; Invalidate(FALSE); return;}
		if(abs(m_RectTrack.right-m_RectTrack.left)<10 && 
			abs(m_RectTrack.bottom-m_RectTrack.top)<10) 
			{ m_flagMouse=FALSE; pFrame->m_flagTemplate=TRUE; Invalidate(FALSE); return;}  

		///
		m_flagMouse = FALSE;
		pFrame->m_flagTemplate = TRUE;

		pFrame->tHeight = m_RectTrack.bottom-m_RectTrack.top;
		pFrame->tWidth = m_RectTrack.right-m_RectTrack.left;
		if(pFrame->m_TempImg==NULL) pFrame->m_TempImg = new unsigned char [pFrame->tHeight*pFrame->tWidth];
		else 
		{
			delete []pFrame->m_TempImg;
			pFrame->m_TempImg = new unsigned char [pFrame->tHeight*pFrame->tWidth];
		}
		int i,j,index;
		for(i=0; i<pFrame->tHeight; i++)
		{
			index = i*pFrame->tWidth;
			for(j=0; j<pFrame->tWidth; j++) 
				pFrame->m_TempImg[index+j]=pDoc->m_InImg[m_RectTrack.top+i][m_RectTrack.left+j];
		}

		Invalidate(FALSE);
	}
	
	CView::OnLButtonUp(nFlags, point);
}

void CWinTestView::OnTempmatchNgc() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 
	CMainFrame *pFrame= (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);

	if(pFrame->m_flagTemplate==FALSE) return;

	POINT OptPos = pDoc->m_TemplateMatch(256, 256, pFrame->m_TempImg, pFrame->tHeight, pFrame->tWidth);

	m_MatchPos.left = OptPos.x;
	m_MatchPos.top = OptPos.y;
	m_MatchPos.right = OptPos.x+pFrame->tWidth;
	m_MatchPos.bottom = OptPos.y+pFrame->tHeight;

	Invalidate(FALSE);
}

void CWinTestView::OnBlockmatchMad() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 
	CMainFrame *pFrame= (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);

	if(pFrame->m_flagTemplate==FALSE) return;

	POINT OptPos = pDoc->m_MatchMAD(256, 256, pFrame->m_TempImg, pFrame->tHeight, pFrame->tWidth);

	m_MatchPos.left = OptPos.x;
	m_MatchPos.top = OptPos.y;
	m_MatchPos.right = OptPos.x+pFrame->tWidth;
	m_MatchPos.bottom = OptPos.y+pFrame->tHeight;

	Invalidate(FALSE);
}

void CWinTestView::OnBlockmatchMse() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 
	CMainFrame *pFrame= (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);

	if(pFrame->m_flagTemplate==FALSE) return;

	POINT OptPos = pDoc->m_MatchMSE(256, 256, pFrame->m_TempImg, pFrame->tHeight, pFrame->tWidth);

	m_MatchPos.left = OptPos.x;
	m_MatchPos.top = OptPos.y;
	m_MatchPos.right = OptPos.x+pFrame->tWidth;
	m_MatchPos.bottom = OptPos.y+pFrame->tHeight;

	Invalidate(FALSE);	
}

