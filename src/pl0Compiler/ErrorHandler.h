#pragma once

#include <string>
using namespace std;

class ErrorHandler
{
public:
	//ErrorHandler();//构造函数，用于初始化错误处理信息
	//ErrorHandler();
	static int errorTot;//错误总数
	int isType(string s);//记录是否是类型
	int ErrorHandler::isAllStartMark(string s);
	int ErrorHandler::isStartMark1(string s);
	int ErrorHandler::isStartMark2(string s);
	int ErrorHandler::isStartMark3(string s);
	int ErrorHandler::isbaseType(string s);
	void lexerError(int x,int y,string message);//词法分析错误处理程序
	void parserError(int x,int y,string message);//语法分析错误处理程序
	void declareError(int x,int y,string message);//语义分析错误处理程序
	void sentenceError(int x,int y,string message);//声明（变量或常量重复定义）错误处理程序

};