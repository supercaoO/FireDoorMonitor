/**
 * @file ���ߺ����ļ�
 * 
 * ����ļ���������־�;��淽��
 * 
 * @author �����Ű�ȫ�����Ŀ��
 * @version 1.0
 */

#ifndef _PRINTUTILS
#define _PRINTUTILS
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

///@brief �ָ����ַ���
extern const string cutoff_rule;

void logging(const string &logging, bool end);
void warning(const string &warning, bool end);

#endif