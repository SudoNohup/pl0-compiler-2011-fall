#include<stdio.h>
#include<string.h>
#include "Lexer.h"
#include "GlobalData.h"
#include <iostream>

char Lexer::ch;
int Lexer::NowNo,Lexer::tot;
int Lexer::NoOfLine;
char Lexer::line[1000];
deque<LexerItem> Lexer::SymbolList;

using namespace std;

//���캯��
Lexer::Lexer()
{
	NowNo=0;
	tot=0;
	NoOfLine=0;
	ch=' ';
}

//�ж��Ƿ��ǿո��

int Lexer::isBlank(char c)
{
	if(c==' '||c=='\t'||c=='\r'||c=='\n') return 1;
	return 0;
}

//�ж��Ƿ�����ĸ
int Lexer::isLetter(char c)
{
	if(c>='a'&&c<='z') return 1;
	if(c>='A'&&c<='Z') return 1;
	return 0;
}

//�ж��Ƿ�����
int Lexer::isNumber(char c)
{
	if(c>='0'&&c<='9') return 1;
	return 0;
}

//�ж��Ƿ��Ǳ�����
int Lexer::isKeyWord(string s)
{
	int i;
	for(i=1;i<=KEYWORD_RESERVE_NUM;i++)
		if(s==KEYWORD[i])
			return i;
	return IDENTIFIER_NUM;		//��������ɷ񣿣�������return "��ʶ��"
}

int Lexer::getch()
{
	if(NowNo==tot)
	{
		if(gets(line+1)!=NULL)//ע��gets��fgets������fgets���ܴ���"\n"//��line[1]��ʼ����
		{
			NoOfLine++;
			NowNo=0;
			tot=strlen(line+1);
			line[++tot]=' ';//����ط��Ҵ������//�Կո�ָ�к���һ�У���ֹ����temp\nend������������������tempend
			line[tot+1]='\0';//Ϊʲô������������
			return 0;
		}
		else//����endoffile����һ���޷��š�
		{
			ch=' ';
			cout<<"program incomplete"<<endl;
			//exit(0);
			return -1;
		}
	}
	NowNo++;//line+1������ͷ��㴦����
	ch=line[NowNo];
	return 0;
}

