
// ComputerVisionDoc.cpp : CComputerVisionDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ComputerVision.h"
#endif

#include "ComputerVisionDoc.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CComputerVisionDoc

IMPLEMENT_DYNCREATE(CComputerVisionDoc, CDocument)

BEGIN_MESSAGE_MAP(CComputerVisionDoc, CDocument)
	ON_COMMAND(ID_LOAD_IMAGE, &CComputerVisionDoc::OnLoadImage)
	ON_COMMAND(ID_SAVE_IMAGE, &CComputerVisionDoc::OnSaveImage)
	ON_COMMAND(ID_FLAG_USE_RESULT, &CComputerVisionDoc::OnFlagUseResult)
	ON_COMMAND(ID_04_GRAY_TRANSFORM, &CComputerVisionDoc::On04GrayTransform)
	ON_COMMAND(ID_04_COLOR_INV, &CComputerVisionDoc::On04ColorInv)
	ON_COMMAND(ID_04_INV, &CComputerVisionDoc::On04Inv)
	ON_COMMAND(ID_MSB_XOR,&CComputerVisionDoc::OnMsbXor)
	ON_COMMAND(ID_LOAD_IMAGE_1,&CComputerVisionDoc::OnLoadImage1)
END_MESSAGE_MAP()


// CComputerVisionDoc 생성/소멸

CComputerVisionDoc::CComputerVisionDoc()
{
	input_width_ = 0;
	input_height_ = 0;
	input_channel_ = 0;
	output_width_ = 0;
	output_height_ = 0;
	output_channel_ = 0;
	input_image_ = NULL;
	output_image_ = NULL;
}

CComputerVisionDoc::~CComputerVisionDoc()
{
	if (input_image_ == NULL) delete[] input_image_;
	if (output_image_ == NULL) delete[] output_image_;
}

BOOL CComputerVisionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CComputerVisionDoc serialization

void CComputerVisionDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// 변수 선언 순서대로 저장
		ar << input_width_ << input_height_ << input_channel_;
		if (input_image_ != NULL) ar.Write(input_image_, input_width_ * input_height_ * input_channel_);
		ar << output_width_ << output_height_ << output_channel_;
		if (output_image_ != NULL) ar.Write(output_image_, output_width_ * output_height_ * output_channel_);
	}
	else
	{
		// 변수 선언 순서대로 불러오기, Read 함수쓰기 전에 할당해야함
		ar >> input_width_ >> input_height_ >> input_channel_;
		int input_image_size = input_width_ * input_height_ * input_channel_;
		Allocate(input_image_, input_image_size);
		if (input_image_size > 0) ar.Read(input_image_, input_image_size);

		ar >> output_width_ >> output_height_ >> output_channel_;
		int output_image_size = output_width_ * output_height_ * output_channel_;
		Allocate(output_image_, output_image_size);
		if (output_image_size > 0) ar.Read(output_image_, output_image_size);
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CComputerVisionDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CComputerVisionDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CComputerVisionDoc::SetSearchContent(const CString& value)
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

// CComputerVisionDoc 진단

#ifdef _DEBUG
void CComputerVisionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CComputerVisionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CComputerVisionDoc 명령

int CComputerVisionDoc::GetInputMFCImage(CImage &image)
{
	if (input_image_ == NULL) return 1;
	RawToBMP(input_image_, input_width_, input_height_, input_channel_, image);

	return 0;
}

int CComputerVisionDoc::GetInput1MFCImage(CImage &image)
{
	if (input_image_1 == NULL) return 1;
	RawToBMP(input_image_1, input_width_1, input_height_1, input_channel_1, image);

	return 0;
}

int CComputerVisionDoc::GetOutputMFCImage(CImage &image)
{
	if (output_image_ == NULL) return 1;
	RawToBMP(output_image_, output_width_, output_height_, output_channel_, image);

	return 0;
}

void CComputerVisionDoc::Allocate(unsigned char *& raw_image, int image_size)
{
	if (image_size <= 0) return;
	if (raw_image != NULL) delete[] raw_image;
	raw_image = new unsigned char[image_size];
}

int CComputerVisionDoc::BMPtoRaw(const CImage &mfc_image, unsigned int &width, unsigned int &height, unsigned int &channel, unsigned char *&raw_image)
{
	if (mfc_image.IsNull()) { AfxMessageBox(_T("비어있는 이미지")); return 1; }

	// 이미지 정보 
	width = mfc_image.GetWidth();
	height = mfc_image.GetHeight();
	channel = mfc_image.GetBPP() / 8;
	if (channel != 1 && channel != 3) { AfxMessageBox(_T("1채널 혹은 3채널 이미지가 아님")); return 2; }

	// 이미지 데이터 읽기
	int data_size = width * height * channel;
	unsigned char *data = new unsigned char[data_size];
	int wc_width = width * channel;
	for (unsigned int h = 0, off = 0; h < height; ++h, off += wc_width)
	{
		memcpy(&data[off], mfc_image.GetPixelAddress(0, h), wc_width);
	}

	// 맵핑, 8비트 이미지는 그레이 이미지로 간주함
	int image_size = data_size;
	Allocate(raw_image, image_size);
	if (channel == 1)
	{
		// 팔레트
		int color_size = mfc_image.GetMaxColorTableEntries();
		RGBQUAD *palette = new RGBQUAD[color_size];
		mfc_image.GetColorTable(0, color_size, palette);
		for (int i = 0; i < data_size; ++i)
		{
			RGBQUAD &color = palette[data[i]];
			raw_image[i] = (unsigned char)(0.114 * color.rgbBlue + 0.587 * color.rgbGreen + 0.299 * color.rgbRed);
		}
	}
	else if (channel == 3)	memcpy(raw_image, data, image_size);
	delete[] data;

	return 0;
}

int CComputerVisionDoc::RawToBMP(const unsigned char *raw_image, unsigned int width, unsigned int height, unsigned int channel, CImage &mfc_image)
{
	if (raw_image == NULL) { AfxMessageBox(_T("비어있는 이미지")); return 1; }
	
	// 이미지 생성
	mfc_image.Destroy();
	mfc_image.Create(width, height, channel * 8);

	// 팔레트 생성, 8비트 이미지는 그레이 이미지로 간주함
	if (channel == 1)
	{
		unsigned int color_size = 256;
		RGBQUAD *palette = new RGBQUAD[color_size];
		for (unsigned int i = 0; i < color_size; ++i)
		{
			RGBQUAD &color = palette[i];
			color.rgbBlue = color.rgbGreen = color.rgbRed = i;
		}
		mfc_image.SetColorTable(0, color_size, palette);
	}

	// 이미지 데이터 생성
	int wc_width = width * channel;
	int data_size = height * width * channel;
	unsigned char *data = new unsigned char[data_size];
	memcpy(data, raw_image, data_size);

	// 복사
	for (unsigned int h = 0, off = 0; h < height; ++h, off += wc_width)
	{
		memcpy(mfc_image.GetPixelAddress(0, h), &data[off], wc_width);
	}
	delete[] data;

	return 0;
}

void CComputerVisionDoc::OnLoadImage()
{
	// 파일 다이얼로그, 경로 선택
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("BMP file (*.bmp)|*.bmp|"));
	CString file_path;
	if (IDOK == dlg.DoModal()) file_path = dlg.GetPathName();

	// 이미지 열기
	CImage mfc_image;
	mfc_image.Load(file_path);
	if (!BMPtoRaw(mfc_image, input_width_, input_height_, input_channel_, input_image_))
	{
		BMPtoRaw(mfc_image, output_width_, output_height_, output_channel_, output_image_);
	}

	if(input_width_ != input_width_1 || input_height_ != input_height_1 || input_channel_ != input_channel_1) 
	{ 
		if (input_image_1 != NULL) delete[] input_image_1;
		input_image_1 = NULL;
		input_width_1 = -1;
		input_height_1 = -1;
		input_channel_1 = -1;

		AfxMessageBox(_T("image_1 clear")); 
	}

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnLoadImage1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 파일 다이얼로그, 경로 선택
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("BMP file (*.bmp)|*.bmp|"));
	CString file_path;
	if (IDOK == dlg.DoModal()) file_path = dlg.GetPathName();

	// 이미지 열기
	CImage mfc_image;
	mfc_image.Load(file_path);

	if(input_image_ == NULL)  { AfxMessageBox(_T("image 0 fist")); return; }
	if(input_width_ != mfc_image.GetWidth())  { AfxMessageBox(_T("width miss")); return; }
	if(input_height_ != mfc_image.GetHeight()) { AfxMessageBox(_T("height miss")); return; }
	if(input_channel_ != mfc_image.GetBPP() / 8) { AfxMessageBox(_T("chennel miss")); return; }


	if (!BMPtoRaw(mfc_image, input_width_1, input_height_1, input_channel_1, input_image_1))
	{
		BMPtoRaw(mfc_image, output_width_, output_height_, output_channel_, output_image_);
	}

	// 모든 뷰 업데이트
	UpdateAllViews(0);

}

