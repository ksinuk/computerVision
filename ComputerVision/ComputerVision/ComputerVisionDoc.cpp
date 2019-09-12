
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
#include "NCC_box.h"

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
	ON_COMMAND(ID_Middle_filter,&CComputerVisionDoc::OnMiddlefilter)
	ON_COMMAND(ID_x2,&CComputerVisionDoc::Onx2)
	ON_COMMAND(ID_plus50,&CComputerVisionDoc::Onplus50)
	ON_COMMAND(ID_deg45,&CComputerVisionDoc::Ondeg45)
	ON_COMMAND(ID_histogram,&CComputerVisionDoc::Onhistogram)
	ON_COMMAND(ID_otzu,&CComputerVisionDoc::OnHistoOtzu)
	ON_COMMAND(ID_Erosion,&CComputerVisionDoc::OnErosion)
	ON_COMMAND(ID_Dilation,&CComputerVisionDoc::OnDilation)
	ON_COMMAND(ID_How_Many_Coin,&CComputerVisionDoc::OnHowManyCoin)
	ON_COMMAND(ID_Search_Capacitor,&CComputerVisionDoc::OnSearchCapacitor)
	ON_COMMAND(ID_Tophat,&CComputerVisionDoc::OnTopHat)
	ON_COMMAND(ID_Opening,&CComputerVisionDoc::OnOpening)
	ON_COMMAND(ID_Closing,&CComputerVisionDoc::OnClosing)
	ON_COMMAND(ID_Downhat,&CComputerVisionDoc::OnDownhat)
	ON_COMMAND(ID_Histogram_Equalization,&CComputerVisionDoc::OnHistogramEqualization)
	ON_COMMAND(ID_Histogram_Stretching,&CComputerVisionDoc::OnHistogramStretching)
	ON_COMMAND(ID_Laplacian,&CComputerVisionDoc::OnLaplacian)
	ON_COMMAND(ID_HSI_Color,&CComputerVisionDoc::OnHsiColor)
END_MESSAGE_MAP()


// CComputerVisionDoc ����/�Ҹ�

CComputerVisionDoc::CComputerVisionDoc()
{
	input_image.width = 0;
	input_image.height = 0;
	input_image.channel = 0;
	output_image.width = 0;
	output_image.height = 0;
	output_image.channel = 0;

	input_image.point = NULL;
	output_image.point = NULL;

	for(int i=0;i<temp_number_;i++)	
	{
		temp_img[i].point = NULL;
		temp_img[i].channel = 0;
		temp_img[i].height = 0;
		temp_img[i].width = 0;
	}

	out_color_histogram[2]=NULL;
	out_color_histogram[1]=NULL;
	out_color_histogram[0]=NULL;

	func_h = 0;
}

CComputerVisionDoc::~CComputerVisionDoc()
{
	if (input_image.point == NULL) delete[] input_image.point;
	if (output_image.point == NULL) delete[] output_image.point;
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
		ar << input_image.width << input_image.height << input_image.channel;
		if (input_image.point != NULL) ar.Write(input_image.point, input_image.width * input_image.height * input_image.channel);
		ar << output_image.width << output_image.height << output_image.channel;
		if (output_image.point != NULL) ar.Write(output_image.point, output_image.width * output_image.height * output_image.channel);
	}
	else
	{
		// ���� ���� ������� �ҷ�����, Read �Լ����� ���� �Ҵ��ؾ���
		ar >> input_image.width >> input_image.height >> input_image.channel;
		int input_image_size = input_image.width * input_image.height * input_image.channel;
		Allocate(input_image.point, input_image_size);
		if (input_image_size > 0) ar.Read(input_image.point, input_image_size);

		ar >> output_image.width >> output_image.height >> output_image.channel;
		int output_image_size = output_image.width * output_image.height * output_image.channel;
		Allocate(output_image.point, output_image_size);
		if (output_image_size > 0) ar.Read(output_image.point, output_image_size);
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
	if (input_image.point == NULL) return 1;
	RawToBMP(input_image.point, input_image.width, input_image.height, input_image.channel, image);

	return 0;
}

int CComputerVisionDoc::GetOutputMFCImage(CImage &image)
{
	if (output_image.point == NULL) return 1;
	RawToBMP(output_image.point, output_image.width, output_image.height, output_image.channel, image);

	return 0;
}

int CComputerVisionDoc::GetTempMFCImage(CImage &image,int n)
{
	if (temp_img[n].point == NULL || n>=20) return 1;
	RawToBMP(temp_img[n].point, temp_img[n].width, temp_img[n].height, temp_img[n].channel, image);

	return 0;
}

