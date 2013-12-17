#pragma once

#include <deque>
#include "LexerItem.h"


using namespace std;

class Lexer//词法分析
{
public:
	Lexer();//构造函数，初始化词法分析Lexer类
	LexerItem getSym(int mode=0);//读下一个单词
	LexerItem getNextSym(int i);//用于跳读第i单词
	void testLexer();					//测试
	int x();							//返回行号
	int y();							//返回列号
	static char ch;			//ch为当前字符
	static int NowNo,tot;		//NowNo为字符编号，tot为该行总字符数
	static int NoOfLine;	//行数
	static char line[1000];	//该行字符串
	static deque<LexerItem>SymbolList;//用双端队列存取读到的单词，便于从后面前面加单词(push_back)，以及从前面取单词(pop_front)
	int getch();		//0正常返回，-1异常
	int isBlank(char c);//判断是否是空格
	int isLetter(char c);//判断是否是字符
	int isNumber(char c);//判断是否是数字
	int isKeyWord(string s);//判断是否是保留字，返回保留字的序号或者直接是标志符的序号
};