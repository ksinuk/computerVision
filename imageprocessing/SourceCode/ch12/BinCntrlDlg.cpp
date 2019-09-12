// BinCntrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinTest.h"
#include "BinCntrlDlg.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "WinTestDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBinCntrlDlg dialog


CBinCntrlDlg::CBinCntrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBinCntrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBinCntrlDlg)
	m_binValDisp = 0;
	//}}AFX_DATA_INIT
}


void CBinCntrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBinCntrlDlg)
	DDX_Control(pDX, IDC_SLIDER1, m_ctrlSlider);
	DDX_Text(pDX, IDC_EDIT1, m_binValDisp);
	DDV_MinMaxInt(pDX, m_binValDisp, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBinCntrlDlg, CDialog)
	//{{AFX_MSG_MAP(CBinCntrlDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnBNApply)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, OnCustomdrawSlider1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBinCntrlDlg message handlers

void CBinCntrlDlg::OnBNApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE); // 잡아온 데이터값으로 다이알로그박스의 표시부 갱신 
	OnOK(); // 다이알로그박스 종료 
}


BOOL CBinCntrlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctrlSlider.SetRange(0,255); // 슬라이드바 값의 범위 설정 
	m_ctrlSlider.SetPos(100); // 현재 슬라이드바의 기본값 설정 

	m_binValDisp = m_ctrlSlider.GetPos(); // 현재 설정되어 있는값을 가져옴   
	UpdateData(FALSE); // 현재 설정데이터값으로 다이알로그박스의 표시부 갱신

	return TRUE;                
}


void CBinCntrlDlg::OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CMainFrame *pFrame= (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	CChildFrame *pChild = (CChildFrame*)pFrame->GetActiveFrame();
	ASSERT(pChild);
	CWinTestDoc *pDoc = (CWinTestDoc*)pChild->GetActiveDocument();
	ASSERT(pDoc);


	// Slider의 현재 위치를 가져옴  
	m_binValDisp = m_ctrlSlider.GetPos();
	UpdateData(FALSE); // 잡아온 데이터값으로 다이알로그박스의 표시부 갱신 

	// 현재 활성화 도큐멘트클래스 아래의 이치화 계산 함수를 호출 
	pDoc->m_BinThres(256,256,m_binValDisp); // 슬라이드에서 설정한 이치화 계수치를 넘겨준다. 	
}

