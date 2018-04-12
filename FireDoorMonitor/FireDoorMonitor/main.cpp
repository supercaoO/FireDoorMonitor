/**
 * @file 程序入口文件
 * 
 * 这个文件只定义了main函数
 * 
 * @author 消防门安全检测项目组
 * @version 1.0
 */
#include <iostream>
#include <fstream>
#include "FireDoor.h"
#include "PrintUtils.h"
#include "CoordinateUtils.h"
#include "onMouse.h"
#include "XMLUtils.h"

using namespace std;

extern int numOfPoint;

/**
 * @brief 程序入口
 * @return 0
 */
int main(int argc, char* argv[]){
	//读取配置文件
	vector<Fdm> fdmVec;
	parseXML(fdmVec);
	if(fdmVec.size() == 0)
		return -1;

	for(auto it = fdmVec.begin(); it != fdmVec.end(); it++){
		logging("请依次点击图中消防门的左上门角点、右上门角点、左下门角点和右下门角点", true);

		//选取门角点
		remove("./coordinate.xml");
		Mat sampleImg = imread(it->getImageUrl() + to_string((long long) it->getBeginIndex()) + it->getImageFormat(), 0);
		resize(sampleImg, sampleImg, Size(1920, 1080));
		namedWindow(it->getImageUrl(), WINDOW_NORMAL); 
		imshow(it->getImageUrl(), sampleImg);
		cv::setMouseCallback(it->getImageUrl(), onMouse, reinterpret_cast<void*> (&sampleImg));
		while(true){
			if(numOfPoint != 4){
				waitKey(1000);
			}
			else {
				numOfPoint = 0;
				break;
			}
		}

		//封装四个角点坐标
		vector<Point2f> cornersPoints = getCornersPoints();

		//打印日志:检测开始
		string startMsg = "开始检测" + it->getImageUrl() + string(" 目录下的 ") + to_string((long long) it->getBeginIndex())
			+ it->getImageFormat() + string(" 到 ") + to_string((long long) it->getEndIndex()) + it->getImageFormat();
		logging(startMsg, true);

		//构造消防门，设置角点坐标，默认状态置为关闭
		FireDoor fireDoor(cornersPoints);

		//顺序检测
		for(int imgNo = it->getBeginIndex(); imgNo != it->getEndIndex() + 1; ++imgNo){
			string nexImgNo = to_string((long long) imgNo);
			//打印日志:图片基本信息
			logging("正在检测 " + nexImgNo + it->getImageFormat(), false);
			//设置下一张图片，重置消防门状态
			string nextImgPath = it->getImageUrl() + nexImgNo + it->getImageFormat();
			fireDoor.setNextImg(nextImgPath);
			fireDoor.setStatus(false);
			//依据条件调用检测方法
			if(fireDoor.judgeByBrightness(it->getBrightnessThreshold())){
				warning("消防门处于打开状态！！！", true);
				continue;
			} else if(fireDoor.judgeByLines()) {
				warning("消防门处于打开状态！！！", true);
				continue;
			} else if(fireDoor.judgeByCounting(it->getPointThreshold())){
				warning("消防门处于打开状态！！！", true);
				continue;
			}
			logging("消防门处于关闭状态", true);
		}
	}
	waitKey(0);
	return 0;
}