#ifndef TOOL_H_
#define TOOL_H_

/// STD
#include <string>
using namespace std;
/// OpenCV
#include <opencv2/opencv.hpp>
using namespace cv;

struct label
{
	Rect box;
	Point *data;
	int area;

	label *first;
	label *next;
	label *prev;
};



int project(Mat &input , Mat &output , int number);
int delete_grass(Mat &img , int y, int x);
label* make_label(Mat img , Point start , label *label_list , Mat check);
int delete_label_list(label *input);
int paint_label(Mat img , label *input);

#endif /// TOOL_H_
