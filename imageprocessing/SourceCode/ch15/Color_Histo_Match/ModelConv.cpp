// ModelConv.cpp: implementation of the CModelConv class.
//
//////////////////////////////////////////////////////////////////////

#include "ModelConv.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModelConv::CModelConv()
{

}

CModelConv::~CModelConv()
{

}



CModelConv::CModelConv(int height, int width)
{
	Initialize(height, width);
}



void CModelConv::Initialize(int height, int width)
{
	this->height		= height;
	this->width			= width;
}



void CModelConv::RGB2HSV(ColorImg *srcImg, HSVImg *tgtImg)
{
	register int i,j;

	float h,s,v, r,g, b;

	for(i=0; i<height; i++)
	{
		int index	= i*width;
		for(j=0; j<width; j++)
		{
			
			r=(float)srcImg[index+j].r/255.0F;
			g=(float)srcImg[index+j].g/255.0F;
			b=(float)srcImg[index+j].b/255.0F;

			RGB_To_HSV(r, g, b, &h, &s, &v);	
			tgtImg[index+j].h=(int)h;
			tgtImg[index+j].s=(BYTE)(s*255.F);
			tgtImg[index+j].v=(BYTE)(v*255.F);			

		}
	}
}


void CModelConv::RGB2GRAY(ColorImg *srcImg, unsigned char *tgtImg)
{
	register int i,j;

	float r,g,b,gray;

	for(i=0; i<height; i++)
	{
		int index	= i*width;
		for(j=0; j<width; j++)
		{
			
			r=(float)srcImg[index+j].r/255.0F;
			g=(float)srcImg[index+j].g/255.0F;
			b=(float)srcImg[index+j].b/255.0F;

			RGB_To_Gray(r,g,b,&gray);

			tgtImg[index+j]= (BYTE)(gray*255);
		}
	}
}













//////////////////////////////////////////////////////////////////////////////////
// RGB_To_Gray - RGB값을 Gray로 바꾼다.
//
// Gray 는 각각 [0,1]사이 값을 갖는다.
// R,G,B는 각각 [0,1]사이 값을 갖는다.

void CModelConv::RGB_To_Gray(float r, float g, float b, float *gray)
{
//  *gray = r * 0.30F + g * 0.59F + b * 0.11F;
  *gray = r * 0.333F + g * 0.333F + b * 0.333F;
}

//////////////////////////////////////////////////////////////////////////////////
// Gray_To_RGB - Gray 값을 RGB로 바꾼다. 
//               이과정은 promotion에 해당하는 과정으로 사실상 별의미가 없다.
// Gray 는 각각 [0,1]사이 값을 갖는다.
// R,G,B는 각각 [0,1]사이 값을 갖는다.

void CModelConv::Gray_To_RGB(float gray, float *r, float *g, float *b)
{
  *r = *g = *b = gray;
}


//////////////////////////////////////////////////////////////////////////////////
// CMY_To_RGB - CMY값을 RGB로 바꾼다.
//
// R,G,B는 각각 [0,1]사이 값을 갖는다.
// C,M,Y는 각각 [0,1]사이 값을 갖는다.

void CModelConv::RGB_To_CMY(float r, float g, float b, float *c, float *m, float *y)
{
  *c = 1.0F - r;
  *m = 1.0F - g;
  *y = 1.0F - b;
}


//////////////////////////////////////////////////////////////////////////////////
// CMY_To_RGB - CMY값을 RGB로 바꾼다.
//
// R,G,B는 각각 [0,1]사이 값을 갖는다.
// C,M,Y는 각각 [0,1]사이 값을 갖는다.

void CModelConv::CMY_To_RGB(float c, float m, float y, float *r, float *g, float *b)
{
  *r = 1.0F - c;
  *g = 1.0F - m;
  *b = 1.0F - y;
}


//////////////////////////////////////////////////////////////////////////////////
// RGB_To_HSV - RGB값을 HSV로 바꾼다.
//
// R,G,B는 각각 [0,1]사이 값을 갖는다.
// H는 [0,360) 범위, S,V값은 각각 [0,1]범위를 갖는다.

void CModelConv::RGB_To_HSV(float r, float g, float b, float *h, float *s, float *v)
{
  float max, min;
    
  max = MAX(r,g);
  max = MAX(max,b);

  min = MIN(r,g);
  min = MIN(min,b);

  *v = max;                                   // 명도(V) = max(r,g,b)
  *s = (max != 0.0F) ? (max-min)/max : 0.0F;    // 채도(S)을 계산, S=0이면 R=G=B=0

  if(*s == 0.0F)
    *h = UNDEFINED;
  else {                                      // 채도(S)는 != 0
    float delta = max - min;                 // 색상(H)를 구한다.

    if(r == max)
      *h = (g - b) / delta;                   // 색상이 Yello와 Magenta사이 
    else if( g == max)
      *h = 2.0F + (b - r) / delta;             // 색상이 Cyan와 Yello사이 
    else if( b == max)
      *h = 4.0F + (r - g) / delta;             // 색상이 Magenta와 Cyan사이

    *h *= 60.0F;

    if(*h < 0.0F)                              // 색상값을 각도로 바꾼다.
      *h += 360.0F;
  }
}


