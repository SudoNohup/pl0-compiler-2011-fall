#pragma once

#include "OptCode.h"
#include <string>
//#include "MiddleCode.h"
//#include <iostream>

using namespace std;

class Kuikong
{
public:
	//static OptCode Opt_code;

	
	static int point;
	static int tail;
	static int flagofflag[10000];
	static OptCode Opt_code;
	void Kuikonginit();
	int isCompare(string s);
	int isJmpTo(string s);
	int isSetFlag(string s);
	int isIfFalse(string s);
	void deleteSetFlag();
	int getNumberOfFlag(string s);
	void updateMiddleCode();
	void getFlagSum();
	void addOptCode(int id);
};
