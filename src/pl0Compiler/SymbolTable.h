#pragma once

#include <string>
#include <deque>

using namespace std;

typedef enum {CONSTINT,CONSTCHAR,INT,CHAR,ARRAYINT,ARRAYCHAR,PROCINT,PROCCHAR,PROCNONE,ERROR} kindType;

class SymbolTableItem//��¼�������ÿ����ʶ������Ϣ
{
public:
	string name;//����
	//objType obj;
	kindType kind;//���ͣ�����Ϊenum{CONSTINT,CONSTCHAR,INT,CHAR,ARRAYINT,ARRAYCHAR,PROCINT,PROCCHAR,PROCNONE,ERROR}���ͣ�
	//int arrayref;//����ʱָ���ӦArrayTable�е���ţ�����ָ��0;
	int arraydim;//������ʱ��ʾ����ά��;���򲻶���		//��������ù��캯����ʼ��Ϊ1;
	bool isnotadr;//true��ʾ���ǵ�ַ��false��ʾ�ǵ�ַ
	//bool isinreg;
	//bool isinmem;
	int regnum;//�ڴ�������ʱ������ڼĴ������
	int size;//��ʾ��ռ�Ĵ�С��һ�����Ϊ1������Ϊ��ӦԪ�ظ�����//��������������ȥ����
	int adr;//�������βΣ��������ʼ��λ����			//���ں�������̣���д���Procnum
	int value;//���ڳ����洢��ֵ
	int id;//ʹ���Ϊһά����Ӧ���б���id
	bool isnotconst;//true��ʾ���ǳ�����false��ʾ�ǳ�����

};


class SymTableContext//��¼ÿ������/���������Ϣ�����е�itemList����SymbolTableItem��
{
private:

public:
	deque<SymbolTableItem>itemList;//��¼
	int lev;//��ʾproc/func���ڲ���
	int outproc;//��¼���ĺ������
	string procName;//��¼������
	int procArgNum;//��¼�����Ĳ�������
	kindType procKind;//��¼����������
	int startProcId;//��¼һά��ʼ��id����������
	bool functionReturnFlag;//��¼�������޷���ֵ
	//void addItem(string name,kindType kind,int value);//
	//string getItemName(int index);
};




class SymbolTable//���ű����е�tableContext����SymTableContext��
{
public:
	int isExists(int procNum,string name);//�ж������ı�ʶ���ڷ��ű����Ƿ���������

	static deque<string> printString;//��¼�ַ���
	static deque<SymTableContext>tableContext;//��¼����/���̣�ÿ��Ԫ���������SymTableContext��
	//static deque<int>arrayList;
	//static int arrayNum;
	//void addArrayItem(string name,kindType kind,int value);
	//int getArrayItemDim(int index);
	//static int number;
	void initSymbolTable();//��ʼ�����ű�
	void addNewItem(int procNum,string name,kindType kind,int value);//�ڷ��ű�procNum�ĺ���/�����������µķ��ţ���ʶ����
	void addNewContext(string name,kindType kind,int nowProcNum);//�����µĺ���/���̣����½�SymTableContext��
	//int getProcIndex(string name);
	//int getProcArgIndex(int index,string id);
	int getCurProcIndex(int ProcNum,string name);//��ȡ��ǰ����/���̵���������λ�ã�
	//int getCurProcArgIndex(int index,string id);//��ȡ��ǰ�����Ĳ���
	kindType canWrite(int procNum,string name);//��ȡ��ʶ���Ƿ���Ը�ֵ�Լ����͵���Ϣ
	kindType canRead(int procNum,string name);//��ȡ��ʶ���Ƿ���Զ�ȡֵ�Լ����͵���Ϣ
	kindType isProcRead(int procNum,string name);//��ȡ����/�����Ƿ���Ե���
	kindType canUse(int procNum,string name);//��ȡ��ʶ���Ƿ����ʹ���Լ����͵���Ϣ
	string getProcArg(int procIndex,string name,int index);//��ȡ��������ʽ������
	int getProcIndexRead(int procNum,string name);//��ȡ�ɵ��ú�����λ��
	void addNewPrintString(string s);//���Ӵ�ӡ���ַ���
	void showTable(int procNum);//��ӡ���ű�
	void getIdNameAdr(int procNum,string name,int &BL,int &ON);//�ӵ�ǰ����/���̻�ȡ����ʹ�õķ��ţ����������㣩��λ����Ϣ(��ά)
	int dimCount(int procNum,int idIndex);//��ȡ���ű���ĳλ��֮ǰ��ʹ�ÿռ�(����м���[ebp+?]ʹ��)
};
