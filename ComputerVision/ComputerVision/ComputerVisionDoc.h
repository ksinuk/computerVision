
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
	int GetTempMFCImage(CImage &image,int n);
	int GetHistogrmMFCImage(CImage &image , int color);

private:
	void Allocate(unsigned char *&raw_image, int image_size);
	int BMPtoRaw(const CImage &mfc_image, unsigned int &width, unsigned int &height, unsigned int &channel, unsigned char *&raw_image);
	int RawToBMP(const unsigned char *raw_image, unsigned int width, unsigned int height, unsigned int channel, CImage &mfc_image);

private:
	// 이미지
	/*unsigned int input_width_, input_height_, input_channel_;
	unsigned char *input_image_;
	unsigned int output_width_, output_height_, output_channel_;
	unsigned char *output_image_;
	unsigned int temp_width_, temp_height_, temp_channel_;
	unsigned char *temp_image_;*/

	int histogram[3][256];
	unsigned char *out_color_histogram[3];

	int temp_number_ = 20;
	struct img_str //이미지용 구조체
	{
		unsigned int width, height, channel,fx,fy;
		unsigned char *point;
	}temp_img[20],input_image,output_image;
	
	struct label //라벨용 구조체
	{
		int num; //라벨 넘버
		int quantity; //픽셀의 양
		unsigned char *point;//픽셀의 좌표가 저장되는 곳
		struct label *next; //다음 라벨
		struct label *prev; //이전 라벨
	}label_first;

	

public:
	int func_h;

	// 이벤트 처리기
	afx_msg void OnLoadImage();
	afx_msg void OnSaveImage();
	afx_msg void OnFlagUseResult();
	afx_msg void On04GrayTransform();
	afx_msg void On04ColorInv();
	afx_msg void On04Inv();
	afx_msg void OnMiddlefilter();
	afx_msg void Onx2();
	afx_msg void Onplus50();
	afx_msg void Ondeg45();
	afx_msg void Onhistogram();
	afx_msg void OnHistoOtzu();
	afx_msg void OnErosion();
	afx_msg void OnDilation();
	afx_msg void OnHowManyCoin();
	afx_msg void OnTopHat();
	afx_msg void OnOpening();
	afx_msg void OnSearchCapacitor();
	afx_msg void OnDownhat();
	afx_msg void OnHistogramEqualization();
	afx_msg void OnHistogramStretching();
	afx_msg void OnLaplacian();
	afx_msg void OnClosing();
	afx_msg void OnHsiColor();

	void make_histogram(img_str &in_img,int color=0);

	

	void Otzu_thresholding();
	
	int Labeling(label &label_out,img_str &out_img,int cut_size=0);
	int Label_list_painting_coin();
	int Label_Painting(label input,unsigned char red,unsigned char green,unsigned char blue);
	int is_it_hole_coin(label input);
	void out_img_reset(unsigned char red,unsigned char green=0,unsigned char blue=0);
	void TopHat(img_str &in_img,img_str &out_img,int mask_n=30);
	void DownHat(img_str &in_img, img_str &out_img,int mask_n=30);
	
	int Ncc_match_sreach(img_str &in_img,img_str &temp_img,img_str &out_img,int ncc_cut);
	
	void make_Opening(img_str &in_img, img_str &out_img,int mask_n=30);
	void make_Closing(img_str &in_img, img_str &out_img,int mask_n=30);
	
	int Histogram_Equalization(img_str &in_img,img_str &out_img);
	int Histogram_Stretching(img_str &in_img,img_str &out_img);
	
	void make_laplacian(img_str &in_img, img_str &out_img);
	int make_middle_filiter(img_str &in_img,img_str &out_img,int n=1);
	int reset_label(label & first);
	int trans_img(img_str & in_img,img_str & out_img);
	int prn_rect(img_str & background,int fx,int fy,int height,int width,int n=1,int red=255,int green=255,int blue=255);
	
};
