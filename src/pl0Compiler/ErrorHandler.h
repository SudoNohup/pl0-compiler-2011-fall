#pragma once

#include <string>
using namespace std;

class ErrorHandler
{
public:
	//ErrorHandler();//���캯�������ڳ�ʼ����������Ϣ
	//ErrorHandler();
	static int errorTot;//��������
	int isType(string s);//��¼�Ƿ�������
	int ErrorHandler::isAllStartMark(string s);
	int ErrorHandler::isStartMark1(string s);
	int ErrorHandler::isStartMark2(string s);
	int ErrorHandler::isStartMark3(string s);
	int ErrorHandler::isbaseType(string s);
	void lexerError(int x,int y,string message);//�ʷ��������������
	void parserError(int x,int y,string message);//�﷨�������������
	void declareError(int x,int y,string message);//����������������
	void sentenceError(int x,int y,string message);//���������������ظ����壩���������

};