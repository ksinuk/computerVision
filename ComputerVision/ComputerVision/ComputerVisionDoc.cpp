
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


// CComputerVisionDoc 생성/소멸

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
		ar << input_image.width << input_image.height << input_image.channel;
		if (input_image.point != NULL) ar.Write(input_image.point, input_image.width * input_image.height * input_image.channel);
		ar << output_image.width << output_image.height << output_image.channel;
		if (output_image.point != NULL) ar.Write(output_image.point, output_image.width * output_image.height * output_image.channel);
	}
	else
	{
		// 변수 선언 순서대로 불러오기, Read 함수쓰기 전에 할당해야함
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

int CComputerVisionDoc::GetHistogrmMFCImage(CImage &image , int color) //히스토 그램용 bmp 생성 함수
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
	if (!BMPtoRaw(mfc_image, input_image.width, input_image.height, input_image.channel, input_image.point))
	{
		BMPtoRaw(mfc_image, output_image.width, output_image.height, output_image.channel, output_image.point);
	}

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnSaveImage()
{
	CImage mfc_image;
	if (RawToBMP(output_image.point, output_image.width, output_image.height, output_image.channel, mfc_image)) return;
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
	input_image.height = output_image.height;
	input_image.width = output_image.width;
	input_image.channel = output_image.channel;
	int input_image_size = input_image.height * input_image.width * input_image.channel;
	Allocate(input_image.point, input_image_size);
	memcpy(input_image.point, output_image.point, input_image_size);
	memset(output_image.point, 0, input_image_size);

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04GrayTransform()
{
	// 예외
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 3) { AfxMessageBox(_T("3채널 이미지만 그레이로 변환 가능")); return; }

	// 출력 이미지 리사이징
	int input_image_size = input_image.width * input_image.height * input_image.channel;
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = 1;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);
	
	// 그레이 변환
	for (int i = 0, j = 0; i < input_image_size; i += 3, ++j)
	{
		output_image.point[j] = (unsigned char)(0.114 * input_image.point[i] + 0.587 * input_image.point[i + 1] + 0.299 * input_image.point[i + 2]);
	}
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04ColorInv()
{
	// 예외
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }

	// 출력 이미지 리사이징
	int input_image_size = input_image.width * input_image.height * input_image.channel;
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	// 색상 뒤집기
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

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04Inv()
{
	// 예외
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }

	// 출력 이미지 리사이징
	int input_image_size = input_image.width * input_image.height * input_image.channel;
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	// 뒤집기
	int h_size = input_image.height;
	int wc_size = input_image.width * input_image.channel;
	for (int h = 0, off = 0, off_inv = input_image_size - wc_size; h < h_size; ++h, off += wc_size, off_inv -= wc_size)
	{
		memcpy(&output_image.point[off_inv], &input_image.point[off], wc_size);
	}
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}


void CComputerVisionDoc::OnMiddlefilter()
{
	// 예외
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }

	// 출력 이미지 리사이징
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	//출력 이미지 초기화
	for(int y = 0; y<output_image.height; y++) //출력 이미지 초기화
		for(int x = 0; x<output_image.width; x++)
			for(int c=0;c<output_image.channel;c++)
				output_image.point[(y*output_image.width+x)*output_image.channel+c] = input_image.point[(y*output_image.width+x)*output_image.channel+c];

	make_middle_filiter(input_image,output_image,5);

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}



void CComputerVisionDoc::Onx2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }

	// 출력 이미지 리사이징
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	//출력 이미지 초기화
	for(int y = 0; y<output_image.height; y++) //출력 이미지 초기화
		for(int x = 0; x<output_image.width; x++)
			for(int c=0;c<output_image.channel;c++)
				output_image.point[(y*output_image.width+x)*output_image.channel+c] = 0;

	//2배 확장
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

	// 모든 뷰 업데이트
	UpdateAllViews(0);	
}


void CComputerVisionDoc::Onplus50()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }

	// 출력 이미지 리사이징
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	//출력 이미지 초기화
	for(int y = 0; y<output_image.height; y++) //출력 이미지 초기화
		for(int x = 0; x<output_image.width; x++)
			for(int c=0;c<output_image.channel;c++)
				output_image.point[(y*output_image.width+x)*output_image.channel+c] = 0;

	//50 이동
	int plus = 50;
	if(output_image.width<50 || output_image.height<50) //작은 이미지 불가
	{ 
		AfxMessageBox(_T("가로 세로가 각각 50보다 작은 이미지")); 
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

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}


