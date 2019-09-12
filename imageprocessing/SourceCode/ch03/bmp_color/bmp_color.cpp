/// �����α׷��� 
/// ���� 256Į�� �̻��� ��� ������ ������ �����  
#include <stdio.h>
#include <windows.h>

#define WIDTHBYTES(bits) (((bits)+31)/32*4) // ���� ���α��̴� 4����Ʈ�� ������� �� 
#define BYTE unsigned char 

void main()
{
	FILE *infile;
	infile=fopen("pshop256.bmp", "rb");
	if(infile==NULL) { printf("There is no file!!!\n"); exit(1); }

	BITMAPFILEHEADER hf;
	fread(&hf,sizeof(BITMAPFILEHEADER),1,infile); // ������带 ���� 
	if(hf.bfType!=0x4D42) exit(1); 

	BITMAPINFOHEADER hInfo;
	fread(&hInfo,sizeof(BITMAPINFOHEADER),1,infile); // ������带 ���� 
	printf("Image Size: (%3dx%3d)\n",hInfo.biWidth,hInfo.biHeight);
	printf("Pallete Type: %dbit Colors\n",hInfo.biBitCount);


	// 256Į�� ������ ���� ������� ����
	if(hInfo.biBitCount<8 ) { printf("Bad File format!!"); exit(1); }

	RGBQUAD *pRGB; 
	if(hInfo.biClrUsed!=0) // �ȷ�Ʈ�� �ִ� ��� 
	{
		pRGB= new RGBQUAD [hInfo.biClrUsed]; // �ȷ�Ʈ�� ũ�⸸ŭ �޸𸮸� �Ҵ��� 
		fread(pRGB,sizeof(RGBQUAD),hInfo.biClrUsed,infile); // �ȷ�Ʈ�� ���Ͽ��� ���� 
	}

	// ������Ÿ�� ������ �޸� �Ҵ� 
	BYTE *lpImg = new BYTE [hInfo.biSizeImage];
	fread(lpImg,sizeof(char),hInfo.biSizeImage,infile); 
	fclose(infile);

	// ������ �̹��� ���ϱ� 
	int rwsize = WIDTHBYTES(hInfo.biBitCount*hInfo.biWidth);
	int rwsize2= WIDTHBYTES(24*hInfo.biWidth); 
	BYTE *lpOutImg = new BYTE [3*rwsize*hInfo.biHeight];
	int index, R, G, B, i,j;


	if(hInfo.biBitCount==24) // ���� �Է¿����� Ʈ��(24��Ʈ) Į���� ���  
	for(i=0; i<hInfo.biHeight; i++)
	{
		for(j=0; j<hInfo.biWidth; j++) 
		{   // �ȷ�Ʈ�� �����Ƿ� ������Ÿ�� �ٷ� Į�� 
			lpOutImg[i*rwsize2+3*j+2] = 255-lpImg[i*rwsize+3*j+2];
			lpOutImg[i*rwsize2+3*j+1] = 255-lpImg[i*rwsize+3*j+1];
			lpOutImg[i*rwsize2+3*j  ] = 255-lpImg[i*rwsize+3*j  ];
		}
	}
	else // Ʈ��Į�� �ƴ� ��� 
	for(i=0; i<hInfo.biHeight; i++)
	{
		for(j=0; j<hInfo.biWidth; j++) 
		{
			index = lpImg[i*rwsize+j]; // ������Ÿ�� �ȷ�Ʈ�� �ε����� 
			R = pRGB[index].rgbRed;    // �ȷ�Ʈ���� ���� ������Ÿ�� ������(R) 
			G = pRGB[index].rgbGreen;  // G
			B = pRGB[index].rgbBlue;   // B
			R = 255-R; G = 255-G; B = 255-B; // ������ ����� 
			lpOutImg[i*rwsize2+3*j+2] = (BYTE)R; 
			lpOutImg[i*rwsize2+3*j+1] = (BYTE)G;
			lpOutImg[i*rwsize2+3*j  ] = (BYTE)B;
		}
	}

	// ���� ��� (24��Ʈ�� Ʈ��Į��� ���) 
	hInfo.biBitCount =24;
	hInfo.biSizeImage = 3*rwsize*hInfo.biHeight;
	hInfo.biClrUsed = hInfo.biClrImportant =0;
	hf.bfOffBits = 54; // �ȷ�Ʈ�� �����Ƿ� ���� ���� 
	hf.bfSize = hf.bfOffBits+hInfo.biSizeImage;

	FILE *outfile = fopen("OutImg24.bmp","wb");
	fwrite(&hf,sizeof(char),sizeof(BITMAPFILEHEADER),outfile);
	fwrite(&hInfo,sizeof(char),sizeof(BITMAPINFOHEADER),outfile);
	fwrite(lpOutImg,sizeof(char),3*rwsize*hInfo.biHeight,outfile); 
	fclose(outfile);


	// �޸� ���� 
	if(hInfo.biClrUsed!=0) delete []pRGB;
	delete []lpOutImg;
	delete []lpImg;
}





