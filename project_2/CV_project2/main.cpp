
/// STD
#include <thread>
#include <random>

/// Tool
#include "tool.h"


const int kWait = 0;

void VehicleDetection(const Mat &mat_org, vector<Rect2d> &bboxes)
{
	/* 
	* TO DO : �̹����κ��� �ٿ�� �ڽ��� ã�� �ڵ带 ���⿡ �߰�
	*/
	Mat image;
	mat_org.copyTo(image);	
	
	label* label = project(image); //������ ��ġ�� ���� ����Ʈ �������� ����

	if(label!=NULL) label = label->first;
	while(label!=NULL)
	{
		Rect2d atom = label->box;

		atom.y += 145.0; //������ ��ġ�� ũ�� ����
		atom.x -= 5.0;
		atom.width +=10.0;
		atom.height +=10.0;

		bboxes.push_back(atom); //������ ��ġ�� vector �������� ����

		label = label->next; //���� ������ ��ġ Ȯ���ϱ�
	}

	delete_label_list(label); //�� ����� ���� ����Ʈ ����


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

		// �˰���
		vector<Rect2d> bboxes;
		VehicleDetection(mat_org, bboxes);
	
		// ���� ���ذ�, ��°� ��
		training_project_tool.Evaluation(bboxes);

		// ���ذ� �ð�ȭ
		Mat mat_image;
		mat_org.copyTo(mat_image);
		training_project_tool.DrawBBoxForGT(mat_image);

		// �˰��� ��� �ð�ȭ
		training_project_tool.DrawBBox(mat_image, bboxes);
		imshow("output", mat_image);

		waitKey(kWait);
	}

	// �˰��� ��
	training_project_tool.PrintResult();
	waitKey(0);

}