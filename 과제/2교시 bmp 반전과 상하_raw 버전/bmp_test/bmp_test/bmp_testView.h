
// bmp_testView.h : Cbmp_testView Ŭ������ �������̽�
//

#pragma once


class Cbmp_testView : public CView
{
protected: // serialization������ ��������ϴ�.
	Cbmp_testView();
	DECLARE_DYNCREATE(Cbmp_testView)

// Ư���Դϴ�.
public:
	Cbmp_testDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~Cbmp_testView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
};

#ifndef _DEBUG  // bmp_testView.cpp�� ����� ����
inline Cbmp_testDoc* Cbmp_testView::GetDocument() const
   { return reinterpret_cast<Cbmp_testDoc*>(m_pDocument); }
#endif

