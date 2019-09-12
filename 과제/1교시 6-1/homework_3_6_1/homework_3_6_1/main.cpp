#include <stdio.h>

void main()
{
	int x, y;
	unsigned char OrgImg[256][256];

	// 디스크에서 영상데이터 읽기
	FILE *infile = fopen("Mandrill.raw","rb");
	if(infile==NULL) { printf("File open error !!"); return; }
	fread(OrgImg,sizeof(char),256*256,infile);
	fclose(infile);

	// 영상 부분 이동 
	for(y=60; y<110; y++)
	{
		for(x=50; x<200; x++) 
		{
			OrgImg[y][x] = OrgImg[y-50][x];
		}
	}

	// 하드디스크에 영상데이터 쓰기 
	FILE *outfile = fopen("Mandrill_homework.raw","wb");
	fwrite(OrgImg,sizeof(char),256*256,outfile);
	fclose(outfile);
}


