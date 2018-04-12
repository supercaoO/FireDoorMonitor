/**
 * @file 工具函数定义文件
 * 
 * 这个文件定义了日志和警告方法
 * 
 * @author 消防门安全检测项目组
 * @version 1.0
 */

#include "PrintUtils.h"

string const cutoff_rule(75, '-');

/**
 * @brief 日志
 * 
 * @param logging 日志信息
 * @param end 是否需要打印分割线
 */
void logging(const string &logging, bool end){
	ofstream os("./TestMsg.txt", ofstream::app);
	cout << "信息：" << logging << endl;
	os << "信息：" << logging << endl;
	if(end){
		cout << cutoff_rule << endl;
		os << cutoff_rule << endl;
	}
}
/**
 * @brief 警告
 * 
 * @param warning 警告信息
 * @param end 是否需要打印分割线
 */
void warning(const string &warning, bool end){
	ofstream os("./TestMsg.txt", ofstream::app);
	cout << "警告：" << warning << endl;
	os << "警告：" << warning << endl;
	if(end){
		cout << cutoff_rule << endl;
		os << cutoff_rule << endl;
	}
}