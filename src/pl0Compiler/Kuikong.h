#pragma once

#include "OptCode.h"
#include <string>
//#include "MiddleCode.h"
//#include <iostream>

using namespace std;

class Kuikong
{
public:
	//static OptCode Kuikong::Opt_code;

	
	static int Kuikong::point;
	static int Kuikong::tail;
	static int Kuikong::flagofflag[10000];
	static OptCode Kuikong::Opt_code;
	void Kuikonginit();
	int isCompare(string s);
	int isJmpTo(string s);
	int Kuikong::isSetFlag(string s);
	int Kuikong::isIfFalse(string s);
	void Kuikong::deleteSetFlag();
	int Kuikong::getNumberOfFlag(string s);
	void Kuikong::updateMiddleCode();
	void Kuikong::getFlagSum();
	void addOptCode(int id);
};