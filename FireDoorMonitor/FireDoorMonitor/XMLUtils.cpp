#include "XMLUtils.h"

void parseXML(vector<Fdm> &fdmVec){
	//����XML�ļ�
	TiXmlDocument xml("./fdm.xml");
	//����޷���ȡ�ļ����򷵻�
	if(!xml.LoadFile()) return ;
	//XML�ļ�����
	TiXmlHandle xmlHandle(&xml);
	//ָ����ڵ��Ԫ��
	TiXmlElement* elem = xmlHandle.FirstChildElement().Element();
	//���ڵ�
	TiXmlHandle root(elem);
	//ָ��firedoor�ڵ��Ԫ��
	TiXmlElement* firedoor = root.FirstChild("firedoors").FirstChild("firedoor").Element();
	//����firedoor�ڵ�
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