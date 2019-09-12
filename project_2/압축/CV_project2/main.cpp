
/// STD
#include <thread>
#include <random>

/// Tool
#include "tool.h"


const int kWait = 0;

void VehicleDetection(const Mat &mat_org, vector<Rect2d> &bboxes)
{
	/* 
	* TO DO : 이미지로부터 바운딩 박스를 찾는 코드를 여기에 추가
	*/
	Mat image;
	mat_org.copyTo(image);	
	
	label* label = project(image); //차량의 위치를 연결 리스트 형식으로 저장

	if(label!=NULL) label = label->first;
	while(label!=NULL)
	{
		Rect2d atom = label->box;

		atom.y += 145.0; //차량의 위치와 크기 조정
		atom.x -= 5.0;
		atom.width +=10.0;
		atom.height +=10.0;

		bboxes.push_back(atom); //차량의 위치를 vector 형식으로 저장

		label = label->next; //다음 차량의 위치 확인하기
	}

	delete_label_list(label); //다 사용한 연결 리스트 삭제


}

int main(int, char **)
{
	const int kDatasetNumber = 40;
	Tool training_project_tool("../training/", kDatasetNumber);
	training_project_tool.Setup();

	for (int image_no = 0; image_no < kDatasetNumber; ++image_no)
	{
		Mat mat_org;
		training_project_tool.Load(image_no, mat_org);

		// 알고리즘
		vector<Rect2d> bboxes;
		VehicleDetection(mat_org, bboxes);
	
		// 현재 기준값, 출력값 평가
		training_project_tool.Evaluation(bboxes);

		// 기준값 시각화
		Mat mat_image;
		mat_org.copyTo(mat_image);
		training_project_tool.DrawBBoxForGT(mat_image);

		// 알고리즘 결과 시각화
		training_project_tool.DrawBBox(mat_image, bboxes);
		imshow("output", mat_image);

		waitKey(kWait);
	}

	// 알고리즘 평가
	training_project_tool.PrintResult();
	waitKey(0);

}