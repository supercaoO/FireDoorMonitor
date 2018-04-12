/**
 * @file ���������Ա�����ļ�
 * 
 * ����ļ�������������п��÷���
 * 
 * @author �����Ű�ȫ�����Ŀ��
 * @version 1.0
 */
#include "FireDoor.h"

/**
 * @brief Ĭ�Ϲ��캯��
 * 
 * �����������ڹر�״̬���������
 */
FireDoor::FireDoor() : status(false){}

/**
 * @brief ���캯��
 * 
 * �����������ڹر�״̬���������ĸ��ǵ�����ʸ�����������
 * 
 * @param cps �������ĸ��ǵ�����ʸ��
 */
FireDoor::FireDoor(vector<Point2f> &cps) : status(false), cornersPoints(cps){}

/**
 * @brief ���캯��
 * 
 * ����������ͼƬ�������������ڹر�״̬���������ĸ��ǵ�����ʸ�����������
 * 
 * @param nextImgPath ��һ��������ͼƬ���ļ�·��
 * @param cps �������ĸ��ǵ�����ʸ��
 */
FireDoor::FireDoor(const string &nextImgPath, vector<Point2f> &cps) : status(false), cornersPoints(cps){
	Mat nextImg = imread(nextImgPath, 0);
	resize(nextImg, nextImg, Size(1920, 1080));
	img = nextImg;
}

/**
 * @brief �������캯��
 * 
 * ����Ŀ�������ŵ�ͼƬ��״̬���ĸ��ǵ�����ʸ���������
 * 
 * @param fireDoor ������������
 */
FireDoor::FireDoor(const FireDoor &fireDoor) : img(fireDoor.img), status(fireDoor.status), cornersPoints(fireDoor.cornersPoints){}

/**
 * @brief �ƶ����캯��
 * 
 * �ƶ�Ŀ�������ŵ�ͼƬ��״̬���ĸ��ǵ�����ʸ���������
 * 
 * @param fireDoor ���ƶ�������
 */
FireDoor::FireDoor(FireDoor &&fireDoor) : img(std::move(fireDoor.img)), status(std::move(fireDoor.status)), cornersPoints(std::move(fireDoor.cornersPoints)){}

/**
 * @brief ������ֵ�����
 * 
 * ����Ŀ�������ŵ�ͼƬ��״̬���ĸ��ǵ�����ʸ������ֵ
 * 
 * @param fireDoor ������������
 */
FireDoor& FireDoor::operator=(const FireDoor &fireDoor){
	img = fireDoor.img;
	status = fireDoor.status;
	cornersPoints = fireDoor.cornersPoints;
	return *this;
}

/**
 * @brief �ƶ���ֵ�����
 * 
 * �ƶ�Ŀ�������ŵ�ͼƬ��״̬���ĸ��ǵ�����ʸ������ֵ
 * 
 * @param fireDoor ���ƶ�������
 */
FireDoor& FireDoor::operator=(FireDoor &&fireDoor){
	if(this != &fireDoor){
		img = std::move(fireDoor.img);
		status = std::move(fireDoor.status);
		cornersPoints = std::move(fireDoor.cornersPoints);
	}
	return *this;
}

/**
 * @brief ��������
 * 
 * ���ٶ����ͷ��ڴ�
 */
FireDoor::~FireDoor() {}

/**
 * @brief ���ȼ�ⷽ��
 * 
 * @param threshold ���ȱ仯�̶���ֵ
 * @return true/false�������Ŵ�/�رգ�
 */
bool FireDoor::judgeByBrightness(double threshold){
	logging("��ʼִ�����ȼ��", false);
	
	double bv = calcBrightnessVariability();;
	logging("ͼ������ȱ仯ֵΪ " + to_string((long double) bv), false);
	status = ((bv > threshold) ? true : status);
	return status;
}

/**
 * @brief ������ⷽ��
 * 
 * @return true/false�������Ŵ�/�رգ�
 */
bool FireDoor::judgeByLines(){
	logging("��ʼִ���������", false);
	Point2f topLeftCornerPoint((cornersPoints.at(0).x + cornersPoints.at(1).x) / 2 - 90, cornersPoints.at(0).y);
	Mat imgCut(img, Rect((int) topLeftCornerPoint.x, (int) topLeftCornerPoint.y, 180, (int) (cornersPoints.at(2).y - cornersPoints.at(0).y)));
	//��Ե��ȡ
	Mat imgForLine;
	Canny(imgCut, imgForLine, 10, 100, 3);
	//������
	imgForLine = closeOperation(imgForLine);
	//��Ե��ȡ
	Canny(imgForLine, imgForLine, 10, 100, 3);
	int lineNum = hough(imgForLine);
	logging("������ⷽ���У������������봦��⵽ " + to_string((long long) lineNum) + " ������", false);
	status = ((lineNum > 2) ? true : status);
	return status;
}

/**
 * @brief ������ⷽ��
 * 
 * @param threshold ������ͷ����������ֵ
 * @return true/false�������Ŵ�/�رգ�
 */
bool FireDoor::judgeByCounting(double threshold){
	logging("��ʼִ�е������", false);
	//��������ͼƬ��С
	resize(img, img, Size(800, 600));
	//��ֵ�˲�
	medianBlur(img, img, 7);
	//����һ��canny��Ե���
	Canny(img, img, 30, 50, 3);
	//����
	img = correct();
	double value = count(getTopImg(), getBottomImg());
	logging("������ⷽ���У���������������ͷ��⵽��ֵΪ " + to_string((long double) value), false);
	status = ((value < threshold) ? true : status);
	return status;
}

