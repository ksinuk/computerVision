// BlobColoring.h: interface for the CBlobColoring class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOBCOLORING_H__AF61EFD9_9D81_4BAE_BC6D_4BC44C7A1B93__INCLUDED_)
#define AFX_BLOBCOLORING_H__AF61EFD9_9D81_4BAE_BC6D_4BC44C7A1B93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBlobColoring  
{
	int top;
	int m_MaxStack;
	int rwsize;
	push(int vx,int vy);
	pop(int *vx,int *vy);
public:
	void fnBlobColoring(int distanceBound);
	int nBlob;
	typedef struct tagBLOBDATA { short ltx, lty, rbx, rby; int n, pc; float cx, cy, r, g, b, u02, u20, u11, m, v; } BLOBDATA;
	BLOBDATA *blobData;
	CBlobColoring(unsigned char *m_InImg, int height, int width);
	virtual ~CBlobColoring();

private:
	void m_FindBoundary();
	void m_FindMoment();
	unsigned char *m_InImg;
	int width;
	int height;
	int distanceBound;
	BOOL BoundDist(int rColor,int gColor,int bColor,int rVal,int gVal,int bVal);
	int *m_Coloring;
	int *m_stacky;
	int *m_stackx;
	unsigned char *passImg;
};

#endif // !defined(AFX_BLOBCOLORING_H__AF61EFD9_9D81_4BAE_BC6D_4BC44C7A1B93__INCLUDED_)
