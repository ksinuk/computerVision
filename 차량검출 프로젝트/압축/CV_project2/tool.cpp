#include "tool.h"

/// STD
#include <fstream>



class Tool::Eval
{

	static const double kIoUThres;

public:
	Eval() : tp_(0), fp_(0), fn_(0), tp_indices_() {}
	~Eval() {}

public:
	int Compute(const vector<Rect2d> &bboxes_output, const vector<BBox> &bboxes_gt)
	{
		vector<bool> assigned(bboxes_output.size(), false);
		tp_indices_.assign(bboxes_output.size(), false);
		for (int i_gt = 0; i_gt < bboxes_gt.size(); ++i_gt)
		{
			// ��� ���� �˻�
			int max_i_out = -1;
			double max_iou = -1.0;
			for (int i_out = 0; i_out < bboxes_output.size(); ++i_out)
			{
				if (assigned[i_out]) continue;
				double iou = GetIoU(bboxes_output[i_out], bboxes_gt[i_gt].second);
				if (iou > max_iou && iou > kIoUThres)
				{
					max_iou = iou;
					max_i_out = i_out;
				}
			}
			// ��ȿ�� ������ ����
			if (max_i_out != -1)
			{
				assigned[max_i_out] = true;
				tp_indices_[max_i_out] = true;
				++tp_;
			}
			else ++fn_;
		}
		// ���ذ��� ��Ī���� ���� �����
		for (int i_out = 0; i_out < bboxes_output.size(); ++i_out) if (!assigned[i_out]) ++fp_;

		return 0;
	}

	const int tp() const { return tp_; }

	const int fp() const { return fp_; }

	const int fn() const { return fn_; }

	const bool IsTruePositive(int output_index) const
	{
		if (output_index >= 0 || output_index < tp_indices_.size()) return tp_indices_[output_index];
		return false;
	}

private:
	double GetIoU(const Rect2d &box_a, const Rect2d &box_b)
	{
		double box_a_x2 = box_a.x + box_a.width;
		double box_b_x2 = box_b.x + box_b.width;
		double box_a_y2 = box_a.y + box_a.height;
		double box_b_y2 = box_b.y + box_b.height;
		double x1 = max(box_a.x, box_b.x);
		double y1 = max(box_a.y, box_b.y);
		double x2 = min(box_a_x2, box_b_x2);
		double y2 = min(box_a_y2, box_b_y2);
		double w = x2 - x1;
		double h = y2 - y1;
		if (w <= 0 || h <= 0) return 0.0;
		double inter = w * h;
		double a_area = (box_a_x2 - box_a.x) * (box_a_y2 - box_a.y);
		double b_area = (box_b_x2 - box_b.x) * (box_b_y2 - box_b.y);
		return inter / (a_area + b_area - inter);
	}
	
private:
	int tp_, fp_, fn_;
	std::vector<bool> tp_indices_;

};

const double Tool::Eval::kIoUThres = 0.5;








Tool::Tool(const string &dataset_dir, int dataset_size)
	: flag_setup_(false), image_no_(-1), dataset_dir_(dataset_dir), dataset_size_(dataset_size), map_gt_()
{
	if (dataset_dir_.back() != '/') dataset_dir_ += "/";
}

Tool::~Tool()
{
}

int Tool::Setup()
{
	// �ʱ�ȭ �˻�
	if (flag_setup_) abort();

	// ���� �˻�
	for (int no = 0; no < dataset_size_; ++no)
	{
		cv::Mat mat_curr = imread(dataset_dir_ + "image/" + to_string(no) + ".bmp");
		if (mat_curr.empty())
		{
			cout << "�����ͼ¿� ������ �̹��� ���� : " << no << "\n";
			abort();
		}
		if (mat_curr.rows != kImageHeight || mat_curr.cols != kImageWidth)
		{
			cout << "������ �̹��� ������ : " << kImageWidth << " x " << kImageHeight << "\n"
				<< "�ε�� �̹��� ������ : " << mat_curr.cols << " x " << mat_curr.rows << "\n"
				<< "���� �ٸ� �̹��� ������ : " << no << "\n";
			abort();
		}
	}
	
	// ���ذ� �ҷ�����
	const string kPathGT = "image_gt/image_gt.txt";
	ifstream fin_gt(dataset_dir_ + kPathGT);
	if (!fin_gt.is_open())
	{
		cout << "���ذ��� ã�� �� ���� : " << dataset_dir_ + kPathGT << "\n";
		abort();
	}
	Rect2d box_entire(0.0, 0.0, kImageWidth, kImageHeight);
	int curr_image_no = -1;
	while (fin_gt >> curr_image_no)
	{
		int bbox_no = -1;
		fin_gt >> bbox_no;
		if (bbox_no > 0)
		{
			std::vector<BBox> bboxes;
			for (int i = 0; i < bbox_no; ++i)
			{
				cv::Rect2d rect;
				std::string class_name;
				fin_gt >> rect.x >> rect.y >> rect.width >> rect.height >> class_name;
				rect = rect & box_entire;
				bboxes.emplace_back(class_name, rect);
			}
			map_gt_[curr_image_no] = bboxes;
		}
	}

	// ����
	std::cout << "�ʱ�ȭ ���� : " << dataset_dir_ << "\n";
	flag_setup_ = true;

	return 0;
}

