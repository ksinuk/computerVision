// ModelConv.h: interface for the CModelConv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODELCONV_H__EBDAE85A_BCA7_4EF4_AE99_35F3EC60B8CB__INCLUDED_)
#define AFX_MODELCONV_H__EBDAE85A_BCA7_4EF4_AE99_35F3EC60B8CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <stdio.h>
#include <MEMORY.H>
#include <windows.h>

#include "FileIO24.h"

#define  FTOI(a) ( (a) < 0 ? ((int)(a-0.5)) : ((int)(a+0.5)) )
#define  uchar unsigned char

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define UNDEFINED (0.0F)



class CModelConv  
{
public:
	CModelConv(int height, int width);
	void Initialize(int height, int width);
	void RGB2HSV(ColorImg *srcImg, HSVImg *tgtImg);
	void RGB2GRAY(ColorImg *srcImg, unsigned char *tgtImg);
	CModelConv();
	virtual ~CModelConv();

private:
	int height, width;
	
	void RGB_To_Gray(float r, float g, float b, float *gray);
	void Gray_To_RGB(float gray, float *r, float *g, float *b);
	void RGB_To_CMY(float r, float g, float b, float *c, float *m, float *y);
	void CMY_To_RGB(float c, float m, float y, float *r, float *g, float *b);
	void RGB_To_HSV(float r, float g, float b, float *h, float *s, float *v);
	void HSV_To_RGB(float h, float s, float v, float *r, float *g, float *b);
	void RGB_To_HSL(float r, float g, float b, float *h, float *l, float *s);
	void HSL_To_RGB(float h, float l, float s, float *r, float *g, float *b);

};

#endif // !defined(AFX_MODELCONV_H__EBDAE85A_BCA7_4EF4_AE99_35F3EC60B8CB__INCLUDED_)