int CComputerVisionDoc::GetHistogrmMFCImage(CImage &image , int color) //������ �׷��� bmp ���� �Լ�
{
	if (out_color_histogram[color] == NULL) return 1;
	RawToBMP(out_color_histogram[color], 300, 300, 1, image);

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
	if (!BMPtoRaw(mfc_image, input_image.width, input_image.height, input_image.channel, input_image.point))
	{
		BMPtoRaw(mfc_image, output_image.width, output_image.height, output_image.channel, output_image.point);
	}

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnSaveImage()
{
	CImage mfc_image;
	if (RawToBMP(output_image.point, output_image.width, output_image.height, output_image.channel, mfc_image)) return;
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
	input_image.height = output_image.height;
	input_image.width = output_image.width;
	input_image.channel = output_image.channel;
	int input_image_size = input_image.height * input_image.width * input_image.channel;
	Allocate(input_image.point, input_image_size);
	memcpy(input_image.point, output_image.point, input_image_size);
	memset(output_image.point, 0, input_image_size);

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04GrayTransform()
{
	// ����
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image.channel != 3) { AfxMessageBox(_T("3ä�� �̹����� �׷��̷� ��ȯ ����")); return; }

	// ��� �̹��� ������¡
	int input_image_size = input_image.width * input_image.height * input_image.channel;
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = 1;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);
	
	// �׷��� ��ȯ
	for (int i = 0, j = 0; i < input_image_size; i += 3, ++j)
	{
		output_image.point[j] = (unsigned char)(0.114 * input_image.point[i] + 0.587 * input_image.point[i + 1] + 0.299 * input_image.point[i + 2]);
	}
	
	// ��� �� ������Ʈ
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04ColorInv()
{
	// ����
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }

	// ��� �̹��� ������¡
	int input_image_size = input_image.width * input_image.height * input_image.channel;
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	// ���� ������
	int h_size = input_image.height;
	int w_size = input_image.width;
	int c_size = input_image.channel;
	int wc_size = w_size * c_size;
	for (int h = 0, off_h = 0; h < h_size; ++h, off_h += wc_size)
	{
		for (int w = 0, off_w = off_h; w < w_size; ++w, off_w += c_size)
		{
			for (int c = 0, off = off_w; c < c_size; ++c, off += 1)
			{
				output_image.point[off] = (unsigned char)(255 - input_image.point[off]);
			}
		}
	}

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04Inv()
{
	// ����
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }

	// ��� �̹��� ������¡
	int input_image_size = input_image.width * input_image.height * input_image.channel;
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	// ������
	int h_size = input_image.height;
	int wc_size = input_image.width * input_image.channel;
	for (int h = 0, off = 0, off_inv = input_image_size - wc_size; h < h_size; ++h, off += wc_size, off_inv -= wc_size)
	{
		memcpy(&output_image.point[off_inv], &input_image.point[off], wc_size);
	}
	
	// ��� �� ������Ʈ
	UpdateAllViews(0);
}


void CComputerVisionDoc::OnMiddlefilter()
{
	// ����
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }

	// ��� �̹��� ������¡
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	//��� �̹��� �ʱ�ȭ
	for(int y = 0; y<output_image.height; y++) //��� �̹��� �ʱ�ȭ
		for(int x = 0; x<output_image.width; x++)
			for(int c=0;c<output_image.channel;c++)
				output_image.point[(y*output_image.width+x)*output_image.channel+c] = input_image.point[(y*output_image.width+x)*output_image.channel+c];

	make_middle_filiter(input_image,output_image,5);

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}



void CComputerVisionDoc::Onx2()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }

	// ��� �̹��� ������¡
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	//��� �̹��� �ʱ�ȭ
	for(int y = 0; y<output_image.height; y++) //��� �̹��� �ʱ�ȭ
		for(int x = 0; x<output_image.width; x++)
			for(int c=0;c<output_image.channel;c++)
				output_image.point[(y*output_image.width+x)*output_image.channel+c] = 0;

	//2�� Ȯ��
	float mux = 2.0;
	for(int y=0;y<output_image.height;y++)
		for(int x = 0; x<output_image.width; x++)
		{
			int off_out = (y*output_image.width+x)*output_image.channel;

			float r_h = (float)y/mux;
			float r_w = (float)x/mux;
			float sh = r_h - floor(r_h);
			float sw = r_w - floor(r_w);

			int off1 = ((int)floor(r_h)*input_image.width+(int)floor(r_w))*output_image.channel;
			int off2 = ((int)floor(r_h)*input_image.width+(int)floor(r_w)+1)*output_image.channel;
			int off4 = ((int)(floor(r_h)+1)*input_image.width+(int)floor(r_w)+1)*output_image.channel;
			int off3 = ((int)(floor(r_h)+1)*input_image.width+(int)floor(r_w))*output_image.channel;

			for(int c = 0; c<output_image.channel; c++)
			{
				float i1 = (float)input_image.point[off1+c];
				float i2 = (float)input_image.point[off2+c];
				float i3 = (float)input_image.point[off3+c];
				float i4 = (float)input_image.point[off4+c];

				output_image.point[off_out+c] = (BYTE)(i1*(1-sw)*(1-sh)+i2*sw*(1-sh)+i3*(1-sw)*sh+i4*sw*sh);
			}
		}

	// ��� �� ������Ʈ
	UpdateAllViews(0);	
}