//////////////////////////////////////////////////////////////////////////////////
// HSV_To_RGB - HSV값을 RGB로 바꾼다.
//
// H는 [0,360) 범위, S,V값은 각각 [0,1]범위를 갖는다.
// R,G,B는 각각 [0,1]값을 갖는다.

void CModelConv::HSV_To_RGB(float h, float s, float v, float *r, float *g, float *b)
{
  if(s == 0.0F)                                // 컬러값은 흑백명암값.
  {
    if(h == UNDEFINED)                        // 이경우 색상 값은 없다.
      *r = *g = *b = v;
    else
      *r = *g = *b = UNDEFINED;
  } else 
  {                                           // S != 0 이므로 색상(H)이 있다.
    float f, p, q, t;
    int i;

    if(h == 360.0F)
      h = 0.0F;

    h /= 60.0F;                                 // H 값은 [0,360)사이 값
    i = (int)h;                                // <=H 인 가장 큰 정수값
    f = h - i;                                 // F 는 H 의 실수 부분
    p = v * (1.0F - s);
    q = v * (1.0F - (s * f));
    t = v * (1.0F - (s * (1.0F - f)));

    switch(i)
    {
    case 0: *r = v; *g = t; *b = p; break;
    case 1: *r = q; *g = v; *b = p; break;
    case 2: *r = p; *g = v; *b = t; break;
    case 3: *r = p; *g = q; *b = v; break;
    case 4: *r = t; *g = p; *b = v; break;
    case 5: *r = v; *g = p; *b = q; break;
    }
  }
}


//////////////////////////////////////////////////////////////////////////////////
// RGB_To_HLS - RGB값을 HLS로 바꾼다.
//
// R,G,B는 각각 [0,1]값을 갖는다.
// H는 [0,360) 범위, S,V값은 각각 [0,1]범위를 갖는다.

void CModelConv::RGB_To_HSL(float	r, float g, float b, float *h, float *s, float *l)
{
  float v;
  float m;
  float vm;
  float r2, g2, b2;

  v = MAX(r,g);
  v = MAX(v,b);
  m = MIN(r,g);
  m = MIN(m,b);

  if ((*l = (m + v) / 2.0F) <= 0.0F) 
    return;
  if ((*s = vm = v - m) > 0.0F) {
		*s /= (*l <= 0.5F) ? (v + m ) : (2.0F - v - m) ;
  } else
	  return;

  r2 = (v - r) / vm;
  g2 = (v - g) / vm;
  b2 = (v - b) / vm;

  if (r == v)
    *h = (g == m ? 5.0F + b2 : 1.0F - g2);
  else if (g == v)
		*h = (b == m ? 1.0F + r2 : 3.0F - b2);
  else
		*h = (r == m ? 3.0F + g2 : 5.0F - r2);

 	*h *= 60.0F;
}


//////////////////////////////////////////////////////////////////////////////////
// HLS_To_RGB - HLS값을 RGB로 바꾼다.
//
// H는 [0,360) 범위, S,V값은 각각 [0,1]범위를 갖는다.
// R,G,B는 각각 [0,1]값을 갖는다.


void CModelConv::HSL_To_RGB(float h, float sl, float l, float *r, float *g, float *b)
{
  float v;

  v = (l <= 0.5F) ? (l * (1.0F + sl)) : (l + sl - l * sl);
  if (v <= 0) {
  	*r = *g = *b = 0.0F;
  } else 
  {
	  float m;
	  float sv;
	  int sextant;
	  float fract, vsf, mid1, mid2;

	  m = l + l - v;
	  sv = (v - m ) / v;
	  h /= 60.0F;
	  sextant = (int)h;	
	  fract = h - sextant;
	  vsf = v * sv * fract;
	  mid1 = m + vsf;
	  mid2 = v - vsf;

	  switch (sextant) {
		  case 0: *r = v;    *g = mid1; *b = m; break;
		  case 1: *r = mid2; *g = v;    *b = m; break;
		  case 2: *r = m;    *g = v;    *b = mid1; break;
		  case 3: *r = m;    *g = mid2; *b = v; break;
		  case 4: *r = mid1; *g = m;    *b = v; break;
		  case 5: *r = v;    *g = m;    *b = mid2; break;
	  }
  }
}


