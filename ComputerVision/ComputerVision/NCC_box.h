#pragma once


// NCC_box 대화 상자입니다.

class NCC_box : public CDialogEx
{
	DECLARE_DYNAMIC(NCC_box)

public:
	NCC_box(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~NCC_box();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int Ncc_prn_cut; //이값보다 매칭값이 큰 지점만 추출
	int fx; //템플릿 사각형의 시작점의 좌표 x
	int fy; //템플릿 사각형의 시작점의 좌표 y
	int lastx; //템플릿 사각형의 마지막점의 좌표 x
	int lasty; //템플릿 사각형의 마지막점의 좌표 y
	afx_msg void OnBnClickedOkNccCut();
};
