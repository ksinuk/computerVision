// NCC_box.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ComputerVision.h"
#include "NCC_box.h"
#include "afxdialogex.h"


// NCC_box ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(NCC_box, CDialogEx)

NCC_box::NCC_box(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	,Ncc_prn_cut(181)
	,fx(18)
	,fy(72)
	,lastx(33)
	,lasty(90)
{}

NCC_box::~NCC_box(){}

void NCC_box::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_NCC_SAPLE_CUT,Ncc_prn_cut);
	DDV_MinMaxInt(pDX,Ncc_prn_cut,0,255);
	DDX_Text(pDX,IDC_NCC_fx,fx);
	DDX_Text(pDX,IDC_NCC_fy,fy);
	DDX_Text(pDX,IDC_NCC_lastx,lastx);
	DDX_Text(pDX,IDC_NCC_lasty,lasty);
}


BEGIN_MESSAGE_MAP(NCC_box, CDialogEx)
	ON_BN_CLICKED(IDC_OK_NCC_CUT,&NCC_box::OnBnClickedOkNccCut)
END_MESSAGE_MAP()

void NCC_box::OnBnClickedOkNccCut()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnOK();
}


