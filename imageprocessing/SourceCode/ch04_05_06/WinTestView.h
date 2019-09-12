// WinTestView.h : interface of the CWinTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINTESTVIEW_H__C2CC4478_DC82_4E98_BF14_F7F57A655CFC__INCLUDED_)
#define AFX_WINTESTVIEW_H__C2CC4478_DC82_4E98_BF14_F7F57A655CFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWinTestView : public CView
{
protected: // create from serialization only
	CWinTestView();
	DECLARE_DYNCREATE(CWinTestView)

// Attributes
public:
	CWinTestDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	unsigned char m_RevImg[256][256];
	int width;
	int height;
	BITMAPINFO *BmInfo;
	virtual ~CWinTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWinTestView)
	afx_msg void OnConstAdd();
	afx_msg void OnConstSub();
	afx_msg void OnConstMul();
	afx_msg void OnConstDiv();
	afx_msg void OnFrmAdd();
	afx_msg void OnFrmSub();
	afx_msg void OnFrmMul();
	afx_msg void OnLutMul();
	afx_msg void OnBinarization();
	afx_msg void OnImghisto();
	afx_msg void OnBinDynamic();
	afx_msg void OnHistoEqual();
	afx_msg void OnHistoStretch();
	afx_msg void OnHistoUpstretch();
	afx_msg void OnHistoSpecFilein();
	afx_msg void OnHistoSpecCal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WinTestView.cpp
inline CWinTestDoc* CWinTestView::GetDocument()
   { return (CWinTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINTESTVIEW_H__C2CC4478_DC82_4E98_BF14_F7F57A655CFC__INCLUDED_)
