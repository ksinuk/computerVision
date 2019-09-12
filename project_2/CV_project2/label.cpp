#include "tool.h"

label* make_label(Mat img , Point start , label* label_list , Mat check) //각각의 차량의 라벨을 만드는 함수
{
	if(img.at<uchar>(start)!=255) return label_list; //잘못된 값이 들어 오면 원래 값 반환

	Point *stack = new Point[img.rows*img.cols]; //임시 스텍
	Point *data = new Point[img.rows*img.cols]; //차량을 만드는 픽셀의 위치를 저장하는 곳
	int num=0 , area=0; //스텍의 위치와 차량의 면적

	Point thisis = start; //저장해야 하는 픽셀
	int y_max=0,y_min=1000,x_min=1000,x_max=0; //차량을 넣을 수 있는 사각형의 좌표들

	while(num>=0)
	{
		img.at<uchar>(thisis) = 0; //확인한 좌표는 삭제
		data[area++] = Point(thisis);

		for(int yy = thisis.y-1;yy<thisis.y+2;yy++)
			for(int xx = thisis.x-1; xx<thisis.x+2; xx++)
			{
				if(xx<0 || xx>=img.cols) continue; //이미지 범위를 넘음
				if(yy<0 || yy>=img.rows) continue;

				Point temp(xx,yy);

				if(img.at<uchar>(temp)==255) 
				{
					img.at<uchar>(temp) = 0; //세로은 픽셀을 찾으면 삭제

					stack[num++] = Point(temp); //찾아낸 픽셀을 저장
					data[area++] = Point(temp);

					if(xx<x_min) x_min=xx; //사각형의 좌표 갱신
					if(xx>x_max) x_max=xx;
					if(yy<y_min) y_min=yy;
					if(yy>y_max) y_max=yy;
				}
			}
		
		if(num!=0) thisis = stack[--num]; //다음 좌표값 추출
		else num--;
	}

	if(area<15) //면적이 작은 차량은 잡음과 구별할 수 없으므로 삭제
	{
		delete [] stack;
		delete [] data;
		return label_list;
	}

	Rect outbox(Point(x_min,y_min),Point(x_max+1 , y_max+1)); 

	if(check.empty()==0) //차량 라이트의 크기를 구한다.
	{
		Rect testbox(Point(x_min+1,y_min+1),Point(x_max-1 , y_max-1));
		Mat test = check(outbox);
		int ok = 0;

		for(int y=0;y<test.rows;y++)
			for(int x = 0; x<test.cols; x++)
				if(test.at<uchar>(y,x)!=0) ok++;

		if(ok<9) //라이트의 크기가 작으면 원래값 반환
		{
			delete [] stack;
			delete[] data;
			return label_list;
		}
	}

	if(label_list==NULL) //연결 리스트를 만들기
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

	delete [] stack; //스텍 삭제
	return label_list; //결과물 출력
}

int delete_label_list(label *input) //연결 리스트 삭제
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

int delete_grass(Mat &img,int y,int x) //이미지 조각을 삭제한다.
{
	int *stack_x = new int[img.rows * img.cols]; //x 좌표를 저장하는 스텍
	int *stack_y = new int[img.rows * img.cols]; //y 좌표를 저장하는 스텍
	int n=0; //스텍의 위치

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
