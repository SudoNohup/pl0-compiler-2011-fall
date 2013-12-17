#pragma once

#include<string>
using namespace std;

class LexerItem//记录词法分析得到的每个单词信息
{
public:
	int kind;
	string symbol;
};