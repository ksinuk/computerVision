
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
#include "Input_Mask.h"

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
	ON_COMMAND(ID_binary,&CComputerVisionDoc::Onbinary)
	ON_COMMAND(ID_outline,&CComputerVisionDoc::Onoutline)
	ON_COMMAND(ID_Niblack_binary,&CComputerVisionDoc::OnNiblackbinary)
	ON_COMMAND(ID_histogram_Equal,&CComputerVisionDoc::OnhistogramEqual)
	ON_COMMAND(ID_histogram_Strach,&CComputerVisionDoc::OnhistogramStrach)
	ON_COMMAND(ID_TopHat,&CComputerVisionDoc::OnTophat)
	ON_COMMAND(ID_Erosion,&CComputerVisionDoc::OnErosion)
	ON_COMMAND(ID_Dilation,&CComputerVisionDoc::OnDilation)
	ON_COMMAND(ID_Middle_Filter,&CComputerVisionDoc::OnMiddleFilter)
	ON_COMMAND(ID_opening,&CComputerVisionDoc::Onopening)
	ON_COMMAND(ID_PROJECT,&CComputerVisionDoc::OnProject)
	ON_COMMAND(ID_Hough_Transform,&CComputerVisionDoc::OnHoughTransform)
	ON_COMMAND(ID_Delete_Dot,&CComputerVisionDoc::OnDeleteDot)
	ON_COMMAND(ID_cuting,&CComputerVisionDoc::Oncuting)
	ON_COMMAND(ID_cut_right,&CComputerVisionDoc::Oncutright)
	ON_COMMAND(ID_cut_left,&CComputerVisionDoc::Oncutleft)
	ON_COMMAND(ID_closing,&CComputerVisionDoc::Onclosing)
	ON_COMMAND(ID_DownHat,&CComputerVisionDoc::OnDownhat)
END_MESSAGE_MAP()


// CComputerVisionDoc 생성/소멸

CComputerVisionDoc::CComputerVisionDoc()
{
	reset_str_img(input_image,1);
	reset_str_img(output_image,1);
	reset_str_img(temp_image,1);
	reset_str_img(original_image,1);
	reset_str_img(graph_image,1);
}

CComputerVisionDoc::~CComputerVisionDoc()
{
	if (input_image.data == NULL) delete[] input_image.data;
	if (output_image.data == NULL) delete[] output_image.data;
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
		ar << input_image.width << input_image.height << output_image.channel;
		if (input_image.data != NULL) ar.Write(input_image.data, input_image.width * input_image.height * output_image.channel);
		ar << output_image.width << output_image.height << output_image.channel;
		if (input_image.data != NULL) ar.Write(input_image.data, output_image.width * output_image.height * output_image.channel);
	}
	else
	{
		// 변수 선언 순서대로 불러오기, Read 함수쓰기 전에 할당해야함
		ar >> input_image.width >> input_image.height >> output_image.channel;
		int input_image_size = input_image.width * input_image.height * output_image.channel;
		Allocate(input_image.data, input_image_size);
		if (input_image_size > 0) ar.Read(input_image.data, input_image_size);

		ar >> output_image.width >> output_image.height >> output_image.channel;
		int output_image_size = output_image.width * output_image.height * output_image.channel;
		Allocate(input_image.data, output_image_size);
		if (output_image_size > 0) ar.Read(input_image.data, output_image_size);
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
	if (input_image.data == NULL) return 1;
	RawToBMP(input_image.data, input_image.width, input_image.height, input_image.channel, image);

	return 0;
}

int CComputerVisionDoc::GetOutputMFCImage(CImage &image)
{
	if (input_image.data == NULL) return 1;
	RawToBMP(output_image.data, output_image.width, output_image.height, output_image.channel, image);

	return 0;
}