void CComputerVisionDoc::OnSaveImage()
{
	CImage mfc_image;
	if (RawToBMP(output_image_, output_width_, output_height_, output_channel_, mfc_image)) return;
	if (mfc_image.IsNull()) return;

	// 파일 다이얼로그, 경로 선택
	CFileDialog dlg(FALSE, _T("bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("BMP file (*.bmp)|*.bmp|"));
	CString file_path;
	if (IDOK == dlg.DoModal()) file_path = dlg.GetPathName();

	// BMP 파일로 저장
	mfc_image.Save(file_path);
}

void CComputerVisionDoc::OnFlagUseResult()
{
	input_height_ = output_height_;
	input_width_ = output_width_;
	input_channel_ = output_channel_;
	int input_image_size = input_height_ * input_width_ * input_channel_;
	Allocate(input_image_, input_image_size);
	memcpy(input_image_, output_image_, input_image_size);
	memset(output_image_, 0, input_image_size);

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04GrayTransform()
{
	// 예외
	if (input_image_ == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_channel_ != 3) { AfxMessageBox(_T("3채널 이미지만 그레이로 변환 가능")); return; }

	// 출력 이미지 리사이징
	int input_image_size = input_width_ * input_height_ * input_channel_;
	output_height_ = input_height_;
	output_width_ = input_width_;
	output_channel_ = 1;
	int output_image_size = output_height_ * output_width_ * output_channel_;
	Allocate(output_image_, output_image_size);
	
	// 그레이 변환
	for (int i = 0, j = 0; i < input_image_size; i += 3, ++j)
	{
		output_image_[j] = (unsigned char)(0.114 * input_image_[i] + 0.587 * input_image_[i + 1] + 0.299 * input_image_[i + 2]);
	}
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04ColorInv()
{
	// 예외
	if (input_image_ == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }

	// 출력 이미지 리사이징
	int input_image_size = input_width_ * input_height_ * input_channel_;
	output_height_ = input_height_;
	output_width_ = input_width_;
	output_channel_ = input_channel_;
	int output_image_size = output_height_ * output_width_ * output_channel_;
	Allocate(output_image_, output_image_size);

	// 색상 뒤집기
	int h_size = input_height_;
	int w_size = input_width_;
	int c_size = input_channel_;
	int wc_size = w_size * c_size;
	for (int h = 0, off_h = 0; h < h_size; ++h, off_h += wc_size)
	{
		for (int w = 0, off_w = off_h; w < w_size; ++w, off_w += c_size)
		{
			for (int c = 0, off = off_w; c < c_size; ++c, off += 1)
			{
				output_image_[off] = (unsigned char)(255 - input_image_[off]);
			}
		}
	}

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04Inv()
{
	// 예외
	if (input_image_ == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }

	// 출력 이미지 리사이징
	int input_image_size = input_width_ * input_height_ * input_channel_;
	output_height_ = input_height_;
	output_width_ = input_width_;
	output_channel_ = input_channel_;
	int output_image_size = output_height_ * output_width_ * output_channel_;
	Allocate(output_image_, output_image_size);

	// 뒤집기
	int h_size = input_height_;
	int wc_size = input_width_ * input_channel_;
	for (int h = 0, off = 0, off_inv = input_image_size - wc_size; h < h_size; ++h, off += wc_size, off_inv -= wc_size)
	{
		memcpy(&output_image_[off_inv], &input_image_[off], wc_size);
	}
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}


void CComputerVisionDoc::OnMsbXor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 예외
	if (input_image_ == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image_1 == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }

	// 출력 이미지 리사이징
	int input_image_size = input_width_ * input_height_ * input_channel_;
	output_height_ = input_height_;
	output_width_ = input_width_;
	output_channel_ = input_channel_;
	int output_image_size = output_height_ * output_width_ * output_channel_;
	Allocate(output_image_, output_image_size);

	// 최상위 1비트 XOR 연산
	for(int y = 0; y<output_height_; y++)
	{
		for(int x = 0; x<output_width_; x++)
		{
			for(int c = 0; c<output_channel_; c++)
			{
				unsigned char in = input_image_[output_channel_*(y*output_width_+x)+c];
				unsigned char b = input_image_1[output_channel_*(y*output_width_+x)+c];
				output_image_[output_channel_*(y*output_width_+x)+c] = (in & 0x7f) | (in & 0x80 ^ b & 0x80);
			}
			
		}
	}
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);

}



