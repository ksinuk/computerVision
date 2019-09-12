
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


int CComputerVisionDoc::make_histogram(int* histogram,str_img &func_image_) // 히스토그램 계산
{
	for(int i=0;i<256;i++)histogram[i]=0;//히스토그램 초기화
	for(int y=0;y<func_image_.height;y++)
		for(int x = 0; x<func_image_.width; x++)
			histogram[func_image_.data[y*func_image_.width+x]]++;

	return 0;
}

int CComputerVisionDoc::search_Otzu(int* histogram) // Otzu 알고리즘을 이용한 최적값 계산
{
	int Otzu_t=0; //이진 이미지를 만들때 이 값을 기준으로 한다.
	float Otzu_min=10e10;

	for(int t=0; t<256; t++)
	{
		// 누적확률q1, q2 계산
		float q1 = 0, q2 = 0;
		for(int i=0; i<t; i++)
			q1 += histogram[i];
		for(int i=t; i<256; i++) 
			q2 += histogram[i];
		if(q1==0 || q2==0) continue;

		// 평균u1, u2 계산
		float u1=0.0f, u2=0.0f;
		for(int i=0; i<t; i++) u1 += i*histogram[i]; u1 /= q1;
		for(int i=t; i<256; i++) u2 += i*histogram[i]; u2 /= q2;

		// 분산s1, s2 계산
		float s1=0.0f, s2=0.0f;
		for(int i=0; i<t; i++) s1 += (i-u1)*(i-u1)*histogram[i]; s1 /= q1;
		for(int i=t; i<256; i++) s2 += (i-u2)*(i-u2)*histogram[i]; s2 /= q2;

		//총분산계산
		float wsv = q1*s1+q2*s2;
		if(wsv <  Otzu_min)
		{
			Otzu_min = wsv;
			Otzu_t= t; // 최소치저장
		}
	}


	return  Otzu_t;
}

int CComputerVisionDoc::make_binary_img(int bin_cut,str_img &in_img,str_img &out_img) //기준값을 받아서 이미지 이진화 
{
	if(out_img.data==NULL){ AfxMessageBox(_T("비어있는 이미지 : make_binary_img")); return -1; }

	for(int y=0;y<out_img.height;y++)
		for(int x = 0; x<out_img.width; x++)
			out_img.data[y*out_img.width+x] = (in_img.data[y*in_img.width+x]>bin_cut) ? 255:0;

	return 0;
}

int CComputerVisionDoc::binary_niblack(str_img & input_func, str_img & output_func) //적응 이치화 계산
{
	str_img in_img,out_img; //함수의 입력 이미지와 출력 이미지의 주소가 같은 경우를 대비하기 위한 임시 입출력 이미지
	reset_str_img(in_img,1); //입력, 출력용 구조체 초기화
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //외부 입력 이미지를 내부로 받아들임
	re_size_img(in_img,out_img); //크기와 채널값을 출력용 이미지와 같게 함

	int lut_ssum[256]; //제곱값을 미리 계산하여 저장
	for(int i=0;i<256;i++)lut_ssum[i] = i*i;

	int *sum_x = new int[in_img.width*in_img.height]; //계산의 일부분을 미리 저장하는 공간
	for(int i = 0; i<in_img.width*in_img.height; i++) sum_x[i] = 0;
	int *ssum_x = new int[in_img.width*in_img.height];
	for(int i = 0; i<in_img.width*in_img.height; i++) ssum_x[i] = 0;

	int w=20;

	for(int y=0;y<in_img.height;y++) //각각의 픽셀의 x축 범위 내에서 픽셀의 합과 제곱합을 계산
		for(int x = 0; x<in_img.width; x++)
		{
			int xl = (x-w>0) ? x-w : 0;
			int xh = (x+w< in_img.width) ? x+w : in_img.width-1;

			for(int j = xl; j<=xh; j++)
			{
				sum_x[y*in_img.width+x] += in_img.data[y*in_img.width+j];
				ssum_x[y*in_img.width+x] += lut_ssum[in_img.data[y*in_img.width+j]];
			}
		}

	for(int y=0;y<in_img.height;y++) //x축 범위내에서 계산한 값들을 y축 기준으로 계산한 후 평균과 분값을 이용하여 결과값 계산
		for(int x = 0; x<in_img.width; x++)
		{
			int sum=0.0,ssum=0.0;

			int yl = (y-w>0) ? y-w : 0; //마스크의 범위
			int yh = (y+w< in_img.height) ? y+w : in_img.height-1;
			int xl = (x-w>0) ? x-w : 0;
			int xh = (x+w< in_img.width) ? x+w : in_img.width-1;
			int count=(yh-yl+1)*(xh-xl+1);

			for(int i=yl;i<=yh;i++) //마스크 내부의 픽셀의 합과 제곱합 최종 계산
			{
				sum += sum_x[i*in_img.width+x];
				ssum += ssum_x[i*in_img.width+x];
			}

			float m = (float)sum/(float)count; //평균
			float v = (float)ssum/(float)count -m*m; //분산
			float thres = m*(1.0 - 0.02*(1+sqrt(v)/128.0)); //기준값

			out_img.data[y*out_img.width+x] = (in_img.data[y*in_img.width+x] < thres) ? 0 : 255; //이진화
		}

	ctrlCV_img(out_img,output_func); //외부로 결과물 출력
	delete [] in_img.data; //임시 입출력 이미지 해제
	delete [] out_img.data;
	return 0;
}