//��ȡ��һ����ʶ��/������/�ֽ��
LexerItem Lexer::getSym(int mode)
{
	LexerItem re;		//����ֵ
	int flag;
	string temp="";
	flag=0;
	if((!SymbolList.empty())&&(mode==0))
	{
		re=SymbolList[0];
		SymbolList.pop_front();
		return re;
	}

	while(isBlank(ch)&&((flag=getch()))==0);//�����м�Ŀո񣨵��ʵķֽ����

	if(flag==-1)
	{
		re.kind = -1;
		return re;
	}//�˾䶨�彫�ںδ���������
	if(isLetter(ch))//����ĸ��ͷ,���Ǳ�ʶ�����߱�����
	{
		do{
			temp+=ch;
		}while(getch()==0&&(isLetter(ch)||isNumber(ch)));
		re.kind=isKeyWord(temp);//����Ǳ�ʶ���򷵻�21:identifier,���򷵻�1-20�е�һ��������ʾ�������͡�
		re.symbol=temp;
		return re;
	}
	else if(isNumber(ch))//�����ֿ�ͷ����Ϊ����
	{
		do{
			temp+=ch;
		}while(getch()==0&&isNumber(ch));
		re.kind=22;//��������ִ�����22:number
		re.symbol=temp;
		return re;
	}
	else if(ch=='\'')//�Ե����ſ�ͷ����Ϊ�ַ�
	{
		getch();
		if(isLetter(ch)||isNumber(ch))
		{
			temp="\'";
			temp+=ch;
			temp+="\'";
		}
		else
		{
			//error ���зǷ��ַ�
			myErrorHandler.lexerError(NoOfLine,NowNo,"�ַ��к��зǷ��ַ�");
		}
		getch();
		if(ch=='\'')
		{
			re.kind=23;//������ַ�����23:character
			re.symbol=temp;
			ch=' ';//���Ը�Ϊgetch������������Ϊ�ո�ʹ�ʷ������´ν���ֱ�ӽ�����˿ո��һ��ѭ��
			return re;
		}
		else
		{
			//errorȱ��ƥ���'
			myErrorHandler.lexerError(NoOfLine,NowNo,"�ַ����ȱ��ƥ��\'");
			re.kind=23;//������ַ�����23:number
			re.symbol=temp;
			//û��ch=' ';??????��Ϊ���������ch�������´ν���ʷ�����ʱ������
			return re;
		}
	}
	else if(ch=='\"')//�ַ���������Щ���⣬��ú�\'�ַ�һ�����������������ܼ��Ϻ���\"��ȷʵ�������⣬ԭ��""�����Ӧ������
	{

		//����1��ֻ���пո����֡�����
		//do{
		//	temp+=ch;
		//}while(getch()==0&&(isLetter(ch)||isNumber(ch)||isBlank(ch)));//���е����⣬isBlankҪ��ֻ���ǿո�' ';

		//����2��ֱ����\"ֹͣ
		int flag=0;
		do{
			temp+=ch;
			if(flag==1&&(!( isLetter(ch)||isNumber(ch)||isBlank(ch))) )
			{
				myErrorHandler.lexerError(NoOfLine,NowNo,"�ַ����ﺬ�з����֡�����ĸ���ǿո���ַ�");
			}
			flag=1;
		}while(getch()==0&&ch!='\"');


		//��������
		if(ch=='\"')
		{
			temp+=ch;
			re.kind=24;//������ַ�������24:string
			re.symbol=temp;
			ch=' ';
			return re;
		}
		else//����ֱ����\"ֹͣ�ķ�����������ȥ����������getch()!=0�ж����¡�
		{
			//errorȱ��ƥ���"
			myErrorHandler.lexerError(NoOfLine,NowNo,"�ַ������ȱ��ƥ��\"");
			temp+="\"";//�����м���������ַ��Ļ��Ͳ������������ˡ���������������
			re.kind=24;
			re.symbol=temp;
			return re;		
		}
	}
	else if(ch=='(')//��С����
	{
		re.kind=25;
		re.symbol=ch;
		ch=' ';//Ҳ���Ը�Ϊgetch()???????
		return re;
	}
	else if(ch==')')//��С����
	{
		re.kind=26;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch=='[')//�������ţ�����ֽ��
	{
		re.kind=27;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch==']')//��������
	{
		re.kind=28;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch==',')//����
	{
		re.kind=29;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch==';')//�ֺ�
	{
		re.kind=30;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch=='+')//�Ӻ�
	{
		re.kind=31;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch=='-')//�Ӻ�
	{
		re.kind=32;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch=='*')//�˺�
	{
		re.kind=33;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch=='/')//����
	{
		re.kind=34;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch=='<')
	{
		getch();
		if(ch=='=')//С�ڵ��ں�
		{
			re.kind=36;
			re.symbol="<=";
			ch=' ';
			return re;
		}
		else if(ch=='>')//���Ⱥ�
		{
			re.kind=40;
			re.symbol="<>";
			ch=' ';
			return re;
		}
		else //С�ں�
		{
			re.kind=35;
			re.symbol="<";
			return re;
		}
	}
	else if(ch=='>')//���ں�
	{
		getch();
		if(ch!='=')
		{
			re.kind=37;
			re.symbol=">";
			return re;
		}
		else//���ڵ��ں�
		{
			re.kind=38;
			re.symbol=">=";
			ch=' ';
			return re;
		}
	}
	else if(ch=='=')
	{
		re.kind=39;
		re.symbol=ch;
		ch=' ';
		return re;

	}
	else if(ch==':')
	{
		getch();
		if(ch=='=')
		{
			re.kind=41;
			re.symbol=":=";
			ch=' ';
			return re;
		}
		else
		{
			re.kind=42;
			re.symbol=':';
			//û��ch=' ';
			return re;
		}
	}
	else if(ch=='.')
	{
		re.kind=43;
		re.symbol=".";
		ch=' ';
		return re;
	}
	else
	{
		//error����ʶ����ַ�
		myErrorHandler.lexerError(NoOfLine,NowNo,"����ʶ����ַ�");
		ch=' ';
		return getSym();
	}
}

//Ԥ���ַ���ѹ��symbolist��deque��
LexerItem Lexer::getNextSym(int n)
{
	int i;
	for(i=SymbolList.size();i<n;i++)
		SymbolList.push_back(getSym());
	return SymbolList[n-1];
}

int Lexer::x()
{
	return NoOfLine;
}

int Lexer::y()
{
	return NowNo;
}

//���Գ���
void Lexer::testLexer()
{
	LexerItem temp;
	Lexer myLexer;
	//freopen("E:\\ProgramData\\Compiler\\test_1.txt","w",stdout);
	do{
		temp=myLexer.getSym();
		cout<<temp.kind<<'\t'<<temp.symbol<<endl;
	}while(temp.kind!=-1);//�������ļ�ĩβʱ���������ɲ������Ƕ������ʱ�����������м���־�ţ���ͻȻendoffile
}




