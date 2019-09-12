#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void find_histoPos(Mat img, int & start, int &end, int direct)
{
	reduce(img, img, direct, REDUCE_AVG);

	int  minFound = 0;
	for (int i = 0; i< (int)img.total(); i++) {
		if (img.at<uchar>(i) < 250) 		// ������� �ƴϸ�
		{
			end = i;					// ������׷� ������ ��ġ
			if (!minFound) {
				start = i;			// ������׷� ù ��ġ
				minFound = 1;
			}
		}
	}
}

Mat find_number(Mat part)
{
	Point start, end;
	find_histoPos(part, start.x, end.x, 0);		// ���� ���� 
	find_histoPos(part, start.y, end.y, 1);   	// ���� ���� 

	return part(Rect(start, end));			// ���ڰ�ü ����
}

Mat  place_middle(Mat number, Size new_size , int outnum)
{
	float size_rate = (float)new_size.height/(float)new_size.width; //��� �̹����� ����
	float number_rate = (float)number.rows/(float)number.cols; //���� �̹����� ����
	Mat output(new_size,number.type(),Scalar(255)) , sample; //��� �̹����� ���� �̹���
	
	if(size_rate>number_rate)
	{
		//���� �̹����� ��� �̹����� ���� �� �ֵ��� ���� ����
		int height = (float)number.rows*(float)new_size.width/(float)number.cols;

		Size outsize(new_size.width , height);
		resize(number, sample, outsize); //���� �̹��� ������¡
	}
	else
	{
		//���� �̹����� ��� �̹����� ���� �� �ֵ��� ���� ����
		int width = (float)number.cols*(float)new_size.height/(float)number.rows;

		Size outsize(width , new_size.height);
		resize(number, sample, outsize); //���� �̹��� ������¡
	}

	Point start = (output.size() - sample.size()) /2; //���� ������
	Rect middle(start, sample.size()); // �߾� ���ɿ���
	sample.copyTo(output(middle)); // ���ڰ�ü�� �߾ӿ� ����	

	if(outnum>=0) //�н��ڷ� ���� �� ���� �̹��� ����
	{
		string demo_file = format("outnum/%02d.bmp", outnum);
		imwrite(demo_file,output);
	}

	output.convertTo(output, CV_32F); //�ڷ����� float�� ����
	return output.reshape(0, 1); // 1�� �����ͷ� ������ ��ȯ
	
	/*int  big = max(number.cols, number.rows);
	Mat  square(big, big, number.type(), Scalar(255));	// ���濵��

	Point start = (square.size() - number.size()) / 2;
	Rect middle(start, number.size());					// �߾� ���ɿ���
	number.copyTo(square(middle));						// ���ڰ�ü�� �߾ӿ� ����	
	
	resize(square, square, new_size);					// ũ�� ����

	string demo_file = format("outnum/%02d.png", outnum);
	imwrite(demo_file,square);

	square.convertTo(square, CV_32F);
	return square.reshape(0, 1);						// 1�� ������ ������ ��ȯ
	*/
}

int main()
{
	Size  size(40, 40);			// �н� �� ũ�� 
	int  K = 15;				// �����ϴ� �̿� ���ü�
	int  Nclass = 10;			// �ν� ����(ī�װ�) ����
	int  Nsample = 20;		// ���ڴ� �н� ���ü�

	string image_file = "train_numbers.png";
	Mat  train_image = imread(image_file, 0);		// ���� �ε�
	CV_Assert(train_image.data);

	threshold(train_image, train_image, 32, 255, CV_THRESH_BINARY);

	Mat  trainData, classLable;
	for (int i = 0, k = 0; i < Nclass; i++) {
		for (int j = 0; j < Nsample; j++, k++)
		{
			Point start(j * size.width, i * size.height);
			Rect  roi(start, size);
			Mat   part = train_image(roi);			// ���� ���� �и�

			Mat  num = find_number(part);		// ���� ���� ����
			Mat  data = place_middle(num, size , i*10+j);	// ����ȭ �� 1�൥���� ����
			trainData.push_back(data);		// �н� ������ ����
			classLable.push_back(i);		// ���̺�
		}
	}

	Ptr<ml::KNearest>  knn = ml::KNearest::create();
	knn->train(trainData, ml::ROW_SAMPLE, classLable);	// k-NN �н�

	
	while(1)
	{
		int no;
		cout << "�����ȣ�� �Է��ϼ���: ";
		cin >> no;									// �����ȣ �Է�

		string demo_file = format("num/%02d.png", no);
		Mat  test_img = imread(demo_file, 0);			// ���� ���� �ε�
		CV_Assert(test_img.data);						// ����ó��

		threshold(test_img, test_img, 128, 255, THRESH_BINARY);	// ����ȭ
		Mat  num = find_number(test_img);				// ���ڰ�ü ����
		Mat  data = place_middle(num, size,-1);			// ���ڰ�ü �� �߽� ��ġ 

		Mat result;
		knn->findNearest(data, K, result);				// ���� �з� ����

		cout << "�з���� : " << result.at<float>(0) << endl;
	}


	return 0; 
}