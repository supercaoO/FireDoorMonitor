/**
 * @file 消防门类成员定义文件
 * 
 * 这个文件定义了类的所有可用方法
 * 
 * @author 消防门安全检测项目组
 * @version 1.0
 */
#include "FireDoor.h"

/**
 * @brief 默认构造函数
 * 
 * 将消防门置于关闭状态，构造对象
 */
FireDoor::FireDoor() : status(false){}

/**
 * @brief 构造函数
 * 
 * 将消防门置于关闭状态，并设置四个角点坐标矢量，构造对象
 * 
 * @param cps 消防门四个角点坐标矢量
 */
FireDoor::FireDoor(vector<Point2f> &cps) : status(false), cornersPoints(cps){}

/**
 * @brief 构造函数
 * 
 * 设置消防门图片，将消防门置于关闭状态，并设置四个角点坐标矢量，构造对象
 * 
 * @param nextImgPath 下一张消防门图片的文件路径
 * @param cps 消防门四个角点坐标矢量
 */
FireDoor::FireDoor(const string &nextImgPath, vector<Point2f> &cps) : status(false), cornersPoints(cps){
	Mat nextImg = imread(nextImgPath, 0);
	resize(nextImg, nextImg, Size(1920, 1080));
	img = nextImg;
}

/**
 * @brief 拷贝构造函数
 * 
 * 拷贝目标消防门的图片，状态和四个角点坐标矢量构造对象
 * 
 * @param fireDoor 被拷贝消防门
 */
FireDoor::FireDoor(const FireDoor &fireDoor) : img(fireDoor.img), status(fireDoor.status), cornersPoints(fireDoor.cornersPoints){}

/**
 * @brief 移动构造函数
 * 
 * 移动目标消防门的图片，状态和四个角点坐标矢量构造对象
 * 
 * @param fireDoor 被移动消防门
 */
FireDoor::FireDoor(FireDoor &&fireDoor) : img(std::move(fireDoor.img)), status(std::move(fireDoor.status)), cornersPoints(std::move(fireDoor.cornersPoints)){}

/**
 * @brief 拷贝赋值运算符
 * 
 * 拷贝目标消防门的图片，状态和四个角点坐标矢量并赋值
 * 
 * @param fireDoor 被拷贝消防门
 */
FireDoor& FireDoor::operator=(const FireDoor &fireDoor){
	img = fireDoor.img;
	status = fireDoor.status;
	cornersPoints = fireDoor.cornersPoints;
	return *this;
}

/**
 * @brief 移动赋值运算符
 * 
 * 移动目标消防门的图片，状态和四个角点坐标矢量并赋值
 * 
 * @param fireDoor 被移动消防门
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
 * @brief 析构函数
 * 
 * 销毁对象，释放内存
 */
FireDoor::~FireDoor() {}

/**
 * @brief 亮度检测方法
 * 
 * @param threshold 亮度变化程度阈值
 * @return true/false（消防门打开/关闭）
 */
bool FireDoor::judgeByBrightness(double threshold){
	logging("开始执行亮度检测", false);
	
	double bv = calcBrightnessVariability();;
	logging("图像的亮度变化值为 " + to_string((long double) bv), false);
	status = ((bv > threshold) ? true : status);
	return status;
}

/**
 * @brief 线条检测方法
 * 
 * @return true/false（消防门打开/关闭）
 */
bool FireDoor::judgeByLines(){
	logging("开始执行线条检测", false);
	Point2f topLeftCornerPoint((cornersPoints.at(0).x + cornersPoints.at(1).x) / 2 - 90, cornersPoints.at(0).y);
	Mat imgCut(img, Rect((int) topLeftCornerPoint.x, (int) topLeftCornerPoint.y, 180, (int) (cornersPoints.at(2).y - cornersPoints.at(0).y)));
	//边缘提取
	Mat imgForLine;
	Canny(imgCut, imgForLine, 10, 100, 3);
	//闭运算
	imgForLine = closeOperation(imgForLine);
	//边缘提取
	Canny(imgForLine, imgForLine, 10, 100, 3);
	int lineNum = hough(imgForLine);
	logging("线条检测方法中，在消防门中央处检测到 " + to_string((long long) lineNum) + " 条线条", false);
	status = ((lineNum > 2) ? true : status);
	return status;
}

/**
 * @brief 点数检测方法
 * 
 * @param threshold 上下门头点数比例阈值
 * @return true/false（消防门打开/关闭）
 */
