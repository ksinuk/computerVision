// WinColorDoc.h : interface of the CWinColorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINCOLORDOC_H__6D157C8B_AE36_11D6_A616_0050DA084ED4__INCLUDED_)
#define AFX_WINCOLORDOC_H__6D157C8B_AE36_11D6_A616_0050DA084ED4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
#define WIDTHBYTES(bits)    (((bits)+31)/32*4)  //4바이트의 배수여야  
//////////////////////////////////////////////////////////////////////////

class CWinColorDoc : public CDocument
{
protected: // create from serialization only
	CWinColorDoc();
	DECLARE_DYNCREATE(CWinColorDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinColorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	void m_ImgConvolution(unsigned char *InImg, unsigned char *OutImg, int height, int width, float *Mask, int winsize);
	void m_RGBShapening(unsigned char *InImg, unsigned char *OutImg, int height, int width);
	void m_ColorEmbossing(unsigned char *InImg, unsigned char *OutImg, int height, int width);
	void m_RGB2Gray(unsigned char *InImg, unsigned char *OutImg, int height, int width);
	void m_RGB2HSI(unsigned char *InImg, unsigned char *OutImg, int height, int width);
	void CopyClipboard(unsigned char *m_CpyImg, int height, int width, int biBitCount);
	int width;
	int height;
	unsigned char *m_OutImg;
	unsigned char *m_InImg;
	RGBQUAD palRGB[256];
	BITMAPINFOHEADER dibHi;
	BITMAPFILEHEADER dibHf;
	virtual ~CWinColorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWinColorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINCOLORDOC_H__6D157C8B_AE36_11D6_A616_0050DA084ED4__INCLUDED_)
