#pragma once


// Input_Mask 대화 상자입니다.

class Input_Mask : public CDialogEx
{
	DECLARE_DYNAMIC(Input_Mask)

public:
	Input_Mask(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Input_Mask();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int mask_h;
	int mask_w;
};
