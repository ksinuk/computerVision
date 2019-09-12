// FormViewOpenCVDoc.h : interface of the CFormViewOpenCVDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORMVIEWOPENCVDOC_H__ED5CF753_4988_4850_BFDC_768F9F4E72DE__INCLUDED_)
#define AFX_FORMVIEWOPENCVDOC_H__ED5CF753_4988_4850_BFDC_768F9F4E72DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFormViewOpenCVDoc : public CDocument
{
protected: // create from serialization only
	CFormViewOpenCVDoc();
	DECLARE_DYNCREATE(CFormViewOpenCVDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormViewOpenCVDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFormViewOpenCVDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFormViewOpenCVDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMVIEWOPENCVDOC_H__ED5CF753_4988_4850_BFDC_768F9F4E72DE__INCLUDED_)
