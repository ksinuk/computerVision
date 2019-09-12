
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


int CComputerVisionDoc::make_TopHat(str_img &input_func,str_img &output_func,int mask_h,int mask_w) //Top - Hat 연산
{
	str_img in_img,out_img; //함수의 입력 이미지와 출력 이미지의 주소가 같은 경우를 대비하기 위한 임시 입출력 이미지
	reset_str_img(in_img,1); //입력, 출력용 구조체 초기화
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //외부 입력 이미지를 내부로 받아들임
	re_size_img(in_img,out_img); //크기와 채널값을 출력용 이미지와 같게 함

	str_img temp_opening; // 임시 이미지 초기화
	reset_str_img(temp_opening,1);
	re_size_img(in_img,temp_opening);
	
	make_opening(in_img , temp_opening , mask_h , mask_w); //임시 이미지에 입력 이미지의 오프닝 연산값을 넣어 둠

	for(int y=0;y<out_img.height;y++) //입력 이미지에 오프닝 연산 이미지를 빼내서 조명 제거 
		for(int x = 0; x<out_img.width; x++)
		{
			int tmep_int = in_img.data[y*in_img.width+x] - temp_opening.data[y*temp_opening.width+x];
			out_img.data[y*out_img.width+x] = (tmep_int>0) ? tmep_int:0; //입력 이미지의 픽셀 값보다 오프닝 픽셀의 크기 가 더 그면 0으로 출력함
		}

	delete [] temp_opening.data;//임시 오프닝 이미지 해제
	ctrlCV_img(out_img,output_func); //외부로 결과물 출력
	delete [] in_img.data; //입출력 임시 이미지 해제
	delete [] out_img.data;		
	return 0;
}

int CComputerVisionDoc::make_DownHat(str_img &input_func,str_img &output_func,int mask_h,int mask_w) // Down Hat 연산
{
	str_img in_img,out_img; //함수의 입력 이미지와 출력 이미지의 주소가 같은 경우를 대비하기 위한 임시 입출력 이미지
	reset_str_img(in_img,1); //입력, 출력용 구조체 초기화
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //외부 입력 이미지를 내부로 받아들임
	re_size_img(in_img,out_img); //크기와 채널값을 출력용 이미지와 같게 함

	str_img temp_closing; // 임시 이미지 리사이징
	reset_str_img(temp_closing,1);
	re_size_img(in_img,temp_closing);
	
	make_closing(in_img , temp_closing , mask_h , mask_w); //close 연산을 임시 이미지에 저장

	for(int y=0;y<out_img.height;y++) //close 연산 이미지에 입력 이미지를 빼서 출력 이미지를 말들어 냄
		for(int x = 0; x<out_img.width; x++)
		{
			int tmep_int = (temp_closing.data[y*temp_closing.width+x] - in_img.data[y*in_img.width+x]);
			out_img.data[y*out_img.width+x] = (tmep_int>0) ? tmep_int:0;
		}

	delete [] temp_closing.data; //임시 이미지 해제
	ctrlCV_img(out_img,output_func); //외부로 결과물 출력
	delete [] in_img.data; //임시 입출력 이미지 해제
	delete [] out_img.data;		
	return 0;
}

int CComputerVisionDoc::make_opening(str_img &in_img, str_img &out_img,int mask_h,int mask_w) //opening 연산
{
	str_img temp_Erosion; // 임시 이미지 리사이징
	reset_str_img(temp_Erosion,1);
	re_size_img(in_img,temp_Erosion);

	make_Erosion(in_img, temp_Erosion, mask_h, mask_w); //침식 연산
	make_Dilation(temp_Erosion, out_img, mask_h, mask_w); //팽창 연산

	delete [] temp_Erosion.data; //임시 이미지 해제

	return 0;
}

int CComputerVisionDoc::make_closing(str_img &in_img, str_img &out_img,int mask_h,int mask_w) //closing 연산
{
	str_img temp_Dilation; // 임시 이미지 리사이징
	reset_str_img(temp_Dilation,1);
	re_size_img(in_img,temp_Dilation);

	make_Dilation(in_img, temp_Dilation, mask_h, mask_w); //팽창 연산
	make_Erosion(temp_Dilation, out_img, mask_h, mask_w); //침식 연산
	 
	delete [] temp_Dilation.data; //임시 이미지 해제

	return 0;
}

