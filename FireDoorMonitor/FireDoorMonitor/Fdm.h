#ifndef _FDM
#define _FDM
#include <string>

using namespace std;

class Fdm{
private:
	string imageUrl;
	string imageFormat;
	int beginIndex;
	int endIndex;
	double brightnessThreshold;
	double pointThreshold;
public:
	Fdm(const string imageUrl_, string imageFormat_, int beginIndex_, int endIndex_, double brightnessThreshold_, double pointThreshold_)
	: imageUrl(imageUrl_), imageFormat(imageFormat_), beginIndex(beginIndex_), endIndex(endIndex_), brightnessThreshold(brightnessThreshold_), pointThreshold(pointThreshold_){}
	string getImageUrl() { return this->imageUrl; }
	string getImageFormat() { return this->imageFormat; }
	int getBeginIndex() { return this->beginIndex; }
	void setBeginIndex(const int beginIndex) { this->beginIndex = beginIndex; }
	int getEndIndex() { return this->endIndex; }
	void setEndIndex(const int endIndex) { this->endIndex = endIndex; }
	double getBrightnessThreshold() { return this->brightnessThreshold; }
	void setBrightnessThreshold(const double brightnessThreshold) { this->brightnessThreshold = brightnessThreshold; }
	double getPointThreshold() { return this->pointThreshold; }
	void setPointThreshold(const double pointThreshold) { this->pointThreshold = pointThreshold; }
};

#endif  