void CComputerVisionDoc::Onplus50()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }

	// ��� �̹��� ������¡
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	//��� �̹��� �ʱ�ȭ
	for(int y = 0; y<output_image.height; y++) //��� �̹��� �ʱ�ȭ
		for(int x = 0; x<output_image.width; x++)
			for(int c=0;c<output_image.channel;c++)
				output_image.point[(y*output_image.width+x)*output_image.channel+c] = 0;

	//50 �̵�
	int plus = 50;
	if(output_image.width<50 || output_image.height<50) //���� �̹��� �Ұ�
	{ 
		AfxMessageBox(_T("���� ���ΰ� ���� 50���� ���� �̹���")); 
		return; 
	}

	for(int y=plus;y<output_image.height;y++)
		for(int x = plus; x<output_image.width; x++)
			for(int c = 0; c<output_image.channel; c++)
			{
				int off = (y*output_image.width+x)*output_image.channel+c;
				int offm50 = ((y-plus)*output_image.width+x-plus)*output_image.channel+c;
				output_image.point[off] = input_image.point[offm50];
			}

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}


void CComputerVisionDoc::Ondeg45()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }

	// ��� �̹��� ������¡
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	//45�� ȸ��
	float deg = 45*3.1415926535/180;
	int center_x = output_image.width/2;
	int center_y = output_image.height/2;

	for(int y=0;y<output_image.height;y++)
		for(int x = 0; x<output_image.width; x++)
		{
			float x_org = cos(deg)*(x-center_x) + sin(deg)*(y-center_y) +center_x;
			float y_org = -sin(deg)*(x-center_x) + cos(deg)*(y-center_y) +center_y;
			
			if(x_org<0||x_org>input_image.width-1||y_org<0||y_org>input_image.height-1) //�̹��� ������ ��� ��� 0���� �Ҵ�
			{
				for(int c = 0; c<output_image.channel; c++) 
					output_image.point[(y*output_image.width+x)*output_image.channel+c] = 0;
			}
			else
			{
				int off_out = (y*output_image.width+x)*output_image.channel;

				float sh = y_org - floor(y_org);
				float sw = x_org - floor(x_org);

				int off1 = (floor(y_org)*input_image.width+floor(x_org))*output_image.channel;
				int off2 = (floor(y_org)*input_image.width+floor(x_org)+1)*output_image.channel;
				int off4 = ((floor(y_org)+1)*input_image.width+floor(x_org)+1)*output_image.channel;
				int off3 = ((floor(y_org)+1)*input_image.width+floor(x_org))*output_image.channel;

				for(int c = 0; c<output_image.channel; c++)
				{
					float i1 = (float)input_image.point[off1+c];
					float i2 = (float)input_image.point[off2+c];
					float i3 = (float)input_image.point[off3+c];
					float i4 = (float)input_image.point[off4+c];

					output_image.point[off_out+c] = (BYTE)(i1*(1-sw)*(1-sh)+i2*sw*(1-sh)+i3*(1-sw)*sh+i4*sw*sh);
				}				
			}			
		}

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}


void CComputerVisionDoc::Onhistogram()
{
		// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// ����
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image.channel != 3 && input_image.channel != 1 ) { AfxMessageBox(_T("�׷��� �Ǵ� �÷��� ����")); return; }

	// ��� �̹��� ������¡
	int input_image_size = input_image.width * input_image.height * input_image.channel;
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);
	
	//��� �̹��� �ʱ�ȭ
	for(int y = 0; y<output_image.height; y++) //��� �̹��� �ʱ�ȭ
		for(int x = 0; x<output_image.width; x++)
			for(int c=0;c<output_image.channel;c++)
				output_image.point[(y*output_image.width+x)*output_image.channel+c] = input_image.point[(y*output_image.width+x)*output_image.channel+c];

	int histogram_size = 300 * 300;
	Allocate(out_color_histogram[2], histogram_size); //red
	Allocate(out_color_histogram[1] , histogram_size); //green
	Allocate(out_color_histogram[0] , histogram_size); //blue

	// ������׷�
	if(input_image.channel==3)
	{
		make_histogram(input_image,2); //red
		make_histogram(input_image,1); //green
		make_histogram(input_image,0); //blue
	}
	else if(input_image.channel==1)
		make_histogram(input_image,0); //gray

	func_h = 1;
	
	// ��� �� ������Ʈ
	UpdateAllViews(0);


}