int Tool::Load(int no, cv::Mat &mat_loaded)
{
	// �ʱ�ȭ �˻�
	CheckSetup();

	// �̹��� ��ȣ �˻�
	if (no < 0 || no >= dataset_size_)
	{
		std::cout << "�����ͼ� ������ ��� : " << no << "\n";
		std::abort();
	}

	// �̹��� �ҷ�����
	mat_loaded = imread(dataset_dir_ + "image/" + std::to_string(no) + ".bmp");

	// ����
	cout << "�̹��� �ҷ����� ���� : " << no << "\n";
	image_no_ = no;

	return 0;
}

int Tool::GetBBoxes(vector<Rect2d>& bboxes)
{
	// �ʱ�ȭ �˻�
	CheckSetup();
	CheckImageNo();

	bboxes.clear();
	const vector<BBox> &bboxes_gt = map_gt_[image_no_];
	for (int i = 0; i < bboxes_gt.size(); ++i) bboxes.emplace_back(bboxes_gt[i].second);

	return 0;
}

int Tool::DrawBBox(cv::Mat &mat_image, const std::vector<cv::Rect2d> &bboxes)
{
	// �ʱ�ȭ �˻�
	CheckSetup();
	CheckImageNo();

	// �ٿ�� �ڽ� ���� �� ����
	const cv::Scalar color_tp = cv::Scalar(25, 220, 25);
	const cv::Scalar color_fp = cv::Scalar(25, 25, 220);
	const double kAlpha = 0.5;
	
	// �� ���࿩�� �˻�
	if (map_eval_.find(image_no_) == map_eval_.end()) Evaluation(bboxes);
	const Eval &eval = map_eval_[image_no_];

	// �׸���
	if (!bboxes.empty())
	{
		cv::Rect2d box_entire(0.0, 0.0, mat_image.cols, mat_image.rows);
		for (int i = 0; i < bboxes.size(); ++i)
		{
			const cv::Scalar &color = eval.IsTruePositive(i) ? color_tp : color_fp;
			cv::Rect2d box = box_entire & bboxes[i];
			cv::Mat roi = mat_image(box);
			cv::Mat color_roi(roi.size(), CV_8UC3, color);
			cv::addWeighted(color, kAlpha, roi, 1.0 - kAlpha, 0.0, roi);
			cv::rectangle(mat_image, box, color, 2);
		}
	}

	return 0;
}

int Tool::DrawBBoxForGT(cv::Mat &mat_org)
{
	// �ʱ�ȭ �˻�
	CheckSetup();
	CheckImageNo();

	// �̹��� �ҷ�����
	mat_org = cv::imread(dataset_dir_ + "image/" + std::to_string(image_no_) + ".bmp");
	if (mat_org.empty())
	{
		std::cout << "�̹��� ���� ���� : " << image_no_ << "\n";
		std::abort();
	}

	// ���ذ� �ٿ�� �ڽ� �׸���
	const std::vector<BBox> &bboxes = map_gt_[image_no_];
	if (!bboxes.empty())
	{
		for (auto &&it : bboxes)
		{
			cv::Scalar color(255, 255, 255);
			if (it.first == "car_hard") color = cv::Scalar(255, 255, 255);
			cv::rectangle(mat_org, it.second, color, 2);
		}
	}

	return 0;
}

int Tool::Evaluation(const std::vector<cv::Rect2d> &bboxes)
{
	// �ʱ�ȭ �˻�
	CheckSetup();
	CheckImageNo();

	// �� �߰�
	const std::vector<BBox> &bboxes_gt = map_gt_[image_no_];
	Eval eval;
	eval.Compute(bboxes, bboxes_gt);
	map_eval_[image_no_] = eval;

	return 0;
}

int Tool::PrintResult()
{
	// �ʱ�ȭ �˻�
	CheckSetup();
	CheckImageNo();

	int eval_number = 0, total_tp = 0, total_fp = 0, total_fn = 0;
	double precision = 0.0, recall = 0.0, f1_measure = 0.0;
	for (auto &&it : map_eval_)
	{
		int tp = it.second.tp();
		int fp = it.second.fp();
		int fn = it.second.fn();
		total_tp += tp;
		total_fp += fp;
		total_fn += fn;
		++eval_number;

		std::cout << "TP : " << tp << "\t"
			<< "FP : " << fp << "\t"
			<< "FN : " << fn << "\n";
	}
	if (total_tp + total_fp != 0) precision = (double)total_tp / (double)(total_tp + total_fp);
	if (total_tp + total_fn != 0) recall = (double)total_tp / (double)(total_tp + total_fn);
	if (precision + recall != 0.0) f1_measure = 2 * precision * recall / (precision + recall);
	
	// ���� ���
	std::stringstream sstr;
	sstr << std::fixed << std::setprecision(3)
		<< "================================================== \n"
		<< "Number of evaluated image: " << eval_number << "\n"
		<< "TP : " << total_tp << "\n"
		<< "FP : " << total_fp << "\n"
		<< "FN : " << total_fn << "\n"
		<< "Precision : " << precision << "\n"
		<< "Recall : " << recall << "\n"
		<< "F1-measure : " << f1_measure << "\n"
		<< "================================================== \n";
	std::cout << sstr.str();
	
	// ���� ���
	std::ofstream fout(dataset_dir_ + "results.txt");
	fout << sstr.str();
	fout.close();

	return 0;
}

void Tool::CheckSetup()
{
	if (!flag_setup_)
	{
		std::cout << "�� ��ü�� �ʱ�ȭ���� ����, Setup�� ���� ȣ���ؾߵ�\n";
		std::abort();
	}
}

void Tool::CheckImageNo()
{
	if (image_no_ < 0 || image_no_ >= dataset_size_)
	{
		std::cout << "�� ��ü�� ������ �̹����� ���� : " << image_no_ << "\n";
		std::abort();
	}
}
