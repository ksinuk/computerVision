
// bmp_testDoc.cpp : Cbmp_testDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "bmp_test.h"
#endif

#include "bmp_testDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cbmp_testDoc

IMPLEMENT_DYNCREATE(Cbmp_testDoc, CDocument)

BEGIN_MESSAGE_MAP(Cbmp_testDoc, CDocument)
	ON_COMMAND(ID_reverse_img,&Cbmp_testDoc::OnReverseImg)
	ON_COMMAND(ID_mirror,&Cbmp_testDoc::OnMirror)
END_MESSAGE_MAP()


// Cbmp_testDoc ����/�Ҹ�

Cbmp_testDoc::Cbmp_testDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

Cbmp_testDoc::~Cbmp_testDoc()
{
}

BOOL Cbmp_testDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// Cbmp_testDoc serialization


void Cbmp_testDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar.Write(m_outimg,256*256); // ó���� ����迭 m_OutImg�� raw���Ϸ� ���� 
	}
	else
	{
		// TODO: add loading code here
		CFile *infile = ar.GetFile(); // �Է��� ������ ����Ʈ�� ������.
		if(infile->GetLength()!=256*256) // raw ������ �ƴ� ��� 
		{			
			BITMAPFILEHEADER hf;
			ar.Read(&hf,sizeof(BITMAPFILEHEADER));
			if(hf.bfType!=0x4D42) //bmp�������� Ȯ��
			{
				AfxMessageBox(_T("bmp������ �ƴմϴ�."));
				return;
			}

			BITMAPINFOHEADER hInfo;
			ar.Read(&hInfo,sizeof(BITMAPINFOHEADER));

			if(hInfo.biHeight!=256||hInfo.biWidth!=256) //256x256 �������� Ȯ��
			{
				AfxMessageBox(_T("���� ũ�Ⱑ 256x256�������� bmp������ �ƴմϴ�."));
				return;
			}

			if(hInfo.biBitCount!=8 ) //������� Ȯ��
			{
				AfxMessageBox(_T("��� �̹����� �����մϴ�."));
				return;
			}

			RGBQUAD hRGB[256]; 
			ar.Read(hRGB,sizeof(RGBQUAD)*256); //�ȷ�Ʈ ���� �б�

			ar.Read(m_inimg,256*256); //�̹��� ���� �б�

			for(int i=0;i<256/2;i++) //���� ���� ����
				for(int j = 0; j<256; j++)
				{
					unsigned char temp=m_inimg[i][j];
					m_inimg[i][j] = m_inimg[255-i][j];
					m_inimg[255-i][j] = temp;
				}

		}
		else //raw ���� �ҷ�����
		{
			ar.Read(m_inimg,infile->GetLength());
		}
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void Cbmp_testDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void Cbmp_testDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void Cbmp_testDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Cbmp_testDoc ����

#ifdef _DEBUG
void Cbmp_testDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cbmp_testDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cbmp_testDoc ���



void Cbmp_testDoc::OnReverseImg() //���� �����
{
	for(int i=0;i<256;i++)
		for(int j=0;j<256;j++)
			m_outimg[i][j] = 255-m_inimg[i][j];

	UpdateAllViews(NULL);
}


void Cbmp_testDoc::OnMirror() //�¿� ����
{
	for(int i=0;i<256;i++)
		for(int j=0;j<256;j++)
			m_outimg[i][j] = m_inimg[i][255-j];

	UpdateAllViews(NULL);
}