void CComputerVisionDoc::OnHistoOtzu()  //������׷� + Otzu ����ȭ �̺�Ʈ ó�� �Լ�
{
	// ����
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("�׷��� �̹����� ����")); return; }

	// ���� ��� �̹��� ������¡
	int input_image_size = input_image.width * input_image.height * input_image.channel;
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);
	//������׷� ��� �̹��� ������¡
	int histo_image_size = 300*300;
	Allocate(out_color_histogram[0] , histo_image_size); //blue, histo_image_size);

	TopHat(input_image,output_image);

	make_histogram(input_image,0);
	Otzu_thresholding();

	func_h = 1;

	UpdateAllViews(0);

}
void CComputerVisionDoc::Otzu_thresholding() //����ȭ + Otzu ���
{
	float Otzu_min = 100000000000000.0;
	int Otzu_t = 0;


	for(int t=0; t<256; t++)
	{
		// ����Ȯ��q1, q2 ���
		float q1 = 0, q2 = 0;
		for(int i=0; i<t; i++)
			q1 += histogram[0][i];
		for(int i=t; i<256; i++) 
			q2 += histogram[0][i];
		if(q1==0 || q2==0) continue;

		// ���u1, u2 ���
		float u1=0.0f, u2=0.0f;
		for(int i=0; i<t; i++) u1 += i*histogram[0][i]; u1 /= q1;
		for(int i=t; i<256; i++) u2 += i*histogram[0][i]; u2 /= q2;

		// �л�s1, s2 ���
		float s1=0.0f, s2=0.0f;
		for(int i=0; i<t; i++) s1 += (i-u1)*(i-u1)*histogram[0][i]; s1 /= q1;
		for(int i=t; i<256; i++) s2 += (i-u2)*(i-u2)*histogram[0][i]; s2 /= q2;

		//�Ѻл���
		float wsv = q1*s1+q2*s2;
		if(wsv <  Otzu_min)
		{
			Otzu_min = wsv;
			Otzu_t= t; // �ּ�ġ����
		}
	}

	// thresholding
	for(int i=0; i<input_image.width*input_image.height; i++)
	{
		if(input_image.point[i]<Otzu_t)
		output_image.point[i]=0;
		else
		output_image.point[i]=255;
	}

	//ȭ�� ���
	UpdateAllViews(0);

	// �Ӱ谪 t ���
	CString msg;
	msg.Format(_T("�Ӱ谪 t : %d"),Otzu_t);
	AfxMessageBox(msg);
}

void CComputerVisionDoc::OnErosion()
{
	// ����
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("�׷��� �� ����")); return; }

	output_image.height = input_image.height; // ��� �̹��� ������¡
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);
	
	out_img_reset(255);//��� �̹��� �ʱ�ȭ

	for(int y = 0; y<input_image.height; y++) //3x3 ����ũ ħ�� ����
		for(int x = 0; x<input_image.width; x++)
		{
			int i=0,j=0,min=300;
			for(i=-1;i<2;i++)
				for(j = -1; j<2; j++)
				{
					if(y+i<0||y+i>=output_image.height||x+j<0||x+j>=output_image.width)continue;
					else if(input_image.point[(y+i)*input_image.width+x+j]<min)
					{
						min = input_image.point[(y+i)*input_image.width+x+j];
					}
				}
			output_image.point[y*output_image.width+x] = min;
		}
	
	UpdateAllViews(0); // ��� �� ������Ʈ
}


void CComputerVisionDoc::OnDilation()
{
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("�׷��� �� ����")); return; }

	output_image.height = input_image.height; // ��� �̹��� ������¡
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);
	
	out_img_reset(0,0,0);//��� �̹��� �ʱ�ȭ

	for(int y = 0; y<input_image.height; y++) //3x3 ����ũ ��â ����
		for(int x = 0; x<input_image.width; x++)
		{
			int i=0,j=0, max=0;
			for(i=-1;i<2;i++)
				for(j = -1; j<2; j++)
				{
					if(y+i<0||y+i>=output_image.height||x+j<0||x+j>=output_image.width)continue;
					else if(input_image.point[(y+i)*input_image.width+x+j]>max)
					{
						max = input_image.point[(y+i)*input_image.width+x+j];		
					}
				}
			output_image.point[y*output_image.width+x] = max;
		}

	UpdateAllViews(0); // ��� �� ������Ʈ
}


void CComputerVisionDoc::OnHowManyCoin() //�̺�Ʈ ó�� �Լ�
{
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("�׷��� �� ����")); return; }

	OnHistoOtzu(); //�ڵ� ��ġȭ , ������ ����� �ҽ� ��Ȱ��
	Labeling(label_first,output_image,50); //�󺧸�
	Label_list_painting_coin(); //���� ���� �� ������

	UpdateAllViews(0); // ��� �� ������Ʈ

}


