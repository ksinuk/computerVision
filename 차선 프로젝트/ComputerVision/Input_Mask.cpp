// Input_Mask.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ComputerVision.h"
#include "Input_Mask.h"
#include "afxdialogex.h"


// Input_Mask 대화 상자입니다.

IMPLEMENT_DYNAMIC(Input_Mask, CDialogEx)

Input_Mask::Input_Mask(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	,mask_h(0)
	,mask_w(0)
{

}

Input_Mask::~Input_Mask()
{
}

void Input_Mask::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_MASK_H,mask_h);
	DDV_MinMaxInt(pDX,mask_h,0,480);
	DDX_Text(pDX,IDC_MASK_W,mask_w);
	DDV_MinMaxInt(pDX, mask_w, 0, 640);
}


BEGIN_MESSAGE_MAP(Input_Mask, CDialogEx)
END_MESSAGE_MAP()


// Input_Mask 메시지 처리기입니다.
