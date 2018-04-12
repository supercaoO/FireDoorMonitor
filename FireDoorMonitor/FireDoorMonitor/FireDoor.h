/**
 * @file ���������ļ�
 * 
 * ����ļ�ֻ��������������
 * 
 * @author �����Ű�ȫ�����Ŀ��
 * @version 1.0
 */

#ifndef _FIREDOOR
#define _FIREDOOR
#include <cv.h>  
#include <highgui.h>  
#include <math.h>  
#include <iostream>
#include <string>
#include <windows.h>
#include <opencv2/opencv.hpp>  
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>  
#include "cxcore.h"
#include "PrintUtils.h"

using namespace cv;
using namespace std;

/**
 * @brief ��������
 * 
 * ��չʾ����Ĺ��к������Ա
 */
class FireDoor{
private:
	Mat img;
	bool status;
	vector<Point2f> cornersPoints;
	double calcBrightnessVariability();
	Mat closeOperation(Mat &newimg);
	int hough(Mat &imgForLine);
	Mat correct();
	Mat getTopImg();
	Mat getBottomImg();
	double count(Mat &topimg, Mat &buttomimg);
public:
	explicit FireDoor();
	explicit FireDoor(vector<Point2f> &cps);
	explicit FireDoor(const string &nextImgPath, vector<Point2f> &cps);
	FireDoor(const FireDoor &fireDoor);
	FireDoor(FireDoor &&fireDoor);
	FireDoor& operator=(const FireDoor &fireDoor);
	FireDoor& operator=(FireDoor &&fireDoor);
	~FireDoor();
	bool judgeByBrightness(double threshold);
	bool judgeByLines();
	bool judgeByCounting(double threshold);
	void setNextImg(Mat &nextImg);
	void setNextImg(string &nextImgPath);
	void setStatus(bool nextStatus);
};
#endif  