int CComputerVisionDoc::Labeling(label &label_out,img_str &in_img,int cut_size) //�󺧸� �Լ�
{
	unsigned char *stack = new unsigned char[in_img.height*in_img.width*2]; //����
	int n=0; //������ ��ġ
	int num=0; //�� ���� �ȼ��� ����

	label_out.prev=NULL;
	label_out.next=NULL;

	label *this_label=NULL; //���� ó������ ��
	label *before_label=NULL; //�� ���� ��

	for(int y=0;y<in_img.height;y++)
		for(int x = 0; x<in_img.width; x++)
		{
			if(in_img.point[y*in_img.width+x]==255) //���� �̹������� �� ��� Ȯ��
			{
				in_img.point[y*in_img.width+x]=128; //����� �ȼ� ����(�� ����)

				if(this_label==NULL) //�� ó�� ���
				{
					this_label=&label_out;
					this_label->point = new unsigned char[in_img.height*in_img.width*2];
				}
				else //2�� ���� ���
				{
					before_label = this_label;
					this_label = new label;
					before_label->next = this_label;
					this_label->point = new unsigned char[in_img.height*in_img.width*2];
				}

				this_label->num = num++; //�� �ѹ� ���̱�
				this_label->quantity = 0; //�ȼ��� �� �ʱ�ȭ
				this_label->prev = before_label; //���Ḯ��Ʈ �ۼ�
				this_label->next = NULL;

				this_label->point[this_label->quantity*2] =x; //�ȼ� ����
				this_label->point[this_label->quantity*2+1] =y;
				this_label->quantity++;

				n=0; stack[2*n]=x; stack[2*n+1]=y; n++; //���� ����

				int xx=x,yy=y; //���� �󺧿� �ִ� ������ ��ǥ��
				while(n>=0) //���ؿ� ��ǥ�� ���� ������
				{
					int ok=0; //�󺧿� �ش��ϴ� �ȼ��� ������ 1, ������ 0
					for(int i=-1;i<2;i++) 
						for(int j =-1; j<2; j++)
						{
							if(yy+i<0||yy+i>in_img.height||xx+j<0||xx+j>in_img.width)continue;

							if(in_img.point[(yy+i)*in_img.width+xx+j]==255) //�ȼ� �߰�
							{
								ok=1;
								in_img.point[(yy+i)*in_img.width+xx+j]=128; //����� �ȼ� ����(�� ����)

								stack[2*n]=xx+j; stack[2*n+1]=yy+i; n++; //���ؿ� ��ǥ ����

								this_label->point[this_label->quantity*2] =xx+j; //���Ḯ��Ʈ�� ��ǥ����
								this_label->point[this_label->quantity*2+1] = yy+i;
								this_label->quantity++; //�ȼ��� �� ����
							}
						}
					if(ok) {yy=stack[2*n+1]; xx=stack[2*n];}
					else { n--;yy=stack[2*n+1]; xx=stack[2*n];} //�ȼ��� ã�� �� ������ �� ���� �ȼ� ���
				}

				if(this_label->quantity<cut_size && num>1) //ã�Ƴ� ���� ���̰� ���� ���� ������ ����
				{
					label *temp_label = before_label; 
					before_label = before_label->prev;
					delete [] this_label->point;
					delete this_label;
					this_label = temp_label;
					this_label->next =NULL;
					num--;
				}
				else if(this_label->quantity<cut_size && num==1) //ù ���� ���� ���
				{
					delete [] this_label->point;
					this_label=NULL;
					num--;
				}
			}
		}

	delete [] stack; //���� ����
	return 0;
}


int CComputerVisionDoc::Label_list_painting_coin()  //���� ���� �� ������
{
	label *this_label=&label_first;

	output_image.channel = 3; // ��� �̹��� ������¡
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	out_img_reset(0,0,0);//��� �̹��� �ʱ�ȭ

	temp_img[0].height = output_image.height; // �ӽ� �̹��� ������¡
	temp_img[0].width = output_image.width;
	temp_img[0].channel = 1;
	int temp_image_size = temp_img[0].height * temp_img[0].width * temp_img[0].channel;
	Allocate(temp_img[0].point, temp_image_size);

	for(int y = 0; y<temp_img[0].height; y++) //�ӽ� �̹��� �ʱ�ȭ
		for(int x = 0; x<temp_img[0].width; x++)
				temp_img[0].point[y*output_image.width+x] = 0;

	int hole0=0,hole1=0; //���� ���۾��� ������ �ִ� ������ ����
	while(this_label != NULL)
	{
		int hole = is_it_hole_coin(*this_label); //���� ���� Ȯ��
		if(hole==1) { Label_Painting(*this_label,255,0,0); hole1++; } //���� �ִ� ���� ĥ�ϱ�
		else if(hole==0) { Label_Painting(*this_label,0,0,255); hole0++; } //���� ���� ���� ĥ�ϱ�

		this_label = this_label->next; //���� ��
	}
	CString msg;
	msg.Format(_T("���� ����: %d , ���۾���: %d"), hole1,hole0);
	AfxMessageBox(msg);

	return 0;
}


int CComputerVisionDoc::Label_Painting(label input,unsigned char red,unsigned char green,unsigned char blue)
{//���� �ȼ� ĥ�ϱ�
	for(int i=0;i<input.quantity;i++)
	{
		int x = input.point[2*i];
		int y = input.point[2*i+1];

		output_image.point[(y*output_image.width+x)*output_image.channel] = blue;
		output_image.point[(y*output_image.width+x)*output_image.channel+1] = green;
		output_image.point[(y*output_image.width+x)*output_image.channel+2] = red;
	}

	return 0;
}


