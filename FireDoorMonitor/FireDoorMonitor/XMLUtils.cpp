#include "XMLUtils.h"

void parseXML(vector<Fdm> &fdmVec){
	//读入XML文件
	TiXmlDocument xml("./fdm.xml");
	//如果无法读取文件，则返回
	if(!xml.LoadFile()) return ;
	//XML文件对象
	TiXmlHandle xmlHandle(&xml);
	//指向根节点的元素
	TiXmlElement* elem = xmlHandle.FirstChildElement().Element();
	//根节点
	TiXmlHandle root(elem);
	//指向firedoor节点的元素
	TiXmlElement* firedoor = root.FirstChild("firedoors").FirstChild("firedoor").Element();
	//遍历firedoor节点
	for(firedoor; firedoor; firedoor = firedoor->NextSiblingElement()){
		TiXmlHandle firedoor_(firedoor);
		TiXmlElement* image = firedoor_.FirstChild("image").Element();
		TiXmlHandle image_(image);
		TiXmlElement* url = image_.FirstChild("url").Element();
		TiXmlElement* format = image_.FirstChild("format").Element();
		TiXmlElement* beginIndex = image_.FirstChild("begin-index").Element();
		TiXmlElement* endIndex = image_.FirstChild("end-index").Element();
		TiXmlElement* threshold = firedoor_.FirstChild("threshold").Element();
		TiXmlHandle threshold_(threshold);
		TiXmlElement* brightness = threshold_.FirstChild("brightness").Element();
		TiXmlElement* point = threshold_.FirstChild("point").Element();
		string urlStr = url->GetText();
		if(urlStr.back() != '/')
			urlStr += "/";
		string formatStr = format->GetText();
		if(formatStr.front() != '.')
			formatStr = "." + formatStr;
		Fdm fdm(urlStr, formatStr, atoi(beginIndex->GetText()), atoi(endIndex->GetText()), atof(brightness->GetText()), atof(point->GetText()));
		fdmVec.push_back(fdm);
	}
}