int CComputerVisionDoc::GetoriginalMFCImage(CImage &image)
{
	if (original_image.data == NULL) return 1;
	RawToBMP(original_image.data, original_image.width, original_image.height, original_image.channel, image);

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
	if (!BMPtoRaw(mfc_image, input_image.width, input_image.height, input_image.channel, input_image.data))
	{
		BMPtoRaw(mfc_image, output_image.width, output_image.height, output_image.channel, output_image.data);
		BMPtoRaw(mfc_image, original_image.width, original_image.height,original_image.channel, original_image.data);
	}

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnSaveImage()
{
	CImage mfc_image;
	if (RawToBMP(output_image.data, output_image.width, output_image.height, output_image.channel, mfc_image)) return;
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
	Allocate(input_image.data, input_image_size);
	memcpy(input_image.data, output_image.data, input_image_size);
	memset(output_image.data, 0, input_image_size);

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04GrayTransform()
{
	// 예외
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (output_image.channel != 3) { AfxMessageBox(_T("3채널 이미지만 그레이로 변환 가능")); return; }

	// 출력 이미지 리사이징
	int input_image_size = input_image.width * input_image.height * input_image.channel;
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = 1;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.data, output_image_size);
	
	// 그레이 변환
	for (int i = 0, j = 0; i < input_image_size; i += 3, ++j)
	{
		output_image.data[j] = (unsigned char)(0.114 * input_image.data[i] + 0.587 * input_image.data[i + 1] + 0.299 * input_image.data[i + 2]);
	}
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04ColorInv()
{
	// 예외
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }

	// 출력 이미지 리사이징
	int input_image_size = input_image.width * input_image.height * output_image.channel;
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = output_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.data, output_image_size);

	// 색상 뒤집기
	int h_size = input_image.height;
	int w_size = input_image.width;
	int c_size = output_image.channel;
	int wc_size = w_size * c_size;
	for (int h = 0, off_h = 0; h < h_size; ++h, off_h += wc_size)
	{
		for (int w = 0, off_w = off_h; w < w_size; ++w, off_w += c_size)
		{
			for (int c = 0, off = off_w; c < c_size; ++c, off += 1)
			{
				output_image.data[off] = (unsigned char)(255 - input_image.data[off]);
			}
		}
	}

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::On04Inv()
{
	// 예외
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }

	// 출력 이미지 리사이징
	int input_image_size = input_image.width * input_image.height * output_image.channel;
	output_image.height = input_image.height;
	output_image.width = input_image.width;
	output_image.channel = output_image.channel;
	int output_image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.data, output_image_size);

	// 뒤집기
	int h_size = input_image.height;
	int wc_size = input_image.width * output_image.channel;
	for (int h = 0, off = 0, off_inv = input_image_size - wc_size; h < h_size; ++h, off += wc_size, off_inv -= wc_size)
	{
		memcpy(&output_image.data[off_inv], &input_image.data[off], wc_size);
	}
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

int CComputerVisionDoc::reset_str_img(str_img& img,int first) //이미지 초기화
{
	img.channel=0;
	img.width=0;
	img.height=0;
	img.fx=0;
	img.fy=0;
	if(first==0 && img.data!=NULL )delete [] img.data;
	img.data=NULL;

	return 0;
}

int CComputerVisionDoc::re_size_img(str_img& in_img,str_img& out_img) //이미지 리사이징
{
	out_img.channel=in_img.channel;
	out_img.width=in_img.width;
	out_img.height=in_img.height;
	out_img.fx=in_img.fx;
	out_img.fy=in_img.fy;
	int image_size = out_img.height * out_img.width * out_img.channel;
	Allocate(out_img.data, image_size);

	for(int y=0;y<out_img.height;y++) //이미지의 픽셀값을 0으로 초기화 한다.
		for(int x=0;x<out_img.width;x++)
			out_img.data[y*out_img.width+x] = 0;

	return 0;
}

int CComputerVisionDoc::ctrlCV_img(str_img& in_img,str_img& out_img) //이미지 복사
{
	out_img.channel=in_img.channel;
	out_img.width=in_img.width;
	out_img.height=in_img.height;
	out_img.fx=in_img.fx;
	out_img.fy=in_img.fy;
	int image_size = out_img.height * out_img.width * out_img.channel;
	Allocate(out_img.data, image_size);

	for(int y=0;y<out_img.height;y++)
		for(int x=0;x<out_img.width;x++)
			out_img.data[y*out_img.width+x] = in_img.data[y*in_img.width+x];

	return 0;
}

