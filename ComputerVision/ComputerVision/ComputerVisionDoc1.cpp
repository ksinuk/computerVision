
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

void CComputerVisionDoc::make_laplacian(img_str &in_img, img_str &out_img)
{
	// 예외
	if (in_img.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (in_img.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	int laplacian_mask[3][3]={{-1,-1,-1},{-1,8,-1},{-1,-1,-1}};

	img_str temp_tophat;
	temp_tophat.channel = in_img.channel;
	temp_tophat.width = in_img.width;
	temp_tophat.height = in_img.height;
	temp_tophat.point = NULL;
	int temp_image_size = temp_tophat.height * temp_tophat.width * temp_tophat.channel;
	Allocate(temp_tophat.point, temp_image_size);

	for(int y=0;y<out_img.height;y++) //출력 이미지 초기화
		for(int x=0;x<out_img.height;x++)
			out_img.point[y*out_img.width+x] = 0;

	for(int y = 0; y<in_img.height; y++) //61x61 마스크 팽창 연산
		for(int x = 0; x<in_img.width; x++)
		{
			int i=0,j=0,in=0;
			for(i=-1;i<=1;i++)
				for(j = -1; j<=1; j++)
				{
					if(y+i<0 || y+i>=in_img.height || x+j<0 || x+j>=in_img.width)continue;
					int mask_point = laplacian_mask[i+1][j+1];
					out_img.point[y*out_img.width+x] += in_img.point[(y+i)*in_img.width+x+j]*mask_point;
				}
		}
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::make_Closing(img_str &in_img, img_str &out_img,int mask_n)
{
	// 예외
	if (in_img.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (in_img.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	img_str temp_tophat;
	temp_tophat.channel = in_img.channel;
	temp_tophat.width = in_img.width;
	temp_tophat.height = in_img.height;
	temp_tophat.point = NULL;
	int temp_image_size = temp_tophat.height * temp_tophat.width * temp_tophat.channel;
	Allocate(temp_tophat.point, temp_image_size);

	// 출력 이미지 리사이징
	out_img.height = in_img.height;
	out_img.width = in_img.width;
	out_img.channel = in_img.channel;
	int output_image_size = out_img.height * out_img.width * out_img.channel;
	Allocate(out_img.point, output_image_size);
	
	for(int y=0;y<out_img.height;y++) //출력 이미지 초기화
		for(int x=0;x<out_img.height;x++)
			out_img.point[y*out_img.width+x] = 0;

	for(int y = 0; y<in_img.height; y++) //61x61 마스크 팽창 연산
		for(int x = 0; x<in_img.width; x++)
		{
			int i=0,j=0,max=0;
			for(i=-mask_n;i<=mask_n;i++)
				for(j = -mask_n; j<=mask_n; j++)
				{
					if(y+i<0||y+i>=temp_tophat.height || x+j<0 || x+j>=temp_tophat.width)continue;
					if(max<in_img.point[(y+i)*in_img.width+x+j])max = in_img.point[(y+i)*in_img.width+x+j];
				}
			temp_tophat.point[y*temp_tophat.width+x] = max;
		}

	for(int y = 0; y<temp_tophat.height; y++) //61x61 마스크 침식 연산
		for(int x = 0; x<temp_tophat.width; x++)
		{
			int i=0,j=0,min=255;
			for(i=-mask_n;i<=mask_n;i++)
				for(j = -mask_n; j<=mask_n; j++)
				{
					if(y+i<0||y+i>=out_img.height||x+j<0||x+j>=out_img.width)continue;
					if(min>temp_tophat.point[(y+i)*temp_tophat.width+x+j])min = temp_tophat.point[(y+i)*temp_tophat.width+x+j];
				}
			out_img.point[y*out_img.width+x] = min;
		}
	
	delete [] temp_tophat.point;
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::make_Opening(img_str &in_img, img_str &out_img,int mask_n)
{
	// 예외
	if (in_img.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (in_img.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	img_str temp_tophat;
	temp_tophat.channel = in_img.channel;
	temp_tophat.width = in_img.width;
	temp_tophat.height = in_img.height;
	temp_tophat.point = NULL;
	int temp_image_size = temp_tophat.height * temp_tophat.width * temp_tophat.channel;
	Allocate(temp_tophat.point, temp_image_size);

	// 출력 이미지 리사이징
	out_img.height = in_img.height;
	out_img.width = in_img.width;
	out_img.channel = in_img.channel;
	int output_image_size = out_img.height * out_img.width * out_img.channel;
	Allocate(out_img.point, output_image_size);
	
	for(int y=0;y<out_img.height;y++) //출력 이미지 초기화
		for(int x=0;x<out_img.height;x++)
			out_img.point[y*out_img.width+x] = 0;

	for(int y = 0; y<in_img.height; y++) //61x61 마스크 팽창 연산
		for(int x = 0; x<in_img.width; x++)
		{
			int i=0,j=0,min=255;
			for(i=-mask_n;i<=mask_n;i++)
				for(j = -mask_n; j<=mask_n; j++)
				{
					if(y+i<0||y+i>=temp_tophat.height || x+j<0 || x+j>=temp_tophat.width)continue;
					if(min>in_img.point[(y+i)*in_img.width+x+j])min = in_img.point[(y+i)*in_img.width+x+j];
				}
			temp_tophat.point[y*temp_tophat.width+x] = min;
		}

	for(int y = 0; y<temp_tophat.height; y++) //61x61 마스크 침식 연산
		for(int x = 0; x<temp_tophat.width; x++)
		{
			int i=0,j=0,max=0;
			for(i=-mask_n;i<=mask_n;i++)
				for(j = -mask_n; j<=mask_n; j++)
				{
					if(y+i<0||y+i>=out_img.height||x+j<0||x+j>=out_img.width)continue;
					if(max<temp_tophat.point[(y+i)*temp_tophat.width+x+j])max = temp_tophat.point[(y+i)*temp_tophat.width+x+j];
				}
			out_img.point[y*out_img.width+x] = max;
		}

	delete[] temp_tophat.point;
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}


int CComputerVisionDoc::Histogram_Equalization(img_str &in_img,img_str &out_img)
{
	// 예외
	if (in_img.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return -1; }
	if (in_img.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return -1; }

	make_histogram(in_img,0);
	
	float Nt = (float)in_img.height*(float)in_img.width;
	float histogram_acc[256];
	histogram_acc[0] = (float)histogram[0][0];
	for(int i=1;i<256;i++) histogram_acc[i] = histogram_acc[i-1]+(float)histogram[0][i];
	for(int i=1;i<256;i++) histogram_acc[i] = histogram_acc[i]*255.0/Nt;

	for(int i=0;i<Nt;i++)out_img.point[i] = histogram_acc[in_img.point[i]];

	return 0;
}

int CComputerVisionDoc::Histogram_Stretching(img_str &in_img,img_str &out_img)
{
	// 예외
	if (in_img.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return -1; }
	if (in_img.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return -1; }

	make_histogram(in_img,0);
	
	float Nt = (float)in_img.height*(float)in_img.width;
	int sum=0 , low = -1 , high = -1;
	float percent=10.0 , low_cut=Nt*percent/100.0 , high_cut=Nt*(100-percent)/100.0;
	for(int i = 0; i<256; i++)
	{
		sum += histogram[0][i];
		if(low<0 && sum>=low_cut)low = i;
		if(high<0 && sum>=high_cut)high = i;
	}

	for(int i=0;i<Nt;i++)
	{
		if(in_img.point[i] <= low)out_img.point[i] = 0;
		else if(in_img.point[i] >= high)out_img.point[i] = 255;
		else out_img.point[i] = (float)(in_img.point[i]-low)*255.0/(float)(high-low);
	}

	return 0;
}

void CComputerVisionDoc::make_histogram(img_str &in_img,int color) //실제 히스토그램 생성 함수
{
	int max=0;
	int min=in_img.height*in_img.width;
	Allocate(out_color_histogram[color] , 300*300);

	for(int i = 0; i<256; i++) histogram[color][i] = 0; //초기화

	for(int y=0 ; y < in_img.height ; y++) //빈도수 측정
		for(int x=0 ; x < in_img.width ; x++)
		{
			histogram[color][ in_img.point[in_img.channel*(y*in_img.width + x)+color] ]++; 
		}

	for(int i = 0; i<256; i++) //최대,최소 구하기
	{
		 if(max <= histogram[color][i]) max = histogram[color][i];
		 if(min >= histogram[color][i]) min = histogram[color][i];
	}

	for(int y = 0; y<300; y++) //출력화면 초기화
	{
		for(int x=0; x< 300; x++)
		{
			if(x!=0 && x!=299 && y!=0 && y!=299)out_color_histogram[color][y*300 + x] = 255; 
			else out_color_histogram[color][y*300 + x] = 0; //윤곽선
		}
	}

	if(max!=min)
	{
		float vd = (float)(max - min);
		int out_histogram[256];
		for(int i=0; i<256; i++) out_histogram[i] = (int)( (histogram[color][i]-min)*255/vd ); //정규화

		for(int x=0 ; x < 256 ; x++) //화면 작성
			for(int y=0 ; y < out_histogram[x] ; y++)
			{
				out_color_histogram[color][(255+22-y)*300+x+22] = 0;
			}
	}
}

int CComputerVisionDoc::make_middle_filiter(img_str &in_img,img_str &out_img,int n)
{
	// 예외
	if (in_img.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return -1; }
	if(n==0) { AfxMessageBox(_T("필터 아님")); return -1; }

	unsigned int *sample = NULL;
	sample = new unsigned int[4*(n+1)*(n+1)]; //샘플값 저장 배열

	int max_p=0;
	for(int y = 0; y<in_img.height; y++) 
		for(int x = 0; x<in_img.width; x++)
			for(int c = 0; c<in_img.channel; c++)
			{
				int off = (y*in_img.width+x)*in_img.channel+c;
			
				int point=0;
				for(int i = -n; i<=n; i++)
					for(int j = -n; j<=n; j++) //샘플 수집
					{
						if(y+i<0 || y+i>=in_img.height || x+j<0 || x+j>=in_img.width)continue;
						*(sample+point++) = input_image.point[off+(i*input_image.width+j)*output_image.channel];
					}

				for(int i = 0; i<point-1; i++) //선택 정렬로 중간값 알아내기
				{
					int min=i;
					for(int j = i+1; j<point; j++)
						if(*(sample+j) < *(sample+min))min = j;

					int temp = *(sample+i);
					*(sample+i) = *(sample+min);
					*(sample+min) = temp;
				}
				output_image.point[off] = *(sample+(point+point%2)/2); //출력 이미지 만들기
				
				if(max_p<point)max_p = point;
			}
	// 모든 뷰 업데이트
	UpdateAllViews(0);
	delete[] sample;
}

void CComputerVisionDoc::DownHat(img_str &in_img, img_str &out_img,int mask_n)
{
	// 예외
	if (in_img.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (in_img.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	img_str temp_hat;
	temp_hat.channel = in_img.channel;
	temp_hat.width = in_img.width;
	temp_hat.height = in_img.height;
	temp_hat.point = NULL;
	int temp_image_size = temp_hat.height * temp_hat.width * temp_hat.channel;
	Allocate(temp_hat.point, temp_image_size);

	for(int y=0;y<out_img.height;y++) //출력 이미지 초기화
		for(int x=0;x<out_img.height;x++)
			out_img.point[y*out_img.width+x] = 0;

	for(int y = 0; y<in_img.height; y++) //30x30 마스크 팽창 연산
		for(int x = 0; x<in_img.width; x++)
		{
			int i=0,j=0,max=0;
			for(i=-mask_n;i<=mask_n;i++)
				for(j = -mask_n; j<=mask_n; j++)
				{
					if(y+i<0||y+i>=temp_hat.height || x+j<0 || x+j>=temp_hat.width)continue;
					if(max<in_img.point[(y+i)*in_img.width+x+j])max = in_img.point[(y+i)*in_img.width+x+j];
				}
			temp_hat.point[y*temp_hat.width+x] = max;
		}

	for(int y = 0; y<temp_hat.height; y++) //3x3 마스크 침식 연산 및 최종 연산
		for(int x = 0; x<temp_hat.width; x++)
		{
			int i=0,j=0,min=255;
			for(i=-mask_n;i<=mask_n;i++)
				for(j = -mask_n; j<=mask_n; j++)
				{
					if(y+i<0||y+i>=out_img.height||x+j<0||x+j>=out_img.width)continue;
					if(min>temp_hat.point[(y+i)*temp_hat.width+x+j])min = temp_hat.point[(y+i)*temp_hat.width+x+j];
				}
			out_img.point[y*out_img.width+x] = (in_img.point[y*out_img.width+x]<min) ? 255-(min-in_img.point[y*out_img.width+x]) : 255;
		}

	delete [] temp_hat.point;
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}

void CComputerVisionDoc::TopHat(img_str &in_img, img_str &out_img,int mask_n)
{
	// 예외
	if (in_img.point == NULL) { AfxMessageBox(_T("비어있는 이미지")); return; }
	if (in_img.channel != 1 ) { AfxMessageBox(_T("그레이 만 가능")); return; }

	img_str temp_downhat;
	temp_downhat.channel = in_img.channel;
	temp_downhat.width = in_img.width;
	temp_downhat.height = in_img.height;
	temp_downhat.point = NULL;
	int temp_image_size = temp_downhat.height * temp_downhat.width *temp_downhat.channel;
	Allocate(temp_downhat.point, temp_image_size);

	for(int y=0;y<out_img.height;y++) //출력 이미지 초기화
		for(int x=0;x<out_img.height;x++)
			out_img.point[y*out_img.width+x] = 0;

	for(int y = 0; y<in_img.height; y++) //61x61 마스크 침식 연산
		for(int x = 0; x<in_img.width; x++)
		{
			int i=0,j=0,min=255;
			for(i=-mask_n;i<=mask_n;i++)
				for(j = -mask_n; j<=mask_n; j++)
				{
					if(y+i<0||y+i>=temp_downhat.height || x+j<0 || x+j>=temp_downhat.width)continue;
					if(min>in_img.point[(y+i)*in_img.width+x+j])min = in_img.point[(y+i)*in_img.width+x+j];
				}
			temp_downhat.point[y*temp_downhat.width+x] = min;
		}

	for(int y = 0; y<temp_downhat.height; y++) //61x61 마스크 팽창 연산 및 최종 연산
		for(int x = 0; x<temp_downhat.width; x++)
		{
			int i=0,j=0,max=0;
			for(i=-mask_n;i<=mask_n;i++)
				for(j = -mask_n; j<=mask_n; j++)
				{
					if(y+i<0||y+i>=out_img.height||x+j<0||x+j>=out_img.width)continue;
					if(max<temp_downhat.point[(y+i)*temp_downhat.width+x+j])max = temp_downhat.point[(y+i)*temp_downhat.width+x+j];
				}
			out_img.point[y*out_img.width+x] = (in_img.point[y*out_img.width+x]>max) ? in_img.point[y*out_img.width+x]-max : 0;
		}
	
	delete[] temp_downhat.point;
	// 모든 뷰 업데이트
	UpdateAllViews(0);
}
