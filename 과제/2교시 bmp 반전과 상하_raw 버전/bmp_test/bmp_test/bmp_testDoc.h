
// bmp_testDoc.h : Cbmp_testDoc Ŭ������ �������̽�
//


#pragma once


class Cbmp_testDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	Cbmp_testDoc();
	DECLARE_DYNCREATE(Cbmp_testDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~Cbmp_testDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	unsigned char m_inimg[256][256];
	unsigned char m_outimg[256][256];
	afx_msg void OnReverseImg();
	afx_msg void OnMirror();
};