void CComputerVisionDoc::Ondeg45()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }

	// 출력 이미지 리사이징
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	//45도 회전
	float deg = 45*3.1415926535/180;
	int center_x = output_image.width/2;
	int center_y = output_image.height/2;

	for(int y=0;y<output_image.height;y++)
		for(int x = 0; x<output_image.width; x++)
		{
			float x_org = cos(deg)*(x-center_x) + sin(deg)*(y-center_y) +center_x;
			float y_org = -sin(deg)*(x-center_x) + cos(deg)*(y-center_y) +center_y;
			
			if(x_org<0||x_org>input_image.width-1||y_org<0||y_org>input_image.height-1) //이미지 범위를 벗어난 경우 0으로 할당
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

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}


void CComputerVisionDoc::Onhistogram()
{
		// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 예외
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 3 && input_image.channel != 1 ) { AfxMessageBox(_T("그레이 또는 컬러만 가능")); return; }

	// 출력 이미지 리사이징
	int input_image_size = input_image.width * input_image.height * input_image.channel;
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);
	
	//출력 이미지 초기화
	for(int y = 0; y<output_image.height; y++) //출력 이미지 초기화
		for(int x = 0; x<output_image.width; x++)
			for(int c=0;c<output_image.channel;c++)
				output_image.point[(y*output_image.width+x)*output_image.channel+c] = input_image.point[(y*output_image.width+x)*output_image.channel+c];

	int histogram_size = 300 * 300;
	Allocate(out_color_histogram[2], histogram_size); //red
	Allocate(out_color_histogram[1] , histogram_size); //green
	Allocate(out_color_histogram[0] , histogram_size); //blue

	// 히스토그램
	if(input_image.channel==3)
	{
		make_histogram(input_image,2); //red
		make_histogram(input_image,1); //green
		make_histogram(input_image,0); //blue
	}
	else if(input_image.channel==1)
		make_histogram(input_image,0); //gray

	func_h = 1;
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);


}

void CComputerVisionDoc::OnHistoOtzu()  //히스토그램 + Otzu 이진화 이벤트 처리 함수
{
	// 예외
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	// 이진 출력 이미지 리사이징
	int input_image_size = input_image.width * input_image.height * input_image.channel;
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);
	//히스토그램 출력 이미지 리사이징
	int histo_image_size = 300*300;
	Allocate(out_color_histogram[0] , histo_image_size); //blue, histo_image_size);

	TopHat(input_image,output_image);

	make_histogram(input_image,0);
	Otzu_thresholding();

	func_h = 1;

	UpdateAllViews(0);

}
void CComputerVisionDoc::Otzu_thresholding() //이진화 + Otzu 계산
{
	float Otzu_min = 100000000000000.0;
	int Otzu_t = 0;


	for(int t=0; t<256; t++)
	{
		// 누적확률q1, q2 계산
		float q1 = 0, q2 = 0;
		for(int i=0; i<t; i++)
			q1 += histogram[0][i];
		for(int i=t; i<256; i++) 
			q2 += histogram[0][i];
		if(q1==0 || q2==0) continue;

		// 평균u1, u2 계산
		float u1=0.0f, u2=0.0f;
		for(int i=0; i<t; i++) u1 += i*histogram[0][i]; u1 /= q1;
		for(int i=t; i<256; i++) u2 += i*histogram[0][i]; u2 /= q2;

		// 분산s1, s2 계산
		float s1=0.0f, s2=0.0f;
		for(int i=0; i<t; i++) s1 += (i-u1)*(i-u1)*histogram[0][i]; s1 /= q1;
		for(int i=t; i<256; i++) s2 += (i-u2)*(i-u2)*histogram[0][i]; s2 /= q2;

		//총분산계산
		float wsv = q1*s1+q2*s2;
		if(wsv <  Otzu_min)
		{
			Otzu_min = wsv;
			Otzu_t= t; // 최소치저장
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

	//화면 출력
	UpdateAllViews(0);

	// 임계값 t 출력
	CString msg;
	msg.Format(_T("임계값 t : %d"),Otzu_t);
	AfxMessageBox(msg);
}

void CComputerVisionDoc::OnErosion()
{
	// 예외
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	output_image.height = input_image.height; // 출력 이미지 리사이징
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);
	
	out_img_reset(255);//출력 이미지 초기화

	for(int y = 0; y<input_image.height; y++) //3x3 마스크 침식 연산
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
	
	UpdateAllViews(0); // 모든 뷰 업데이트
}


void CComputerVisionDoc::OnDilation()
{
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	output_image.height = input_image.height; // 출력 이미지 리사이징
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);
	
	out_img_reset(0,0,0);//출력 이미지 초기화

	for(int y = 0; y<input_image.height; y++) //3x3 마스크 팽창 연산
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

	UpdateAllViews(0); // 모든 뷰 업데이트
}


