
// bmp_testDoc.cpp : Cbmp_testDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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


// Cbmp_testDoc 생성/소멸

Cbmp_testDoc::Cbmp_testDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

Cbmp_testDoc::~Cbmp_testDoc()
{
}

BOOL Cbmp_testDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// Cbmp_testDoc serialization


void Cbmp_testDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar.Write(m_outimg,256*256); // 처리된 영상배열 m_OutImg를 raw파일로 저장 
	}
	else
	{
		// TODO: add loading code here
		CFile *infile = ar.GetFile(); // 입력할 파일의 포인트를 가져옴.
		if(infile->GetLength()!=256*256) // raw 파일이 아닐 경우 
		{			
			BITMAPFILEHEADER hf;
			ar.Read(&hf,sizeof(BITMAPFILEHEADER));
			if(hf.bfType!=0x4D42) //bmp파일인지 확인
			{
				AfxMessageBox(_T("bmp파일이 아닙니다."));
				return;
			}

			BITMAPINFOHEADER hInfo;
			ar.Read(&hInfo,sizeof(BITMAPINFOHEADER));

			if(hInfo.biHeight!=256||hInfo.biWidth!=256) //256x256 파일인진 확인
			{
				AfxMessageBox(_T("파일 크기가 256x256사이즈의 bmp파일이 아닙니다."));
				return;
			}

			if(hInfo.biBitCount!=8 ) //흑백인지 확인
			{
				AfxMessageBox(_T("흑백 이미지만 지원합니다."));
				return;
			}

			RGBQUAD hRGB[256]; 
			ar.Read(hRGB,sizeof(RGBQUAD)*256); //팔레트 정보 읽기

			ar.Read(m_inimg,256*256); //이미지 정보 읽기

			for(int i=0;i<256/2;i++) //상하 반전 보정
				for(int j = 0; j<256; j++)
				{
					unsigned char temp=m_inimg[i][j];
					m_inimg[i][j] = m_inimg[255-i][j];
					m_inimg[255-i][j] = temp;
				}

		}
		else //raw 파일 불러오기
		{
			ar.Read(m_inimg,infile->GetLength());
		}
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void Cbmp_testDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
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

// 검색 처리기를 지원합니다.
void Cbmp_testDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// Cbmp_testDoc 진단

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


// Cbmp_testDoc 명령



void Cbmp_testDoc::OnReverseImg() //역상 만들기
{
	for(int i=0;i<256;i++)
		for(int j=0;j<256;j++)
			m_outimg[i][j] = 255-m_inimg[i][j];

	UpdateAllViews(NULL);
}


void Cbmp_testDoc::OnMirror() //좌우 반전
{
	for(int i=0;i<256;i++)
		for(int j=0;j<256;j++)
			m_outimg[i][j] = m_inimg[i][255-j];

	UpdateAllViews(NULL);
}
