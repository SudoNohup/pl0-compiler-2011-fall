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
	int isAllStartMark(string s);
	int isStartMark1(string s);
	int isStartMark2(string s);
	int isStartMark3(string s);
	int isbaseType(string s);
	void lexerError(int x,int y,string message);//�ʷ��������������
	void parserError(int x,int y,string message);//�﷨�������������
	void declareError(int x,int y,string message);//����������������
	void sentenceError(int x,int y,string message);//���������������ظ����壩���������

};
