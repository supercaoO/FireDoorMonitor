/**
 * @file ��������ļ�
 * 
 * ����ļ�ֻ������main����
 * 
 * @author �����Ű�ȫ�����Ŀ��
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
 * @brief �������
 * @return 0
 */
int main(int argc, char* argv[]){
	//��ȡ�����ļ�
	vector<Fdm> fdmVec;
	parseXML(fdmVec);
	if(fdmVec.size() == 0)
		return -1;

	for(auto it = fdmVec.begin(); it != fdmVec.end(); it++){
		logging("�����ε��ͼ�������ŵ������Žǵ㡢�����Žǵ㡢�����Žǵ�������Žǵ�", true);

		//ѡȡ�Žǵ�
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

		//��װ�ĸ��ǵ�����
		vector<Point2f> cornersPoints = getCornersPoints();

		//��ӡ��־:��⿪ʼ
		string startMsg = "��ʼ���" + it->getImageUrl() + string(" Ŀ¼�µ� ") + to_string((long long) it->getBeginIndex())
			+ it->getImageFormat() + string(" �� ") + to_string((long long) it->getEndIndex()) + it->getImageFormat();
		logging(startMsg, true);

		//���������ţ����ýǵ����꣬Ĭ��״̬��Ϊ�ر�
		FireDoor fireDoor(cornersPoints);

		//˳����
		for(int imgNo = it->getBeginIndex(); imgNo != it->getEndIndex() + 1; ++imgNo){
			string nexImgNo = to_string((long long) imgNo);
			//��ӡ��־:ͼƬ������Ϣ
			logging("���ڼ�� " + nexImgNo + it->getImageFormat(), false);
			//������һ��ͼƬ������������״̬
			string nextImgPath = it->getImageUrl() + nexImgNo + it->getImageFormat();
			fireDoor.setNextImg(nextImgPath);
			fireDoor.setStatus(false);
			//�����������ü�ⷽ��
			if(fireDoor.judgeByBrightness(it->getBrightnessThreshold())){
				warning("�����Ŵ��ڴ�״̬������", true);
				continue;
			} else if(fireDoor.judgeByLines()) {
				warning("�����Ŵ��ڴ�״̬������", true);
				continue;
			} else if(fireDoor.judgeByCounting(it->getPointThreshold())){
				warning("�����Ŵ��ڴ�״̬������", true);
				continue;
			}
			logging("�����Ŵ��ڹر�״̬", true);
		}
	}
	waitKey(0);
	return 0;
}