#pragma once


// NCC_box ��ȭ �����Դϴ�.

class NCC_box : public CDialogEx
{
	DECLARE_DYNAMIC(NCC_box)

public:
	NCC_box(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~NCC_box();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int Ncc_prn_cut; //�̰����� ��Ī���� ū ������ ����
	int fx; //���ø� �簢���� �������� ��ǥ x
	int fy; //���ø� �簢���� �������� ��ǥ y
	int lastx; //���ø� �簢���� ���������� ��ǥ x
	int lasty; //���ø� �簢���� ���������� ��ǥ y
	afx_msg void OnBnClickedOkNccCut();
};