int CComputerVisionDoc::is_it_hole_coin(label input) //���� ���� ���� Ȯ��
{	
	for(int i=0;i<input.quantity;i++) //���� �׸���
	{
		int x = input.point[2*i];
		int y = input.point[2*i+1];

		temp_img[0].point[y*temp_img[0].width+x] = 255;
	}

	for(int y = 0; y<temp_img[0].height; y++) //�ܰ����� �׸���
		for(int x = 0; x<temp_img[0].width; x++)
			if(temp_img[0].point[y*temp_img[0].width+x]==255)
			{
				int del_ok = 1;
				for(int i = -1; i<2; i++)
				{
					if(temp_img[0].point[(y+i)*temp_img[0].width+x]==0 || temp_img[0].point[y*temp_img[0].width+x+i]==0 ) 
					{
						del_ok=0;
						break;
					}

				}
				if(del_ok)temp_img[0].point[y*temp_img[0].width+x]=128;
			}

	int num=0;//�ܰ��� ���� ����
	unsigned char *stack = new unsigned char[output_image.height*output_image.width*2];
	int n=0;
	float len=0.0,max=0.0; //�ܰ����� ����

	for(int y=0;y<temp_img[0].height;y++)
		for(int x = 0; x<temp_img[0].width; x++)
		{
			if(temp_img[0].point[y*temp_img[0].width+x]==255)
			{
				num++;

				n=0; stack[2*n]=x; stack[2*n+1]=y; n++; //���� ����
				len+=1; //���� ����

				int xx=x,yy=y;
				while(n>=0)
				{
					int ok=0;
					for(int i=-1;i<2;i++)
						for(int j = -1; j<2; j++)
						{
							if(yy+i<0||yy+i>temp_img[0].height || xx+j<0 || xx+j>temp_img[0].width)continue;

							if(temp_img[0].point[(yy+i)*temp_img[0].width+xx+j]==255)
							{
								ok=1;
								temp_img[0].point[(yy+i)*output_image.width+xx+j]=199;
								stack[2*n]=xx+j; stack[2*n+1]=yy+i; n++; len+=1;
							}
						}
					if(ok) {yy=stack[2*n+1]; xx=stack[2*n];}
					else { n--;yy=stack[2*n+1]; xx=stack[2*n];}
				}
				if(max<len)max=len;
			}
		}

	for(int y=0;y<temp_img[0].height;y++) //�ӽ� �̹����� �ܰ����� ����
		for(int x = 0; x<temp_img[0].width; x++)
		{
			if(temp_img[0].point[y*temp_img[0].width+x]==128)temp_img[0].point[y*temp_img[0].width+x]=0;
		}

	float check = 4.0*3.14159*(float)input.quantity/(max*max); //���� �������� Ȯ��
	if(check<0.5 || check>2.0)num=-1; //������ �ƴϸ� ���� ����

	delete [] stack;
	return num-1;
}


void CComputerVisionDoc::out_img_reset(unsigned char red,unsigned char green,unsigned char blue) //��� �̹��� �ʱ�ȭ
{
	if(output_image.channel == 3)
		for(int y = 0; y<output_image.height; y++) 
			for(int x = 0; x<output_image.width; x++)
			{
				output_image.point[(y*output_image.width+x)*output_image.channel] = blue;
				output_image.point[(y*output_image.width+x)*output_image.channel+1] = green;
				output_image.point[(y*output_image.width+x)*output_image.channel+2] = red;
			}
	if(output_image.channel == 1)
		for(int y = 0; y<output_image.height; y++) 
			for(int x = 0; x<output_image.width; x++)
				output_image.point[y*output_image.width+x] = red;
}




void CComputerVisionDoc::OnSearchCapacitor() //Ŀ�н��� �˻��� �̺�Ʈ ó�� �Լ�
{
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; } // ����
	if (input_image.channel != 1 ) { AfxMessageBox(_T("�׷��� �� ����")); return; }

	NCC_box in_ncc; //��ȭ ���� ȣ��
	in_ncc.DoModal();

	output_image.height = input_image.height; // ��� �̹��� ������¡
	output_image.width = input_image.width;
	output_image.channel = 3;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	temp_img[1].height = in_ncc.lasty-in_ncc.fy; // ���ø� �̹��� ������¡
	temp_img[1].width = in_ncc.lastx-in_ncc.fx;
	temp_img[1].fx = in_ncc.fx;
	temp_img[1].fy = in_ncc.fy;
	temp_img[1].channel = input_image.channel;
	int temp_image_size1 = temp_img[1].height * temp_img[1].width  * temp_img[1].channel;
	Allocate(temp_img[1].point, temp_image_size1);

	for(int y=0;y<temp_img[1].height;y++) //���� ���ø� �̹��� ����
		for(int x=0;x<temp_img[1].width;x++)
			temp_img[1].point[y*temp_img[1].width+x] = input_image.point[(y+in_ncc.fy)*input_image.width+x+in_ncc.fx];
			
	Ncc_match_sreach(input_image,temp_img[1],output_image,in_ncc.Ncc_prn_cut); //�˻� ����

	UpdateAllViews(0);
}

