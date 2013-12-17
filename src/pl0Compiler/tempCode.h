#pragma once

#include <string>
#include <deque>

using namespace std;

class tempCodeItem{
public:
	string opr;
	string target1;
	string target2;
	string result;
};

class tempCode{
public:
	static deque<tempCodeItem>codeList;
	void addCode(string opr,string target1,string target2,string result);
	void show();
};
