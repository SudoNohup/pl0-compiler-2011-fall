#pragma once
#include <string>
#include <deque>
using namespace std;

class OptASMGenerator//����������
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
	void symbol(int procIndex,string s,string reg);//�����reg�Ĵ�������
	void symbolAdr(int procIndex,string s,string reg);
	void symbolAdrEqual(int procIndex,string s,string tempreg,string reslutreg);
	void symbolAdrByte(int procIndex,string s,string reg);//���ַ�ר��
	string GetMemSymbol(int procIndex,string s);
	int GetSymbol(int procIndex,string s,string &temps);
	string OptASMGenerator::GetSymbolAdr(int procIndex,string s);
	bool OptASMGenerator::isInReg(string s);
	int OptASMGenerator::GotWholeReg(int procIndex,string s);
	int OptASMGenerator::OccupyReg(int procIndex,string s);
	void OptASMGenerator::movToMem(int procIndex,int i);
	void OptASMGenerator::logOutReg(int procIndex);
	string OptASMGenerator::AssignReg(int procIndex,string a,string b,string c);
	void OptASMGenerator::setGlobalOK(int ok);
	static int GlobalOK;//ȫ�ּĴ����Ż�����
	static string prefix;
	static int codePoint1;
	static int codePoint2;
	static int judgeFlagNum;
	static int judgeArrayFlagNum;
	//static int addr;
	static int RegNum;
	static deque <string> Register;//�ֲ��Ĵ���
	static deque <string> registerToMem;//�ֲ��Ĵ������ֲ�������ӳ��
	static deque <string> GlobalRegister;//ȫ�ּĴ���
	static deque <int> varFlag;
	void init();//���캯������ʼ��
	void generate();//���������ɣ�����init();data();code();
	void data();//��������������
	void code();//���м���Ԫʽ���������ɴ�����
};