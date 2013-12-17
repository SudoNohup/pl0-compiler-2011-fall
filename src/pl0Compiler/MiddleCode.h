#pragma once

#include <deque>
#include <string>

using namespace std;
class MiddleCodeItem//中间代码的结构（四元式）
{
public:
	string opr;
	string target1;
	string target2;
	string result;
};

class MiddleCode//记录中间代码
{
public:
	static deque<MiddleCodeItem>codeList;//中间代码
	void addMiddleCode(string opr,string target1,string target2,string result);//增加中间代码
	void showMiddleCode();//展示中间代码
};