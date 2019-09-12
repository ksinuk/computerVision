// CamTest.h : main header file for the CAMTEST application
//

#if !defined(AFX_CAMTEST_H__A6651423_0173_4E93_BC44_529E28D936B1__INCLUDED_)
#define AFX_CAMTEST_H__A6651423_0173_4E93_BC44_529E28D936B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCamTestApp:
// See CamTest.cpp for the implementation of this class
//

class CCamTestApp : public CWinApp
{
public:
	CCamTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCamTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMTEST_H__A6651423_0173_4E93_BC44_529E28D936B1__INCLUDED_)
