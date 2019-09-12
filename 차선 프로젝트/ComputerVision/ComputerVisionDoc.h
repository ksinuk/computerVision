
// ComputerVisionDoc.h : CComputerVisionDoc 클래스의 인터페이스
//


#pragma once


class CComputerVisionDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CComputerVisionDoc();
	DECLARE_DYNCREATE(CComputerVisionDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CComputerVisionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS


	// 사용자 정의 함수
public:
	int GetInputMFCImage(CImage &image);
	int GetOutputMFCImage(CImage &image);
	int GetoriginalMFCImage(CImage &image);

private:
	void Allocate(unsigned char *&raw_image, int image_size);
	int BMPtoRaw(const CImage &mfc_image, unsigned int &width, unsigned int &height, unsigned int &channel, unsigned char *&raw_image);
	int RawToBMP(const unsigned char *raw_image, unsigned int width, unsigned int height, unsigned int channel, CImage &mfc_image);

private:
	struct str_img
	{
		unsigned int width, height, channel, fx, fy;
		unsigned char *data;
	}output_image,input_image,temp_image,original_image,graph_image;

public:
	// 이벤트 처리기
	afx_msg void OnLoadImage();
	afx_msg void OnSaveImage();
	afx_msg void OnFlagUseResult();
	afx_msg void On04GrayTransform();
	afx_msg void On04ColorInv();
	afx_msg void On04Inv();
	afx_msg void Onbinary();
	afx_msg void Onoutline();
	afx_msg void OnNiblackbinary();
	afx_msg void OnhistogramEqual();
	afx_msg void OnTophat();
	afx_msg void OnErosion();
	afx_msg void OnDilation();
	afx_msg void OnMiddleFilter();
	afx_msg void OnhistogramStrach();
	afx_msg void Onopening();
	afx_msg void OnHoughTransform();
	afx_msg void OnDeleteDot();
	afx_msg void Oncuting();
	afx_msg void Oncutright();
	afx_msg void Oncutleft();
	afx_msg void Onclosing();
	afx_msg void OnDownhat();


	afx_msg void OnProject(); //프로젝트용 이벤트 함수

	int reset_str_img(str_img& img,int first=0); //이미지 초기화
	int re_size_img(str_img& in_img,str_img& out_img); //이미지 리사이징
	int ctrlCV_img(str_img& in_img,str_img& out_img); //이미지 복사
	
	
	//histogramDoc.cpp
	int make_histogram(int* histogram,str_img &func_image_); // 히스토그램 계산
	int search_Otzu(int* histogram); // Otzu 알고리즘을 이용한 최적값 계산
	int make_binary_img(int bin_cut,str_img &input_image_,str_img &out_img); //기준값을 받아서 이미지 이진화 
	int binary_niblack(str_img &in_img,str_img &out_img); //적응 이치화 계산
	int make_histogram_Equal(str_img & in_img,str_img & out_img); //히스토그램 평활화 계산
	int make_histogram_Strach(str_img & in_img,str_img & out_img); //히스토그램 스크레치 계산
	
	//MorphologyDoc.cpp
	int make_TopHat(str_img &in_img, str_img &out_img,int mask_h,int mask_w); //Top - Hat 연산
	int make_DownHat(str_img &input_func,str_img &output_func,int mask_h,int mask_w); // Down Hat 연산
	int make_opening(str_img &in_img, str_img &out_img,int mask_h,int mask_w); //opening 연산
	int make_closing(str_img &in_img, str_img &out_img,int mask_h,int mask_w); //closing 연산
	int make_Erosion(str_img &in_img, str_img &out_img,int mask_h,int mask_w); //침식 연산 
	int make_Dilation(str_img &in_img, str_img &out_img,int mask_h,int mask_w); //팽창 연산
	

	//maskDoc.cpp
	int sobel_edge(str_img &in_img,str_img &out_img); //외곽선 추출용 소벨 엣지 함수
	int make_Middle_filter(str_img &in_img,str_img &out_img,int mask_h,int mask_w); //중간값 필터
	int delete_dot(str_img &in_img, str_img &out_img, int cut); //기준보다 작은 크기의 이진 이미지 제거

	//HoughDoc.cpp
	int make_Hough_Transform(str_img &inimg,str_img &graph,str_img &outimg); //차선 검출용 허프 변환 함수
};
