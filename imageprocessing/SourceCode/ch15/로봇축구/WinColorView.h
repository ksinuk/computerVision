// WinColorView.h : interface of the CWinColorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINCOLORVIEW_H__6D157C8D_AE36_11D6_A616_0050DA084ED4__INCLUDED_)
#define AFX_WINCOLORVIEW_H__6D157C8D_AE36_11D6_A616_0050DA084ED4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWinColorView : public CView
{
protected: // create from serialization only
	CWinColorView();
	DECLARE_DYNCREATE(CWinColorView)

// Attributes
public:
	CWinColorDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinColorView)
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
	int rwsize;
	int width;
	int height;
	BITMAPINFO *BmInfo;
	virtual ~CWinColorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWinColorView)
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnChannelsplitRgb();
	afx_msg void OnUpdateChannelsplitRgb(CCmdUI* pCmdUI);
	afx_msg void OnChannelsplitHsi();
	afx_msg void OnUpdateChannelsplitHsi(CCmdUI* pCmdUI);
	afx_msg void OnColor2gray();
	afx_msg void OnUpdateColor2gray(CCmdUI* pCmdUI);
	afx_msg void OnRgbEmbossing();
	afx_msg void OnUpdateRgbEmbossing(CCmdUI* pCmdUI);
	afx_msg void OnRgbShapenning();
	afx_msg void OnUpdateRgbShapenning(CCmdUI* pCmdUI);
	afx_msg void OnImgDynbin();
	afx_msg void OnBlobcoloring();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WinColorView.cpp
inline CWinColorDoc* CWinColorView::GetDocument()
   { return (CWinColorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINCOLORVIEW_H__6D157C8D_AE36_11D6_A616_0050DA084ED4__INCLUDED_)
