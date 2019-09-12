#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void find_histoPos(Mat img, int & start, int &end, int direct)
{
	reduce(img, img, direct, REDUCE_AVG);

	int  minFound = 0;
	for (int i = 0; i< (int)img.total(); i++) {
		if (img.at<uchar>(i) < 250) 		// 빈라인이 아니면
		{
			end = i;					// 히스토그램 마지막 위치
			if (!minFound) {
				start = i;			// 히스토그램 첫 위치
				minFound = 1;
			}
		}
	}
}

Mat find_number(Mat part)
{
	Point start, end;
	find_histoPos(part, start.x, end.x, 0);		// 수직 투영 
	find_histoPos(part, start.y, end.y, 1);   	// 수평 투영 

	return part(Rect(start, end));			// 숫자객체 영상
}

Mat  place_middle(Mat number, Size new_size , int outnum)
{
	float size_rate = (float)new_size.height/(float)new_size.width; //출력 이미지의 비율
	float number_rate = (float)number.rows/(float)number.cols; //숫자 이미지의 비율
	Mat output(new_size,number.type(),Scalar(255)) , sample; //출력 이미지와 샘플 이미지
	
	if(size_rate>number_rate)
	{
		//숫자 이미지가 출력 이미지에 넣을 수 있도록 높이 조절
		int height = (float)number.rows*(float)new_size.width/(float)number.cols;

		Size outsize(new_size.width , height);
		resize(number, sample, outsize); //숫자 이미지 리사이징
	}
	else
	{
		//숫자 이미지가 출력 이미지에 넣을 수 있도록 넓이 조절
		int width = (float)number.cols*(float)new_size.height/(float)number.rows;

		Size outsize(width , new_size.height);
		resize(number, sample, outsize); //숫자 이미지 리사이징
	}

	Point start = (output.size() - sample.size()) /2; //복사 시작점
	Rect middle(start, sample.size()); // 중앙 관심영역
	sample.copyTo(output(middle)); // 숫자객체를 중앙에 복사	

	if(outnum>=0) //학습자료 수집 중 샘플 이미지 저장
	{
		string demo_file = format("outnum/%02d.bmp", outnum);
		imwrite(demo_file,output);
	}

	output.convertTo(output, CV_32F); //자료형을 float로 변경
	return output.reshape(0, 1); // 1행 데이터로 변경후 반환
	
	/*int  big = max(number.cols, number.rows);
	Mat  square(big, big, number.type(), Scalar(255));	// 정방영상

	Point start = (square.size() - number.size()) / 2;
	Rect middle(start, number.size());					// 중앙 관심영역
	number.copyTo(square(middle));						// 숫자객체를 중앙에 복사	
	
	resize(square, square, new_size);					// 크기 변경

	string demo_file = format("outnum/%02d.png", outnum);
	imwrite(demo_file,square);

	square.convertTo(square, CV_32F);
	return square.reshape(0, 1);						// 1행 데이터 변경후 반환
	*/
}

int main()
{
	Size  size(40, 40);			// 학습 셀 크기 
	int  K = 15;				// 선출하는 이웃 샘플수
	int  Nclass = 10;			// 인식 숫자(카테고리) 개수
	int  Nsample = 20;		// 숫자당 학습 샘플수

	string image_file = "train_numbers.png";
	Mat  train_image = imread(image_file, 0);		// 영상 로드
	CV_Assert(train_image.data);

	threshold(train_image, train_image, 32, 255, CV_THRESH_BINARY);

	Mat  trainData, classLable;
	for (int i = 0, k = 0; i < Nclass; i++) {
		for (int j = 0; j < Nsample; j++, k++)
		{
			Point start(j * size.width, i * size.height);
			Rect  roi(start, size);
			Mat   part = train_image(roi);			// 숫자 영상 분리

			Mat  num = find_number(part);		// 숫자 영역 영상
			Mat  data = place_middle(num, size , i*10+j);	// 정규화 및 1행데이터 구성
			trainData.push_back(data);		// 학습 데이터 수집
			classLable.push_back(i);		// 레이블링
		}
	}

	Ptr<ml::KNearest>  knn = ml::KNearest::create();
	knn->train(trainData, ml::ROW_SAMPLE, classLable);	// k-NN 학습

	
	while(1)
	{
		int no;
		cout << "영상번호를 입력하세요: ";
		cin >> no;									// 영상번호 입력

		string demo_file = format("num/%02d.png", no);
		Mat  test_img = imread(demo_file, 0);			// 실험 영상 로드
		CV_Assert(test_img.data);						// 예외처리

		threshold(test_img, test_img, 128, 255, THRESH_BINARY);	// 이진화
		Mat  num = find_number(test_img);				// 숫자객체 검출
		Mat  data = place_middle(num, size,-1);			// 숫자객체 셀 중심 배치 

		Mat result;
		knn->findNearest(data, K, result);				// 숫자 분류 수행

		cout << "분류결과 : " << result.at<float>(0) << endl;
	}


	return 0; 
}