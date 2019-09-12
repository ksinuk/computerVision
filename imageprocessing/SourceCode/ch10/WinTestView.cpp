// WinTestView.cpp : implementation of the CWinTestView class
//

#include "stdafx.h"
#include "WinTest.h"

#include "WinTestDoc.h"
#include "WinTestView.h"

#include "BinCntrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CWinTestView

IMPLEMENT_DYNCREATE(CWinTestView, CView)

BEGIN_MESSAGE_MAP(CWinTestView, CView)
	//{{AFX_MSG_MAP(CWinTestView)
	ON_COMMAND(IDM_CONST_ADD, OnConstAdd)
	ON_COMMAND(IDM_CONST_SUB, OnConstSub)
	ON_COMMAND(IDM_CONST_MUL, OnConstMul)
	ON_COMMAND(IDM_CONST_DIV, OnConstDiv)
	ON_COMMAND(IDM_FRM_ADD, OnFrmAdd)
	ON_COMMAND(IDM_FRM_SUB, OnFrmSub)
	ON_COMMAND(IDM_FRM_MUL, OnFrmMul)
	ON_COMMAND(IDM_LUT_MUL, OnLutMul)
	ON_COMMAND(IDM_BINARIZATION, OnBinarization)
	ON_COMMAND(IDM_IMGHISTO, OnImghisto)
	ON_COMMAND(IDM_BIN_DYNAMIC, OnBinDynamic)
	ON_COMMAND(IDM_HISTO_EQUAL, OnHistoEqual)
	ON_COMMAND(IDM_HISTO_STRETCH, OnHistoStretch)
	ON_COMMAND(IDM_HISTO_UPSTRETCH, OnHistoUpstretch)
	ON_COMMAND(IDM_HISTO_SPEC_FILEIN, OnHistoSpecFilein)
	ON_COMMAND(IDM_HISTO_SPEC_CAL, OnHistoSpecCal)
	ON_COMMAND(IDM_SMOOTHING_BOX, OnSmoothingBox)
	ON_COMMAND(IDM_SMOOTHING_GAUSSIAN, OnSmoothingGaussian)
	ON_COMMAND(IDM_SHARPENING_LAPLACIAN, OnSharpeningLaplacian)
	ON_COMMAND(IDM_EDGE_PREWITT, OnEdgePrewitt)
	ON_COMMAND(IDM_EDGE_SOBEL, OnEdgeSobel)
	ON_COMMAND(IDM_ZOOMOUT, OnZoomout)
	ON_COMMAND(IDM_ZOOMIN, OnZoomin)
	ON_COMMAND(IDM_ROTATION, OnRotation)
	ON_COMMAND(IDM_2D_FFT, On2dFft)
	ON_COMMAND(IDM_MORPHOLOGY_BINARY_DILATION, OnMorphologyBinaryDilation)
	ON_COMMAND(IDM_MORPHOLOGY_BINARY_EROSION, OnMorphologyBinaryErosion)
	ON_COMMAND(IDM_MORPHOLOGY_GRAY_DILATION, OnMorphologyGrayDilation)
	ON_COMMAND(IDM_MORPHOLOGY_GRAY_EROSION, OnMorphologyGrayErosion)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinTestView construction/destruction

CWinTestView::CWinTestView()
{
	// TODO: add construction code here
	height = width = 256;
	int rwsize = (((width)+31)/32*4);  // 4바이트의 배수여야 함
	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO)+256*sizeof(RGBQUAD));

	BmInfo->bmiHeader.biBitCount=8; 
	BmInfo->bmiHeader.biClrImportant=256;
	BmInfo->bmiHeader.biClrUsed=256;
	BmInfo->bmiHeader.biCompression=0;
	BmInfo->bmiHeader.biHeight = height; 
	BmInfo->bmiHeader.biPlanes=1;
	BmInfo->bmiHeader.biSize=40;
	BmInfo->bmiHeader.biSizeImage=rwsize*height; 
	BmInfo->bmiHeader.biWidth =width; 
	BmInfo->bmiHeader.biXPelsPerMeter=0;
	BmInfo->bmiHeader.biYPelsPerMeter=0;
	for(int i=0; i<256; i++) // Palette number is 256
	{
		BmInfo->bmiColors[i].rgbRed= BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbBlue = i; 
		BmInfo->bmiColors[i].rgbReserved = 0;
	}
}

CWinTestView::~CWinTestView()
{
	free(BmInfo);
}

BOOL CWinTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWinTestView drawing

void CWinTestView::OnDraw(CDC* pDC)
{
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	for(int i=0; i<height; i++)
		for(int j=0; j<width; j++) m_RevImg[i][j]=pDoc->m_InImg[height-i-1][j];

	SetDIBitsToDevice(pDC->GetSafeHdc(),0,0,width,height,
						0,0,0,height,m_RevImg,BmInfo, DIB_RGB_COLORS);

	for(i=0; i<height; i++)
		for(int j=0; j<width; j++) m_RevImg[i][j]=pDoc->m_OutImg[height-i-1][j];

	SetDIBitsToDevice(pDC->GetSafeHdc(),300,0,width,height,
						0,0,0,height,m_RevImg,BmInfo, DIB_RGB_COLORS);

//	for(int i=0; i<256; i++)
//	{
//		for(int j=0; j<256; j++)
//		{
//			unsigned char InVal= pDoc->m_InImg[i][j];
//			unsigned char OutVal= pDoc->m_OutImg[i][j];

//			pDC->SetPixel(j,i,RGB(InVal,InVal,InVal));
//			pDC->SetPixel(j+300,i,RGB(OutVal,OutVal,OutVal));
//		}
//	}
}

