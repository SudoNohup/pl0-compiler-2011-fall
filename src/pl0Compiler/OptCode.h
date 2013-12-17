#pragma once

#include <string>
#include <deque>

using namespace std;

class OptCodeItem{
public:
	string opr;
	string target1;
	string target2;
	string result;
};

class OptCode{
public:
	static deque<OptCodeItem>codeList;
	void addCode(string opr,string target1,string target2,string result);
	void show();
};
