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
	* 생성자
	* 데이터셋 경로와 데이터 개수
	*/
	explicit Tool(const std::string &dataset_dir, int dataset_size);
	~Tool();

public:
	/*
	* 클래스 초기화를 위한 함수
	* 클래스의 다른 메소드를 사용하기 전에 호출되어야함
	*/
	int Setup();

	/*
	* 해당 번호의 이미지를 불러옴
	*
	* Input@ no : 이미지 번호
	* Output@ mat_image : 해당 번호의 이미지
	*/
	int Load(int no, cv::Mat &mat_image);

	/*
	* 현재 이미지의 기준값 바운딩 박스를 불러옴
	*
	* Output@ bboxes : 바운딩 박스들의 위치 및 크기(x, y, width, height)
	*/
	int GetBBoxes(std::vector<cv::Rect2d> &bboxes);

	/*
	* 바운딩 박스 그리기
	*
	* Input/Output@ mat_image : 그리기 대상 이미지
	* Input@ bboxes : 바운딩 박스들의 위치 및 크기(x, y, width, height)
	* Input@ color : 바운딩 박스 색상
	*/
	int DrawBBox(cv::Mat &mat_image, const std::vector<cv::Rect2d> &bboxes);

	/*
	* 기준값의 바운딩 박스들 그리기
	*
	* Output@ mat_image : 기준값이 그려진 이미지
	*/
	int DrawBBoxForGT(cv::Mat &mat_image);

	/*
	* 현재 이미지 평가
	*
	* Input@ bboxes : 평가될 바운딩 박스들
	*/
	int Evaluation(const std::vector<cv::Rect2d> &bboxes);

	/*
	* 모든 누적된 평가 결과 출력
	*/
	int PrintResult();

private:
	void CheckSetup();
	void CheckImageNo();

private:
	// 셋업
	bool flag_setup_;
	int image_no_;

	// 데이터셋 정보
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
