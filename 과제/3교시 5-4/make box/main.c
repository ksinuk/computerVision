#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) 
{
	int i=0,j=0;
	unsigned char out[256][256]={0};
	FILE *fd=NULL;
	
	for(i=0;i<256;i++)
		for(j=0;j<256;j++)
		{
			if(i>64 && i<192 && j>64 && j<192)out[i][j]=255;
			else out[i][j] = 0;
		}
			
	fd = fopen("box.raw","wb+");
	fwrite(out,1,256*256,fd);
	fclose(fd);
	
	
	
	return 0;
}
