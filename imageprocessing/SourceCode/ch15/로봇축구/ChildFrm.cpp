// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "WinColor.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#include "WinColorDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
void CChildFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	CRect rect, rectC;
	GetWindowRect(&rect); GetClientRect(&rectC);

	CSize sizeImg;

	sizeImg.cx= ((CWinColorDoc *)GetActiveDocument())->width;
	sizeImg.cy= ((CWinColorDoc *)GetActiveDocument())->height;
	int cx = sizeImg.cx + rect.Width() - rectC.Width() +4;	
	int cy = sizeImg.cy + rect.Height() - rectC.Height() +4;
	SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_SHOWWINDOW); 
	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}
