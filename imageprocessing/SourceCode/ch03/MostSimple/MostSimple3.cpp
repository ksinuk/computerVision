/// 이프로그램은 RAW형식을 파일을 읽고 쓰는 가장 간단한 
/// 영상처리 프로그램이다.
#include <stdio.h>

void main()
{
	int i, j;
	unsigned char OrgImg[256][256];

	// 디스크에서 영상데이터 읽기
	FILE *infile = fopen("mandrill.raw","rb");
	if(infile==NULL) { printf("File open error !!"); return; }
	fread(OrgImg,sizeof(char),256*256,infile);
	fclose(infile);

	// 역상계산을 위한 영상처리 
	for(i=10; i<50; i++)
	{
		for(j=60; j<200; j++) 
		{
			OrgImg[50+(50-i)][j] = OrgImg[i][j];
		}
	}

	// 하드디스크에 영상데이터 쓰기 
	FILE *outfile = fopen("mandrill_inv.raw","wb");
	fwrite(OrgImg,sizeof(char),256*256,outfile);
	fclose(outfile);
}