/////////////////////////////////////////////////////////////////////////////
// CWinTestView printing

BOOL CWinTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWinTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWinTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWinTestView diagnostics

#ifdef _DEBUG
void CWinTestView::AssertValid() const
{
	CView::AssertValid();
}

void CWinTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinTestDoc* CWinTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinTestDoc)));
	return (CWinTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinTestView message handlers



void CWinTestView::OnConstAdd() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int tempVal = pDoc->m_InImg[i][j]+60;
			tempVal = tempVal > 255 ? 255: tempVal;
			tempVal = tempVal <   0 ?   0: tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}

	Invalidate(FALSE); //화면 갱신 
}

void CWinTestView::OnConstSub() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int tempVal = pDoc->m_InImg[i][j]-60;
			tempVal = tempVal > 255 ? 255: tempVal;
			tempVal = tempVal <   0 ?   0: tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}

	Invalidate(FALSE); //화면 갱신 
}

void CWinTestView::OnConstMul() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int tempVal = (int)(pDoc->m_InImg[i][j]*1.4);
			tempVal = tempVal > 255 ? 255: tempVal;
			tempVal = tempVal <   0 ?   0: tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}

	Invalidate(FALSE); //화면 갱신 
}

void CWinTestView::OnConstDiv() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int tempVal = (int)(pDoc->m_InImg[i][j]/1.4);
			tempVal = tempVal > 255 ? 255: tempVal;
			tempVal = tempVal <   0 ?   0: tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}

	Invalidate(FALSE); //화면 갱신 
}

void CWinTestView::OnFrmAdd() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->TwoImgLoad();					// 빼기 연산을 할 두 영상을 입력한다.
	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int tempVal = pDoc->m_InImg1[i][j]+pDoc->m_InImg2[i][j];
			tempVal = tempVal > 255 ? 255: tempVal;
			tempVal = tempVal <   0 ?   0: tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}

	Invalidate(FALSE); //화면 갱신 
}

void CWinTestView::OnFrmSub() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->TwoImgLoad();					// 빼기 연산을 할 두 영상을 입력한다.
	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int tempVal = pDoc->m_InImg1[i][j]-pDoc->m_InImg2[i][j];
			tempVal = tempVal > 255 ? 255: tempVal;
			tempVal = tempVal <   0 ?   0: tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}

	Invalidate(FALSE); //화면 갱신 
}

void CWinTestView::OnFrmMul() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->TwoImgLoad();					// 빼기 연산을 할 두 영상을 입력한다.
	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			int tempVal = pDoc->m_InImg1[i][j] & pDoc->m_InImg2[i][j];
			tempVal = tempVal > 255 ? 255: tempVal;
			tempVal = tempVal <   0 ?   0: tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}

	Invalidate(FALSE); //화면 갱신 
}

void CWinTestView::OnLutMul() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	unsigned char LUT[256]; // LUT로 사용할 메모리를 선언 

	// LUT값을 계산한다 
	for(int i=0; i<256; i++) 
	{
		int temp=(int)((float)i*1.4);
		LUT[i] = temp > 255 ? 255 : temp;
	}

	// LUT를 통하여 영상을 처리한다
	for(i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			pDoc->m_OutImg[i][j]=LUT[pDoc->m_InImg[i][j]];
		}
	}

	Invalidate(FALSE); //화면 갱신 
}


void CWinTestView::OnImghisto() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	pDoc->m_ImgHisto(256,256);		// 히스토그램 계산 함수 호출 
	Invalidate(FALSE); //화면 갱신 	
}

void CWinTestView::OnBinarization() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++)
		{
			if(pDoc->m_InImg[i][j]>100) pDoc->m_OutImg[i][j]=255;
			else pDoc->m_OutImg[i][j]=0;
		}
	}

	Invalidate(FALSE); //화면 갱신 	
}



void CWinTestView::OnBinDynamic() 
{
	// TODO: Add your command handler code here
	CBinCntrlDlg pbinCtrlDlg;
	pbinCtrlDlg.DoModal();	
}

void CWinTestView::OnHistoEqual() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	// 도큐멘트 클래스에 있는 히스토그램 평활화 함수 호출
	pDoc->m_HistoEqual(256,256);  

	Invalidate(FALSE); //화면 갱신 	
}

void CWinTestView::OnHistoStretch() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	// 도큐멘트 클래스에 있는 히스토그램 평활화 함수 호출
	pDoc->m_HistoStretch(256,256);  

	Invalidate(FALSE); //화면 갱신 		
}

