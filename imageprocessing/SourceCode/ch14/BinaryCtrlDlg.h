#if !defined(AFX_BINARYCTRLDLG_H__F1E5307D_52C8_4F98_B0ED_D138C437D46D__INCLUDED_)
#define AFX_BINARYCTRLDLG_H__F1E5307D_52C8_4F98_B0ED_D138C437D46D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BinaryCtrlDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBinaryCtrlDlg dialog

class CBinaryCtrlDlg : public CDialog
{
// Construction
public:
	unsigned char *m_TmpImg;
	CBinaryCtrlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBinaryCtrlDlg)
	enum { IDD = IDD_DIALOG1 };
	CSliderCtrl	m_CtrlSlider;
	int		m_ThresValueDisp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBinaryCtrlDlg)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBinaryCtrlDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BITMAPINFO *BmInfo;
	int width;
	int height;
	int rwsize;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BINARYCTRLDLG_H__F1E5307D_52C8_4F98_B0ED_D138C437D46D__INCLUDED_)
