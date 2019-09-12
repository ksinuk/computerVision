/// �����α׷��� RAW������ ������ �а� ���� ���� ������ 
/// ����ó�� ���α׷��̴�.
#include <stdio.h>

void main()
{
	int i, j;
	unsigned char OrgImg[256][256];

	// ��ũ���� �������� �б�
	FILE *infile = fopen("mandrill.raw","rb");
	if(infile==NULL) { printf("File open error !!"); return; }
	fread(OrgImg,sizeof(char),256*256,infile);
	fclose(infile);

	// �������� ���� ����ó�� 
	for(i=10; i<50; i++)
	{
		for(j=60; j<200; j++) 
		{
			OrgImg[50+(50-i)][j] = OrgImg[i][j];
		}
	}

	// �ϵ��ũ�� �������� ���� 
	FILE *outfile = fopen("mandrill_inv.raw","wb");
	fwrite(OrgImg,sizeof(char),256*256,outfile);
	fclose(outfile);
}