/**
 * @brief ������һ��������ͼƬ
 * 
 * @param nextImg ��һ��������ͼƬ
 */
void FireDoor::setNextImg(Mat &nextImg){
	resize(nextImg, nextImg, Size(1920, 1080));
	img = nextImg;
}

/**
 * @brief ������һ��������ͼƬ
 * 
 * @param nextImgPath ��һ��������ͼƬ���ļ�·��
 */
void FireDoor::setNextImg(string &nextImgPath){
	Mat nextImg = imread(nextImgPath, 0);
	setNextImg(nextImg);
}
/**
 * @brief ���������ŵ�״̬
 * 
 * @param nextStatus ��һ�������ŵ�״̬
 */
void FireDoor::setStatus(bool nextStatus){
	status = nextStatus;
}

//�������ȱ仯�̶Ȳ��Ż�
double FireDoor::calcBrightnessVariability(){
	Mat covar, mean1;
	Mat tempImg = correct();
	float rowProportion = 800.0f / 1920.0f;
	float colProportion = 600.0f / 1080.0f;
	Mat imgCut(tempImg, Rect(126, 95, 550, 405));
	calcCovarMatrix(imgCut, covar, mean1, CV_COVAR_NORMAL | CV_COVAR_ROWS);
	Mat mean2, stdDev;
	meanStdDev(mean1, mean2, stdDev);
	return stdDev.at<double>(0, 0);
}

//�Բ���ͼƬִ�б����㲢����
Mat FireDoor::closeOperation(Mat &imgForLine){
	Mat tempImgForLine;
	Mat element5(43, 43, CV_8U, cv::Scalar(1));	//5*5�����Σ�8λuchar�ͣ�ȫ1�ṹԪ��  
	morphologyEx(imgForLine, tempImgForLine, cv::MORPH_CLOSE, element5, Point(-1, -1), 1);
	return tempImgForLine;
}

//����任���������в������ڵ�������
int FireDoor::hough(Mat &imgForLine){
	Mat ColorimgForLine;
	cvtColor(imgForLine, ColorimgForLine, CV_GRAY2BGR);
	vector<Vec4i> lines;
	HoughLinesP(imgForLine, lines, 1, CV_PI / 180, 100, 400, 80);	//�����ڶ�������700ò�Ƹ���400 80
	
	int temp = 0;
	for(size_t i = 0; i < lines.size(); ++i){
		line(ColorimgForLine, Point(lines[i][0], lines[i][1]),
			Point(lines[i][2], lines[i][3]), Scalar(255, 0, 255), 2, 8);
		++temp;
	}
	return temp;
}

//��������
Mat FireDoor::correct(){
	Mat tempImg = img.clone();
	resize(tempImg, tempImg, Size(800, 600));
	float rowProportion = 800.0f / 1920.0f;
	float colProportion = 600.0f / 1080.0f;
	//���800��600�ֱ����µ������ŵ��ĸ��ǵ�
	vector<Point2f> cornersCoor;
	cornersCoor.push_back(Point2f(cornersPoints.at(0).x * rowProportion, cornersPoints.at(0).y * colProportion));
	cornersCoor.push_back(Point2f(cornersPoints.at(1).x * rowProportion, cornersPoints.at(1).y * colProportion));
	cornersCoor.push_back(Point2f(cornersPoints.at(2).x * rowProportion, cornersPoints.at(2).y * colProportion));
	cornersCoor.push_back(Point2f(cornersPoints.at(3).x * rowProportion, cornersPoints.at(3).y * colProportion));
	//�ĸ��ǵ��ӦҪУ����λ��
	vector<Point2f> cornersCoor_trans;
	cornersCoor_trans.push_back(Point2f(100, 100));
	cornersCoor_trans.push_back(Point2f(700, 100));
	cornersCoor_trans.push_back(Point2f(100, 500));
	cornersCoor_trans.push_back(Point2f(700, 500));
	//ͼ�������ӳ���ϵ
	Mat transform = getPerspectiveTransform(cornersCoor, cornersCoor_trans);
	Mat resultImg;
	warpPerspective(tempImg, resultImg, transform, tempImg.size());
	return resultImg;
}

//��������ͷ����ͼƬ
Mat FireDoor::getTopImg(){
	Mat topImg(img, Rect(126, 480, 550, 40));
	return topImg;
}

//��������ͷ����ͼƬ
Mat FireDoor::getBottomImg(){
	Mat bottomImg(img, Rect(126, 497, 550, 12));
	return bottomImg;
}

//����������ͷ�����������
double FireDoor::count(Mat &topimg, Mat &buttomimg){
	//������ͳ�Ƹ�����
	double pointnum0 = 0, pointnum1 = 0;
	for (MatIterator_<uchar> grayit = topimg.begin<uchar>(), grayend = topimg.end<uchar>(); grayit != grayend; ++grayit){
		if (*grayit > 0)
			++pointnum0;
	}
	for (MatIterator_<uchar> grayit = buttomimg.begin<uchar>(), grayend = buttomimg.end<uchar>(); grayit != grayend; ++grayit){
		if (*grayit > 0)
			++pointnum1;
	}
	return pointnum1 / pointnum0;
}