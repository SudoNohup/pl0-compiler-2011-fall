#pragma once

#include <deque>
#include <string>

using namespace std;
class MiddleCodeItem//�м����Ľṹ����Ԫʽ��
{
public:
	string opr;
	string target1;
	string target2;
	string result;
};

class MiddleCode//��¼�м����
{
public:
	static deque<MiddleCodeItem>codeList;//�м����
	void addMiddleCode(string opr,string target1,string target2,string result);//�����м����
	void showMiddleCode();//չʾ�м����
};