int CComputerVisionDoc::Ncc_match_sreach(img_str &in_img,img_str &temp_img_,img_str  &out_img,int ncc_cut)
{
	if (in_img.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return -1; } // ����
	int temp_image_size =temp_img_.height*temp_img_.width; //���ø� �̹��� ũ��
	img_str temp_out[2]; //����� �ӽ� �����(����, ����)

	for(int i = 0; i<2; i++) //�ӽ� �̹��� �ʱ�ȭ
	{   temp_out[i].channel = 1;
		temp_out[i].height = out_img.height;
		temp_out[i].width = out_img.width;
		temp_out[i].point = new unsigned char[out_img.height*out_img.width];
	}

	float t=0.0 , tt=0.0,t_out; //���ø� �̹����� �����͸� �̸� ���
	for(int y=0;y<temp_img_.height;y++)
		for(int x = 0; x<temp_img_.width; x++)
		{   t+=(float)temp_img_.point[y*temp_img_.width+x];
			tt+=(float)temp_img_.point[y*temp_img_.width+x]*(float)temp_img_.point[y*temp_img_.width+x];
		}
	t_out = temp_image_size*tt-t*t;

	for(int fy=0;fy<input_image.height;fy++) //Ž�������� ���ؼ� ��Ī�� ���
		for(int fx = 0; fx<input_image.width; fx++)
		{   float g[2] = {0.0, 0.0};
			float gg[2] = {0.0, 0.0};
			float gt[2] = {0.0, 0.0};
			
			if(temp_img_.height+fy<input_image.height && temp_img_.width+fx<input_image.width) //���� Ŀ�н��� ���ø�
				for(int y = fy; y <temp_img_.height+fy; y++)
					for(int x = fx; x<temp_img_.width+fx; x++)
					{   g[0]+=(float)in_img.point[y*input_image.width+x];
						gg[0]+=(float)in_img.point[y*input_image.width+x]*(float)in_img.point[y*input_image.width+x];
						gt[0]+=(float)in_img.point[y*input_image.width+x]*(float)temp_img_.point[(y-fy)*temp_img_.width+x-fx];
					}
			else g[0]=-1; //�̹����� ������ ���

			if(temp_img_.width+fy<input_image.height && temp_img_.height+fx<input_image.width) //���� Ŀ�н��� ���ø�
				for(int y = fy; y <temp_img_.width+fy; y++)
					for(int x = fx; x<temp_img_.height+fx; x++)
					{   g[1]+=(float)in_img.point[y*input_image.width+x];
						gg[1]+=(float)in_img.point[y*input_image.width+x]*(float)in_img.point[y*input_image.width+x];
						gt[1]+=(float)in_img.point[y*input_image.width+x]*(float)temp_img_.point[(x-fx)*temp_img_.width+y-fy];
					}
			else g[1]=-1;

			float r[2], de_r[2]; //��Ī���� ���ڿ� �и�
			int out_score[2]={0,0}; //���� ��Ī��
			for(int i = 0; i<2; i++)
			{   if(g[i]<0)
				{   temp_out[i].point[fy*out_img.width+fx] = 0;
					continue;
				}

				r[i] = temp_image_size*gt[i]-g[i]*t;
				de_r[i] = (temp_image_size*gg[i]-g[i]*g[i])*t_out;

				if(r[i]<0 || de_r[i]<0.0001) out_score[i] = 0;
				else out_score[i] = (int)(r[i]*r[i]*255.0/de_r[i]);
				if(ncc_cut < out_score[i]) temp_out[i].point[fy*out_img.width+fx] = 255; // �ӽ� ������ ������ �ȼ��� ��Ī�� ����
				else temp_out[i].point[fy*out_img.width+fx] = 0;
			}
		}

	label label_Capacitor0,label_Capacitor1; //Ŀ����ͷ� ����Ǵ� ��ġ�� �󺧸�
	label_Capacitor0.point = NULL; label_Capacitor1.point = NULL;
	Labeling(label_Capacitor0,temp_out[0]); //����� ���� ���� ����ü label�� ���� Ŀ�н����� ��ġ�� ����� �ӽ� �̹��� 
	Labeling(label_Capacitor1,temp_out[1]);
	trans_img(in_img,out_img); //�Է� �̹����� ��� �̹����� ����

	label *label_point=&label_Capacitor0; //���� ���ؼ� Ŀ�н����� ��ġ�� �Է� �̹����� �簢������ ��Ÿ��
	int hc=0 , wc=0; //����, ���� Ŀ�н����� ����
	while(label_point!=NULL)
	{   int main_x=0,main_y=0; //�簢���� ������
		for(int i = 0; i<label_point->quantity; i++)
		{   main_x += label_point->point[2*i];
			main_y += label_point->point[2*i+1];
		}
		main_x /= label_point->quantity;
		main_y /= label_point->quantity;

		prn_rect(out_img,main_x,main_y,temp_img_.height,temp_img_.width,3,255,0,0); //�簢�� ��¿� �Լ�

		label_point = label_point->next;
		hc++;
	}
	label_point=&label_Capacitor1; //���� Ŀ�н����� ��ġ�� �簢������ ��Ÿ��
	while(label_point!=NULL)
	{   int main_x=0,main_y=0;
		for(int i = 0; i<label_point->quantity; i++)
		{
			main_x += label_point->point[2*i];
			main_y += label_point->point[2*i+1];
		}
		main_x /= label_point->quantity;
		main_y /= label_point->quantity;

		prn_rect(out_img,main_x,main_y,temp_img_.height,temp_img_.width,3,0,0,255);

		label_point = label_point->next;
		wc++;
	}

	prn_rect(out_img,temp_img_.fx,temp_img_.fy,temp_img_.height,temp_img_.width,3,255,255,0); //���ø����� ����� Ŀ�м����� ��ġ

	UpdateAllViews(0); //��� ���
	CString msg; 
	msg.Format(_T("���� : %d , ���� : %d"), hc,wc);
	AfxMessageBox(msg);
	
	reset_label(label_Capacitor0); //�޸� ���� 
	reset_label(label_Capacitor1); //���� ����Ʈ ���� �����ϴ� �Լ�
	delete [] temp_out[0].point; delete [] temp_out[1].point; //�ӽ� �̹��� ����
	return 0;
}

