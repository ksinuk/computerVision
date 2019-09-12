// FormViewOpenCVView.h : interface of the CFormViewOpenCVView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORMVIEWOPENCVVIEW_H__7BA7AED0_BBD0_4D09_9E4E_30883DB96E6E__INCLUDED_)
#define AFX_FORMVIEWOPENCVVIEW_H__7BA7AED0_BBD0_4D09_9E4E_30883DB96E6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//=============================================================================
// OpenCV Includes
//=============================================================================
#include <cv.h>
#include <cxcore.h>
#include <cvaux.h>
#include <highgui.h>

#include "drawimage.h"

class CFormViewOpenCVView : public CFormView
{
protected: // create from serialization only
	CFormViewOpenCVView();
	DECLARE_DYNCREATE(CFormViewOpenCVView)

public:
	//{{AFX_DATA(CFormViewOpenCVView)
	enum{ IDD = IDD_FORMVIEWOPENCV_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CFormViewOpenCVDoc* GetDocument();

public:
	int m_iNumRowDisp,m_iNumColDisp;

	HANDLE m_hRectBitmapInfoHdlGrey; 
	HANDLE m_hRectBitmapInfoHdlColor;

	IplImage *m_pImgIplDispColor;
	IplImage *m_pImgIplDispGrey;
	IplImage *m_pImgIplIn,*m_pImgIplOut;

// Operations
public:
	int dspIplImg(CWnd* pWnd,IplImage* pImgIpl,HANDLE hBitmapInfo,int iNumRow,int iNumCol);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormViewOpenCVView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFormViewOpenCVView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFormViewOpenCVView)
	afx_msg void OnLoadImage();
	afx_msg void OnProcessing();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FormViewOpenCVView.cpp
inline CFormViewOpenCVDoc* CFormViewOpenCVView::GetDocument()
   { return (CFormViewOpenCVDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMVIEWOPENCVVIEW_H__7BA7AED0_BBD0_4D09_9E4E_30883DB96E6E__INCLUDED_)
