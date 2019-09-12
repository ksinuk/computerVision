
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


int CComputerVisionDoc::sobel_edge(str_img &input_func,str_img &output_func) //외곽선 추출용 소벨 엣지 함수
{
	if (input_func.data == NULL) { AfxMessageBox(_T("비어있는 이미지 sobel_edge")); return -1; }
	if (input_func.channel != 1) { AfxMessageBox(_T("그레이 이미지만 가능 sobel_edge")); return -1; }

	str_img in_img,out_img; //함수의 입력 이미지와 출력 이미지의 주소가 같은 경우를 대비하기 위한 임시 입출력 이미지
	reset_str_img(in_img,1); //입력, 출력용 구조체 초기화
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //외부 입력 이미지를 내부로 받아들임
	re_size_img(in_img,out_img); //크기와 채널값을 출력용 이미지와 같게 함

	int sobel_x[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}}; //소벨 엣지용 마스크
	int sobel_y[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};

	for(int y=1;y<in_img.height-1;y++) //소벨 엣지 계산
		for(int x = 1; x<in_img.width-1; x++)
		{
			int temp_x=0 , temp_y = 0;
			for(int i=-1;i<2;i++)
				for(int j = -1; j<2; j++)
				{
					temp_x+=in_img.data[(y+i)*in_img.width+x+j]*sobel_x[i+1][j+1];
					temp_y+=in_img.data[(y+i)*in_img.width+x+j]*sobel_y[i+1][j+1];
				}
			int temp_out = (temp_x > 0) ? temp_x : -temp_x; //소벨 엣지 x값 대입
			temp_out += (temp_y > 0) ? temp_y : -temp_y; //y값 대입
			out_img.data[y*out_img.width+x] = temp_out/8; //엣지 계산시 최대값이 255*8이므로 8로 나눔
		}

	for(int i=0;i<in_img.width;i++) //이미지의 가장자리는 근접한 소벨 엣지 계산으로 대체함 , x축
	{
		out_img.data[i] = out_img.data[in_img.width+i];
		out_img.data[(out_img.height-1)*out_img.width+i] = out_img.data[(out_img.height-2)*out_img.width+i];
	}
	for(int i=0;i<out_img.height;i++) //이미지의 가장자리는 근접한 소벨 엣지 계산으로 대체함 , y축
	{
		out_img.data[i*in_img.width] = out_img.data[i*in_img.width+1];
		out_img.data[(i+1)*in_img.width-1] = out_img.data[(i+1)*in_img.width-2];
	}

	ctrlCV_img(out_img,output_func); //결과 이미지를 외부로 출력
	delete [] in_img.data; //임시 입출력 이미지 해제
	delete [] out_img.data;
	return 0;
}

int CComputerVisionDoc::make_Middle_filter(str_img &input_func,str_img &output_func,int mask_h,int mask_w) //중간값 필터
{
	str_img in_img,out_img; //함수의 입력 이미지와 출력 이미지의 주소가 같은 경우를 대비하기 위한 구조체 
	reset_str_img(in_img,1); //입력, 출력용 구조체 초기화
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //외부 입력 이미지를 내부로 받아들임
	re_size_img(in_img,out_img); //크기와 채널값을 출력용 이미지와 같게 함

	int mask_size = (2*mask_h+1)*(2*mask_w+1); //사용할 마스크의 크기

	for(int y=0 ; y<in_img.height ; y++) //샘플값 추출 후 중간값을 출력용 이미지로 입력
		for(int x = 0; x<in_img.width; x++)
		{
			int fy = (y-mask_h < 0) ? 0 : y-mask_h; //이하 4줄 모두 마스크의 범위를 나타내는 좌표 값
			int ly = (y+mask_h >= in_img.height) ? in_img.height-1 : y+mask_h;
			int fx = (x-mask_w < 0) ? 0 : x-mask_w;
			int lx = (x+mask_w >= in_img.width) ? in_img.width-1 : x+mask_w;
			
			int *sample = new int[mask_size]; //샘플값을 받아들이는 배열
			int ctn=0 , middle_cut=0; //샘플의 갯수와 사용할 중간값의 위치

			for(int i=fy ; i<=ly ; i++) //샘플을 입력 이미지도 부터 받아드림
				for(int j = fx; j<=lx; j++)
				{
					sample[ctn++] = in_img.data[i*in_img.width+j];
				}
			
			merge_sort(sample,0,mask_size-1); //병합 정렬로 중간값을 알아냄 

			middle_cut=(ctn+ctn%2)/2; //사용할 중간 값의 위치
			out_img.data[y*out_img.width+x] = sample[middle_cut];
			delete [] sample;
		}

	ctrlCV_img(out_img,output_func); //외부로 이미지 출력
	delete [] in_img.data; //임시 이미지 해제
	delete [] out_img.data;
	return 0;
}

