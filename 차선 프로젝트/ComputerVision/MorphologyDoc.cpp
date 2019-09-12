
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


int CComputerVisionDoc::make_TopHat(str_img &input_func,str_img &output_func,int mask_h,int mask_w) //Top - Hat ����
{
	str_img in_img,out_img; //�Լ��� �Է� �̹����� ��� �̹����� �ּҰ� ���� ��츦 ����ϱ� ���� �ӽ� ����� �̹���
	reset_str_img(in_img,1); //�Է�, ��¿� ����ü �ʱ�ȭ
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //�ܺ� �Է� �̹����� ���η� �޾Ƶ���
	re_size_img(in_img,out_img); //ũ��� ä�ΰ��� ��¿� �̹����� ���� ��

	str_img temp_opening; // �ӽ� �̹��� �ʱ�ȭ
	reset_str_img(temp_opening,1);
	re_size_img(in_img,temp_opening);
	
	make_opening(in_img , temp_opening , mask_h , mask_w); //�ӽ� �̹����� �Է� �̹����� ������ ���갪�� �־� ��

	for(int y=0;y<out_img.height;y++) //�Է� �̹����� ������ ���� �̹����� ������ ���� ���� 
		for(int x = 0; x<out_img.width; x++)
		{
			int tmep_int = in_img.data[y*in_img.width+x] - temp_opening.data[y*temp_opening.width+x];
			out_img.data[y*out_img.width+x] = (tmep_int>0) ? tmep_int:0; //�Է� �̹����� �ȼ� ������ ������ �ȼ��� ũ�� �� �� �׸� 0���� �����
		}

	delete [] temp_opening.data;//�ӽ� ������ �̹��� ����
	ctrlCV_img(out_img,output_func); //�ܺη� ����� ���
	delete [] in_img.data; //����� �ӽ� �̹��� ����
	delete [] out_img.data;		
	return 0;
}

int CComputerVisionDoc::make_DownHat(str_img &input_func,str_img &output_func,int mask_h,int mask_w) // Down Hat ����
{
	str_img in_img,out_img; //�Լ��� �Է� �̹����� ��� �̹����� �ּҰ� ���� ��츦 ����ϱ� ���� �ӽ� ����� �̹���
	reset_str_img(in_img,1); //�Է�, ��¿� ����ü �ʱ�ȭ
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //�ܺ� �Է� �̹����� ���η� �޾Ƶ���
	re_size_img(in_img,out_img); //ũ��� ä�ΰ��� ��¿� �̹����� ���� ��

	str_img temp_closing; // �ӽ� �̹��� ������¡
	reset_str_img(temp_closing,1);
	re_size_img(in_img,temp_closing);
	
	make_closing(in_img , temp_closing , mask_h , mask_w); //close ������ �ӽ� �̹����� ����

	for(int y=0;y<out_img.height;y++) //close ���� �̹����� �Է� �̹����� ���� ��� �̹����� ����� ��
		for(int x = 0; x<out_img.width; x++)
		{
			int tmep_int = (temp_closing.data[y*temp_closing.width+x] - in_img.data[y*in_img.width+x]);
			out_img.data[y*out_img.width+x] = (tmep_int>0) ? tmep_int:0;
		}

	delete [] temp_closing.data; //�ӽ� �̹��� ����
	ctrlCV_img(out_img,output_func); //�ܺη� ����� ���
	delete [] in_img.data; //�ӽ� ����� �̹��� ����
	delete [] out_img.data;		
	return 0;
}

int CComputerVisionDoc::make_opening(str_img &in_img, str_img &out_img,int mask_h,int mask_w) //opening ����
{
	str_img temp_Erosion; // �ӽ� �̹��� ������¡
	reset_str_img(temp_Erosion,1);
	re_size_img(in_img,temp_Erosion);

	make_Erosion(in_img, temp_Erosion, mask_h, mask_w); //ħ�� ����
	make_Dilation(temp_Erosion, out_img, mask_h, mask_w); //��â ����

	delete [] temp_Erosion.data; //�ӽ� �̹��� ����

	return 0;
}

int CComputerVisionDoc::make_closing(str_img &in_img, str_img &out_img,int mask_h,int mask_w) //closing ����
{
	str_img temp_Dilation; // �ӽ� �̹��� ������¡
	reset_str_img(temp_Dilation,1);
	re_size_img(in_img,temp_Dilation);

	make_Dilation(in_img, temp_Dilation, mask_h, mask_w); //��â ����
	make_Erosion(temp_Dilation, out_img, mask_h, mask_w); //ħ�� ����
	 
	delete [] temp_Dilation.data; //�ӽ� �̹��� ����

	return 0;
}

