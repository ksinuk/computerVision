
// bmp_test.h : bmp_test ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// Cbmp_testApp:
// �� Ŭ������ ������ ���ؼ��� bmp_test.cpp�� �����Ͻʽÿ�.
//

class Cbmp_testApp : public CWinApp
{
public:
	Cbmp_testApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cbmp_testApp theApp;