void CComputerVisionDoc::OnProject() //프로젝트용 이벤트 함수
{
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	str_img cut_right, cut_left, temp1_left, temp2_left, temp1_right, temp2_right , temp_hough;
	reset_str_img(cut_right,1); //입력이미지의 오른쪽 아래 부분 초기화
	reset_str_img(cut_left,1); //입력 이미지의 왼쪽 아래 부분 초기화
	reset_str_img(temp1_right,1); //오른쪽 이미지 처리용 이미지 초기화
	reset_str_img(temp2_right,1); //오른쪽 이미지 처리용 이미지 초기화
	reset_str_img(temp1_left,1); //왼쪽 이미지 처리용 이미지 초기화
	reset_str_img(temp2_left,1); //왼쪽 이미지 처리용 이미지 초기화
	reset_str_img(temp_hough,1); //허프 연산 처리용 이미지 초기화

	cut_right.channel = 1; //오른쪽 아래 이미지 생성
	cut_right.fx = input_image.width/2;
	cut_right.fy = 240;
	cut_right.height = input_image.height-30-cut_right.fy;
	cut_right.width = (input_image.width - 40)/2;
	cut_right.data = NULL;
	Allocate(cut_right.data, cut_right.width*cut_right.height*cut_right.channel);
	for(int y = 0; y<cut_right.height; y++) //입력 이미지로 부터 오른쪽 아래 이미지 입력 받음
		for(int x = 0; x<cut_right.width; x++)
			cut_right.data[y*cut_right.width+x] = input_image.data[(y+cut_right.fy)*input_image.width+x+cut_right.fx];

	re_size_img(cut_right,cut_left); //왼쪽 아래 이미지 생성
	cut_left.fx = 20;
	for(int y = 0; y<cut_left.height; y++)
		for(int x = 0; x<cut_left.width; x++)
			cut_left.data[y*cut_left.width+x] = input_image.data[(y+cut_left.fy)*input_image.width+x+cut_left.fx];

	
	re_size_img(cut_right,temp1_right);// 임시 처리 이미지 리사이징
	re_size_img(cut_left,temp1_left);
	re_size_img(cut_right,temp2_right);
	re_size_img(cut_left,temp2_left);
	
	temp_hough.channel = 1; //허프 처리용 이미지 생성
	temp_hough.height = cut_right.height;
	temp_hough.width = (input_image.width - 40);
	temp_hough.fx = 20;
	temp_hough.fy = cut_right.fy;
	temp_hough.data = NULL;
	Allocate(temp_hough.data, temp_hough.width*temp_hough.height*temp_hough.channel);

	/**************** 외곽선 추출 **************/
	make_Middle_filter(cut_right,temp1_right,1,1); //중간값 필터
	make_Middle_filter(cut_left,temp1_left,1,1);
	
	make_TopHat(temp1_left,temp1_left,10,10); //top-hat 연산
	make_TopHat(temp1_right,temp1_right,10,10);

	make_histogram_Strach(cut_right,temp2_right); //스트레치 연산
	make_histogram_Strach(cut_left,temp2_left);

	for(int y=0;y<temp1_left.height;y++) //top-hat 연산의 결과물과 스트레치 연산의 결과물 중에서 작은 픽셀의 값을 사용한다.
		for(int x = 0; x<temp1_left.width; x++)
		{
			int r1 = temp1_right.data[y*temp1_left.width+x];
			int r2 = temp2_right.data[y*temp1_left.width+x];
			int l1 = temp1_left.data[y*temp1_left.width+x];
			int l2 = temp2_left.data[y*temp1_left.width+x];
			temp1_left.data[y*temp1_left.width+x] = (l1 < l2) ? l1 : l2;
			temp1_right.data[y*temp1_left.width+x] = (r1 < r2) ? r1 : r2;
		}
	
	make_histogram_Strach(temp1_left,temp1_left); //다시 스트레치 연산
	make_histogram_Strach(temp1_right,temp1_right);
	
	int histogram[256]; //임시 이미지를 이진화
	make_histogram(histogram,temp1_left);
	int bin_cut = search_Otzu(histogram);
	make_binary_img(bin_cut,temp1_left,temp1_left);
	make_histogram(histogram,temp1_right);
	bin_cut = search_Otzu(histogram);
	make_binary_img(bin_cut,temp1_right,temp1_right);

	for(int y = 0; y<temp1_left.height; y++) //왼쪽 이미지와 오른쪽 이미지를 하나로 통합
		for(int x = 0; x<temp1_left.width; x++)
		{
			int out = temp1_left.data[y*temp1_left.width+x];
			temp_hough.data[(y+temp1_left.fy-temp_hough.fy)*temp_hough.width+x+temp1_left.fx-temp_hough.fx] = out;
		}
	for(int y = 0; y<temp1_right.height; y++)
		for(int x = 0; x<temp1_right.width; x++)
		{
			int out = temp1_right.data[y*temp1_right.width+x];
			temp_hough.data[(y+temp1_right.fy-temp_hough.fy)*temp_hough.width+x+temp1_right.fx-temp_hough.fx] = out;
		}

	sobel_edge(temp_hough,temp_hough); //외곽선 추출

	make_histogram(histogram,temp_hough); //외곽선 이미지를 이진화
	bin_cut = search_Otzu(histogram);
	make_binary_img(bin_cut,temp_hough,temp_hough);

	delete_dot(temp_hough,temp_hough,30); //작은 이진 이미지 삭제

	/************ 허프 변환 ************/
	make_Hough_Transform(temp_hough,graph_image,temp_hough); //허프 변환으로 차선 찾기

	output_image.channel=3; //컬러 출력 이미지 만들기
	output_image.width=input_image.width;
	output_image.height=input_image.height;
	output_image.fx=input_image.fx;
	output_image.fy=input_image.fy;
	int image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.data, image_size);

	for(int y=0;y<output_image.height;y++) //먼저 입력 이미지에서 출력 이미지로 복사
		for(int x=0;x<output_image.width;x++)
		{
			output_image.data[(y*output_image.width+x)*3] = input_image.data[y*output_image.width+x];
			output_image.data[(y*output_image.width+x)*3+1] = input_image.data[y*output_image.width+x];
			output_image.data[(y*output_image.width+x)*3+2] = input_image.data[y*output_image.width+x];
		}
	for(int y=0;y<temp_hough.height;y++) //차선을 빨간색으로 출력 이미지에 입력
		for(int x=0;x<temp_hough.width;x++)
		{
			if(temp_hough.data[y*temp_hough.width+x]!=0)
			{
				int out_y = y+temp_hough.fy;
				int out_x = x+temp_hough.fx;

				int fy = (out_y-1<0) ? 0 : out_y-1;
				int ly = (out_y+1>=output_image.height) ? output_image.height-1 : out_y+1;
				int fx = (out_x-1<0) ? 0 : out_x-1;
				int lx = (out_x+1>=output_image.width) ? output_image.width-1 : out_x+1;

				for(int i=fy;i<=ly;i++)
					for(int j = fx; j<=lx; j++)
					{
						output_image.data[(i*output_image.width+j)*3] = 0;
						output_image.data[(i*output_image.width+j)*3+1] = 0;
						output_image.data[(i*output_image.width+j)*3+2] = 255;
					}
			}
		}


	/************** 힙 해제 및 출력 ***************/
	delete [] cut_right.data;
	delete [] cut_left.data;
	delete [] temp1_left.data;
	delete [] temp1_right.data;
	delete [] temp2_left.data;
	delete [] temp2_right.data;
	delete [] temp_hough.data;
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

