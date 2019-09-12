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

int CComputerVisionDoc::make_Hough_Transform(str_img &input_func,str_img &graph,str_img &output_func) //���� ����� ���� ��ȯ �Լ�
{
	str_img inimg,outimg; //�Լ��� �Է� �̹����� ��� �̹����� �ּҰ� ���� ��츦 ����ϱ� ���� �ӽ� ����� �̹���
	reset_str_img(inimg,1); //�Է�, ��¿� ����ü �ʱ�ȭ
	reset_str_img(outimg,1);
	ctrlCV_img(input_func,inimg); //�ܺ� �Է� �̹����� ���η� �޾Ƶ���
	re_size_img(inimg,outimg); //ũ��� ä�ΰ��� ��¿� �̹����� ���� ��

	int d_size = sqrt(inimg.height*inimg.height + inimg.width*inimg.width); //�밢���� ����
	double fcos[3600],fsin[3600],p2d = 3.1415926535/180.0; //�ﰢ�Լ��� ���� �����ϴ� �迭�� ���ʵ�� ������ rad ������ ��ȯ�ϴ� ����
	int rad_size = 3600; //������ 0.1�� ���� ��ȯ
	float rad_unit = 10; //���� 1���� ����µ� �ʿ��� ��
	int thres = 120; //���� ���θ� �����ϴ� ������

	int *hough = new int[d_size*rad_size]; //���� ���� �����
	for(int d=0;d<d_size;d++) //���� ����� �ʱ�ȭ
		for(int r = 0; r<rad_size; r++)
			hough[d*rad_size+r] = 0;

	for(int y=0;y<outimg.height;y++) //��� �̹��� �ʱ�ȭ
		for(int x=0;x<outimg.width;x++)
			outimg.data[y*outimg.width+x] = 0;

	for(int i = 0; i<rad_size; i++) //�ﰢ�Լ��� �̸� ���
	{
		fcos[i] = cos(p2d*(float)i/rad_unit);
		fsin[i] = sin(p2d*(float)i/rad_unit);
	}

	for(int y=0;y<inimg.height;y++) //���� �̹������� ������ �������� ���� ���ϱ�
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

	int mask_d = 25 , mask_r = 65; //����� ������ ���� ������ ������ �߿��� ���� ���� �ȼ��� ������ ������ ����Ѵ�.
	for(int d=0;d<d_size;d++)
		for(int r = 0; r<rad_size; r++)
		{
			if(hough[d*rad_size+r]>0)
			{
				int fd = (d<mask_d) ? 0 : d-mask_d; //�� ����� �������� ����
				int ld = (d+mask_d>d_size-1) ? d_size-1 : d+mask_d;
				int fr = (r<mask_r) ? 0 : r-mask_r;
				int lr = (r+mask_r>rad_size-1) ? rad_size-1 : r+mask_r;

				for(int i=fd;i<=ld;i++)
					for(int j = fr; j<=lr; j++)
					{
						if(hough[i*rad_size+j]!=0)
						{    //�ֺ��� �ڽź��� �� ���� �ȼ��� ������ ������ ������ �ڱ� �ڽ��� ������ �����Ѵ�.
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
	FILE *fd = fopen("project_road_line.txt","w+"); //���� �Ķ���� ��¿� ����

	for(int d=0;d<d_size;d++) 
		for(int r = 0; r<rad_size; r++)
		{
			if(hough[d*rad_size+r]>80)  //�ȼ� ���� 80 �̻��� ������ ����Ѵ�.
			{
				//������ ���� �� �ִ� �ȼ��� �ִ밪�� ���� �ȼ��� ���� ���Ѵ�.

				float fh = d/fsin[r]; //x���� 0�� �� y��
				float lh = (d-fcos[r]*outimg.width)/fsin[r]; //x���� �ִ밪 �� �� y��
				float fw = d/fcos[r]; //y���� 0�� �� x��
				float lw = (d-fsin[r]*outimg.height)/fcos[r]; //y���� �ִ밪 �� �� x��

				int box[4]={0,0,0,0};
				if(fh>0 && fh<outimg.height)box[0]=1; //x���� 0�� �� y���� �̹��� ���ο� �ִ�.
				if(lh>0 && lh<outimg.height)box[1]=1; //x���� �ִ밪 �� �� y���� �̹��� ���ο� �ִ�.
				if(fw>0 && fw<outimg.width)box[2]=1; //y���� 0�� �� x���� �̹��� ���ο� �ִ�.
				if(lw>0 && lw<outimg.width)box[3]=1; //y���� �ִ밪 �� �� x���� �̹��� ���ο� �ִ�.

				float temp_th=0; //�̹��� ���ο� �ִ� ������ ����
				//������ ��쿡 �̹��� ���ο� �ִ� ������ ���̸� ���Ѵ�. 
				if(box[0]!=0 && box[1]!=0) temp_th = (lh-fh)*(lh-fh) + outimg.width*outimg.width;
				else if(box[0]!=0 && box[2]!=0) temp_th = fh*fh+fw*fw;
				else if(box[0]!=0 && box[3]!=0) temp_th = (outimg.height-fh)*(outimg.height-fh)+lw*lw;
				else if(box[1]!=0 && box[2]!=0) temp_th = lh*lh + (outimg.width-fw)*(outimg.width-fw);
				else if(box[1]!=0 && box[3]!=0) temp_th = (outimg.height-lh)*(outimg.height-lh) + (outimg.width-lw)*(outimg.width-lw);
				else if(box[2]!=0 && box[3]!=0) temp_th = (lw-fw)*(lw-fw) + outimg.height*outimg.height;
				else continue; 

				thres = sqrt(temp_th)*0.5; //�ȼ��� ���� ������ ������ ������ �Ǿ�� �Ѵ�.

				if(hough[d*rad_size+r]>thres) //�����ϴ� �伱(����)�� ã�Ҵ�.
				{
					double out_d = d+(fcos[r]*20.0+fsin[r]*240.0); //�߶� �̹������� ���� �̹����� ����

					fprintf(fd,"%.10lf\t%.10lf\t%.10lf\n",-fcos[r]/out_d,-fsin[r]/out_d,1.0); //�ؽ�Ʈ ���Ͽ� ������ �Ķ���͸� �Է��Ѵ�.
					for(int y = 0; y<outimg.height; y++) //��¿� �̹����� ������ �׸���.
					{
						int x = (d-y*fsin[r])/fcos[r];
						if(x>=0 && x<outimg.width) outimg.data[y*outimg.width+x] = 255;
					}
					for(int x = 0; x<outimg.width; x++) //��¿� �̹����� ������ �׸���.
					{
						int y = (d-x*fcos[r])/fsin[r];
						if(y>=0 && y<outimg.height) outimg.data[y*outimg.width+x] = 255;
					}
				}
			}
		}

	fclose(fd);
	delete [] hough; //�ӽ� �̹��� ����
	ctrlCV_img(outimg,output_func); //�ܺη� ����� ���
	delete [] inimg.data; //�ӽ� ����� �̹��� ����
	delete [] outimg.data;
	return 0;
}