#include "CoordinateUtils.h"
#include "PrintUtils.h"

vector<Point2f> getCornersPoints(){
	logging("选取门角点完成", false);
	vector<Point2f> cornersPoints;
	ifstream is("./coordinate.xml");
	stringstream ss;
	string tempStr;
	float x, y;
	string position[] = { "左上门角坐标", "右上门角坐标", "左下门角坐标", "右下门角坐标"};
	for(int i = 0; i != 4; ++i){
		is >> tempStr;
		ss << tempStr;
		ss >> x;
		ss.clear();
		is >> tempStr;
		ss << tempStr;
		ss >> y;
		ss.clear();
		if(i == 3){
			logging(position[i] + "(" + to_string((long long) x) + ", " + to_string((long long) y) + ")", true);
		} else {
			logging(position[i] + "(" + to_string((long long) x) + ", " + to_string((long long) y) + ")", false);
		}
		Point2f p(x, y);
		cornersPoints.push_back(p);
	}
	return cornersPoints;
}