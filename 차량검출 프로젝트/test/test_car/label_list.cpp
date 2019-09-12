#include "main_header.h"

label* make_label(Mat img , Point start , label* label_list , Mat check)
{
	if(img.at<uchar>(start)!=255) return label_list;

	Point *stack = new Point[img.rows*img.cols];
	Point *data = new Point[img.rows*img.cols];
	int num=0 , area=0;

	Point thisis = start;
	int y_max=0,y_min=1000,x_min=1000,x_max=0;

	while(num>=0)
	{
		img.at<uchar>(thisis) = 0;
		data[area++] = Point(thisis);

		for(int yy = thisis.y-1;yy<thisis.y+2;yy++)
			for(int xx = thisis.x-1; xx<thisis.x+2; xx++)
			{
				if(xx<0 || xx>=img.cols) continue;
				if(yy<0 || yy>=img.rows) continue;

				Point temp(xx,yy);

				if(img.at<uchar>(temp)==255)
				{
					img.at<uchar>(temp) = 0;

					stack[num++] = Point(temp);
					data[area++] = Point(temp);

					if(xx<x_min) x_min=xx;
					if(xx>x_max) x_max=xx;
					if(yy<y_min) y_min=yy;
					if(yy>y_max) y_max=yy;
				}
			}
		
		if(num!=0) thisis = stack[--num];
		else num--;
	}
	if(area<15)
	{
		delete [] stack;
		delete [] data;
		return label_list;
	}

	Rect outbox(Point(x_min,y_min),Point(x_max+1 , y_max+1));

	if(check.empty()==0)
	{
		Rect testbox(Point(x_min+1,y_min+1),Point(x_max-1 , y_max-1));
		Mat test = check(outbox);
		int ok = 0;

		for(int y=0;y<test.rows;y++)
			for(int x = 0; x<test.cols; x++)
				if(test.at<uchar>(y,x)!=0) ok++;

		if(ok<9)
		{
			delete [] stack;
			delete[] data;
			return label_list;
		}

	}



	if(label_list==NULL)
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

	delete [] stack;
	return label_list;
}

int delete_label_list(label *input)
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

int paint_label(Mat img , label *input)
{
	if(input==NULL) return -1;
	input = input->first;

	while(input!=NULL)
	{
		Rect temp = input->box;
		rectangle(img,temp,Scalar(255,255,255),1);
		input = input->next;
	}

	return 0;
}



int delete_grass(Mat &img,int y,int x)
{
	int *stack_x = new int[img.rows * img.cols];
	int *stack_y = new int[img.rows * img.cols];
	int n=0;

	while(n>=0)
	{
		if(img.at<uchar>(y,x)==255)
		{
			img.at<uchar>(y,x) = 0;

			for(int yy = y-1;yy<y+2;yy++)
				for(int xx = x-1; xx<x+2; xx++)
				{
					if(xx<0 || xx>=img.cols) continue;
					if(yy<0 || yy>=img.rows) continue;

					if(img.at<uchar>(yy,xx)==255)
					{
						stack_x[n] = xx;
						stack_y[n] = yy;
						n++;
					}
				}

			n--;
			x = stack_x[n];
			y = stack_y[n];
		}
		else break;
	}

	return 0;
}
