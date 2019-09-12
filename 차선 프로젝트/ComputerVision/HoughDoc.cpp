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

int CComputerVisionDoc::make_Hough_Transform(str_img &input_func,str_img &graph,str_img &output_func) //차선 검출용 허프 변환 함수
{
	str_img inimg,outimg; //함수의 입력 이미지와 출력 이미지의 주소가 같은 경우를 대비하기 위한 임시 입출력 이미지
	reset_str_img(inimg,1); //입력, 출력용 구조체 초기화
	reset_str_img(outimg,1);
	ctrlCV_img(input_func,inimg); //외부 입력 이미지를 내부로 받아들임
	re_size_img(inimg,outimg); //크기와 채널값을 출력용 이미지와 같게 함

	int d_size = sqrt(inimg.height*inimg.height + inimg.width*inimg.width); //대각선의 길이
	double fcos[3600],fsin[3600],p2d = 3.1415926535/180.0; //삼각함수의 값을 저장하는 배열과 육십등분 각도를 rad 각도로 변환하는 단위
	int rad_size = 3600; //각도를 0.1도 마다 변환
	float rad_unit = 10; //각도 1도를 만드는데 필요한 양
	int thres = 120; //차선 여부를 구분하는 기준점

	int *hough = new int[d_size*rad_size]; //허프 연산 저장소
	for(int d=0;d<d_size;d++) //허프 저장소 초기화
		for(int r = 0; r<rad_size; r++)
			hough[d*rad_size+r] = 0;

	for(int y=0;y<outimg.height;y++) //출력 이미지 초기화
		for(int x=0;x<outimg.width;x++)
			outimg.data[y*outimg.width+x] = 0;

	for(int i = 0; i<rad_size; i++) //삼각함수를 미리 계산
	{
		fcos[i] = cos(p2d*(float)i/rad_unit);
		fsin[i] = sin(p2d*(float)i/rad_unit);
	}

	for(int y=0;y<inimg.height;y++) //이진 이미지에서 직선의 방정식의 변수 구하기
		for(int x = 0; x<inimg.width; x++)
		{
			if(inimg.data[y*inimg.width+x]==255)
			{
				for(int i = 0; i<rad_size; i++)
				{
					int d=fcos[i]*x+fsin[i]*y;
					if(d>0 && d<d_size)hough[d*rad_size+i]++;
				}
			}
			else inimg.data[y*inimg.width+x]=0;
		}

	int mask_d = 25 , mask_r = 65; //비슷한 변수를 가진 직선의 방정식 중에서 가장 많은 픽셀을 가지는 직선만 사용한다.
	for(int d=0;d<d_size;d++)
		for(int r = 0; r<rad_size; r++)
		{
			if(hough[d*rad_size+r]>0)
			{
				int fd = (d<mask_d) ? 0 : d-mask_d; //비교 대상인 변수들의 범위
				int ld = (d+mask_d>d_size-1) ? d_size-1 : d+mask_d;
				int fr = (r<mask_r) ? 0 : r-mask_r;
				int lr = (r+mask_r>rad_size-1) ? rad_size-1 : r+mask_r;

				for(int i=fd;i<=ld;i++)
					for(int j = fr; j<=lr; j++)
					{
						if(hough[i*rad_size+j]!=0)
						{    //주변에 자신보다 더 많은 픽셀을 가지는 직선이 있으면 자기 자신의 직선을 삭제한다.
							if(hough[d*rad_size+r]<=hough[i*rad_size+j] && !(d==i && r==j)) 
							{
								hough[d*rad_size+r] = 0;
								j+=100000;i+=100000;
							}
						}
					}
			}
		}

	/******************* print **************************/
	FILE *fd = fopen("project_road_line.txt","w+"); //직선 파라미터 출력용 파일

	for(int d=0;d<d_size;d++) 
		for(int r = 0; r<rad_size; r++)
		{
			if(hough[d*rad_size+r]>80)  //픽셀 값이 80 이상인 직선만 출력한다.
			{
				//직선에 넣을 수 있는 픽셀의 최대값과 실제 픽셀의 양을 비교한다.

				float fh = d/fsin[r]; //x값이 0일 때 y값
				float lh = (d-fcos[r]*outimg.width)/fsin[r]; //x값이 최대값 일 때 y값
				float fw = d/fcos[r]; //y값이 0일 때 x값
				float lw = (d-fsin[r]*outimg.height)/fcos[r]; //y값이 최대값 일 때 x값

				int box[4]={0,0,0,0};
				if(fh>0 && fh<outimg.height)box[0]=1; //x값이 0일 때 y값이 이미지 내부에 있다.
				if(lh>0 && lh<outimg.height)box[1]=1; //x값이 최대값 일 때 y값이 이미지 내부에 있다.
				if(fw>0 && fw<outimg.width)box[2]=1; //y값이 0일 때 x값이 이미지 내부에 있다.
				if(lw>0 && lw<outimg.width)box[3]=1; //y값이 최대값 일 때 x값이 이미지 내부에 있다.

				float temp_th=0; //이미지 내부에 있는 직선의 길이
				//각각의 경우에 이미지 내부에 있는 직선의 길이를 구한다. 
				if(box[0]!=0 && box[1]!=0) temp_th = (lh-fh)*(lh-fh) + outimg.width*outimg.width;
				else if(box[0]!=0 && box[2]!=0) temp_th = fh*fh+fw*fw;
				else if(box[0]!=0 && box[3]!=0) temp_th = (outimg.height-fh)*(outimg.height-fh)+lw*lw;
				else if(box[1]!=0 && box[2]!=0) temp_th = lh*lh + (outimg.width-fw)*(outimg.width-fw);
				else if(box[1]!=0 && box[3]!=0) temp_th = (outimg.height-lh)*(outimg.height-lh) + (outimg.width-lw)*(outimg.width-lw);
				else if(box[2]!=0 && box[3]!=0) temp_th = (lw-fw)*(lw-fw) + outimg.height*outimg.height;
				else continue; 

				thres = sqrt(temp_th)*0.5; //픽셀의 양이 직선의 길이의 절판이 되어야 한다.

				if(hough[d*rad_size+r]>thres) //만족하는 긴선(차선)을 찾았다.
				{
					double out_d = d+(fcos[r]*20.0+fsin[r]*240.0); //잘라난 이미지에서 원본 이미지로 복원

					fprintf(fd,"%.10lf\t%.10lf\t%.10lf\n",-fcos[r]/out_d,-fsin[r]/out_d,1.0); //텍스트 파일에 직선의 파라미터를 입력한다.
					for(int y = 0; y<outimg.height; y++) //출력용 이미지에 직선을 그린다.
					{
						int x = (d-y*fsin[r])/fcos[r];
						if(x>=0 && x<outimg.width) outimg.data[y*outimg.width+x] = 255;
					}
					for(int x = 0; x<outimg.width; x++) //출력용 이미지에 직선을 그린다.
					{
						int y = (d-x*fcos[r])/fsin[r];
						if(y>=0 && y<outimg.height) outimg.data[y*outimg.width+x] = 255;
					}
				}
			}
		}

	fclose(fd);
	delete [] hough; //임시 이미지 해제
	ctrlCV_img(outimg,output_func); //외부로 결과물 출력
	delete [] inimg.data; //임시 입출력 이미지 해제
	delete [] outimg.data;
	return 0;
}