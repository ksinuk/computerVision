// FileIO24.cpp: implementation of the CFileIO24 class.
//
//////////////////////////////////////////////////////////////////////

#include "FileIO24.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileIO24::CFileIO24()
{
	orgImg = NULL;
	outImg = NULL;
	hRGB   = NULL;
	
	height = width = 0;
}

CFileIO24::~CFileIO24()
{
	if(orgImg) delete []orgImg;
	if(outImg) delete []outImg;
	if(hRGB)   delete []hRGB;
}


ColorImg* CFileIO24::GetOutImg()
{
	return outImg;
}

ColorImg* CFileIO24::GetOrgImg()
{
	return orgImg;
}	

int CFileIO24::GetHeight()
{
	return height;
}

int CFileIO24::GetWidth()
{
	return width;
}

CFileIO24::CFileIO24(char *inbuff)
{
	orgImg = NULL;
	outImg = NULL;
	hRGB   = NULL;
	height = width = 0;

	ReadBMP24(inbuff);
}

void CFileIO24::ReadBMP24(char *inbuff)
{
//	printf("VisLAB(R), Made by D.J.Kang, 2007\n\n");
	FILE	*infile;
	if((infile = fopen(inbuff, "rb")) == NULL) 
	{ 
	 	printf("\nCan't open %s \n", inbuff);
		return;
	}


	GetBMPHeaderInfo(infile);
	
	orgImg = new ColorImg [height*width];
	outImg = new ColorImg [height*width];


	register int i, j, h;

	long rwsize=WIDTHBYTES(width*24);

//	printf("\nReading the 24bit BMP file. ");

	fseek(infile, 0, 0);
	fread(bmpHeader, sizeof(char), header, infile);

	ColorImg *pixel_in=new ColorImg [rwsize];

	for(i = height; i > 0; i--) 
	{
		fseek(infile, (((i-1)*rwsize)+header), 0);
		fread(pixel_in, sizeof(ColorImg), width, infile);
		h = height - i;
		for(j = 0; j < width; j++) 
		{
			orgImg[h*width+j].r = pixel_in[j].r;
			orgImg[h*width+j].g = pixel_in[j].g;
			orgImg[h*width+j].b = pixel_in[j].b;
		}
	}
	delete []pixel_in;

	memcpy(outImg, orgImg, height*width*sizeof(ColorImg));


//	printf("--> OK!! \n");
}

void CFileIO24::WriteBMP24(char *infile)
{
	long rwsize=WIDTHBYTES(width*24);
	uchar* tmpImage = new uchar [rwsize*height];

	register int c, r;
	for(c=0; c<height; c++) 
	{
		for(r=0; r<width; r++) 
		{
			tmpImage[c*rwsize +3*r  ]  =outImg[(height-c-1)*width+r].b;
			tmpImage[c*rwsize +3*r+1]  =outImg[(height-c-1)*width+r].g;
			tmpImage[c*rwsize +3*r+2]  =outImg[(height-c-1)*width+r].r;
		}
	}

	FILE *outf=fopen(infile,"wb");
	fwrite(bmpHeader, sizeof(char), header, outf);
	fwrite(tmpImage, sizeof(char), rwsize*height, outf);
	fclose(outf);
	delete []tmpImage;
}

void CFileIO24::GetBMPHeaderInfo(FILE *infile)
{
	long	head, wdth, hght;
	
	fseek(infile, 10L, 0);
	fread(&head, sizeof(long), 1, infile);

	fseek(infile, 18L, 0);
	fread(&wdth, sizeof(long), 1, infile);
	fread(&hght, sizeof(long), 1, infile);

	fseek(infile, 0L, 0);
	
	header = (int)head;
	width  = (int)wdth;
	height = (int)hght;

//	fprintf(stderr, "\nInformation for input file !!\n");
	fprintf(stderr, "\nWidth = %d , Height = %d, Header = %d\n", width, height, header);
}

