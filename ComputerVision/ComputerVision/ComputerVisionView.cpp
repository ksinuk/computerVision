
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
#include "NCC_box.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CComputerVisionView

IMPLEMENT_DYNCREATE(CComputerVisionView, CView)

BEGIN_MESSAGE_MAP(CComputerVisionView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
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
	
	CImage input_image, output_image ,temp_img[3],histo_img[3];
	if (!pDoc->GetInputMFCImage(input_image)) 
		input_image.Draw(pDC->GetSafeHdc(), 0, 0, input_image.GetWidth(), input_image.GetHeight());
	if (!pDoc->GetOutputMFCImage(output_image)) 
		output_image.Draw(pDC->GetSafeHdc(), output_image.GetWidth() + 10, 0, output_image.GetWidth(), output_image.GetHeight());

	int height = input_image.GetHeight();
	int width = input_image.GetWidth();

	if(pDoc->func_h ==0)
	{
		if (!pDoc->GetTempMFCImage(temp_img[0],0)) //HSI�� H
			temp_img[0].Draw(pDC->GetSafeHdc(), 0, height + 20, temp_img[0].GetWidth(),temp_img[0].GetHeight());
		if (!pDoc->GetTempMFCImage(temp_img[1],1)) //HSI�� S
			temp_img[1].Draw(pDC->GetSafeHdc(), width + 10, height + 20, temp_img[1].GetWidth(),temp_img[1].GetHeight());
		if (!pDoc->GetTempMFCImage(temp_img[2],2)) //HSI�� I
			temp_img[2].Draw(pDC->GetSafeHdc(), 2*width + 20, height + 20, temp_img[2].GetWidth(),temp_img[2].GetHeight());
	}
	else 
	{
		if (!pDoc->GetHistogrmMFCImage(histo_img[2],2)) 
			histo_img[2].Draw(pDC->GetSafeHdc(), 0, input_image.GetHeight() + 20 , 300, 300); //red ���
		if (!pDoc->GetHistogrmMFCImage(histo_img[1],1)) 
			histo_img[1].Draw(pDC->GetSafeHdc(), 300+20, input_image.GetHeight() + 20 , 300, 300); // green ���
		if (!pDoc->GetHistogrmMFCImage(histo_img[0],0)) 
			histo_img[0].Draw(pDC->GetSafeHdc(), 600+40, input_image.GetHeight() + 20 , 300, 300); //blue ���
	}
	

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


// CComputerVisionView �޽��� ó����


void CComputerVisionView::OnLButtonDown(UINT nFlags,CPoint point)
{
	/*if(point.y<0 || point.y>=height) return;
	if(point.x<0||point.x>=width) return;*/

	/*m_RectTrack.left= m_RectTrack.right= point.x;
	m_RectTrack.top= m_RectTrack.bottom= point.y;
	m_flagMouse= TRUE;
	CMainFrame*pFrame= (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	pFrame->m_flagTemplate= FALSE;*/

	CView::OnLButtonDown(nFlags,point);
}
