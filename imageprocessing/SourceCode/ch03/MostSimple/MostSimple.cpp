/// �����α׷��� RAW������ ������ �а� ���� ���� ������ 
/// ����ó�� ���α׷��̴�.
#include <stdio.h>

void main()
{
	int i, j;
	unsigned char OrgImg[256][256];

	// ��ũ���� �������� �б�
	FILE *infile = fopen("coin.raw","rb");
	if(infile==NULL) { printf("File open error !!"); return; }
	fread(OrgImg,sizeof(char),256*256,infile);
	fclose(infile);

	// �������� ���� ����ó�� 
	for(i=0; i<256; i++)
	{
		for(j=0; j<256; j++) 
		{
			OrgImg[i][j] = 255-OrgImg[i][j];
		}
	}

	// �ϵ��ũ�� �������� ���� 
	FILE *outfile = fopen("coin_inv.raw","wb");
	fwrite(OrgImg,sizeof(char),256*256,outfile);
	fclose(outfile);
}
