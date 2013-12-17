#pragma once

#include <string>
#include <deque>

using namespace std;

typedef enum {CONSTINT,CONSTCHAR,INT,CHAR,ARRAYINT,ARRAYCHAR,PROCINT,PROCCHAR,PROCNONE,ERROR} kindType;

class SymbolTableItem//记录声明后的每个标识符的信息
{
public:
	string name;//名字
	//objType obj;
	kindType kind;//类型（定义为enum{CONSTINT,CONSTCHAR,INT,CHAR,ARRAYINT,ARRAYCHAR,PROCINT,PROCCHAR,PROCNONE,ERROR}类型）
	//int arrayref;//数组时指向对应ArrayTable中的序号，否则指向0;
	int arraydim;//有数组时表示数组维数;否则不定义		//最后试试用构造函数初始化为1;
	bool isnotadr;//true表示不是地址，false表示是地址
	//bool isinreg;
	//bool isinmem;
	int regnum;//在代码生成时标记所在寄存器编号
	int size;//表示所占的大小，一般变量为1，数组为相应元素个数。//这样冗余甚至能去掉。
	int adr;//变量（形参）：相对起始的位移量			//对于函数或过程，填写入口Procnum
	int value;//对于常量存储其值
	int id;//使其变为一维表，相应所有变量id
	bool isnotconst;//true表示不是常数，false表示是常数。

};


class SymTableContext//记录每个函数/过程体的信息，其中的itemList包含SymbolTableItem类
{
private:

public:
	deque<SymbolTableItem>itemList;//记录
	int lev;//表示proc/func所在层数
	int outproc;//记录外层的函数编号
	string procName;//记录函数名
	int procArgNum;//记录函数的参数个数
	kindType procKind;//记录函数的种类
	int startProcId;//记录一维表开始的id，用作索引
	bool functionReturnFlag;//记录函数有无返回值
	//void addItem(string name,kindType kind,int value);//
	//string getItemName(int index);
};




class SymbolTable//符号表，其中的tableContext包含SymTableContext类
{
public:
	int isExists(int procNum,string name);//判断声明的标识符在符号表中是否曾经出现

	static deque<string> printString;//记录字符串
	static deque<SymTableContext>tableContext;//记录函数/过程，每个元素是上面的SymTableContext类
	//static deque<int>arrayList;
	//static int arrayNum;
	//void addArrayItem(string name,kindType kind,int value);
	//int getArrayItemDim(int index);
	//static int number;
	void initSymbolTable();//初始化符号表
	void addNewItem(int procNum,string name,kindType kind,int value);//在符号表procNum的函数/过程中增加新的符号（标识符）
	void SymbolTable::addNewContext(string name,kindType kind,int nowProcNum);//增加新的函数/过程，即新建SymTableContext类
	//int SymbolTable::getProcIndex(string name);
	//int SymbolTable::getProcArgIndex(int index,string id);
	int SymbolTable::getCurProcIndex(int ProcNum,string name);//获取当前函数/过程的索引（即位置）
	//int SymbolTable::getCurProcArgIndex(int index,string id);//获取当前函数的参数
	kindType SymbolTable::canWrite(int procNum,string name);//获取标识符是否可以赋值以及类型的信息
	kindType SymbolTable::canRead(int procNum,string name);//获取标识符是否可以读取值以及类型的信息
	kindType SymbolTable::isProcRead(int procNum,string name);//获取函数/过程是否可以调用
	kindType SymbolTable::canUse(int procNum,string name);//获取标识符是否可以使用以及类型的信息
	string SymbolTable::getProcArg(int procIndex,string name,int index);//获取函数的形式参数名
	int SymbolTable::getProcIndexRead(int procNum,string name);//获取可调用函数的位置
	void SymbolTable::addNewPrintString(string s);//增加打印的字符串
	void SymbolTable::showTable(int procNum);//打印符号表
	void SymbolTable::getIdNameAdr(int procNum,string name,int &BL,int &ON);//从当前函数/过程获取可以使用的符号（本层或者外层）的位置信息(二维)
	int SymbolTable::dimCount(int procNum,int idIndex);//获取符号表中某位置之前已使用空间(汇编中计算[ebp+?]使用)
};