
// ComputerVisionDoc.cpp : CComputerVisionDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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


int CComputerVisionDoc::sobel_edge(str_img &input_func,str_img &output_func) //�ܰ��� ����� �Һ� ���� �Լ�
{
	if (input_func.data == NULL) { AfxMessageBox(_T("����ִ� �̹��� sobel_edge")); return -1; }
	if (input_func.channel != 1) { AfxMessageBox(_T("�׷��� �̹����� ���� sobel_edge")); return -1; }

	str_img in_img,out_img; //�Լ��� �Է� �̹����� ��� �̹����� �ּҰ� ���� ��츦 ����ϱ� ���� �ӽ� ����� �̹���
	reset_str_img(in_img,1); //�Է�, ��¿� ����ü �ʱ�ȭ
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //�ܺ� �Է� �̹����� ���η� �޾Ƶ���
	re_size_img(in_img,out_img); //ũ��� ä�ΰ��� ��¿� �̹����� ���� ��

	int sobel_x[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}}; //�Һ� ������ ����ũ
	int sobel_y[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};

	for(int y=1;y<in_img.height-1;y++) //�Һ� ���� ���
		for(int x = 1; x<in_img.width-1; x++)
		{
			int temp_x=0 , temp_y = 0;
			for(int i=-1;i<2;i++)
				for(int j = -1; j<2; j++)
				{
					temp_x+=in_img.data[(y+i)*in_img.width+x+j]*sobel_x[i+1][j+1];
					temp_y+=in_img.data[(y+i)*in_img.width+x+j]*sobel_y[i+1][j+1];
				}
			int temp_out = (temp_x > 0) ? temp_x : -temp_x; //�Һ� ���� x�� ����
			temp_out += (temp_y > 0) ? temp_y : -temp_y; //y�� ����
			out_img.data[y*out_img.width+x] = temp_out/8; //���� ���� �ִ밪�� 255*8�̹Ƿ� 8�� ����
		}

	for(int i=0;i<in_img.width;i++) //�̹����� �����ڸ��� ������ �Һ� ���� ������� ��ü�� , x��
	{
		out_img.data[i] = out_img.data[in_img.width+i];
		out_img.data[(out_img.height-1)*out_img.width+i] = out_img.data[(out_img.height-2)*out_img.width+i];
	}
	for(int i=0;i<out_img.height;i++) //�̹����� �����ڸ��� ������ �Һ� ���� ������� ��ü�� , y��
	{
		out_img.data[i*in_img.width] = out_img.data[i*in_img.width+1];
		out_img.data[(i+1)*in_img.width-1] = out_img.data[(i+1)*in_img.width-2];
	}

	ctrlCV_img(out_img,output_func); //��� �̹����� �ܺη� ���
	delete [] in_img.data; //�ӽ� ����� �̹��� ����
	delete [] out_img.data;
	return 0;
}

int CComputerVisionDoc::make_Middle_filter(str_img &input_func,str_img &output_func,int mask_h,int mask_w) //�߰��� ����
{
	str_img in_img,out_img; //�Լ��� �Է� �̹����� ��� �̹����� �ּҰ� ���� ��츦 ����ϱ� ���� ����ü 
	reset_str_img(in_img,1); //�Է�, ��¿� ����ü �ʱ�ȭ
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //�ܺ� �Է� �̹����� ���η� �޾Ƶ���
	re_size_img(in_img,out_img); //ũ��� ä�ΰ��� ��¿� �̹����� ���� ��

	int mask_size = (2*mask_h+1)*(2*mask_w+1); //����� ����ũ�� ũ��

	for(int y=0 ; y<in_img.height ; y++) //���ð� ���� �� �߰����� ��¿� �̹����� �Է�
		for(int x = 0; x<in_img.width; x++)
		{
			int fy = (y-mask_h < 0) ? 0 : y-mask_h; //���� 4�� ��� ����ũ�� ������ ��Ÿ���� ��ǥ ��
			int ly = (y+mask_h >= in_img.height) ? in_img.height-1 : y+mask_h;
			int fx = (x-mask_w < 0) ? 0 : x-mask_w;
			int lx = (x+mask_w >= in_img.width) ? in_img.width-1 : x+mask_w;
			
			int *sample = new int[mask_size]; //���ð��� �޾Ƶ��̴� �迭
			int ctn=0 , middle_cut=0; //������ ������ ����� �߰����� ��ġ

			for(int i=fy ; i<=ly ; i++) //������ �Է� �̹����� ���� �޾Ƶ帲
				for(int j = fx; j<=lx; j++)
				{
					sample[ctn++] = in_img.data[i*in_img.width+j];
				}
			
			merge_sort(sample,0,mask_size-1); //���� ���ķ� �߰����� �˾Ƴ� 

			middle_cut=(ctn+ctn%2)/2; //����� �߰� ���� ��ġ
			out_img.data[y*out_img.width+x] = sample[middle_cut];
			delete [] sample;
		}

	ctrlCV_img(out_img,output_func); //�ܺη� �̹��� ���
	delete [] in_img.data; //�ӽ� �̹��� ����
	delete [] out_img.data;
	return 0;
}

