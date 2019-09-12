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
	UpdateData(FALSE); // ��ƿ� �����Ͱ����� ���̾˷α׹ڽ��� ǥ�ú� ���� 
	OnOK(); // ���̾˷α׹ڽ� ���� 
}


BOOL CBinCntrlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctrlSlider.SetRange(0,255); // �����̵�� ���� ���� ���� 
	m_ctrlSlider.SetPos(100); // ���� �����̵���� �⺻�� ���� 

	m_binValDisp = m_ctrlSlider.GetPos(); // ���� �����Ǿ� �ִ°��� ������   
	UpdateData(FALSE); // ���� ���������Ͱ����� ���̾˷α׹ڽ��� ǥ�ú� ����

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


	// Slider�� ���� ��ġ�� ������  
	m_binValDisp = m_ctrlSlider.GetPos();
	UpdateData(FALSE); // ��ƿ� �����Ͱ����� ���̾˷α׹ڽ��� ǥ�ú� ���� 

	// ���� Ȱ��ȭ ��ť��ƮŬ���� �Ʒ��� ��ġȭ ��� �Լ��� ȣ�� 
	pDoc->m_BinThres(256,256,m_binValDisp); // �����̵忡�� ������ ��ġȭ ���ġ�� �Ѱ��ش�. 	
}

