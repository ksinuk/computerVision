// WinTestDoc.h : interface of the CWinTestDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINTESTDOC_H__E682D0BC_346E_4898_B344_8C6544AD7861__INCLUDED_)
#define AFX_WINTESTDOC_H__E682D0BC_346E_4898_B344_8C6544AD7861__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWinTestDoc : public CDocument
{
protected: // create from serialization only
	CWinTestDoc();
	DECLARE_DYNCREATE(CWinTestDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinTestDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	void m_HistoSpec(int height, int width);
	void m_HistoSpec_FileIn();
	void m_HistoUpStretch(int height, int width, int lowPercent, int highPercent);
	void m_HistoStretch(int height, int width);
	void m_HistoEqual(int height, int width);
	void m_BinThres(int height, int width, int binThres);
	void m_ImgHisto(int height, int width);
	void TwoImgLoad();
	unsigned char m_InImg2[256][256];
	unsigned char m_InImg1[256][256];
	unsigned char m_OutImg[256][256];
	unsigned char m_InImg[256][256];
	virtual ~CWinTestDoc();

	//7장 마스크 기반 영상 처리 구현 부분 
	void m_EdgeSobel(int height,int width);
	void m_EdgePrewitt(int height,int width);
	void m_SharpeningLaplacian(int height,int width);
	void m_SmoothingGaussian(int height,int width);
	void m_SmoothingBox(int height,int width);

	//8장 영상의 기하학적 변환 구현 
	void m_Rotation(int height,int width,int center_r,int center_c,float rotationAngle);
	void m_ZoomOut(int height,int width,float zoomoutfactor);
	void m_ZoomIn(int height,int width,float zoominfactor);

	//9장 주파수 영역에서의 영상 처리 구현 
	void m_FFT1D(int NumData,float *pOneDRealImage,float *pOneDImImage,int Forward);
	void m_FFT2D(int height,int width,int Forward);


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	int m_HistoArr[256];
	//{{AFX_MSG(CWinTestDoc)
	afx_msg void OnReverseImg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINTESTDOC_H__E682D0BC_346E_4898_B344_8C6544AD7861__INCLUDED_)
