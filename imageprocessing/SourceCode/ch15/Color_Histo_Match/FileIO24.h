// FileIO24.h: interface for the CFileIO24 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEIO24_H__5629D6F1_711E_41BC_B9C7_6A5CDF6DC658__INCLUDED_)
#define AFX_FILEIO24_H__5629D6F1_711E_41BC_B9C7_6A5CDF6DC658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <MEMORY.H>
#include <windows.h>
#define  FTOI(a) ( (a) < 0 ? ((int)(a-0.5)) : ((int)(a+0.5)) )
#define  WIDTHBYTES(bits) (((bits)+31)/32*4)
#define  uchar unsigned char


// Define Color structure
typedef struct { uchar b,g,r;		 } ColorImg;
typedef struct { uchar s,v; short h; } HSVImg;
typedef struct { uchar chr,chg;		 } ChromaImg;

class CFileIO24  
{

public:

	void WriteBMP8(unsigned char* tgtImg,int height,int width,char name[]);
	void WriteBMP24(char *infile);
	void ReadBMP24(char *inbuff);
	CFileIO24(char *inbuff);
	int GetWidth();
	int GetHeight();
	ColorImg* GetOrgImg();
	ColorImg* GetOutImg();
	CFileIO24();
	virtual ~CFileIO24();

private:
	uchar bmpHeader[2048];
	unsigned int header;
	void GetBMPHeaderInfo(FILE *infile);
	ColorImg* orgImg;
	RGBQUAD* hRGB;
	ColorImg* outImg;
	int width;
	int height;
	BITMAPINFOHEADER hInfo;
	BITMAPFILEHEADER hf; 

	void putint(FILE *fp, int i);
	void putshort(FILE *fp, short i);
	typedef struct 
	{ 
		short bmpType;
		long  bmpFsize;
		short bmpReserved1,bmpReserved2;
		long  bmpOffBits,bmpSSize,bmpWidth,bmpHeight;
		short bmpPlanes,bmpBitCount;
		long  bmpCompression,bmpSizeImage,bmpXPelsPerMeter,bmpYPelsPerMeter;
		long  bmpClrUsed,bmpClrImportant;
	} BMPHEADER;
};

#endif // !defined(AFX_FILEIO24_H__5629D6F1_711E_41BC_B9C7_6A5CDF6DC658__INCLUDED_)