void CComputerVisionDoc::OnTopHat()
{
	// ����
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("�׷��� �� ����")); return; }

	output_image.height = input_image.height; // ��� �̹��� ������¡
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	TopHat(input_image,output_image,30);

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnDownhat()
{
	// ����
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("�׷��� �� ����")); return; }

	output_image.height = input_image.height; // ��� �̹��� ������¡
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	DownHat(input_image,output_image,30);

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnHistogramEqualization()
{
	// ����
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("�׷��� �� ����")); return; }

	output_image.height = input_image.height; // ��� �̹��� ������¡
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	Histogram_Equalization(input_image,output_image);

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnHistogramStretching()
{
	// ����
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("�׷��� �� ����")); return; }

	output_image.height = input_image.height; // ��� �̹��� ������¡
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	Histogram_Stretching(input_image,output_image);

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnLaplacian()
{
	// ����
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("�׷��� �� ����")); return; }

	output_image.height = input_image.height; // ��� �̹��� ������¡
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	make_laplacian(input_image,output_image);

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}


void CComputerVisionDoc::OnOpening()
{
	// ����
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("�׷��� �� ����")); return; }

	make_Opening(input_image,output_image);

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnClosing()
{
	// ����
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("�׷��� �� ����")); return; }

	make_Closing(input_image,output_image);

	// ��� �� ������Ʈ
	UpdateAllViews(0);
}


int CComputerVisionDoc::reset_label(label & first)
{
	label *thisis = first.next;
	label *next_label=NULL;
	delete [] first.point;

	while(thisis!=NULL)
	{
		delete[] thisis->point;
		next_label = thisis->next;
		delete[] thisis;
		thisis = next_label;
	}

	return 0;
}


int CComputerVisionDoc::trans_img(img_str & in_img,img_str & out_img)
{
	for(int y=0;y<out_img.height;y++)
		for(int x=0;x<out_img.width;x++)
			for(int c=0;c<out_img.channel;c++)
				out_img.point[(y*out_img.width+x)*out_img.channel+c] = in_img.point[(y*in_img.width+x)*in_img.channel+c];

	return 0;
}


int CComputerVisionDoc::prn_rect(img_str & background,int fx,int fy,int height,int width,int n,int red,int green,int blue)
{//�簢�� ��� �Լ�
	for(int y=fy-n;y<fy+height+n+1;y++)
		for(int x = fx-n; x<fx+width+n+1; x++)
		{
			if(y<0 || y>=background.height || x<0 || x>=background.width) continue;
			if(y<fy || y>fy+height || x<fx || x>fx+width) 
			{
				if(background.channel==1) background.point[y*background.width+x] = red;
				else if(background.channel==3) 
				{
					background.point[(y*background.width+x)*background.channel+2] = red;
					background.point[(y*background.width+x)*background.channel+1] = green;
					background.point[(y*background.width+x)*background.channel+0] = blue;
				}
			}
		}
	return 0;
}

void CComputerVisionDoc::OnHsiColor()
{
	if (input_image.point == NULL) { AfxMessageBox(_T("����ִ� �̹���")); return; }
	if (input_image.channel != 3 ) { AfxMessageBox(_T("�÷� �� ����")); return; }

	// ��� �̹��� ������¡
	temp_img[0].height = temp_img[1].height = temp_img[2].height = input_image.height; 
	temp_img[0].width = temp_img[1].width = temp_img[2].width = input_image.width;
	temp_img[0].channel = temp_img[1].channel = temp_img[2].channel = 1;
	int output_image_size = input_image.height * input_image.width;
	Allocate(temp_img[0].point, output_image_size);
	Allocate(temp_img[1].point, output_image_size);
	Allocate(temp_img[2].point, output_image_size);

	int height = input_image.height;
	int width = input_image.width;
	float pi = 3.1415926535;

	for(int y=0 ; y<height;y++)
		for(int x = 0; x<width; x++)
		{
			int r = input_image.point[(y*width+x)*3+2]; //�÷� ����
			int g = input_image.point[(y*width+x)*3+1];
			int b = input_image.point[(y*width+x)*3+0];

			float min = (r>b) ? b : r; //rgb�� �ּҰ� ���ϱ�
			min = (min>g) ? g : min;

			int i = (r+g+b)/3;
			int s = 255 - 255.0*min*3.0/(float)(r+g+b);

			float angle = 0.5*(2*r-b-g)/sqrt((r-g)*(r-g)+(r-b)*(g-b));
			float hf = acos(angle)*180/pi;
			if(b>g)hf = 360.0-hf;
			int h = hf*255.0/360.0;

			temp_img[0].point[y*width+x] = h;
			temp_img[1].point[y*width+x] = s;
			temp_img[2].point[y*width+x] = i;
		}

	func_h = 0;
	UpdateAllViews(0); //ȭ�� ���
}


