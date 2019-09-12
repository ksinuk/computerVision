#include <opencv2/opencv.hpp> 

int main()
{
	int i, j;
	unsigned char OrgImg[256][256];

	IplImage *img=cvCreateImage(cvSize(256,256), 8, 1);  

	// ��ũ���� raw �������� �б�
	FILE *infile = fopen("Mandrill_homework.raw","rb");
	if(infile==NULL) { printf("File open error !!"); return 0; }
	fread(OrgImg,sizeof(char),256*256,infile);
	fclose(infile);

	// �������� ���� ����ó�� 
	for(int y=0; y<256; y++)
	{
		uchar* ptr = (uchar*)(img->imageData + y*img->widthStep);
		for(int x=0; x<256; x++) 
		{
			ptr[x] = OrgImg[y][x];
		}
	}

	cvSaveImage("Mandrill_homework.bmp", img);
	cvReleaseImage(&img);
	return 0;
}