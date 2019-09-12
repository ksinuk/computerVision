/// 이프로그램은 
/// 흑백및 256칼라 이상의 모든 영상의 역상을 계산함  
#include <stdio.h>
#include <windows.h>

#define WIDTHBYTES(bits) (((bits)+31)/32*4) // 영상 가로길이는 4바이트의 배수여야 함 
#define BYTE unsigned char 

void main()
{
	FILE *infile;
	infile=fopen("pshop256.bmp", "rb");
	if(infile==NULL) { printf("There is no file!!!\n"); exit(1); }

	BITMAPFILEHEADER hf;
	fread(&hf,sizeof(BITMAPFILEHEADER),1,infile); // 파일헤드를 읽음 
	if(hf.bfType!=0x4D42) exit(1); 

	BITMAPINFOHEADER hInfo;
	fread(&hInfo,sizeof(BITMAPINFOHEADER),1,infile); // 영상헤드를 읽음 
	printf("Image Size: (%3dx%3d)\n",hInfo.biWidth,hInfo.biHeight);
	printf("Pallete Type: %dbit Colors\n",hInfo.biBitCount);


	// 256칼라 이하의 경우는 취급하지 않음
	if(hInfo.biBitCount<8 ) { printf("Bad File format!!"); exit(1); }

	RGBQUAD *pRGB; 
	if(hInfo.biClrUsed!=0) // 팔레트가 있는 경우 
	{
		pRGB= new RGBQUAD [hInfo.biClrUsed]; // 팔레트의 크기만큼 메모리를 할당함 
		fread(pRGB,sizeof(RGBQUAD),hInfo.biClrUsed,infile); // 팔레트를 파일에서 읽음 
	}

	// 영상데이타를 저장할 메모리 할당 
	BYTE *lpImg = new BYTE [hInfo.biSizeImage];
	fread(lpImg,sizeof(char),hInfo.biSizeImage,infile); 
	fclose(infile);

	// 역상의 이미지 구하기 
	int rwsize = WIDTHBYTES(hInfo.biBitCount*hInfo.biWidth);
	int rwsize2= WIDTHBYTES(24*hInfo.biWidth); 
	BYTE *lpOutImg = new BYTE [3*rwsize*hInfo.biHeight];
	int index, R, G, B, i,j;


	if(hInfo.biBitCount==24) // 만일 입력영상이 트루(24비트) 칼라인 경우  
	for(i=0; i<hInfo.biHeight; i++)
	{
		for(j=0; j<hInfo.biWidth; j++) 
		{   // 팔레트가 없으므로 영상데이타가 바로 칼라값 
			lpOutImg[i*rwsize2+3*j+2] = 255-lpImg[i*rwsize+3*j+2];
			lpOutImg[i*rwsize2+3*j+1] = 255-lpImg[i*rwsize+3*j+1];
			lpOutImg[i*rwsize2+3*j  ] = 255-lpImg[i*rwsize+3*j  ];
		}
	}
	else // 트루칼라가 아닌 경우 
	for(i=0; i<hInfo.biHeight; i++)
	{
		for(j=0; j<hInfo.biWidth; j++) 
		{
			index = lpImg[i*rwsize+j]; // 영상데이타는 팔레트의 인덱스임 
			R = pRGB[index].rgbRed;    // 팔레트에서 실제 영상데이타를 가져옴(R) 
			G = pRGB[index].rgbGreen;  // G
			B = pRGB[index].rgbBlue;   // B
			R = 255-R; G = 255-G; B = 255-B; // 역상을 계산함 
			lpOutImg[i*rwsize2+3*j+2] = (BYTE)R; 
			lpOutImg[i*rwsize2+3*j+1] = (BYTE)G;
			lpOutImg[i*rwsize2+3*j  ] = (BYTE)B;
		}
	}

	// 영상 출력 (24비트인 트루칼라로 출력) 
	hInfo.biBitCount =24;
	hInfo.biSizeImage = 3*rwsize*hInfo.biHeight;
	hInfo.biClrUsed = hInfo.biClrImportant =0;
	hf.bfOffBits = 54; // 팔레트가 없으므로 값이 변함 
	hf.bfSize = hf.bfOffBits+hInfo.biSizeImage;

	FILE *outfile = fopen("OutImg24.bmp","wb");
	fwrite(&hf,sizeof(char),sizeof(BITMAPFILEHEADER),outfile);
	fwrite(&hInfo,sizeof(char),sizeof(BITMAPINFOHEADER),outfile);
	fwrite(lpOutImg,sizeof(char),3*rwsize*hInfo.biHeight,outfile); 
	fclose(outfile);


	// 메모리 해제 
	if(hInfo.biClrUsed!=0) delete []pRGB;
	delete []lpOutImg;
	delete []lpImg;
}