int merge_sort(int a[],int start,int end) //���� ����
{
	if(start<end)
	{
		int mid = (start + end)/2;
		merge_sort(a,start,mid); //�迭�� 2���� ���� �� ������ �迭�� ���� ����
		merge_sort(a,mid+1,end);
		sort2array(a,start,mid,end); //���ĵ� �迭�� �ϳ��� ��ħ
	}
	return 0;
}

int sort2array(int a[],int start,int mid,int end) //���ĵ� 2���� �迭�� �ϳ��� ��ġ�� �Լ�
{
	int *output = new int[end-start+1]; //�ӽ� �迭
	int i=start;
	int j=mid+1;
	int k=0;

	while(i<=mid && j<=end) //������ �迭�� ���� ���� �� �߿��� ���� ���� ���� �ӽ� �迭�� ����
	{
		if(a[i]<=a[j])output[k++] = a[i++];
		else output[k++] = a[j++];
	}

	if(i>mid) //��� 1���� �迭�� ������� ���� �迭�� �������� ���� �ӽ� �迭�� ����
		for(int t=j;k<=end-start;k++,t++) output[k] = a[t];
	else
		for(int t=i;k<=end-start;k++,t++) output[k] = a[t];

	for(i=start;i<=end;i++)a[i]=output[i-start]; //�ӽ� �迭�� ����� ���ĵ� �����͸� ���
	delete [] output; //�ӽ� �迭 ����
	return 0;
}

int CComputerVisionDoc::delete_dot(str_img & input_func, str_img & output_func, int cut) //���غ��� ���� ũ���� ���� �̹��� ����
{
	str_img in_img,out_img; //�ӽ� ����¿� �̹���
	reset_str_img(in_img,1); //�ӽ� �̹��� �ʱ�ȭ
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); // �Է� ������ ����
	re_size_img(in_img,out_img); // ��� �������� ũ��� ä�ΰ� ����

	int *stack = new int[in_img.height*in_img.width*2]; //���� ��ġ �ӽ� ����� ����
	int *pixel = new int[in_img.height*in_img.width*2]; //������ ���� �̹����� �ȼ����� ����� ����
	int pointer=0; //������ ��ġ
	int size=0; //���ݱ��� �˾Ƴ� �ȼ��� ��

	for(int y=0 ; y<in_img.height ; y++) //������ ���� �̹����� ������ ã��
		for(int x = 0; x<in_img.width; x++)
		{
			if(in_img.data[y*in_img.width+x]==255) // ����� �� �� ���� �̹��� �߰�
			{
				pointer=1; //������ ���� ��ġ
				size=1;
				stack[2*pointer] = x; //���ؿ� ��ǥ �Է�
				stack[2*pointer+1] = y;
				pixel[2*size] = x;
				pixel[2*size+1] = y;

				in_img.data[y*in_img.width+x] = 128; //����� �ȼ� ����(�� ����)
				int yy=y,xx=x; //�� ��ǥ �αٿ� ���� ���� �̹����� �ȼ��� �ִٰ� ����

				while(pointer>0) //���� �̹����� �ȼ��� ã�������� ���� �ݺ� 
				{
					int ok=0; //�� ������ 0�� �ƴϸ� ��ǥ xx,yy ��ó�� �ȼ��� �ִ� ����
					for(int i=yy-1;i<=yy+1;i++)
						for(int j = xx-1; j<=xx+1; j++)
						{
							if(i<0 || i >=in_img.height || j<0 || j>=in_img.width) continue; //�̹��� ������ ����� ���� �ȼ��� �Ѿ��
							if(in_img.data[i*in_img.width+j]==255) //���� �̹��� �ȼ� ��ó�� �Ǵٸ� �ȼ��� ã�Ƴ���
							{
								ok=1; //�ȼ��� �ִ� ��ȣ�� �����
								pointer++;
								size++;
								stack[2*pointer] = j; //���ؿ� ��ǥ �Է�
								stack[2*pointer+1] = i;
								pixel[2*size] = j; //���� �̹����� �ȼ� ����ҿ��� ��ǥ �Է�
								pixel[2*size+1] = i;

								in_img.data[i*in_img.width+j]=128;  //����� �ȼ� ����(�� ����)
							}
						}
					if(ok!=0) {yy=stack[2*pointer+1];xx=stack[2*pointer]; } //��ó�� �ȼ��� �־����� ���������� ã�� ��ǥ ��ó�� Ž��
					else  {pointer--; yy=stack[2*pointer+1]; xx=stack[2*pointer]; } //ã�Ƴ� �ȼ��� ������ �� ���� �ȼ� ���
				}

				if(size<cut) //���� �̹����� �ȼ��� �� ã�Ƴ��� �̹����� ũ��� ���ذ��� ���Ѵ�.
				{
					for(int i=1;i<=size;i++) //���غ��� ���� �̹����̸� ��� �̹����� ����� ���� �ʴ´�.
						out_img.data[pixel[2*i+1]*in_img.width+pixel[2*i]] = 0;
				}
				else
				{
					for(int i=1;i<=size;i++) //���غ��� ū �̹����̸� ��� �̹����� ��� �Ѵ�.
						out_img.data[pixel[2*i+1]*in_img.width+pixel[2*i]] = 255;
				}
				
			}
		}

	ctrlCV_img(out_img,output_func); //�ܺη� �̹��� ���
	delete [] stack; //���ذ� �ӽ� �̹��� ����
	delete []pixel;
	delete [] in_img.data;
	delete [] out_img.data;
	return 0;
}