void CComputerVisionDoc::OnHowManyCoin() //이벤트 처리 함수
{
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	OnHistoOtzu(); //자동 이치화 , 이전에 사용한 소스 재활용
	Labeling(label_first,output_image,50); //라벨링
	Label_list_painting_coin(); //동전 구분 및 페인팅

	UpdateAllViews(0); // 모든 뷰 업데이트

}


int CComputerVisionDoc::Labeling(label &label_out,img_str &in_img,int cut_size) //라벨링 함수
{
	unsigned char *stack = new unsigned char[in_img.height*in_img.width*2]; //스텍
	int n=0; //스텍의 위치
	int num=0; //라벨 안의 픽세의 개수

	label_out.prev=NULL;
	label_out.next=NULL;

	label *this_label=NULL; //현재 처리중인 라벨
	label *before_label=NULL; //그 이전 라벨

	for(int y=0;y<in_img.height;y++)
		for(int x = 0; x<in_img.width; x++)
		{
			if(in_img.point[y*in_img.width+x]==255) //이진 이미지에서 라벨 대상 확인
			{
				in_img.point[y*in_img.width+x]=128; //사용한 픽셀 삭제(값 변경)

				if(this_label==NULL) //라벨 처음 사용
				{
					this_label=&label_out;
					this_label->point = new unsigned char[in_img.height*in_img.width*2];
				}
				else //2번 이후 사용
				{
					before_label = this_label;
					this_label = new label;
					before_label->next = this_label;
					this_label->point = new unsigned char[in_img.height*in_img.width*2];
				}

				this_label->num = num++; //라벨 넘버 붙이기
				this_label->quantity = 0; //픽셀의 양 초기화
				this_label->prev = before_label; //연결리스트 작성
				this_label->next = NULL;

				this_label->point[this_label->quantity*2] =x; //픽셀 저장
				this_label->point[this_label->quantity*2+1] =y;
				this_label->quantity++;

				n=0; stack[2*n]=x; stack[2*n+1]=y; n++; //스텍 저장

				int xx=x,yy=y; //같은 라벨에 있는 원소의 좌표값
				while(n>=0) //스텍에 좌표가 남아 있으면
				{
					int ok=0; //라벨에 해당하는 픽셀이 있으면 1, 없으면 0
					for(int i=-1;i<2;i++) 
						for(int j =-1; j<2; j++)
						{
							if(yy+i<0||yy+i>in_img.height||xx+j<0||xx+j>in_img.width)continue;

							if(in_img.point[(yy+i)*in_img.width+xx+j]==255) //픽셀 발견
							{
								ok=1;
								in_img.point[(yy+i)*in_img.width+xx+j]=128; //사용한 픽셀 삭제(값 변경)

								stack[2*n]=xx+j; stack[2*n+1]=yy+i; n++; //스텍에 좌표 저장

								this_label->point[this_label->quantity*2] =xx+j; //연결리스트에 좌표저장
								this_label->point[this_label->quantity*2+1] = yy+i;
								this_label->quantity++; //픽셀의 양 조정
							}
						}
					if(ok) {yy=stack[2*n+1]; xx=stack[2*n];}
					else { n--;yy=stack[2*n+1]; xx=stack[2*n];} //픽셀의 찾을 수 없으면 그 이전 픽셀 사용
				}

				if(this_label->quantity<cut_size && num>1) //찾아낸 라벨의 넓이가 기준 보다 작으면 삭제
				{
					label *temp_label = before_label; 
					before_label = before_label->prev;
					delete [] this_label->point;
					delete this_label;
					this_label = temp_label;
					this_label->next =NULL;
					num--;
				}
				else if(this_label->quantity<cut_size && num==1) //첫 라벨이 삭제 대상
				{
					delete [] this_label->point;
					this_label=NULL;
					num--;
				}
			}
		}

	delete [] stack; //스텍 해제
	return 0;
}


