// WinTest.h : main header file for the WINTEST application
//

#if !defined(AFX_WINTEST_H__D0A55B2C_D269_4F90_ACC6_F63CF896F0D6__INCLUDED_)
#define AFX_WINTEST_H__D0A55B2C_D269_4F90_ACC6_F63CF896F0D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinTestApp:
// See WinTest.cpp for the implementation of this class
//

class CWinTestApp : public CWinApp
{
public:
	CWinTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWinTestApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINTEST_H__D0A55B2C_D269_4F90_ACC6_F63CF896F0D6__INCLUDED_)
