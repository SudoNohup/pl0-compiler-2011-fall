#pragma once

#include <string>
#include <deque>

using namespace std;

class DAGCodeItem{
public:
	string opr;
	string target1;
	string target2;
	string result;
};

class DAGCode{
public:
	static deque<DAGCodeItem>codeList;
	void addCode(string opr,string target1,string target2,string result);
	void show();
};
