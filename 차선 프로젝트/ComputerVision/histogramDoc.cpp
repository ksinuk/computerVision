
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


int CComputerVisionDoc::make_histogram(int* histogram,str_img &func_image_) // ������׷� ���
{
	for(int i=0;i<256;i++)histogram[i]=0;//������׷� �ʱ�ȭ
	for(int y=0;y<func_image_.height;y++)
		for(int x = 0; x<func_image_.width; x++)
			histogram[func_image_.data[y*func_image_.width+x]]++;

	return 0;
}

int CComputerVisionDoc::search_Otzu(int* histogram) // Otzu �˰����� �̿��� ������ ���
{
	int Otzu_t=0; //���� �̹����� ���鶧 �� ���� �������� �Ѵ�.
	float Otzu_min=10e10;

	for(int t=0; t<256; t++)
	{
		// ����Ȯ��q1, q2 ���
		float q1 = 0, q2 = 0;
		for(int i=0; i<t; i++)
			q1 += histogram[i];
		for(int i=t; i<256; i++) 
			q2 += histogram[i];
		if(q1==0 || q2==0) continue;

		// ���u1, u2 ���
		float u1=0.0f, u2=0.0f;
		for(int i=0; i<t; i++) u1 += i*histogram[i]; u1 /= q1;
		for(int i=t; i<256; i++) u2 += i*histogram[i]; u2 /= q2;

		// �л�s1, s2 ���
		float s1=0.0f, s2=0.0f;
		for(int i=0; i<t; i++) s1 += (i-u1)*(i-u1)*histogram[i]; s1 /= q1;
		for(int i=t; i<256; i++) s2 += (i-u2)*(i-u2)*histogram[i]; s2 /= q2;

		//�Ѻл���
		float wsv = q1*s1+q2*s2;
		if(wsv <  Otzu_min)
		{
			Otzu_min = wsv;
			Otzu_t= t; // �ּ�ġ����
		}
	}


	return  Otzu_t;
}

int CComputerVisionDoc::make_binary_img(int bin_cut,str_img &in_img,str_img &out_img) //���ذ��� �޾Ƽ� �̹��� ����ȭ 
{
	if(out_img.data==NULL){ AfxMessageBox(_T("����ִ� �̹��� : make_binary_img")); return -1; }

	for(int y=0;y<out_img.height;y++)
		for(int x = 0; x<out_img.width; x++)
			out_img.data[y*out_img.width+x] = (in_img.data[y*in_img.width+x]>bin_cut) ? 255:0;

	return 0;
}

int CComputerVisionDoc::binary_niblack(str_img & input_func, str_img & output_func) //���� ��ġȭ ���
{
	str_img in_img,out_img; //�Լ��� �Է� �̹����� ��� �̹����� �ּҰ� ���� ��츦 ����ϱ� ���� �ӽ� ����� �̹���
	reset_str_img(in_img,1); //�Է�, ��¿� ����ü �ʱ�ȭ
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //�ܺ� �Է� �̹����� ���η� �޾Ƶ���
	re_size_img(in_img,out_img); //ũ��� ä�ΰ��� ��¿� �̹����� ���� ��

	int lut_ssum[256]; //�������� �̸� ����Ͽ� ����
	for(int i=0;i<256;i++)lut_ssum[i] = i*i;

	int *sum_x = new int[in_img.width*in_img.height]; //����� �Ϻκ��� �̸� �����ϴ� ����
	for(int i = 0; i<in_img.width*in_img.height; i++) sum_x[i] = 0;
	int *ssum_x = new int[in_img.width*in_img.height];
	for(int i = 0; i<in_img.width*in_img.height; i++) ssum_x[i] = 0;

	int w=20;

	for(int y=0;y<in_img.height;y++) //������ �ȼ��� x�� ���� ������ �ȼ��� �հ� �������� ���
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

	for(int y=0;y<in_img.height;y++) //x�� ���������� ����� ������ y�� �������� ����� �� ��հ� �а��� �̿��Ͽ� ����� ���
		for(int x = 0; x<in_img.width; x++)
		{
			int sum=0.0,ssum=0.0;

			int yl = (y-w>0) ? y-w : 0; //����ũ�� ����
			int yh = (y+w< in_img.height) ? y+w : in_img.height-1;
			int xl = (x-w>0) ? x-w : 0;
			int xh = (x+w< in_img.width) ? x+w : in_img.width-1;
			int count=(yh-yl+1)*(xh-xl+1);

			for(int i=yl;i<=yh;i++) //����ũ ������ �ȼ��� �հ� ������ ���� ���
			{
				sum += sum_x[i*in_img.width+x];
				ssum += ssum_x[i*in_img.width+x];
			}

			float m = (float)sum/(float)count; //���
			float v = (float)ssum/(float)count -m*m; //�л�
			float thres = m*(1.0 - 0.02*(1+sqrt(v)/128.0)); //���ذ�

			out_img.data[y*out_img.width+x] = (in_img.data[y*in_img.width+x] < thres) ? 0 : 255; //����ȭ
		}

	ctrlCV_img(out_img,output_func); //�ܺη� ����� ���
	delete [] in_img.data; //�ӽ� ����� �̹��� ����
	delete [] out_img.data;
	return 0;
}

