
// ComputerVisionDoc.h : CComputerVisionDoc Ŭ������ �������̽�
//


#pragma once


class CComputerVisionDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CComputerVisionDoc();
	DECLARE_DYNCREATE(CComputerVisionDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CComputerVisionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS


	// ����� ���� �Լ�
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
	// �̺�Ʈ ó����
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


	afx_msg void OnProject(); //������Ʈ�� �̺�Ʈ �Լ�

	int reset_str_img(str_img& img,int first=0); //�̹��� �ʱ�ȭ
	int re_size_img(str_img& in_img,str_img& out_img); //�̹��� ������¡
	int ctrlCV_img(str_img& in_img,str_img& out_img); //�̹��� ����
	
	
	//histogramDoc.cpp
	int make_histogram(int* histogram,str_img &func_image_); // ������׷� ���
	int search_Otzu(int* histogram); // Otzu �˰����� �̿��� ������ ���
	int make_binary_img(int bin_cut,str_img &input_image_,str_img &out_img); //���ذ��� �޾Ƽ� �̹��� ����ȭ 
	int binary_niblack(str_img &in_img,str_img &out_img); //���� ��ġȭ ���
	int make_histogram_Equal(str_img & in_img,str_img & out_img); //������׷� ��Ȱȭ ���
	int make_histogram_Strach(str_img & in_img,str_img & out_img); //������׷� ��ũ��ġ ���
	
	//MorphologyDoc.cpp
	int make_TopHat(str_img &in_img, str_img &out_img,int mask_h,int mask_w); //Top - Hat ����
	int make_DownHat(str_img &input_func,str_img &output_func,int mask_h,int mask_w); // Down Hat ����
	int make_opening(str_img &in_img, str_img &out_img,int mask_h,int mask_w); //opening ����
	int make_closing(str_img &in_img, str_img &out_img,int mask_h,int mask_w); //closing ����
	int make_Erosion(str_img &in_img, str_img &out_img,int mask_h,int mask_w); //ħ�� ���� 
	int make_Dilation(str_img &in_img, str_img &out_img,int mask_h,int mask_w); //��â ����
	

	//maskDoc.cpp
	int sobel_edge(str_img &in_img,str_img &out_img); //�ܰ��� ����� �Һ� ���� �Լ�
	int make_Middle_filter(str_img &in_img,str_img &out_img,int mask_h,int mask_w); //�߰��� ����
	int delete_dot(str_img &in_img, str_img &out_img, int cut); //���غ��� ���� ũ���� ���� �̹��� ����

	//HoughDoc.cpp
	int make_Hough_Transform(str_img &inimg,str_img &graph,str_img &outimg); //���� ����� ���� ��ȯ �Լ�
};
