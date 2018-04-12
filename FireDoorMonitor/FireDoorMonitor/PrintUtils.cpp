/**
 * @file ���ߺ��������ļ�
 * 
 * ����ļ���������־�;��淽��
 * 
 * @author �����Ű�ȫ�����Ŀ��
 * @version 1.0
 */

#include "PrintUtils.h"

string const cutoff_rule(75, '-');

/**
 * @brief ��־
 * 
 * @param logging ��־��Ϣ
 * @param end �Ƿ���Ҫ��ӡ�ָ���
 */
void logging(const string &logging, bool end){
	ofstream os("./TestMsg.txt", ofstream::app);
	cout << "��Ϣ��" << logging << endl;
	os << "��Ϣ��" << logging << endl;
	if(end){
		cout << cutoff_rule << endl;
		os << cutoff_rule << endl;
	}
}
/**
 * @brief ����
 * 
 * @param warning ������Ϣ
 * @param end �Ƿ���Ҫ��ӡ�ָ���
 */
void warning(const string &warning, bool end){
	ofstream os("./TestMsg.txt", ofstream::app);
	cout << "���棺" << warning << endl;
	os << "���棺" << warning << endl;
	if(end){
		cout << cutoff_rule << endl;
		os << cutoff_rule << endl;
	}
}