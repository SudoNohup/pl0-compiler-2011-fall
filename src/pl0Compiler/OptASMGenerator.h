#pragma once
#include <string>
#include <deque>
using namespace std;

class OptASMGenerator//汇编代码生成
{
public:
	void setObjectfileName(char s[]);
	void procStart(int index);
	void procEnd(int index);
	void WriteString(int procIndex,string s);
	void WriteExpression(int procIndex,string s);
	void Read(int procIndex,string s);
	void arrayEqual(int procIndex,string a,string b,string c);
	void equalArray(int procIndex,string a,string b,string c);
	void Becomes(int procIndex,string a,string b);
	void Add(int procIndex,string a,string b,string c);
	void Sub(int procIndex,string a,string b,string c);
	void Mul(int procIndex,string a ,string b,string c);
	void Div(int procIndex,string a,string b,string c);
	void Compare(int procIndex,string op,string a,string b,string c);
	void CompareFalse(int procIndex,string op,string a,string b,string c);
	void IfFalse(int procIndex,string a,string b);
	string opCMP(string op);
	string opCompareReverse(string op);
	void SetFlag(int procIndex,string a);
	void JmpTo(int procIndex,string a);
	void ValueEqual(int procIndex,string a,string b,string c);
	void Display(int procIndex,string a,string b);
	void Call(int procIndex,string a);
	void CallValue(int procIndex,string a,string b);
	int isCompare(string s);
	int isCompareFalse(string s);
	string getNewJudge();
	string getNewArrayJudge();
	void symbol(int procIndex,string s,string reg);//结果在reg寄存器里面
	void symbolAdr(int procIndex,string s,string reg);
	void symbolAdrEqual(int procIndex,string s,string tempreg,string reslutreg);
	void symbolAdrByte(int procIndex,string s,string reg);//读字符专用
	string GetMemSymbol(int procIndex,string s);
	int GetSymbol(int procIndex,string s,string &temps);
	string GetSymbolAdr(int procIndex,string s);
	bool isInReg(string s);
	int GotWholeReg(int procIndex,string s);
	int OccupyReg(int procIndex,string s);
	void movToMem(int procIndex,int i);
	void logOutReg(int procIndex);
	string AssignReg(int procIndex,string a,string b,string c);
	void setGlobalOK(int ok);
	static int GlobalOK;//全局寄存器优化开关
	static string prefix;
	static int codePoint1;
	static int codePoint2;
	static int judgeFlagNum;
	static int judgeArrayFlagNum;
	//static int addr;
	static int RegNum;
	static deque <string> Register;//局部寄存器
	static deque <string> registerToMem;//局部寄存器到局部变量的映射
	static deque <string> GlobalRegister;//全局寄存器
	static deque <int> varFlag;
	void init();//构造函数，初始化
	void generate();//整体汇编生成，调用init();data();code();
	void data();//数据区代码生成
	void code();//对中间四元式解析，生成代码区
};
