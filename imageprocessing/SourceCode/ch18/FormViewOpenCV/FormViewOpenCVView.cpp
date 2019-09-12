// FormViewOpenCVView.cpp : implementation of the CFormViewOpenCVView class
//

#include "stdafx.h"
#include "FormViewOpenCV.h"

#include "FormViewOpenCVDoc.h"
#include "FormViewOpenCVView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormViewOpenCVView

IMPLEMENT_DYNCREATE(CFormViewOpenCVView, CFormView)

BEGIN_MESSAGE_MAP(CFormViewOpenCVView, CFormView)
	//{{AFX_MSG_MAP(CFormViewOpenCVView)
	ON_BN_CLICKED(IDC_LOAD_IMAGE, OnLoadImage)
	ON_BN_CLICKED(IDC_PROCESSING, OnProcessing)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormViewOpenCVView construction/destruction

CFormViewOpenCVView::CFormViewOpenCVView()
	: CFormView(CFormViewOpenCVView::IDD)
{
	//{{AFX_DATA_INIT(CFormViewOpenCVView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CFormViewOpenCVView::~CFormViewOpenCVView()
{
}

void CFormViewOpenCVView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormViewOpenCVView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CFormViewOpenCVView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CFormViewOpenCVView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

		// Set up bitmap info structures for each image
	m_iNumRowDisp=480;
	m_iNumColDisp=640;

    m_hRectBitmapInfoHdlGrey=CreateGreyscaleBitmapInfo(m_iNumRowDisp,
													   m_iNumColDisp,
													   m_iNumColDisp,
													   FALSE);
	m_hRectBitmapInfoHdlColor=CreateColorBitmapInfo(m_iNumRowDisp,
													m_iNumColDisp,
													m_iNumColDisp,
													FALSE);
	m_pImgIplDispColor=cvCreateImage(cvSize(m_iNumColDisp,m_iNumRowDisp),IPL_DEPTH_8U,3);
	m_pImgIplDispGrey=cvCreateImage(cvSize(m_iNumColDisp,m_iNumRowDisp),IPL_DEPTH_8U,1);
}

/////////////////////////////////////////////////////////////////////////////
// CFormViewOpenCVView printing

BOOL CFormViewOpenCVView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFormViewOpenCVView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFormViewOpenCVView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CFormViewOpenCVView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CFormViewOpenCVView diagnostics

#ifdef _DEBUG
void CFormViewOpenCVView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormViewOpenCVView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFormViewOpenCVDoc* CFormViewOpenCVView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFormViewOpenCVDoc)));
	return (CFormViewOpenCVDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormViewOpenCVView message handlers

void CFormViewOpenCVView::OnLoadImage() 
{
	CString strFn;
	CFileDialog dlgFile(TRUE);
	dlgFile.m_ofn.lpstrTitle="Select Image";

	if(IDOK==dlgFile.DoModal())
	{
		strFn=dlgFile.GetPathName();
		m_pImgIplIn=cvLoadImage(LPCTSTR(strFn));

		cvResize(m_pImgIplIn,m_pImgIplDispColor);
		dspIplImg(CWnd::GetDlgItem(IDC_WINDOW_IN),m_pImgIplDispColor,m_hRectBitmapInfoHdlColor,
			m_iNumRowDisp,m_iNumColDisp); 
	}
}

void CFormViewOpenCVView::OnProcessing() 
{
	// smoothing
	m_pImgIplOut=cvCloneImage(m_pImgIplIn);
	cvSmooth(m_pImgIplIn,m_pImgIplOut);
	cvSmooth(m_pImgIplOut,m_pImgIplOut);
	cvSmooth(m_pImgIplOut,m_pImgIplOut);

	cvResize(m_pImgIplOut,m_pImgIplDispColor);
	dspIplImg(CWnd::GetDlgItem(IDC_WINDOW_OUT),m_pImgIplDispColor,m_hRectBitmapInfoHdlColor,
			m_iNumRowDisp,m_iNumColDisp); 
}
int CFormViewOpenCVView::dspIplImg(CWnd* pWnd,IplImage* pImgIpl,HANDLE hBitmapInfo,int iNumRow,int iNumCol)
{
	CRect rectDisp;
	BYTE *pImg;

	pImg=(BYTE *)pImgIpl->imageData;
		
	pWnd->GetClientRect(&rectDisp);

	/////////////////////////////////////////////////////////////////////////////
	// draw image to screen
    if(pImg!=NULL)
    {
		HDC hDC;
		CRect rectImg;

		hDC=::GetDC(pWnd->GetSafeHwnd());
     
		rectImg.left=rectImg.top=0;
		rectImg.right=iNumCol;
		rectImg.bottom=iNumRow;

		// put it to the screen
		PaintImage(hDC,
				   &rectDisp, // display rectangle
				   &rectImg, // image rectangle
				   hBitmapInfo,
				   pImg);
	   ::ReleaseDC(pWnd->GetSafeHwnd(),hDC);
    }

	return 0;
}