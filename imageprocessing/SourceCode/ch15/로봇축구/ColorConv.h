// ColorConv.h: interface for the CColorConv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORCONV_H__7F421C61_B4F3_11D6_A616_0050DA084ED4__INCLUDED_)
#define AFX_COLORCONV_H__7F421C61_B4F3_11D6_A616_0050DA084ED4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CColorConv  
{
	#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
	#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
	#define UNDEFINED (0.0F)
public:
	void HSI_To_RGB(float h,float s,float i,float *r,float *g,float *b);
	void RGB_To_HSI(float r,float g,float b,float *h,float *s,float *i);
	void RGB_To_YIQ(float r,float g,float b, float *y,float *i,float *q);
	void RGB_To_Gray(float r, float g, float b, float *gray);
	void Gray_To_RGB(float gray, float *r, float *g, float *b);
	void RGB_To_CMY(float r, float g, float b, float *c, float *m, float *y);
	void CMY_To_RGB(float c, float m, float y, float *r, float *g, float *b);
	void RGB_To_HSV(float r, float g, float b, float *h, float *s, float *v);
	void HSV_To_RGB(float h, float s, float v, float *r, float *g, float *b);
	void RGB_To_HSL(float r, float g, float b, float *h, float *l, float *s);
	void HSL_To_RGB(float h, float l, float s, float *r, float *g, float *b);
	CColorConv();
	virtual ~CColorConv();

};

#endif // !defined(AFX_COLORCONV_H__7F421C61_B4F3_11D6_A616_0050DA084ED4__INCLUDED_)
