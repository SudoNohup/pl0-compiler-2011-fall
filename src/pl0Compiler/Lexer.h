#pragma once

#include <deque>
#include "LexerItem.h"


using namespace std;

class Lexer//�ʷ�����
{
public:
	Lexer();//���캯������ʼ���ʷ�����Lexer��
	LexerItem getSym(int mode=0);//����һ������
	LexerItem getNextSym(int i);//����������i����
	void testLexer();					//����
	int x();							//�����к�
	int y();							//�����к�
	static char ch;			//chΪ��ǰ�ַ�
	static int NowNo,tot;		//NowNoΪ�ַ���ţ�totΪ�������ַ���
	static int NoOfLine;	//����
	static char line[1000];	//�����ַ���
	static deque<LexerItem>SymbolList;//��˫�˶��д�ȡ�����ĵ��ʣ����ڴӺ���ǰ��ӵ���(push_back)���Լ���ǰ��ȡ����(pop_front)
	int getch();		//0�������أ�-1�쳣
	int isBlank(char c);//�ж��Ƿ��ǿո�
	int isLetter(char c);//�ж��Ƿ����ַ�
	int isNumber(char c);//�ж��Ƿ�������
	int isKeyWord(string s);//�ж��Ƿ��Ǳ����֣����ر����ֵ���Ż���ֱ���Ǳ�־�������
};