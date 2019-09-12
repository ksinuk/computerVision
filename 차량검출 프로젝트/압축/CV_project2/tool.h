#ifndef TOOL_H_
#define TOOL_H_

/// STD
#include <string>
using namespace std;
/// OpenCV
#include <opencv2/opencv.hpp>
using namespace cv;

class Tool
{

	const int kImageHeight = 480;
	const int kImageWidth = 640;

public:
	/*
	* ������
	* �����ͼ� ��ο� ������ ����
	*/
	explicit Tool(const std::string &dataset_dir, int dataset_size);
	~Tool();

public:
	/*
	* Ŭ���� �ʱ�ȭ�� ���� �Լ�
	* Ŭ������ �ٸ� �޼ҵ带 ����ϱ� ���� ȣ��Ǿ����
	*/
	int Setup();

	/*
	* �ش� ��ȣ�� �̹����� �ҷ���
	*
	* Input@ no : �̹��� ��ȣ
	* Output@ mat_image : �ش� ��ȣ�� �̹���
	*/
	int Load(int no, cv::Mat &mat_image);

	/*
	* ���� �̹����� ���ذ� �ٿ�� �ڽ��� �ҷ���
	*
	* Output@ bboxes : �ٿ�� �ڽ����� ��ġ �� ũ��(x, y, width, height)
	*/
	int GetBBoxes(std::vector<cv::Rect2d> &bboxes);

	/*
	* �ٿ�� �ڽ� �׸���
	*
	* Input/Output@ mat_image : �׸��� ��� �̹���
	* Input@ bboxes : �ٿ�� �ڽ����� ��ġ �� ũ��(x, y, width, height)
	* Input@ color : �ٿ�� �ڽ� ����
	*/
	int DrawBBox(cv::Mat &mat_image, const std::vector<cv::Rect2d> &bboxes);

	/*
	* ���ذ��� �ٿ�� �ڽ��� �׸���
	*
	* Output@ mat_image : ���ذ��� �׷��� �̹���
	*/
	int DrawBBoxForGT(cv::Mat &mat_image);

	/*
	* ���� �̹��� ��
	*
	* Input@ bboxes : �򰡵� �ٿ�� �ڽ���
	*/
	int Evaluation(const std::vector<cv::Rect2d> &bboxes);

	/*
	* ��� ������ �� ��� ���
	*/
	int PrintResult();

private:
	void CheckSetup();
	void CheckImageNo();

private:
	// �¾�
	bool flag_setup_;
	int image_no_;

	// �����ͼ� ����
	std::string dataset_dir_;
	int dataset_size_;
	using BBox = std::pair<std::string, cv::Rect2d>;
	std::map<int, std::vector<BBox>> map_gt_;
	class Eval;
	std::map<int, Eval> map_eval_;

};

struct label
{
	Rect box;
	Point *data;
	int area;

	label *first;
	label *next;
	label *prev;
};

label* project(Mat &input);
int delete_grass(Mat &img , int y, int x);
label* make_label(Mat img , Point start , label *label_list , Mat check);
int delete_label_list(label *input);

#endif /// TOOL_H_
