#pragma once


// Input_Mask ��ȭ �����Դϴ�.

class Input_Mask : public CDialogEx
{
	DECLARE_DYNAMIC(Input_Mask)

public:
	Input_Mask(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Input_Mask();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int mask_h;
	int mask_w;
};
