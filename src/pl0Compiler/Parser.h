#pragma once

#include "SymbolTable.h"
#include "LexerItem.h"

using namespace std;

class Parser//�﷨�������������
{
public:
	static LexerItem sym;//�ʷ������õ��ĵ�����Ϣ
	static int nowProcNum;	//���̱��
	static kindType t;	//�﷨�Ƶ� ���ͱ���t
	static string idName;//��ʶ��
	static int tempNum;//��ʱ����No
	static int tempAdrNum;//��ʱ��ַ����No���ñ����д�ŵ�������Ԫ�ػ��ߴ���ַʱ�ĵ�ַ��
	static int flagNum;//��ת���No
	static int stringNum;//�ַ�����No
	static int level;//��level��
	static bool functionFlag;

	void initParser();//����ڹ��캯�������ڳ�ʼ��
	int isbaseType(string s);//�Ƿ��ǻ�������
	int isCompare(string s);//�Ƿ��ǹ�ϵ�����
	int isAdd(string s);//�Ƿ��ǼӼ������
	int isMul(string s);//�Ƿ��ǳ˳������
	void program();//����
	void block();//�ֳ���
	void constDes();//����˵������
	void constDefine();//��������
	//void constant();//<����>
	string constValue();//<����>
	void varDes();//����˵������
	void varDefine();//����˵��
	void function(int lastProcNum);//����˵������
	void functionStart(int lastProcNum);//�����ײ�
	void procedure(int lastProcNum);//����˵������
	void procedureStart(int lastProcNum);//�����ײ�
	void argTable();//��ʽ������
	void arg(bool tempflag);//��ʽ������
	void complexSentense(int lastProcNum);//�������
	void statement(int lastProcNum);//���
	void becomesState(int lastProcNum);//��ֵ���
	string expression(int lastProcNum);//���ʽ
	string term(int lastProcNum);//��
	string factor(int lastProcNum);//����
	string callFunction(int lastProcNum);//�����������
	string callProcedure(int lastProcNum);//���̵������
	int valueTable(int lastProcNum,int ProcIndex,string procName);//ʵ�ڲ�����
	void ifState(int lastProcNum);//�������
	string condition(int lastProcNum);//����
	void caseState(int lastProcNum);//������
	void situationTable(int lastProcNum,string target,string flag);//,string smallflag);//�����Ԫ��
	void forState(int lastProcNum);//forѭ�����
	void readState(int lastProcNum);//�����
	void writeState(int lastProcNum);//д���
	string getNewTemp(int lastProcNum,bool flag=true);//��ȡ�µ���ʱ����,flag���ڼ�¼isnotadr�Ƿ��ǵ�ַ
	string getNowTemp();//�õ���ǰ��ʱ����
	string getNewFlag();//��ȡ�µ���ת��
	string getNowFlag();//�õ���ǰ��ת��
	string getNewString();//��ȡ�µ��ַ������
	string getNowString();//�õ���ǰ�ַ������
	//string numtoStr(int num);
};