int CComputerVisionDoc::make_Erosion(str_img &input_func,str_img &output_func,int mask_h,int mask_w) //침식 연산
{
	str_img in_img,out_img; //함수의 입력 이미지와 출력 이미지의 주소가 같은 경우를 대비하기 위한 임시 입출력 이미지
	reset_str_img(in_img,1); //입력, 출력용 구조체 초기화
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //외부 입력 이미지를 내부로 받아들임
	re_size_img(in_img,out_img); //크기와 채널값을 출력용 이미지와 같게 함

	int *min_array_w = new int[in_img.height*in_img.width]; //침식 연산의 일부분을 임시 저장하는 공간
	
	for(int y=0 ; y<in_img.height ; y++) //기준점과 같은 y축을 가진 픽셀들의 최소값을 구한다.
		for(int x = 0; x<in_img.width; x++)
		{
			int fx = (x-mask_w < 0) ? 0 : x-mask_w; //좌표 x의 범위는 마스크 값 이내로 한다.
			int lx = (x+mask_w >= in_img.width) ? in_img.width-1 : x+mask_w;
			int min=255;

			for(int i = fx; i<=lx; i++)
			{
				if(min>in_img.data[y*in_img.width+i]) min = in_img.data[y*in_img.width+i];
			}
			min_array_w[y*out_img.width+x] = min; //범위 내의 최소값을 임시 배열에 저장한다.
		}

	for(int y=0 ; y<in_img.height ; y++) // 임시 저장 공간을 이용해서 각각의 좌표의 침식 연산 값을 알아 낸다.
		for(int x = 0; x<in_img.width; x++)
		{
			int fy = (y-mask_h < 0) ? 0 : y-mask_h;
			int ly = (y+mask_h >= in_img.height) ? in_img.height-1 : y+mask_h;
			int min=255;

			for(int i=fy ; i<=ly ; i++) //min_array_w에서 기준점과 같은 x축을 가지는 값들의 최소 값을 구한다.
			{
				if(min>min_array_w[i*in_img.width+x]) min = min_array_w[i*in_img.width+x]; 
			}
			out_img.data[y*out_img.width+x] = min; //최종 연산값을 출력 이미지에 저장한다.
		}

	delete [] min_array_w; //임시 저장소 해제
	ctrlCV_img(out_img,output_func); //외부로 결과물 출력
	delete [] in_img.data; //임시 입출력 이미지 해제
	delete [] out_img.data;	
	return 0;
}

int CComputerVisionDoc::make_Dilation(str_img &input_func,str_img &output_func,int mask_h,int mask_w) //팽창 연산
{
	str_img in_img,out_img; //함수의 입력 이미지와 출력 이미지의 주소가 같은 경우를 대비하기 위한 임시 입출력 이미지
	reset_str_img(in_img,1); //입력, 출력용 구조체 초기화
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //외부 입력 이미지를 내부로 받아들임
	re_size_img(in_img,out_img); //크기와 채널값을 출력용 이미지와 같게 함

	unsigned char *max_array_w = new unsigned char[in_img.height*in_img.width]; //팽창 연산의 일부분을 임시 저장하는 공간 
	
	for(int y=0 ; y<in_img.height ; y++) //기준점과 같은 y축을 가진 픽셀들의 최대값을 구한다.
		for(int x = 0; x<in_img.width; x++)
		{
			int fx = (x-mask_w < 0) ? 0 : x-mask_w; //좌표 x의 범위는 마스크 값 이내로 한다.
			int lx = (x+mask_w >= in_img.width) ? in_img.width-1 : x+mask_w;
			int max=0;

			for(int i = fx; i<=lx; i++)
			{
				if(max<in_img.data[y*in_img.width+i]) max = in_img.data[y*in_img.width+i];
			}
			max_array_w[y*out_img.width+x] = max; //범위 내의 최대값을 임시 배열에 저장한다.
		}

	for(int y=0 ; y<in_img.height ; y++) // 임시 저장 공간을 이용해서 각각의 좌표의 팽창 연산 값을 알아 낸다.
		for(int x = 0; x<in_img.width; x++)
		{
			int fy = (y-mask_h < 0) ? 0 : y-mask_h;
			int ly = (y+mask_h >= in_img.height) ? in_img.height-1 : y+mask_h;
			int max=0;

			for(int i=fy ; i<=ly ; i++) //max_array_w에서 기준점과 같은 x축을 가지는 값들의 최소 값을 구한다.
			{
				if(max<max_array_w[i*in_img.width+x]) max = max_array_w[i*in_img.width+x];
			}
			out_img.data[y*out_img.width+x] = max; //최종 연산값을 출력 이미지에 저장한다.
		}

	delete [] max_array_w; //임시 저장소 해제
	ctrlCV_img(out_img,output_func); //외부로 결과물 출력
	delete [] in_img.data; //임시 입출력 이미지 해제
	delete [] out_img.data;		
	return 0;
}