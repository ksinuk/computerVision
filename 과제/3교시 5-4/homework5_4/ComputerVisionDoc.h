
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
	int GetInput1MFCImage(CImage &image);
	int GetOutputMFCImage(CImage &image);

private:
	void Allocate(unsigned char *&raw_image, int image_size);
	int BMPtoRaw(const CImage &mfc_image, unsigned int &width, unsigned int &height, unsigned int &channel, unsigned char *&raw_image);
	int RawToBMP(const unsigned char *raw_image, unsigned int width, unsigned int height, unsigned int channel, CImage &mfc_image);

private:
	// 이미지
	unsigned int input_width_, input_height_, input_channel_;
	unsigned char *input_image_;

	unsigned int input_width_1, input_height_1, input_channel_1;
	unsigned char *input_image_1;

	unsigned int output_width_, output_height_, output_channel_;
	unsigned char *output_image_;

public:
	// 이벤트 처리기
	afx_msg void OnLoadImage();
	afx_msg void OnSaveImage();
	afx_msg void OnFlagUseResult();
	afx_msg void On04GrayTransform();
	afx_msg void On04ColorInv();
	afx_msg void On04Inv();
	afx_msg void OnMsbXor();
	afx_msg void OnLoadImage1();
};
