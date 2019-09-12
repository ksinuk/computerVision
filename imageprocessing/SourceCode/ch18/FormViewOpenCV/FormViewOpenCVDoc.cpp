// FormViewOpenCVDoc.cpp : implementation of the CFormViewOpenCVDoc class
//

#include "stdafx.h"
#include "FormViewOpenCV.h"

#include "FormViewOpenCVDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormViewOpenCVDoc

IMPLEMENT_DYNCREATE(CFormViewOpenCVDoc, CDocument)

BEGIN_MESSAGE_MAP(CFormViewOpenCVDoc, CDocument)
	//{{AFX_MSG_MAP(CFormViewOpenCVDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormViewOpenCVDoc construction/destruction

CFormViewOpenCVDoc::CFormViewOpenCVDoc()
{
	// TODO: add one-time construction code here

}

CFormViewOpenCVDoc::~CFormViewOpenCVDoc()
{
}

BOOL CFormViewOpenCVDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFormViewOpenCVDoc serialization

void CFormViewOpenCVDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFormViewOpenCVDoc diagnostics

#ifdef _DEBUG
void CFormViewOpenCVDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFormViewOpenCVDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormViewOpenCVDoc commands