int CComputerVisionDoc::Label_list_painting_coin()  //동전 구분 및 페인팅
{
	label *this_label=&label_first;

	output_image.channel = 3; // 출력 이미지 리사이징
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	out_img_reset(0,0,0);//출력 이미지 초기화

	temp_img[0].height = output_image.height; // 임시 이미지 리사이징
	temp_img[0].width = output_image.width;
	temp_img[0].channel = 1;
	int temp_image_size = temp_img[0].height * temp_img[0].width * temp_img[0].channel;
	Allocate(temp_img[0].point, temp_image_size);

	for(int y = 0; y<temp_img[0].height; y++) //임시 이미지 초기화
		for(int x = 0; x<temp_img[0].width; x++)
				temp_img[0].point[y*output_image.width+x] = 0;

	int hole0=0,hole1=0; //각각 구멍없는 동전과 있는 동전의 개수
	while(this_label != NULL)
	{
		int hole = is_it_hole_coin(*this_label); //구멍 여부 확인
		if(hole==1) { Label_Painting(*this_label,255,0,0); hole1++; } //구멍 있는 동전 칠하기
		else if(hole==0) { Label_Painting(*this_label,0,0,255); hole0++; } //구멍 없는 동전 칠하기

		this_label = this_label->next; //다음 라벨
	}
	CString msg;
	msg.Format(_T("구멍 있음: %d , 구멍없음: %d"), hole1,hole0);
	AfxMessageBox(msg);

	return 0;
}