/*********************************************************************************************/
/************************** 이하 테스트용 이벤트 함수 ****************************************/
/*********************************************************************************************/

void CComputerVisionDoc::Onbinary()
{
	// 예외
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (output_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	// 출력 이미지 리사이징
	re_size_img(input_image,output_image);

	//이진화
	int histogram[256];
	make_histogram(histogram,input_image);
	int bin_cut = search_Otzu(histogram);
	make_binary_img(bin_cut,input_image,output_image);
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::Onoutline()
{
	// 예외
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지 Onoutline")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능 Onoutline")); return; }

	// 출력 이미지 리사이징
	re_size_img(input_image,output_image);

	//이진화
	sobel_edge(input_image,output_image);
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnNiblackbinary()
{
	// 예외
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	// 출력 이미지 리사이징
	re_size_img(input_image,output_image);

	binary_niblack(input_image,output_image);
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);



}

void CComputerVisionDoc::OnhistogramEqual()
{
	// 예외
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	// 출력 이미지 리사이징
	re_size_img(input_image,output_image);

	make_histogram_Equal(input_image,output_image);
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnhistogramStrach()
{
	// 예외
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	// 출력 이미지 리사이징
	re_size_img(input_image,output_image);

	make_histogram_Strach(input_image,output_image);
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnTophat()
{
	// 예외
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	// 출력 이미지 리사이징
	re_size_img(input_image,output_image);

	Input_Mask data;
	data.mask_h=10;
	data.mask_w=10;
	data.DoModal();

	make_TopHat(input_image , output_image , data.mask_h , data.mask_w);
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::OnDownhat()
{
	// 예외
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	// 출력 이미지 리사이징
	re_size_img(input_image,output_image);

	Input_Mask data;
	data.mask_h=10;
	data.mask_w=10;
	data.DoModal();

	make_DownHat(input_image , output_image , data.mask_h , data.mask_w);
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}


void CComputerVisionDoc::OnErosion()
{
	// 예외
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	Input_Mask data;
	data.mask_h=1;
	data.mask_w=1;
	data.DoModal();

	// 출력 이미지 리사이징
	re_size_img(input_image,output_image);

	make_Erosion(input_image,output_image,data.mask_h,data.mask_w);
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);

}


void CComputerVisionDoc::OnDilation()
{
	// 예외
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	Input_Mask data;
	data.mask_h=1;
	data.mask_w=1;
	data.DoModal();

	// 출력 이미지 리사이징
	re_size_img(input_image,output_image);

	make_Dilation(input_image,output_image,data.mask_h,data.mask_w);
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}


void CComputerVisionDoc::OnMiddleFilter()
{
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	Input_Mask data;
	data.mask_h=1;
	data.mask_w=1;
	data.DoModal();

	// 출력 이미지 리사이징
	re_size_img(input_image,output_image);

	make_Middle_filter(input_image,output_image,data.mask_h,data.mask_w);
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}


void CComputerVisionDoc::Onopening()
{
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	Input_Mask data;
	data.mask_h=1;
	data.mask_w=1;
	data.DoModal();

	// 출력 이미지 리사이징
	re_size_img(input_image,output_image);

	make_opening(input_image,output_image,data.mask_h,data.mask_w);
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::Onclosing()
{
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	Input_Mask data;
	data.mask_h=1;
	data.mask_w=1;
	data.DoModal();

	// 출력 이미지 리사이징
	re_size_img(input_image,output_image);

	make_closing(input_image,output_image,data.mask_h,data.mask_w);
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}




void CComputerVisionDoc::OnHoughTransform()
{
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	str_img hough;
	reset_str_img(hough,1);
	re_size_img(input_image,hough);
	// 출력 이미지 리사이징
	output_image.channel=3;
	output_image.width=input_image.width;
	output_image.height=input_image.height;
	output_image.fx=input_image.fx;
	output_image.fy=input_image.fy;
	int image_size = output_image.height * output_image.width * output_image.channel;
	Allocate(output_image.data, image_size);

	make_Hough_Transform(input_image,graph_image,hough);

	for(int y=0;y<output_image.height;y++)
		for(int x=0;x<output_image.width;x++)
		{
			int yy = 0;
			output_image.data[(y*output_image.width+x)*3] = input_image.data[y*input_image.width+x] & (255-hough.data[y*input_image.width+x]);
			output_image.data[(y*output_image.width+x)*3+1] = input_image.data[y*input_image.width+x] & (255-hough.data[y*input_image.width+x]);
			output_image.data[(y*output_image.width+x)*3+2] = input_image.data[y*input_image.width+x] | hough.data[y*input_image.width+x];
		}
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}


void CComputerVisionDoc::OnDeleteDot()
{
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	// 출력 이미지 리사이징
	re_size_img(input_image,output_image);

	delete_dot(input_image,output_image,20);
	
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}


void CComputerVisionDoc::Oncuting()
{
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	output_image.channel = 1;
	output_image.height = (input_image.height-60)/2;
	output_image.width = input_image.width - 40;
	output_image.fx = 20;
	output_image.fy = input_image.height/2;
	output_image.data = NULL;
	Allocate(output_image.data, output_image.width*output_image.height*output_image.channel);

	for(int y=0;y<output_image.height;y++)
		for(int x=0;x<output_image.width;x++)
			output_image.data[y*output_image.width+x] = input_image.data[(y+output_image.fy)*input_image.width+x+output_image.fx];

	make_Middle_filter(output_image,output_image,1,1);

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}


void CComputerVisionDoc::Oncutright()
{
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	output_image.channel = 1;
	output_image.height = (input_image.height-60)/2;
	output_image.width = (input_image.width - 40)/2;
	output_image.fx = input_image.width/2;
	output_image.fy = input_image.height/2;
	output_image.data = NULL;
	Allocate(output_image.data, output_image.width*output_image.height*output_image.channel);

	for(int y=0;y<output_image.height;y++)
		for(int x=0;x<output_image.width;x++)
			output_image.data[y*output_image.width+x] = input_image.data[(y+output_image.fy)*input_image.width+x+output_image.fx];

	make_Middle_filter(output_image,output_image,1,1);

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}


void CComputerVisionDoc::Oncutleft()
{
	if (input_image.data == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (input_image.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능")); return; }

	output_image.channel = 1;
	output_image.height = (input_image.height-60)/2;
	output_image.width = (input_image.width - 40)/2;
	output_image.fx = 20;
	output_image.fy = input_image.height/2;
	output_image.data = NULL;
	Allocate(output_image.data, output_image.width*output_image.height*output_image.channel);

	for(int y=0;y<output_image.height;y++)
		for(int x=0;x<output_image.width;x++)
			output_image.data[y*output_image.width+x] = input_image.data[(y+output_image.fy)*input_image.width+x+output_image.fx];

	make_Middle_filter(output_image,output_image,1,1);

	// 모든 뷰 업데이트
	UpdateAllViews(0);
}