int CComputerVisionDoc::make_histogram_Equal(str_img & input_func, str_img & output_func) //히스토그램 평활화 계산
{
	str_img in_img,out_img; //함수의 입력 이미지와 출력 이미지의 주소가 같은 경우를 대비하기 위한 임시 입출력 이미지
	reset_str_img(in_img,1); //입력, 출력용 구조체 초기화
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //외부 입력 이미지를 내부로 받아들임
	re_size_img(in_img,out_img); //크기와 채널값을 출력용 이미지와 같게 함

	int histogram[256]; //히스토그램 계산값
	make_histogram(histogram,in_img); //히스토그램 계산 함수
	float Nt = in_img.height*in_img.width-histogram[0]; //픽셀값이 0이 아닌 픽셀의 양
	
	for(int i=2;i<256;i++)histogram[i] += histogram[i-1]; //픽셀 값 1 - 255 범위 안에서 누적분포값 계산
	for(int i=1;i<256;i++)histogram[i] = (float)histogram[i]*255.0/Nt; //누적 분포값의 최대값을 255로 조정
	histogram[0]=0; //픽셀 값이 0인 픽셀은 예외로 출력을 0으로 한다.

	for(int y=0 ; y<out_img.height ;y++) //결과물 만들기
		for(int x=0 ; x<out_img.width ; x++)
			out_img.data[y*out_img.width+x] = histogram[in_img.data[y*in_img.width+x]];

	ctrlCV_img(out_img,output_func); //외부로 결과물 출력
	delete [] in_img.data; //임시 입출력 이미지 해제
	delete [] out_img.data;
	return 0;
}

int CComputerVisionDoc::make_histogram_Strach(str_img & input_func, str_img & output_func) //히스토그램 스크레치 계산
{
	str_img in_img,out_img; //함수의 입력 이미지와 출력 이미지의 주소가 같은 경우를 대비하기 위한 임시 입출력 이미지
	reset_str_img(in_img,1); //입력, 출력용 구조체 초기화
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //외부 입력 이미지를 내부로 받아들임
	re_size_img(in_img,out_img); //크기와 채널값을 출력용 이미지와 같게 함

	int histogram[256]; //히스토그램 계산값
	make_histogram(histogram,in_img); //히스토그램 계산 함수
	float Nt = in_img.height*in_img.width-histogram[0]; //픽셀값이 0이 아닌 픽셀의 양
	int low_cut = Nt*0.5 , high_cut = Nt*0.1 , sum = 0;//최소, 최대 기준값과 히스토그램의 누적합
	int low_i=-1 , high_i = 0; //픽셀값이 이 값보다 작거나 크면 결과값을 0이나 255로 한다.

	for(int i = 1; i<256; i++) //최소 기준값 구하기
	{
		sum+=histogram[i];
		if(low_cut<=sum){low_i = i;sum=0;break;}
	}
	for(int i = 255; i>=1; i--) //최대 기준값 구하기
	{
		sum+=histogram[i];
		if(high_cut<=sum){high_i = i;break;}
	}
	for(int i = 0; i<low_i; i++) histogram[i]=0; //히스토그램 배열에 출력값 입력
	for(int i = low_i; i<high_i; i++) histogram[i]=255.0*(float)(i-low_i)/(float)(high_i - low_i);
	for(int i = high_i; i<256; i++) histogram[i]=255;

	for(int y=0 ; y<out_img.height ;y++) //픽셀의 값을 히스토그램 배열에 넣어서 저장된 출력값을 출력
		for(int x=0 ; x<out_img.width ; x++)
			out_img.data[y*out_img.width+x] = histogram[in_img.data[y*in_img.width+x]];

	ctrlCV_img(out_img,output_func); //외부로 결과물 출력
	delete [] in_img.data; //임시 입출력 이미지 해제
	delete [] out_img.data;
	return 0;
}