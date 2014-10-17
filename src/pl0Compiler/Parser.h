#pragma once

#include "SymbolTable.h"
#include "LexerItem.h"

using namespace std;

class Parser//语法分析与语义分析
{
public:
	static LexerItem sym;//词法分析得到的单词信息
	static int nowProcNum;	//过程编号
	static kindType t;	//语法制导 类型变量t
	static string idName;//标识名
	static int tempNum;//临时变量No
	static int tempAdrNum;//临时地址变量No（该变量中存放的是数组元素或者传地址时的地址）
	static int flagNum;//跳转点的No
	static int stringNum;//字符串的No
	static int level;//第level层
	static bool functionFlag;

	void initParser();//相对于构造函数，用于初始化
	int isbaseType(string s);//是否是基本类型
	int isCompare(string s);//是否是关系运算符
	int isAdd(string s);//是否是加减运算符
	int isMul(string s);//是否是乘除运算符
	void program();//程序
	void block();//分程序
	void constDes();//常量说明部分
	void constDefine();//常量定义
	//void constant();//<常量>
	string constValue();//<常量>
	void varDes();//变量说明部分
	void varDefine();//变量说明
	void function(int lastProcNum);//函数说明部分
	void functionStart(int lastProcNum);//函数首部
	void procedure(int lastProcNum);//过程说明部分
	void procedureStart(int lastProcNum);//过程首部
	void argTable();//形式参数表
	void arg(bool tempflag);//形式参数段
	void complexSentense(int lastProcNum);//复合语句
	void statement(int lastProcNum);//语句
	void becomesState(int lastProcNum);//赋值语句
	string expression(int lastProcNum);//表达式
	string term(int lastProcNum);//项
	string factor(int lastProcNum);//因子
	string callFunction(int lastProcNum);//函数调用语句
	string callProcedure(int lastProcNum);//过程调用语句
	int valueTable(int lastProcNum,int ProcIndex,string procName);//实在参数表
	void ifState(int lastProcNum);//条件语句
	string condition(int lastProcNum);//条件
	void caseState(int lastProcNum);//情况语句
	void situationTable(int lastProcNum,string target,string flag);//,string smallflag);//情况表元素
	void forState(int lastProcNum);//for循环语句
	void readState(int lastProcNum);//读语句
	void writeState(int lastProcNum);//写语句
	string getNewTemp(int lastProcNum,bool flag=true);//获取新的临时变量,flag用于记录isnotadr是否是地址
	string getNowTemp();//得到当前临时变量
	string getNewFlag();//获取新的跳转点
	string getNowFlag();//得到当前跳转点
	string getNewString();//获取新的字符串标记
	string getNowString();//得到当前字符串标记
	//string numtoStr(int num);
};
