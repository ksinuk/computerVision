/// 흑백 BMP 영상의 역상을 계산  
#include <stdio.h>
#include <windows.h>
#define WIDTHBYTES(bits) (((bits)+31)/32*4) // 영상 가로길이는 4바이트의 배수여야 함 
#define BYTE unsigned char 

void main()
{
	FILE *infile;
	infile=fopen("talent.bmp", "rb");
	if(infile==NULL) { printf("There is no file!!!\n"); exit(1); }

	BITMAPFILEHEADER hf;
	fread(&hf,sizeof(BITMAPFILEHEADER),1,infile); // 파일헤드를 읽음 
	if(hf.bfType!=0x4D42) exit(1); 

	BITMAPINFOHEADER hInfo;
	fread(&hInfo,sizeof(BITMAPINFOHEADER),1,infile); // 영상헤드를 읽음 
	printf("Image Size: (%3dx%3d)\n",hInfo.biWidth,hInfo.biHeight);
	printf("Pallete Type: %dbit Colors\n",hInfo.biBitCount);


	// 256칼라 이하의 경우는 취급하지 않음
	if(hInfo.biBitCount!=8 ) { printf("Bad File format!!"); exit(1); }

	RGBQUAD hRGB[256]; 
	fread(hRGB,sizeof(RGBQUAD),256,infile); // 팔레트를 파일에서 읽음 

	// 영상데이타를 저장할 메모리 할당 
	BYTE *lpImg = new BYTE [hInfo.biSizeImage];
	fread(lpImg,sizeof(char),hInfo.biSizeImage,infile); 
	fclose(infile);

	// 역상의 이미지 구하기 
	int rwsize = WIDTHBYTES(hInfo.biBitCount*hInfo.biWidth);

	for(int i=0; i<hInfo.biHeight; i++)
	{
		for(int j=0; j<hInfo.biWidth; j++) 
		{   
			lpImg[i*rwsize+j] = 255-lpImg[i*rwsize+j];
		}
	}

	// 영상 출력 
	FILE *outfile = fopen("OutImg.bmp","wb");
	fwrite(&hf,sizeof(char),sizeof(BITMAPFILEHEADER),outfile);
	fwrite(&hInfo,sizeof(char),sizeof(BITMAPINFOHEADER),outfile);
	fwrite(hRGB,sizeof(RGBQUAD),256,outfile);
	fwrite(lpImg,sizeof(char),hInfo.biSizeImage,outfile); 
	fclose(outfile);


	// 메모리 해제 
	delete []lpImg;
}