void CWinTestView::OnHistoUpstretch() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	// 도큐멘트 클래스에 있는 개선된 히스토그램 스트레칭함수 호출
	pDoc->m_HistoUpStretch(256,256,20,20);  

	Invalidate(FALSE); //화면 갱신 			
}

void CWinTestView::OnHistoSpecFilein() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	// 도큐멘트 클래스에 있는 히스토그램지정을 위한 영상입력함수 호출
	pDoc->m_HistoSpec_FileIn();  
}

void CWinTestView::OnHistoSpecCal() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 

	// 도큐멘트 클래스에 있는 히스토그램지정 연산함수 호출
	pDoc->m_HistoSpec(256, 256);  

	Invalidate(FALSE); //화면 갱신 			
}
void CWinTestView::OnSmoothingGaussian() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->m_SmoothingGaussian(256,256);
	
	Invalidate(FALSE); // 화면 갱신 
}
void CWinTestView::OnSmoothingBox() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->m_SmoothingBox(256,256);
	
	Invalidate(FALSE); // 화면 갱신 
}
void CWinTestView::OnSharpeningLaplacian() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->m_SharpeningLaplacian(256,256);
	
	Invalidate(FALSE); // 화면 갱신 
}
void CWinTestView::OnEdgePrewitt() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->m_EdgePrewitt(256,256);
	
	Invalidate(FALSE); // 화면 갱신 
}
void CWinTestView::OnEdgeSobel() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->m_EdgeSobel(256,256);
	
	Invalidate(FALSE); // 화면 갱신 
}
void CWinTestView::OnZoomout() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->m_ZoomOut(256,256,0.7);
	
	Invalidate(FALSE); // 화면 갱신
}

void CWinTestView::OnZoomin() 
{
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->m_ZoomIn(256,256,1.3);
	
	Invalidate(FALSE); // 화면 갱신
}
void CWinTestView::OnRotation() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->m_Rotation(256,256,(256/2),(256/2),30);
	
	Invalidate(FALSE); // 화면 갱신
}
void CWinTestView::On2dFft() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	pDoc->m_FFT2D(256,256,1);
	
	Invalidate(FALSE); // 화면 갱신
}
void CWinTestView::OnMorphologyBinaryErosion() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	//구조 요소를 생성후 값을 지정 
	pDoc->m_AllocStructureElementBinary(4);
	pDoc->m_SetStructureElementBinary(0,4,0);
	pDoc->m_SetStructureElementBinary(1,0,-1);
	pDoc->m_SetStructureElementBinary(2,0,0);
	pDoc->m_SetStructureElementBinary(3,0,1);

	pDoc->m_MorphologyBinaryErosion(256,256);

	//동적 할당 메모리 해제 
	pDoc->m_FreeStructureElementBinary();

	Invalidate(FALSE); // 화면 갱신	
}
void CWinTestView::OnMorphologyBinaryDilation() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	//구조 요소를 생성후 값을 지정 
	pDoc->m_AllocStructureElementBinary(4);//structure element number + 1
	pDoc->m_SetStructureElementBinary(0,4,0);
	pDoc->m_SetStructureElementBinary(1,0,-1);
	pDoc->m_SetStructureElementBinary(2,0,0);
	pDoc->m_SetStructureElementBinary(3,0,1);

	pDoc->m_MorphologyBinaryDilation(256,256);

	//동적 할당 메모리 해제 
	pDoc->m_FreeStructureElementBinary();

	Invalidate(FALSE); // 화면 갱신
}
void CWinTestView::OnMorphologyGrayErosion() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	//구조 요소를 생성후 값을 지정 
	pDoc->m_AllocStructureElementGray(6);
	pDoc->m_SetStructureElementGray(0,6,0,0);
	pDoc->m_SetStructureElementGray(1,-1,0,1);
	pDoc->m_SetStructureElementGray(2,0,-1,1);
	pDoc->m_SetStructureElementGray(3,0,0,2);
	pDoc->m_SetStructureElementGray(4,0,1,1);
	pDoc->m_SetStructureElementGray(5,1,0,1);

	pDoc->m_MorphologyGrayErosion(256,256);

	//동적 할당 메모리 해제 
	pDoc->m_FreeStructureElementGray();

	Invalidate(FALSE); // 화면 갱신	
}
void CWinTestView::OnMorphologyGrayDilation() 
{
	// TODO: Add your command handler code here
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	//구조 요소를 생성후 값을 지정 
	pDoc->m_AllocStructureElementGray(6);
	pDoc->m_SetStructureElementGray(0,6,0,0);
	pDoc->m_SetStructureElementGray(1,-1,0,1);
	pDoc->m_SetStructureElementGray(2,0,-1,1);
	pDoc->m_SetStructureElementGray(3,0,0,2);
	pDoc->m_SetStructureElementGray(4,0,1,1);
	pDoc->m_SetStructureElementGray(5,1,0,1);

	pDoc->m_MorphologyGrayDilation(256,256);

	//동적 할당 메모리 해제 
	pDoc->m_FreeStructureElementGray();

	Invalidate(FALSE); // 화면 갱신	
}

