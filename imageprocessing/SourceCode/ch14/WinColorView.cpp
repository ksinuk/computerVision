// WinColorView.cpp : implementation of the CWinColorView class
//

#include "stdafx.h"
#include "WinColor.h"

#include "WinColorDoc.h"
#include "WinColorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#include "BinaryCtrlDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CWinColorView

IMPLEMENT_DYNCREATE(CWinColorView, CView)

BEGIN_MESSAGE_MAP(CWinColorView, CView)
	//{{AFX_MSG_MAP(CWinColorView)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(IDM_CHANNELSPLIT_RGB, OnChannelsplitRgb)
	ON_UPDATE_COMMAND_UI(IDM_CHANNELSPLIT_RGB, OnUpdateChannelsplitRgb)
	ON_COMMAND(IDM_CHANNELSPLIT_HSI, OnChannelsplitHsi)
	ON_UPDATE_COMMAND_UI(IDM_CHANNELSPLIT_HSI, OnUpdateChannelsplitHsi)
	ON_COMMAND(IDM_COLOR2GRAY, OnColor2gray)
	ON_UPDATE_COMMAND_UI(IDM_COLOR2GRAY, OnUpdateColor2gray)
	ON_COMMAND(IDM_RGB_EMBOSSING, OnRgbEmbossing)
	ON_UPDATE_COMMAND_UI(IDM_RGB_EMBOSSING, OnUpdateRgbEmbossing)
	ON_COMMAND(IDM_RGB_SHAPENNING, OnRgbShapenning)
	ON_UPDATE_COMMAND_UI(IDM_RGB_SHAPENNING, OnUpdateRgbShapenning)
	ON_COMMAND(IDM_IMG_DYNBIN, OnImgDynbin)
	ON_UPDATE_COMMAND_UI(IDM_SOBEL, OnUpdateSobel)
	ON_COMMAND(IDM_SOBEL, OnSobel)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinColorView construction/destruction

CWinColorView::CWinColorView()
{
	// TODO: add construction code here
	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO)+256*sizeof(RGBQUAD));
	for(int i=0; i<256; i++) 
	{
		BmInfo->bmiColors[i].rgbRed= BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbBlue = i; 
		BmInfo->bmiColors[i].rgbReserved = 0;
	}
}

CWinColorView::~CWinColorView()
{
	if(BmInfo) delete BmInfo;
}

BOOL CWinColorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWinColorView drawing

void CWinColorView::OnDraw(CDC* pDC)
{
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
	if(pDoc->m_InImg==NULL) return;

	height = pDoc->dibHi.biHeight;
	width = pDoc->dibHi.biWidth;
	rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount*pDoc->dibHi.biWidth);
	BmInfo->bmiHeader = pDoc->dibHi;

	SetDIBitsToDevice(pDC->GetSafeHdc(),0,0,width,height,
						0,0,0,height,pDoc->m_InImg,BmInfo, DIB_RGB_COLORS);
}

/////////////////////////////////////////////////////////////////////////////
// CWinColorView printing

BOOL CWinColorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWinColorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWinColorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWinColorView diagnostics

#ifdef _DEBUG
void CWinColorView::AssertValid() const
{
	CView::AssertValid();
}

void CWinColorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinColorDoc* CWinColorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinColorDoc)));
	return (CWinColorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinColorView message handlers

void CWinColorView::OnEditCopy() 
{
	// TODO: Add your command handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->CopyClipboard(pDoc->m_InImg,height,width,pDoc->dibHi.biBitCount);
}

void CWinColorView::OnEditPaste() 
{
	// TODO: Add your command handler code here
	if(!IsClipboardFormatAvailable(CF_DIB)) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void CWinColorView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(IsClipboardFormatAvailable(CF_DIB));	
}

void CWinColorView::OnChannelsplitRgb() 
{
	// TODO: Add your command handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int index, i, j;
	for(int k=2; k>=0; k--)
	{
		for(i=0; i<height*rwsize; i++) pDoc->m_OutImg[i]=0;
		for(i=0; i<height; i++)
		{
			index = (height-i-1)*rwsize;
			for(j=0; j<width; j++) pDoc->m_OutImg[index+3*j+k]= pDoc->m_InImg[index+3*j+k];
		}
		pDoc->CopyClipboard(pDoc->m_OutImg,height,width,24);
		AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
	}
}

void CWinColorView::OnUpdateChannelsplitRgb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount==24);
}

void CWinColorView::OnChannelsplitHsi() 
{
	// TODO: Add your command handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_RGB2HSI(pDoc->m_InImg,pDoc->m_OutImg,height,width);

	int index, i, j;
	int grRWSIZE = WIDTHBYTES(8*width);
	unsigned char *GrayImg = new unsigned char [height*grRWSIZE];
	for(int k=0; k<3; k++)
	{
		for(i=0; i<height; i++)
		{
			index = (height-i-1)*rwsize;
			for(j=0; j<width; j++) GrayImg[(height-i-1)*grRWSIZE+j]= pDoc->m_OutImg[index+3*j+k];
		}
		pDoc->CopyClipboard(GrayImg,height,width,8);
		AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
	}
	delete []GrayImg;	
}

void CWinColorView::OnUpdateChannelsplitHsi(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount==24);
}

void CWinColorView::OnColor2gray() 
{
	// TODO: Add your command handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_RGB2Gray(pDoc->m_InImg,pDoc->m_OutImg,height,width);

	int index, i, j;
	int grRWSIZE = WIDTHBYTES(8*width);
	unsigned char *GrayImg = new unsigned char [height*grRWSIZE];
	for(i=0; i<height; i++)
	{
		index = (height-i-1)*rwsize;
		for(j=0; j<width; j++) GrayImg[(height-i-1)*grRWSIZE+j]= pDoc->m_OutImg[index+3*j];
	}

	pDoc->CopyClipboard(GrayImg,height,width,8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
	delete []GrayImg;	
}

void CWinColorView::OnUpdateColor2gray(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount==24);		
}

void CWinColorView::OnRgbEmbossing() 
{
	// TODO: Add your command handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_ColorEmbossing(pDoc->m_InImg,pDoc->m_OutImg,height,width);

	pDoc->CopyClipboard(pDoc->m_OutImg,height,width,24);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void CWinColorView::OnUpdateRgbEmbossing(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount==24);	
}

void CWinColorView::OnRgbShapenning() 
{
	// TODO: Add your command handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_RGBShapening(pDoc->m_InImg,pDoc->m_OutImg,height,width);

	pDoc->CopyClipboard(pDoc->m_OutImg,height,width,24);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void CWinColorView::OnUpdateRgbShapenning(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount==24);		
}

void CWinColorView::OnImgDynbin() 
{
	// TODO: Add your command handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CBinaryCtrlDlg pBinCtrlDlg;
	pBinCtrlDlg.DoModal();
}

void CWinColorView::OnUpdateSobel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(pDoc->dibHi.biBitCount==8);		
}

void CWinColorView::OnSobel() 
{
	// TODO: Add your command handler code here
	CWinColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_ImgSobel(pDoc->m_InImg,pDoc->m_OutImg,height,width);

	pDoc->CopyClipboard(pDoc->m_OutImg,height,width,8);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}
