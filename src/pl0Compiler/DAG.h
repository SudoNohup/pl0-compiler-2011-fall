#pragma once

#include <deque>
#include <string>
#include "DAGCode.h"
#include "tempCode.h"
using namespace std;

class DAGItem
{
public:
	string Name;
	int Left;
	int Right;
	bool isleaf;
	deque <int> FatherNode;
};

class DAG
{
public:
	static int point;
	static int tail;
	static DAGCode Opt_Code;
	static deque<string> varName;
	static deque<int> varID;
	static deque<DAGItem>joint;
	static deque<DAGItem>line;
	static deque<int>lineNo;
	static int father[10000];
	tempCode arrayEqual;
	static int procNum;
	void DAGinit();
	void addOptCode(int id);
	int isCalc(string op);
	int findInJoint(string name,int left,int right);
	int findInVar(string name);
	int isVar(string id);
	void linkVar(string id,int i);
	void arrayEqualAdd(int point);
	void DAGdeal(int head,int tail);
	void fatherAdd(int child,int father);
	deque<string> getJointSymbol(int id);//获取节点表中某节点对应的重合的变量名。
	int getNoFather();
	void DAGline();
	int getPoint(string id);
	void updateMiddleCode();
};

