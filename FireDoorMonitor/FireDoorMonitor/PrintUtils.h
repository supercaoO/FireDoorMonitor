/**
 * @file 工具函数文件
 * 
 * 这个文件声明了日志和警告方法
 * 
 * @author 消防门安全检测项目组
 * @version 1.0
 */

#ifndef _PRINTUTILS
#define _PRINTUTILS
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

///@brief 分割线字符串
extern const string cutoff_rule;

void logging(const string &logging, bool end);
void warning(const string &warning, bool end);

#endif