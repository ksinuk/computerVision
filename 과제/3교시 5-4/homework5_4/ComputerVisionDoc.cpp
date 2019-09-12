
// ComputerVisionDoc.cpp : CComputerVisionDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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


// CComputerVisionDoc ����/�Ҹ�

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

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CComputerVisionDoc serialization

void CComputerVisionDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// ���� ���� ������� ����
		ar << input_width_ << input_height_ << input_channel_;
		if (input_image_ != NULL) ar.Write(input_image_, input_width_ * input_height_ * input_channel_);
		ar << output_width_ << output_height_ << output_channel_;
		if (output_image_ != NULL) ar.Write(output_image_, output_width_ * output_height_ * output_channel_);
	}
	else
	{
		// ���� ���� ������� �ҷ�����, Read �Լ����� ���� �Ҵ��ؾ���
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

// ����� �׸��� �����մϴ�.
void CComputerVisionDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CComputerVisionDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CComputerVisionDoc ����

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


// CComputerVisionDoc ���

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
	if (mfc_image.IsNull()) { AfxMessageBox(_T("����ִ� �̹���")); return 1; }

	// �̹��� ���� 
	width = mfc_image.GetWidth();
	height = mfc_image.GetHeight();
	channel = mfc_image.GetBPP() / 8;
	if (channel != 1 && channel != 3) { AfxMessageBox(_T("1ä�� Ȥ�� 3ä�� �̹����� �ƴ�")); return 2; }

	// �̹��� ������ �б�
	int data_size = width * height * channel;
	unsigned char *data = new unsigned char[data_size];
	int wc_width = width * channel;
	for (unsigned int h = 0, off = 0; h < height; ++h, off += wc_width)
	{
		memcpy(&data[off], mfc_image.GetPixelAddress(0, h), wc_width);
	}

	// ����, 8��Ʈ �̹����� �׷��� �̹����� ������
	int image_size = data_size;
	Allocate(raw_image, image_size);
	if (channel == 1)
	{
		// �ȷ�Ʈ
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
	if (raw_image == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return 1; }
	
	// �̹��� ����
	mfc_image.Destroy();
	mfc_image.Create(width, height, channel * 8);

	// �ȷ�Ʈ ����, 8��Ʈ �̹����� �׷��� �̹����� ������
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

	// �̹��� ������ ����
	int wc_width = width * channel;
	int data_size = height * width * channel;
	unsigned char *data = new unsigned char[data_size];
	memcpy(data, raw_image, data_size);

	// ����
	for (unsigned int h = 0, off = 0; h < height; ++h, off += wc_width)
	{
		memcpy(mfc_image.GetPixelAddress(0, h), &data[off], wc_width);
	}
	delete[] data;

	return 0;
}

void CComputerVisionDoc::OnLoadImage()
{
	// ���� ���̾�α�, ��� ����
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("BMP file (*.bmp)|*.bmp|"));
	CString file_path;
	if (IDOK == dlg.DoModal()) file_path = dlg.GetPathName();

	// �̹��� ����
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

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnLoadImage1()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� ���̾�α�, ��� ����
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("BMP file (*.bmp)|*.bmp|"));
	CString file_path;
	if (IDOK == dlg.DoModal()) file_path = dlg.GetPathName();

	// �̹��� ����
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

	// ��� �� ������Ʈ
	UpdateAllViews(0);

}

void CComputerVisionDoc::OnSaveImage()
{
	CImage mfc_image;
	if (RawToBMP(output_image_, output_width_, output_height_, output_channel_, mfc_image)) return;
	if (mfc_image.IsNull()) return;

	// ���� ���̾�α�, ��� ����
	CFileDialog dlg(FALSE, _T("bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("BMP file (*.bmp)|*.bmp|"));
	CString file_path;
	if (IDOK == dlg.DoModal()) file_path = dlg.GetPathName();

	// BMP ���Ϸ� ����
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

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04GrayTransform()
{
	// ����
	if (input_image_ == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_channel_ != 3) { AfxMessageBox(_T("3ä�� �̹����� �׷��̷� ��ȯ ����")); return; }

	// ��� �̹��� ������¡
	int input_image_size = input_width_ * input_height_ * input_channel_;
	output_height_ = input_height_;
	output_width_ = input_width_;
	output_channel_ = 1;
	int output_image_size = output_height_ * output_width_ * output_channel_;
	Allocate(output_image_, output_image_size);
	
	// �׷��� ��ȯ
	for (int i = 0, j = 0; i < input_image_size; i += 3, ++j)
	{
		output_image_[j] = (unsigned char)(0.114 * input_image_[i] + 0.587 * input_image_[i + 1] + 0.299 * input_image_[i + 2]);
	}
	
	// ��� �� ������Ʈ
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04ColorInv()
{
	// ����
	if (input_image_ == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }

	// ��� �̹��� ������¡
	int input_image_size = input_width_ * input_height_ * input_channel_;
	output_height_ = input_height_;
	output_width_ = input_width_;
	output_channel_ = input_channel_;
	int output_image_size = output_height_ * output_width_ * output_channel_;
	Allocate(output_image_, output_image_size);

	// ���� ������
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

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04Inv()
{
	// ����
	if (input_image_ == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }

	// ��� �̹��� ������¡
	int input_image_size = input_width_ * input_height_ * input_channel_;
	output_height_ = input_height_;
	output_width_ = input_width_;
	output_channel_ = input_channel_;
	int output_image_size = output_height_ * output_width_ * output_channel_;
	Allocate(output_image_, output_image_size);

	// ������
	int h_size = input_height_;
	int wc_size = input_width_ * input_channel_;
	for (int h = 0, off = 0, off_inv = input_image_size - wc_size; h < h_size; ++h, off += wc_size, off_inv -= wc_size)
	{
		memcpy(&output_image_[off_inv], &input_image_[off], wc_size);
	}
	
	// ��� �� ������Ʈ
	UpdateAllViews(0);
}


void CComputerVisionDoc::OnMsbXor()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// ����
	if (input_image_ == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image_1 == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }

	// ��� �̹��� ������¡
	int input_image_size = input_width_ * input_height_ * input_channel_;
	output_height_ = input_height_;
	output_width_ = input_width_;
	output_channel_ = input_channel_;
	int output_image_size = output_height_ * output_width_ * output_channel_;
	Allocate(output_image_, output_image_size);

	// �ֻ��� 1��Ʈ XOR ����
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
	
	// ��� �� ������Ʈ
	UpdateAllViews(0);

}



