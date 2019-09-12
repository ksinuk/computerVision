// BinaryCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinColor.h"
#include "BinaryCtrlDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#include "MainFrm.h"
#include "ChildFrm.h"
#include "WinColorDoc.h"
#include "WinColorView.h"
/////////////////////////////////////////////////////////////////////////////
// CBinaryCtrlDlg dialog


CBinaryCtrlDlg::CBinaryCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBinaryCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBinaryCtrlDlg)
	m_ThresValueDisp = 0;
	//}}AFX_DATA_INIT
}


void CBinaryCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBinaryCtrlDlg)
	DDX_Control(pDX, IDC_SLIDER1, m_CtrlSlider);
	DDX_Text(pDX, IDC_EDIT1, m_ThresValueDisp);
	DDV_MinMaxInt(pDX, m_ThresValueDisp, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBinaryCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CBinaryCtrlDlg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, OnCustomdrawSlider1)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBinaryCtrlDlg message handlers

BOOL CBinaryCtrlDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

void CBinaryCtrlDlg::OnOK() 
{
	// TODO: Add extra validation here
	CMainFrame *pFrame=(CMainFrame*)AfxGetMainWnd();
	CChildFrame *pChild=(CChildFrame*)pFrame->GetActiveFrame();
	CWinColorDoc *pDoc=(CWinColorDoc*)pChild->GetActiveDocument();
	CWinColorView *pView=(CWinColorView*)pChild->GetActiveView();

	memcpy(pDoc->m_InImg,m_TmpImg,rwsize*height*sizeof(char));

	pView->Invalidate(FALSE);
	
	delete []m_TmpImg;	
	CDialog::OnOK();
}

void CBinaryCtrlDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	delete []m_TmpImg;	
	
	CDialog::OnCancel();
}



BOOL CBinaryCtrlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMainFrame *pFrame=(CMainFrame*)AfxGetMainWnd();
	CChildFrame *pChild=(CChildFrame*)pFrame->GetActiveFrame();
	CWinColorDoc *pDoc=(CWinColorDoc*)pChild->GetActiveDocument();
	CWinColorView *pView=(CWinColorView*)pChild->GetActiveView();

	height = pDoc->height;
	width = pDoc->width;
	rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount*width);
	BmInfo = pView->BmInfo;

	m_TmpImg = new unsigned char [height*rwsize];
	memcpy(pDoc->m_OutImg,pDoc->m_InImg,rwsize*height*sizeof(char));

	m_CtrlSlider.SetRange(0,255);
	m_CtrlSlider.SetPos(100);
	m_ThresValueDisp =m_CtrlSlider.GetPos();
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBinaryCtrlDlg::OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CMainFrame *pFrame=(CMainFrame*)AfxGetMainWnd();
	CChildFrame *pChild=(CChildFrame*)pFrame->GetActiveFrame();
	CWinColorDoc *pDoc=(CWinColorDoc*)pChild->GetActiveDocument();
	CWinColorView *pView=(CWinColorView*)pChild->GetActiveView();

	m_ThresValueDisp=m_CtrlSlider.GetPos();

	int index, index2, i, j, LUT[256];
	for(i=0; i<256; i++) LUT[i] = i > m_ThresValueDisp ? (unsigned char)255 : 0;

	if(pDoc->dibHi.biBitCount==24)
	{
		for(i=0; i<height; i++)
		{
			index = (height-i-1)*rwsize;
			for(j=0; j<width; j++)
			{
				index2 = index+3*j;
				m_TmpImg[index2  ] = LUT[(int)(pDoc->m_OutImg[index2  ])];
				m_TmpImg[index2+1] = LUT[(int)(pDoc->m_OutImg[index2+1])];
				m_TmpImg[index2+2] = LUT[(int)(pDoc->m_OutImg[index2+2])];
			}
		}
	}
	else if(pDoc->dibHi.biBitCount==8)
	{
		for(i=0; i<height; i++)
		{
			index = (height-i-1)*rwsize;
			for(j=0; j<width; j++)
			{
				index2 = index+j;
				m_TmpImg[index2  ] = LUT[(int)(pDoc->m_OutImg[index2  ])];
			}
		}
	}

	UpdateData(FALSE);

	CRect rect;
	GetDlgItem(IDC_IMG_HISTO_VIEW)->GetWindowRect(&rect);
	ScreenToClient(rect);
	InvalidateRect(&rect, FALSE);
}

void CBinaryCtrlDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CPaintDC dcView(GetDlgItem(IDC_IMG_HISTO_VIEW));

	CRect rect;
	GetDlgItem(IDC_IMG_HISTO_VIEW)->GetClientRect(&rect);

	StretchDIBits(dcView.m_hDC,rect.left,rect.top,rect.right,rect.bottom, 0, 0, 
		width, height, m_TmpImg,BmInfo, BI_RGB, SRCCOPY);
	
	// Do not call CDialog::OnPaint() for painting messages
}
