
// ComputerVisionView.h : CComputerVisionView Ŭ������ �������̽�
//

#pragma once


class CComputerVisionView : public CView
{
protected: // serialization������ ��������ϴ�.
	CComputerVisionView();
	DECLARE_DYNCREATE(CComputerVisionView)

// Ư���Դϴ�.
public:
	CComputerVisionDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// �����Դϴ�.
public:
	virtual ~CComputerVisionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
};

#ifndef _DEBUG  // ComputerVisionView.cpp�� ����� ����
inline CComputerVisionDoc* CComputerVisionView::GetDocument() const
   { return reinterpret_cast<CComputerVisionDoc*>(m_pDocument); }
#endif

