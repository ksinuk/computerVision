
// ComputerVisionView.cpp : CComputerVisionView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "ComputerVision.h"
#endif

#include "ComputerVisionDoc.h"
#include "ComputerVisionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CComputerVisionView

IMPLEMENT_DYNCREATE(CComputerVisionView, CView)

BEGIN_MESSAGE_MAP(CComputerVisionView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CComputerVisionView ����/�Ҹ�

CComputerVisionView::CComputerVisionView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CComputerVisionView::~CComputerVisionView()
{
}

BOOL CComputerVisionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CComputerVisionView �׸���

void CComputerVisionView::OnDraw(CDC* pDC)
{
	CComputerVisionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	CImage input_image, output_image, orignal_image;
	if (!pDoc->GetInputMFCImage(input_image)) input_image.Draw(pDC->GetSafeHdc(), 0, 0, input_image.GetWidth(), input_image.GetHeight());
	if (!pDoc->GetOutputMFCImage(output_image)) output_image.Draw(pDC->GetSafeHdc(), input_image.GetWidth() + 10, 0, output_image.GetWidth(), output_image.GetHeight());
	if (!pDoc->GetoriginalMFCImage(orignal_image)) orignal_image.Draw(pDC->GetSafeHdc(), 0, input_image.GetHeight()+10, orignal_image.GetWidth(), orignal_image.GetHeight());
}

void CComputerVisionView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CComputerVisionView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CComputerVisionView ����

#ifdef _DEBUG
void CComputerVisionView::AssertValid() const
{
	CView::AssertValid();
}

void CComputerVisionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CComputerVisionDoc* CComputerVisionView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CComputerVisionDoc)));
	return (CComputerVisionDoc*)m_pDocument;
}
#endif //_DEBUG
