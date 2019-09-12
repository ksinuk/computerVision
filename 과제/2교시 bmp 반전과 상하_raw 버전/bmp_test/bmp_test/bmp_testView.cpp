
// bmp_testView.cpp : Cbmp_testView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "bmp_test.h"
#endif

#include "bmp_testDoc.h"
#include "bmp_testView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cbmp_testView

IMPLEMENT_DYNCREATE(Cbmp_testView, CView)

BEGIN_MESSAGE_MAP(Cbmp_testView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Cbmp_testView ����/�Ҹ�

Cbmp_testView::Cbmp_testView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

Cbmp_testView::~Cbmp_testView()
{
}

BOOL Cbmp_testView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// Cbmp_testView �׸���

void Cbmp_testView::OnDraw(CDC* pDC)
{
	Cbmp_testDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	
	for(int i=0; i<256; i++)
	{
		for(int j=0; j<256; j++)
		{
			unsigned char InVal= pDoc->m_inimg[i][j];
			unsigned char OutVal= pDoc->m_outimg[i][j];

			pDC->SetPixel(j,i,RGB(InVal,InVal,InVal));
			pDC->SetPixel(j+300,i,RGB(OutVal,OutVal,OutVal));
		}
	}
}



// Cbmp_testView �μ�

BOOL Cbmp_testView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void Cbmp_testView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void Cbmp_testView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// Cbmp_testView ����

#ifdef _DEBUG
void Cbmp_testView::AssertValid() const
{
	CView::AssertValid();
}

void Cbmp_testView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cbmp_testDoc* Cbmp_testView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cbmp_testDoc)));
	return (Cbmp_testDoc*)m_pDocument;
}
#endif //_DEBUG


// Cbmp_testView �޽��� ó����