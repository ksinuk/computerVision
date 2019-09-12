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
	afx_msg void OnBinarOtzu();
	afx_msg void OnBinarAdap();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void AdaptiveBinarization(unsigned char *orgImg, unsigned char *outImg, int height, int width);
	void Otzu_Threshold(unsigned char* orgImg, unsigned char* outImg, int height, int width);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINTESTDOC_H__E682D0BC_346E_4898_B344_8C6544AD7861__INCLUDED_)
