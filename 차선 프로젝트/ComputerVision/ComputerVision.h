
// ComputerVision.h : ComputerVision ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CComputerVisionApp:
// �� Ŭ������ ������ ���ؼ��� ComputerVision.cpp�� �����Ͻʽÿ�.
//

class CComputerVisionApp : public CWinAppEx
{
public:
	CComputerVisionApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CComputerVisionApp theApp;

//���� ���� �Լ�
int sort2array(int a[],int start,int mid,int end);
int merge_sort(int a[],int start,int end);
