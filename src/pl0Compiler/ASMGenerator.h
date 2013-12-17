#pragma once
#include <string>
using namespace std;

class ASMGenerator//����������
{
public:
	void setObjectfileName(char s[]);
	void procStart(int index);
	void procEnd(int index);
	void WriteString(string s);
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
	static string prefix;
	static int codePoint1;
	static int codePoint2;
	static int judgeFlagNum;
	static int judgeArrayFlagNum;
	void init();//���캯������ʼ��
	void generate();//���������ɣ�����init();data();code();
	void data();//��������������
	void code();//���м���Ԫʽ���������ɴ�����
};