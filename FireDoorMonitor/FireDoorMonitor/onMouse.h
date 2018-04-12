#ifndef _ONMOUSE
#define _ONMOUSE

#include "opencv.hpp"
#include "highgui.h"
#include <opencv2/core/core.hpp>
#include <fstream>

using namespace cv;
using namespace std;

int numOfPoint = 0;

//1.回调函数签名  
void onMouse(int event, int x, int y, int flags, void* param);

//2.定义回调函数  
void onMouse(int event, int x, int y, int flags, void* param){
	Mat *im = reinterpret_cast<Mat*>(param);
	std::ofstream os("./coordinate.xml", ofstream::app);
	switch (event){
	case CV_EVENT_LBUTTONDOWN:
		os << x << " " << y << " ";
		++numOfPoint;
		break;
	case CV_EVENT_RBUTTONDOWN:
		os << x << " " << y << " ";
		++numOfPoint;
		break;
	}
}
#endif  