void CFileIO24::WriteBMP8(unsigned char *tgtImg, int height, int width, char name[])
{
//	printf("COVRA(R), Made by D.J.Kang, 1998\n");
	long rwsize=WIDTHBYTES(width*8);
	uchar* tmpImage = new uchar [rwsize*height];
	///
	for(int r=0;r<height;r++) 
		for(int c=0; c<width; c++) 
			tmpImage[r*rwsize +c  ]  =tgtImg[(height-r-1)*width+c];


	///
	BMPHEADER hBMP1;

	int nc=256; // # of colormap (each map has 4-byte)
	hBMP1.bmpWidth=width;
	hBMP1.bmpHeight=height;
	hBMP1.bmpType=19778; // 0x4D42, Ascii code of "BM"
	hBMP1.bmpFsize= 14 + // size of bitmap file header
							40 + // size of bitmap info header
							nc*4 + // size of colormap
						    rwsize*height; // size of image data 
	hBMP1.bmpReserved1=0;
	hBMP1.bmpReserved2=0;
	hBMP1.bmpOffBits=sizeof(RGBQUAD)*256+54;
	hBMP1.bmpSSize=40;
	hBMP1.bmpPlanes=1;
	hBMP1.bmpBitCount=8;
	hBMP1.bmpCompression=0;
	hBMP1.bmpSizeImage=rwsize*height;
	hBMP1.bmpXPelsPerMeter=75*39; // 75dpi * 39" per meter
	hBMP1.bmpYPelsPerMeter=75*39; // 75dpi * 39" per meter
	hBMP1.bmpClrUsed=256;
	hBMP1.bmpClrImportant=256;

	///
	FILE *outf=fopen(name,"wb");

	///
	putshort(outf,hBMP1.bmpType);
	putint(outf,hBMP1.bmpFsize);
	putshort(outf,hBMP1.bmpReserved1);
	putshort(outf,hBMP1.bmpReserved2); 
	putint(outf,hBMP1.bmpOffBits);
	putint(outf,hBMP1.bmpSSize);
	putint(outf,hBMP1.bmpWidth);
	putint(outf,hBMP1.bmpHeight);
	putshort(outf,hBMP1.bmpPlanes);
	putshort(outf,hBMP1.bmpBitCount);
	putint(outf,hBMP1.bmpCompression);
	putint(outf,hBMP1.bmpSizeImage);
	putint(outf,hBMP1.bmpXPelsPerMeter);
	putint(outf,hBMP1.bmpYPelsPerMeter);
	putint(outf,hBMP1.bmpClrUsed);
	putint(outf,hBMP1.bmpClrImportant);

	/// write out the colormap
	// In case of 8-bit Gray-level image, please write down the color MAP.
	RGBQUAD CMAP[256];
	for(int i=0; i<256; i++)
	{
		CMAP[i].rgbBlue		= i;
		CMAP[i].rgbGreen	= i;
		CMAP[i].rgbRed		= i;
		CMAP[i].rgbReserved = 0;
	}
	///
	fwrite(CMAP,sizeof(char),4*256,outf);


	/// write image data
	fwrite(tmpImage, sizeof(char), rwsize*height, outf);
	fclose(outf);
	delete []tmpImage;
}

void CFileIO24::putshort(FILE *fp, short i)
{
  int c, c1;

  c = ((unsigned int ) i) & 0xff;  c1 = (((unsigned int) i)>>8) & 0xff;
  putc(c, fp);   putc(c1,fp);
}

void CFileIO24::putint(FILE *fp, int i)
{
  int c, c1, c2, c3;
  c  = ((unsigned int ) i)      & 0xff;  
  c1 = (((unsigned int) i)>>8)  & 0xff;
  c2 = (((unsigned int) i)>>16) & 0xff;
  c3 = (((unsigned int) i)>>24) & 0xff;

  putc(c, fp);   putc(c1,fp);  putc(c2,fp);  putc(c3,fp);
}