int merge_sort(int a[],int start,int end) //병합 정렬
{
	if(start<end)
	{
		int mid = (start + end)/2;
		merge_sort(a,start,mid); //배열을 2개로 나눈 뒤 각각의 배열을 병합 정력
		merge_sort(a,mid+1,end);
		sort2array(a,start,mid,end); //정렬된 배열을 하나로 합침
	}
	return 0;
}

int sort2array(int a[],int start,int mid,int end) //정렬된 2개의 배열을 하나로 합치는 함수
{
	int *output = new int[end-start+1]; //임시 배열
	int i=start;
	int j=mid+1;
	int k=0;

	while(i<=mid && j<=end) //각각의 배열의 가장 작은 값 중에서 가장 작은 것을 임시 배열에 저장
	{
		if(a[i]<=a[j])output[k++] = a[i++];
		else output[k++] = a[j++];
	}

	if(i>mid) //어느 1개의 배열이 비었으면 남은 배열의 데이터의 값을 임시 배열에 저장
		for(int t=j;k<=end-start;k++,t++) output[k] = a[t];
	else
		for(int t=i;k<=end-start;k++,t++) output[k] = a[t];

	for(i=start;i<=end;i++)a[i]=output[i-start]; //임시 배열에 저장된 정렬된 데이터를 출력
	delete [] output; //임시 배열 해제
	return 0;
}

int CComputerVisionDoc::delete_dot(str_img & input_func, str_img & output_func, int cut) //기준보다 작은 크기의 이진 이미지 제거
{
	str_img in_img,out_img; //임시 입출력용 이미지
	reset_str_img(in_img,1); //임시 이미지 초기화
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); // 입력 데이터 복사
	re_size_img(in_img,out_img); // 출력 데이터의 크기와 채널값 조정

	int *stack = new int[in_img.height*in_img.width*2]; //연산 위치 임시 저장용 스텍
	int *pixel = new int[in_img.height*in_img.width*2]; //각각의 이진 이미지의 픽셀값이 저장된 스텍
	int pointer=0; //스텍의 위치
	int size=0; //지금까지 알아낸 픽셀의 양

	for(int y=0 ; y<in_img.height ; y++) //각각의 이진 이미지의 시작점 찾기
		for(int x = 0; x<in_img.width; x++)
		{
			if(in_img.data[y*in_img.width+x]==255) // 계산이 안 된 이진 이미지 발견
			{
				pointer=1; //스텍의 시작 위치
				size=1;
				stack[2*pointer] = x; //스텍에 좌표 입력
				stack[2*pointer+1] = y;
				pixel[2*size] = x;
				pixel[2*size+1] = y;

				in_img.data[y*in_img.width+x] = 128; //사용한 픽셀 삭제(값 변경)
				int yy=y,xx=x; //이 좌표 인근에 같은 이진 이미지의 픽셀이 있다고 가정

				while(pointer>0) //이진 이미지의 픽셀을 찾을때가지 무한 반복 
				{
					int ok=0; //이 변수가 0이 아니면 좌표 xx,yy 근처에 픽셀이 있는 거임
					for(int i=yy-1;i<=yy+1;i++)
						for(int j = xx-1; j<=xx+1; j++)
						{
							if(i<0 || i >=in_img.height || j<0 || j>=in_img.width) continue; //이미지 범위를 벗어나면 다음 픽셀로 넘어가기
							if(in_img.data[i*in_img.width+j]==255) //이진 이미지 픽셀 근처에 또다른 픽셀을 찾아내면
							{
								ok=1; //픽셀이 있는 신호를 만들고
								pointer++;
								size++;
								stack[2*pointer] = j; //스텍에 좌표 입력
								stack[2*pointer+1] = i;
								pixel[2*size] = j; //이진 이미지의 픽셀 저장소에도 좌표 입력
								pixel[2*size+1] = i;

								in_img.data[i*in_img.width+j]=128;  //사용한 픽셀 삭제(값 변경)
							}
						}
					if(ok!=0) {yy=stack[2*pointer+1];xx=stack[2*pointer]; } //근처에 픽셀이 있었으면 마지막으로 찾은 자표 근처를 탐색
					else  {pointer--; yy=stack[2*pointer+1]; xx=stack[2*pointer]; } //찾아낸 픽셀이 없으면 그 이전 픽셀 사용
				}

				if(size<cut) //이진 이미지의 픽셀을 다 찾아내서 이미지의 크기와 기준값과 비교한다.
				{
					for(int i=1;i<=size;i++) //기준보다 작은 이미지이면 출력 이미지에 출력을 하지 않는다.
						out_img.data[pixel[2*i+1]*in_img.width+pixel[2*i]] = 0;
				}
				else
				{
					for(int i=1;i<=size;i++) //기준보다 큰 이미지이면 출력 이미지에 출력 한다.
						out_img.data[pixel[2*i+1]*in_img.width+pixel[2*i]] = 255;
				}
				
			}
		}

	ctrlCV_img(out_img,output_func); //외부로 이미지 출력
	delete [] stack; //스텍과 임시 이미지 해제
	delete []pixel;
	delete [] in_img.data;
	delete [] out_img.data;
	return 0;
}