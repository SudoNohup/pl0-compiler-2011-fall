#pragma once

#include <deque>
#include <string>
using namespace std;

class BaseBlock
{
public:
	static int flag[10000];
	static deque<string>BaseBlock::activeVar;
	//static int flagToProc[10000];
	static deque<int> in[1000];
	static deque<int> out[1000];
	static deque<string> use[1000];
	static deque<string> define[1000];
	static deque<string> varIn[1000];
	static deque<string> varOut[1000];
	void init();
	void print();
	int findFlag(string s);
	int BaseBlock::isCompareFalse(string s);
	int BaseBlock::existsOut(int blockID,int id);
	int BaseBlock::isCalc(string op);
	void  BaseBlock::addUse(int blockID,string id);
	void BaseBlock::addDefine(int blockID,string id);
	int BaseBlock::isCompare(string s);
	void activeVarInit();
	int BaseBlock::existsVarIn(int blockID,string id);
	int BaseBlock::existsVarOut(int blockID,string id);
	int existsDefine(int blockID,string id);
	void BaseBlock::activeVarBegin();
	bool BaseBlock::activeVarExists(string s);
	//void BaseBlock::conflictGraghBuilt();
	//void 
};