int CComputerVisionDoc::make_histogram_Equal(str_img & input_func, str_img & output_func) //������׷� ��Ȱȭ ���
{
	str_img in_img,out_img; //�Լ��� �Է� �̹����� ��� �̹����� �ּҰ� ���� ��츦 ����ϱ� ���� �ӽ� ����� �̹���
	reset_str_img(in_img,1); //�Է�, ��¿� ����ü �ʱ�ȭ
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //�ܺ� �Է� �̹����� ���η� �޾Ƶ���
	re_size_img(in_img,out_img); //ũ��� ä�ΰ��� ��¿� �̹����� ���� ��

	int histogram[256]; //������׷� ��갪
	make_histogram(histogram,in_img); //������׷� ��� �Լ�
	float Nt = in_img.height*in_img.width-histogram[0]; //�ȼ����� 0�� �ƴ� �ȼ��� ��
	
	for(int i=2;i<256;i++)histogram[i] += histogram[i-1]; //�ȼ� �� 1 - 255 ���� �ȿ��� ���������� ���
	for(int i=1;i<256;i++)histogram[i] = (float)histogram[i]*255.0/Nt; //���� �������� �ִ밪�� 255�� ����
	histogram[0]=0; //�ȼ� ���� 0�� �ȼ��� ���ܷ� ����� 0���� �Ѵ�.

	for(int y=0 ; y<out_img.height ;y++) //����� �����
		for(int x=0 ; x<out_img.width ; x++)
			out_img.data[y*out_img.width+x] = histogram[in_img.data[y*in_img.width+x]];

	ctrlCV_img(out_img,output_func); //�ܺη� ����� ���
	delete [] in_img.data; //�ӽ� ����� �̹��� ����
	delete [] out_img.data;
	return 0;
}

int CComputerVisionDoc::make_histogram_Strach(str_img & input_func, str_img & output_func) //������׷� ��ũ��ġ ���
{
	str_img in_img,out_img; //�Լ��� �Է� �̹����� ��� �̹����� �ּҰ� ���� ��츦 ����ϱ� ���� �ӽ� ����� �̹���
	reset_str_img(in_img,1); //�Է�, ��¿� ����ü �ʱ�ȭ
	reset_str_img(out_img,1);
	ctrlCV_img(input_func,in_img); //�ܺ� �Է� �̹����� ���η� �޾Ƶ���
	re_size_img(in_img,out_img); //ũ��� ä�ΰ��� ��¿� �̹����� ���� ��

	int histogram[256]; //������׷� ��갪
	make_histogram(histogram,in_img); //������׷� ��� �Լ�
	float Nt = in_img.height*in_img.width-histogram[0]; //�ȼ����� 0�� �ƴ� �ȼ��� ��
	int low_cut = Nt*0.5 , high_cut = Nt*0.1 , sum = 0;//�ּ�, �ִ� ���ذ��� ������׷��� ������
	int low_i=-1 , high_i = 0; //�ȼ����� �� ������ �۰ų� ũ�� ������� 0�̳� 255�� �Ѵ�.

	for(int i = 1; i<256; i++) //�ּ� ���ذ� ���ϱ�
	{
		sum+=histogram[i];
		if(low_cut<=sum){low_i = i;sum=0;break;}
	}
	for(int i = 255; i>=1; i--) //�ִ� ���ذ� ���ϱ�
	{
		sum+=histogram[i];
		if(high_cut<=sum){high_i = i;break;}
	}
	for(int i = 0; i<low_i; i++) histogram[i]=0; //������׷� �迭�� ��°� �Է�
	for(int i = low_i; i<high_i; i++) histogram[i]=255.0*(float)(i-low_i)/(float)(high_i - low_i);
	for(int i = high_i; i<256; i++) histogram[i]=255;

	for(int y=0 ; y<out_img.height ;y++) //�ȼ��� ���� ������׷� �迭�� �־ ����� ��°��� ���
		for(int x=0 ; x<out_img.width ; x++)
			out_img.data[y*out_img.width+x] = histogram[in_img.data[y*in_img.width+x]];

	ctrlCV_img(out_img,output_func); //�ܺη� ����� ���
	delete [] in_img.data; //�ӽ� ����� �̹��� ����
	delete [] out_img.data;
	return 0;
}