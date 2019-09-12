#include "tool.h"

label* make_label(Mat img , Point start , label* label_list , Mat check) //������ ������ ���� ����� �Լ�
{
	if(img.at<uchar>(start)!=255) return label_list; //�߸��� ���� ��� ���� ���� �� ��ȯ

	Point *stack = new Point[img.rows*img.cols]; //�ӽ� ����
	Point *data = new Point[img.rows*img.cols]; //������ ����� �ȼ��� ��ġ�� �����ϴ� ��
	int num=0 , area=0; //������ ��ġ�� ������ ����

	Point thisis = start; //�����ؾ� �ϴ� �ȼ�
	int y_max=0,y_min=1000,x_min=1000,x_max=0; //������ ���� �� �ִ� �簢���� ��ǥ��

	while(num>=0)
	{
		img.at<uchar>(thisis) = 0; //Ȯ���� ��ǥ�� ����
		data[area++] = Point(thisis);

		for(int yy = thisis.y-1;yy<thisis.y+2;yy++)
			for(int xx = thisis.x-1; xx<thisis.x+2; xx++)
			{
				if(xx<0 || xx>=img.cols) continue; //�̹��� ������ ����
				if(yy<0 || yy>=img.rows) continue;

				Point temp(xx,yy);

				if(img.at<uchar>(temp)==255) 
				{
					img.at<uchar>(temp) = 0; //������ �ȼ��� ã���� ����

					stack[num++] = Point(temp); //ã�Ƴ� �ȼ��� ����
					data[area++] = Point(temp);

					if(xx<x_min) x_min=xx; //�簢���� ��ǥ ����
					if(xx>x_max) x_max=xx;
					if(yy<y_min) y_min=yy;
					if(yy>y_max) y_max=yy;
				}
			}
		
		if(num!=0) thisis = stack[--num]; //���� ��ǥ�� ����
		else num--;
	}

	if(area<15) //������ ���� ������ ������ ������ �� �����Ƿ� ����
	{
		delete [] stack;
		delete [] data;
		return label_list;
	}

	Rect outbox(Point(x_min,y_min),Point(x_max+1 , y_max+1)); 

	if(check.empty()==0) //���� ����Ʈ�� ũ�⸦ ���Ѵ�.
	{
		Rect testbox(Point(x_min+1,y_min+1),Point(x_max-1 , y_max-1));
		Mat test = check(outbox);
		int ok = 0;

		for(int y=0;y<test.rows;y++)
			for(int x = 0; x<test.cols; x++)
				if(test.at<uchar>(y,x)!=0) ok++;

		if(ok<9) //����Ʈ�� ũ�Ⱑ ������ ������ ��ȯ
		{
			delete [] stack;
			delete[] data;
			return label_list;
		}
	}

	if(label_list==NULL) //���� ����Ʈ�� �����
	{
		label_list = new label;
		label_list->prev = NULL;
		label_list->first = label_list;
		label_list->next = NULL;
	}
	else
	{
		label_list->next = new label;
		label *temp = label_list;
		label_list = label_list->next;
		label_list->prev = temp;
		label_list->first = temp->first;
		label_list->next = NULL;
	}
	label_list->area = area;
	label_list->data = data;
	label_list->box = outbox;

	delete [] stack; //���� ����
	return label_list; //����� ���
}

int delete_label_list(label *input) //���� ����Ʈ ����
{
	if(input==NULL) return -1;

	label *ptr=input->first;
	label *temp=NULL;

	while(ptr!=NULL)
	{
		if(input->data != NULL) delete [] ptr->data;
		temp = ptr;
		ptr = ptr->next;
		if(temp != input) delete temp;
	}

	input = NULL;

	return 0;
}

int delete_grass(Mat &img,int y,int x) //�̹��� ������ �����Ѵ�.
{
	int *stack_x = new int[img.rows * img.cols]; //x ��ǥ�� �����ϴ� ����
	int *stack_y = new int[img.rows * img.cols]; //y ��ǥ�� �����ϴ� ����
	int n=0; //������ ��ġ

	while(n>=0)
	{
		img.at<uchar>(y,x) = 0;

		for(int yy = y-1;yy<y+2;yy++)
			for(int xx = x-1; xx<x+2; xx++)
			{
				if(xx<0 || xx>=img.cols) continue;
				if(yy<0 || yy>=img.rows) continue;

				if(img.at<uchar>(yy,xx)==255)
				{
					img.at<uchar>(yy,xx) =0;

					stack_x[n] = xx;
					stack_y[n] = yy;
					n++;
				}
			}

		n--;
		if(n<0) break;
		x = stack_x[n];
		y = stack_y[n];
	}

	return 0;
}