bool FireDoor::judgeByCounting(double threshold){
	logging("开始执行点数检测", false);
	//重新设置图片大小
	resize(img, img, Size(800, 600));
	//中值滤波
	medianBlur(img, img, 7);
	//进行一次canny边缘检测
	Canny(img, img, 30, 50, 3);
	//矫正
	img = correct();
	double value = count(getTopImg(), getBottomImg());
	logging("点数检测方法中，在消防门上下门头检测到阈值为 " + to_string((long double) value), false);
	status = ((value < threshold) ? true : status);
	return status;
}

/**
 * @brief 设置下一张消防门图片
 * 
 * @param nextImg 下一张消防门图片
 */
void FireDoor::setNextImg(Mat &nextImg){
	resize(nextImg, nextImg, Size(1920, 1080));
	img = nextImg;
}

/**
 * @brief 设置下一张消防门图片
 * 
 * @param nextImgPath 下一张消防门图片的文件路径
 */
void FireDoor::setNextImg(string &nextImgPath){
	Mat nextImg = imread(nextImgPath, 0);
	setNextImg(nextImg);
}
/**
 * @brief 设置消防门的状态
 * 
 * @param nextStatus 下一个消防门的状态
 */
void FireDoor::setStatus(bool nextStatus){
	status = nextStatus;
}

//计算亮度变化程度并放回
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

//对参数图片执行闭运算并返回
Mat FireDoor::closeOperation(Mat &imgForLine){
	Mat tempImgForLine;
	Mat element5(43, 43, CV_8U, cv::Scalar(1));	//5*5正方形，8位uchar型，全1结构元素  
	morphologyEx(imgForLine, tempImgForLine, cv::MORPH_CLOSE, element5, Point(-1, -1), 1);
	return tempImgForLine;
}

//霍夫变换，返回门中部区域内的线条数
int FireDoor::hough(Mat &imgForLine){
	Mat ColorimgForLine;
	cvtColor(imgForLine, ColorimgForLine, CV_GRAY2BGR);
	vector<Vec4i> lines;
	HoughLinesP(imgForLine, lines, 1, CV_PI / 180, 100, 400, 80);	//倒数第二个参数700貌似更好400 80
	
	int temp = 0;
	for(size_t i = 0; i < lines.size(); ++i){
		line(ColorimgForLine, Point(lines[i][0], lines[i][1]),
			Point(lines[i][2], lines[i][3]), Scalar(255, 0, 255), 2, 8);
		++temp;
	}
	return temp;
}

//矫正函数
Mat FireDoor::correct(){
	Mat tempImg = img.clone();
	resize(tempImg, tempImg, Size(800, 600));
	float rowProportion = 800.0f / 1920.0f;
	float colProportion = 600.0f / 1080.0f;
	//标记800×600分辨率下的消防门的四个角点
	vector<Point2f> cornersCoor;
	cornersCoor.push_back(Point2f(cornersPoints.at(0).x * rowProportion, cornersPoints.at(0).y * colProportion));
	cornersCoor.push_back(Point2f(cornersPoints.at(1).x * rowProportion, cornersPoints.at(1).y * colProportion));
	cornersCoor.push_back(Point2f(cornersPoints.at(2).x * rowProportion, cornersPoints.at(2).y * colProportion));
	cornersCoor.push_back(Point2f(cornersPoints.at(3).x * rowProportion, cornersPoints.at(3).y * colProportion));
	//四个角点对应要校正的位置
	vector<Point2f> cornersCoor_trans;
	cornersCoor_trans.push_back(Point2f(100, 100));
	cornersCoor_trans.push_back(Point2f(700, 100));
	cornersCoor_trans.push_back(Point2f(100, 500));
	cornersCoor_trans.push_back(Point2f(700, 500));
	//图像矫正的映射关系
	Mat transform = getPerspectiveTransform(cornersCoor, cornersCoor_trans);
	Mat resultImg;
	warpPerspective(tempImg, resultImg, transform, tempImg.size());
	return resultImg;
}

//返回上门头区域图片
Mat FireDoor::getTopImg(){
	Mat topImg(img, Rect(126, 480, 550, 40));
	return topImg;
}

//返回下门头区域图片
Mat FireDoor::getBottomImg(){
	Mat bottomImg(img, Rect(126, 497, 550, 12));
	return bottomImg;
}

//计算上下门头区域点数比例
double FireDoor::count(Mat &topimg, Mat &buttomimg){
	//迭代器统计各点数
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