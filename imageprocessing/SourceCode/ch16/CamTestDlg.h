// CamTestDlg.h : header file
//

#if !defined(AFX_CAMTESTDLG_H__ED53CE0C_D994_4632_8068_B2444AA925B3__INCLUDED_)
#define AFX_CAMTESTDLG_H__ED53CE0C_D994_4632_8068_B2444AA925B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vfw.h"

/////////////////////////////////////////////////////////////////////////////
// CCamTestDlg dialog

class CCamTestDlg : public CDialog
{
// Construction
public:
	HWND m_hWndCap;
	CCamTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCamTestDlg)
	enum { IDD = IDD_CAMTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCamTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

LRESULT CALLBACK	capCallbackOnFrame(HWND hWnd, LPVIDEOHDR lpVHdr);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMTESTDLG_H__ED53CE0C_D994_4632_8068_B2444AA925B3__INCLUDED_)