int CComputerVisionDoc::Label_Painting(label input,unsigned char red,unsigned char green,unsigned char blue)
{//라벨의 픽셀 칠하기
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


int CComputerVisionDoc::is_it_hole_coin(label input) //동전 구멍 여부 확인
{	
	for(int i=0;i<input.quantity;i++) //동전 그리기
	{
		int x = input.point[2*i];
		int y = input.point[2*i+1];

		temp_img[0].point[y*temp_img[0].width+x] = 255;
	}

	for(int y = 0; y<temp_img[0].height; y++) //외곽선만 그리기
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

	int num=0;//외곽선 갯수 세기
	unsigned char *stack = new unsigned char[output_image.height*output_image.width*2];
	int n=0;
	float len=0.0,max=0.0; //외곽선의 길이

	for(int y=0;y<temp_img[0].height;y++)
		for(int x = 0; x<temp_img[0].width; x++)
		{
			if(temp_img[0].point[y*temp_img[0].width+x]==255)
			{
				num++;

				n=0; stack[2*n]=x; stack[2*n+1]=y; n++; //스텍 저장
				len+=1; //길이 조정

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

	for(int y=0;y<temp_img[0].height;y++) //임시 이미지에 외곽선만 남김
		for(int x = 0; x<temp_img[0].width; x++)
		{
			if(temp_img[0].point[y*temp_img[0].width+x]==128)temp_img[0].point[y*temp_img[0].width+x]=0;
		}

	float check = 4.0*3.14159*(float)input.quantity/(max*max); //실제 동전인지 확인
	if(check<0.5 || check>2.0)num=-1; //동전이 아니면 오류 판정

	delete [] stack;
	return num-1;
}


void CComputerVisionDoc::out_img_reset(unsigned char red,unsigned char green,unsigned char blue) //출력 이미지 초기화
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




void CComputerVisionDoc::OnSearchCapacitor() //커패시터 검색용 이벤트 처리 함수
{
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; } // 예외
	if (input_image.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	NCC_box in_ncc; //대화 상자 호출
	in_ncc.DoModal();

	output_image.height = input_image.height; // 출력 이미지 리사이징
	output_image.width = input_image.width;
	output_image.channel = 3;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	temp_img[1].height = in_ncc.lasty-in_ncc.fy; // 템플릿 이미지 리사이징
	temp_img[1].width = in_ncc.lastx-in_ncc.fx;
	temp_img[1].fx = in_ncc.fx;
	temp_img[1].fy = in_ncc.fy;
	temp_img[1].channel = input_image.channel;
	int temp_image_size1 = temp_img[1].height * temp_img[1].width  * temp_img[1].channel;
	Allocate(temp_img[1].point, temp_image_size1);

	for(int y=0;y<temp_img[1].height;y++) //실제 템플릿 이미지 추출
		for(int x=0;x<temp_img[1].width;x++)
			temp_img[1].point[y*temp_img[1].width+x] = input_image.point[(y+in_ncc.fy)*input_image.width+x+in_ncc.fx];
			
	Ncc_match_sreach(input_image,temp_img[1],output_image,in_ncc.Ncc_prn_cut); //검색 시작

	UpdateAllViews(0);
}

int CComputerVisionDoc::Ncc_match_sreach(img_str &in_img,img_str &temp_img_,img_str  &out_img,int ncc_cut)
{
	if (in_img.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return -1; } // 예외
	int temp_image_size =temp_img_.height*temp_img_.width; //템플릿 이미지 크기
	img_str temp_out[2]; //결과값 임시 저장용(수직, 수평)

	for(int i = 0; i<2; i++) //임시 이미지 초기화
	{   temp_out[i].channel = 1;
		temp_out[i].height = out_img.height;
		temp_out[i].width = out_img.width;
		temp_out[i].point = new unsigned char[out_img.height*out_img.width];
	}

	float t=0.0 , tt=0.0,t_out; //템플릿 이미지의 데이터를 미리 계산
	for(int y=0;y<temp_img_.height;y++)
		for(int x = 0; x<temp_img_.width; x++)
		{   t+=(float)temp_img_.point[y*temp_img_.width+x];
			tt+=(float)temp_img_.point[y*temp_img_.width+x]*(float)temp_img_.point[y*temp_img_.width+x];
		}
	t_out = temp_image_size*tt-t*t;

	for(int fy=0;fy<input_image.height;fy++) //탐색루프를 통해서 매칭값 계산
		for(int fx = 0; fx<input_image.width; fx++)
		{   float g[2] = {0.0, 0.0};
			float gg[2] = {0.0, 0.0};
			float gt[2] = {0.0, 0.0};
			
			if(temp_img_.height+fy<input_image.height && temp_img_.width+fx<input_image.width) //수직 커패시터 탬플릿
				for(int y = fy; y <temp_img_.height+fy; y++)
					for(int x = fx; x<temp_img_.width+fx; x++)
					{   g[0]+=(float)in_img.point[y*input_image.width+x];
						gg[0]+=(float)in_img.point[y*input_image.width+x]*(float)in_img.point[y*input_image.width+x];
						gt[0]+=(float)in_img.point[y*input_image.width+x]*(float)temp_img_.point[(y-fy)*temp_img_.width+x-fx];
					}
			else g[0]=-1; //이미지의 범위를 벗어남

			if(temp_img_.width+fy<input_image.height && temp_img_.height+fx<input_image.width) //수평 커패시터 탬플릿
				for(int y = fy; y <temp_img_.width+fy; y++)
					for(int x = fx; x<temp_img_.height+fx; x++)
					{   g[1]+=(float)in_img.point[y*input_image.width+x];
						gg[1]+=(float)in_img.point[y*input_image.width+x]*(float)in_img.point[y*input_image.width+x];
						gt[1]+=(float)in_img.point[y*input_image.width+x]*(float)temp_img_.point[(x-fx)*temp_img_.width+y-fy];
					}
			else g[1]=-1;

			float r[2], de_r[2]; //매칭값의 분자와 분모
			int out_score[2]={0,0}; //실제 매칭값
			for(int i = 0; i<2; i++)
			{   if(g[i]<0)
				{   temp_out[i].point[fy*out_img.width+fx] = 0;
					continue;
				}

				r[i] = temp_image_size*gt[i]-g[i]*t;
				de_r[i] = (temp_image_size*gg[i]-g[i]*g[i])*t_out;

				if(r[i]<0 || de_r[i]<0.0001) out_score[i] = 0;
				else out_score[i] = (int)(r[i]*r[i]*255.0/de_r[i]);
				if(ncc_cut < out_score[i]) temp_out[i].point[fy*out_img.width+fx] = 255; // 임시 변수에 각각의 픽셀의 매칭값 저장
				else temp_out[i].point[fy*out_img.width+fx] = 0;
			}
		}

	label label_Capacitor0,label_Capacitor1; //커페시터로 예상되는 위치를 라벨링
	label_Capacitor0.point = NULL; label_Capacitor1.point = NULL;
	Labeling(label_Capacitor0,temp_out[0]); //결과를 전달 받을 구조체 label과 예상 커패시터의 위치가 저장된 임시 이미지 
	Labeling(label_Capacitor1,temp_out[1]);
	trans_img(in_img,out_img); //입력 이미지를 출력 이미지로 복사

	label *label_point=&label_Capacitor0; //라벨을 통해서 커패시터의 위치를 입력 이미지에 사각형으로 나타냄
	int hc=0 , wc=0; //수직, 수평 커패시터의 갯수
	while(label_point!=NULL)
	{   int main_x=0,main_y=0; //사각형의 시작점
		for(int i = 0; i<label_point->quantity; i++)
		{   main_x += label_point->point[2*i];
			main_y += label_point->point[2*i+1];
		}
		main_x /= label_point->quantity;
		main_y /= label_point->quantity;

		prn_rect(out_img,main_x,main_y,temp_img_.height,temp_img_.width,3,255,0,0); //사각형 출력용 함수

		label_point = label_point->next;
		hc++;
	}
	label_point=&label_Capacitor1; //수평 커패시터의 위치를 사각형으로 나타냄
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

	prn_rect(out_img,temp_img_.fx,temp_img_.fy,temp_img_.height,temp_img_.width,3,255,255,0); //템플릿으로 사용한 커패서터의 위치

	UpdateAllViews(0); //결과 출력
	CString msg; 
	msg.Format(_T("수직 : %d , 수평 : %d"), hc,wc);
	AfxMessageBox(msg);
	
	reset_label(label_Capacitor0); //메모리 해제 
	reset_label(label_Capacitor1); //연결 리스트 라벨을 해제하는 함수
	delete [] temp_out[0].point; delete [] temp_out[1].point; //임시 이미지 해제
	return 0;
}

void CComputerVisionDoc::OnTopHat()
{
	// 예외
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	output_image.height = input_image.height; // 출력 이미지 리사이징
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	TopHat(input_image,output_image,30);

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnDownhat()
{
	// 예외
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	output_image.height = input_image.height; // 출력 이미지 리사이징
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	DownHat(input_image,output_image,30);

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnHistogramEqualization()
{
	// 예외
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	output_image.height = input_image.height; // 출력 이미지 리사이징
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	Histogram_Equalization(input_image,output_image);

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnHistogramStretching()
{
	// 예외
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	output_image.height = input_image.height; // 출력 이미지 리사이징
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	Histogram_Stretching(input_image,output_image);

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnLaplacian()
{
	// 예외
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	output_image.height = input_image.height; // 출력 이미지 리사이징
	output_image.width = input_image.width;
	output_image.channel = input_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.point, output_image_size);

	make_laplacian(input_image,output_image);

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}


void CComputerVisionDoc::OnOpening()
{
	// 예외
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	make_Opening(input_image,output_image);

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnClosing()
{
	// 예외
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	make_Closing(input_image,output_image);

	// 모든 뷰 업데이트
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
{//사각형 출력 함수
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
	if (input_image.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 3 ) { AfxMessageBox(_T("컬러 만 가능")); return; }

	// 출력 이미지 리사이징
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
			int r = input_image.point[(y*width+x)*3+2]; //컬러 추출
			int g = input_image.point[(y*width+x)*3+1];
			int b = input_image.point[(y*width+x)*3+0];

			float min = (r>b) ? b : r; //rgb중 최소값 구하기
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
	UpdateAllViews(0); //화면 출력
}


