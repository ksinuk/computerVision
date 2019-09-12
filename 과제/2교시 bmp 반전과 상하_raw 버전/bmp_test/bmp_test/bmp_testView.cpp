
// bmp_testView.cpp : Cbmp_testView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Cbmp_testView 생성/소멸

Cbmp_testView::Cbmp_testView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

Cbmp_testView::~Cbmp_testView()
{
}

BOOL Cbmp_testView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// Cbmp_testView 그리기

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



// Cbmp_testView 인쇄

BOOL Cbmp_testView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void Cbmp_testView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void Cbmp_testView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// Cbmp_testView 진단

#ifdef _DEBUG
void Cbmp_testView::AssertValid() const
{
	CView::AssertValid();
}

void Cbmp_testView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cbmp_testDoc* Cbmp_testView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cbmp_testDoc)));
	return (Cbmp_testDoc*)m_pDocument;
}
#endif //_DEBUG


// Cbmp_testView 메시지 처리기