int CComputerVisionDoc::make_Erosion(str_img &input_func,str_img &output_func,int mask_h,int mask_w) //ħ�� ����
{
	str_img in_img,out_img; //�Լ��� �Է� �̹����� ��� �̹����� �ּҰ� ���� ��츦 ����ϱ� ���� �ӽ� ����� �̹���
	reset_str_img(in_img,1); //�Է�, ��¿� ����ü �ʱ�ȭ
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //�ܺ� �Է� �̹����� ���η� �޾Ƶ���
	re_size_img(in_img,out_img); //ũ��� ä�ΰ��� ��¿� �̹����� ���� ��

	int *min_array_w = new int[in_img.height*in_img.width]; //ħ�� ������ �Ϻκ��� �ӽ� �����ϴ� ����
	
	for(int y=0 ; y<in_img.height ; y++) //�������� ���� y���� ���� �ȼ����� �ּҰ��� ���Ѵ�.
		for(int x = 0; x<in_img.width; x++)
		{
			int fx = (x-mask_w < 0) ? 0 : x-mask_w; //��ǥ x�� ������ ����ũ �� �̳��� �Ѵ�.
			int lx = (x+mask_w >= in_img.width) ? in_img.width-1 : x+mask_w;
			int min=255;

			for(int i = fx; i<=lx; i++)
			{
				if(min>in_img.data[y*in_img.width+i]) min = in_img.data[y*in_img.width+i];
			}
			min_array_w[y*out_img.width+x] = min; //���� ���� �ּҰ��� �ӽ� �迭�� �����Ѵ�.
		}

	for(int y=0 ; y<in_img.height ; y++) // �ӽ� ���� ������ �̿��ؼ� ������ ��ǥ�� ħ�� ���� ���� �˾� ����.
		for(int x = 0; x<in_img.width; x++)
		{
			int fy = (y-mask_h < 0) ? 0 : y-mask_h;
			int ly = (y+mask_h >= in_img.height) ? in_img.height-1 : y+mask_h;
			int min=255;

			for(int i=fy ; i<=ly ; i++) //min_array_w���� �������� ���� x���� ������ ������ �ּ� ���� ���Ѵ�.
			{
				if(min>min_array_w[i*in_img.width+x]) min = min_array_w[i*in_img.width+x]; 
			}
			out_img.data[y*out_img.width+x] = min; //���� ���갪�� ��� �̹����� �����Ѵ�.
		}

	delete [] min_array_w; //�ӽ� ����� ����
	ctrlCV_img(out_img,output_func); //�ܺη� ����� ���
	delete [] in_img.data; //�ӽ� ����� �̹��� ����
	delete [] out_img.data;	
	return 0;
}

int CComputerVisionDoc::make_Dilation(str_img &input_func,str_img &output_func,int mask_h,int mask_w) //��â ����
{
	str_img in_img,out_img; //�Լ��� �Է� �̹����� ��� �̹����� �ּҰ� ���� ��츦 ����ϱ� ���� �ӽ� ����� �̹���
	reset_str_img(in_img,1); //�Է�, ��¿� ����ü �ʱ�ȭ
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //�ܺ� �Է� �̹����� ���η� �޾Ƶ���
	re_size_img(in_img,out_img); //ũ��� ä�ΰ��� ��¿� �̹����� ���� ��

	unsigned char *max_array_w = new unsigned char[in_img.height*in_img.width]; //��â ������ �Ϻκ��� �ӽ� �����ϴ� ���� 
	
	for(int y=0 ; y<in_img.height ; y++) //�������� ���� y���� ���� �ȼ����� �ִ밪�� ���Ѵ�.
		for(int x = 0; x<in_img.width; x++)
		{
			int fx = (x-mask_w < 0) ? 0 : x-mask_w; //��ǥ x�� ������ ����ũ �� �̳��� �Ѵ�.
			int lx = (x+mask_w >= in_img.width) ? in_img.width-1 : x+mask_w;
			int max=0;

			for(int i = fx; i<=lx; i++)
			{
				if(max<in_img.data[y*in_img.width+i]) max = in_img.data[y*in_img.width+i];
			}
			max_array_w[y*out_img.width+x] = max; //���� ���� �ִ밪�� �ӽ� �迭�� �����Ѵ�.
		}

	for(int y=0 ; y<in_img.height ; y++) // �ӽ� ���� ������ �̿��ؼ� ������ ��ǥ�� ��â ���� ���� �˾� ����.
		for(int x = 0; x<in_img.width; x++)
		{
			int fy = (y-mask_h < 0) ? 0 : y-mask_h;
			int ly = (y+mask_h >= in_img.height) ? in_img.height-1 : y+mask_h;
			int max=0;

			for(int i=fy ; i<=ly ; i++) //max_array_w���� �������� ���� x���� ������ ������ �ּ� ���� ���Ѵ�.
			{
				if(max<max_array_w[i*in_img.width+x]) max = max_array_w[i*in_img.width+x];
			}
			out_img.data[y*out_img.width+x] = max; //���� ���갪�� ��� �̹����� �����Ѵ�.
		}

	delete [] max_array_w; //�ӽ� ����� ����
	ctrlCV_img(out_img,output_func); //�ܺη� ����� ���
	delete [] in_img.data; //�ӽ� ����� �̹��� ����
	delete [] out_img.data;		
	return 0;
}