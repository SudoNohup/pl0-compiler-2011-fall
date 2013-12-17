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

//构造函数
Lexer::Lexer()
{
	NowNo=0;
	tot=0;
	NoOfLine=0;
	ch=' ';
}

//判断是否是空格符

int Lexer::isBlank(char c)
{
	if(c==' '||c=='\t'||c=='\r'||c=='\n') return 1;
	return 0;
}

//判断是否是字母
int Lexer::isLetter(char c)
{
	if(c>='a'&&c<='z') return 1;
	if(c>='A'&&c<='Z') return 1;
	return 0;
}

//判断是否数字
int Lexer::isNumber(char c)
{
	if(c>='0'&&c<='9') return 1;
	return 0;
}

//判断是否是保留字
int Lexer::isKeyWord(string s)
{
	int i;
	for(i=1;i<=KEYWORD_RESERVE_NUM;i++)
		if(s==KEYWORD[i])
			return i;
	return IDENTIFIER_NUM;		//这样定义可否？？？？，return "标识符"
}

int Lexer::getch()
{
	if(NowNo==tot)
	{
		if(gets(line+1)!=NULL)//注意gets和fgets的区别，fgets可能带入"\n"//从line[1]开始计数
		{
			NoOfLine++;
			NowNo=0;
			tot=strlen(line+1);
			line[++tot]=' ';//这个地方我处理错了//以空格分割本行和下一行，防止出现temp\nend这样的情况，整体读出tempend
			line[tot+1]='\0';//为什么这样处理？？？
			return 0;
		}
		else//到达endoffile，下一行无符号。
		{
			ch=' ';
			cout<<"program incomplete"<<endl;
			//exit(0);
			return -1;
		}
	}
	NowNo++;//line+1，这里就方便处理了
	ch=line[NowNo];
	return 0;
}

//获取下一个标识符/保留字/分界符
LexerItem Lexer::getSym(int mode)
{
	LexerItem re;		//返回值
	int flag;
	string temp="";
	flag=0;
	if((!SymbolList.empty())&&(mode==0))
	{
		re=SymbolList[0];
		SymbolList.pop_front();
		return re;
	}

	while(isBlank(ch)&&((flag=getch()))==0);//消除中间的空格（单词的分界符）

	if(flag==-1)
	{
		re.kind = -1;
		return re;
	}//此句定义将在何处处理？？？
	if(isLetter(ch))//以字母开头,则是标识符或者保留字
	{
		do{
			temp+=ch;
		}while(getch()==0&&(isLetter(ch)||isNumber(ch)));
		re.kind=isKeyWord(temp);//如果是标识符则返回21:identifier,否则返回1-20中的一个整数表示单词类型。
		re.symbol=temp;
		return re;
	}
	else if(isNumber(ch))//以数字开头，则为数字
	{
		do{
			temp+=ch;
		}while(getch()==0&&isNumber(ch));
		re.kind=22;//如果是数字串，则22:number
		re.symbol=temp;
		return re;
	}
	else if(ch=='\'')//以单引号开头，则为字符
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
			//error 含有非法字符
			myErrorHandler.lexerError(NoOfLine,NowNo,"字符中含有非法字符");
		}
		getch();
		if(ch=='\'')
		{
			re.kind=23;//如果是字符，则23:character
			re.symbol=temp;
			ch=' ';//可以改为getch（）？？？置为空格，使词法分析下次进入直接进入过滤空格的一句循环
			return re;
		}
		else
		{
			//error缺少匹配的'
			myErrorHandler.lexerError(NoOfLine,NowNo,"字符最后缺少匹配\'");
			re.kind=23;//如果是字符，则23:number
			re.symbol=temp;
			//没有ch=' ';??????因为这里读到的ch可能在下次进入词法分析时有意义
			return re;
		}
	}
	else if(ch=='\"')//字符串处理有些问题，最好和\'字符一样处理，这样出错仍能加上后面\"，确实是有问题，原来""必须对应！！！
	{

		//方案1：只能有空格、数字、整数
		//do{
		//	temp+=ch;
		//}while(getch()==0&&(isLetter(ch)||isNumber(ch)||isBlank(ch)));//还有点问题，isBlank要求只能是空格' ';

		//方案2：直到有\"停止
		int flag=0;
		do{
			temp+=ch;
			if(flag==1&&(!( isLetter(ch)||isNumber(ch)||isBlank(ch))) )
			{
				myErrorHandler.lexerError(NoOfLine,NowNo,"字符串里含有非数字、非字母、非空格的字符");
			}
			flag=1;
		}while(getch()==0&&ch!='\"');


		//公共部分
		if(ch=='\"')
		{
			temp+=ch;
			re.kind=24;//如果是字符串，则24:string
			re.symbol=temp;
			ch=' ';
			return re;
		}
		else//按照直到有\"停止的方案，不可以去掉，可能是getch()!=0中断所致。
		{
			//error缺少匹配的"
			myErrorHandler.lexerError(NoOfLine,NowNo,"字符串最后缺少匹配\"");
			temp+="\"";//“”中间的是所有字符的话就不加这条限制了。！！！！！！！
			re.kind=24;
			re.symbol=temp;
			return re;		
		}
	}
	else if(ch=='(')//左小括号
	{
		re.kind=25;
		re.symbol=ch;
		ch=' ';//也可以改为getch()???????
		return re;
	}
	else if(ch==')')//右小括号
	{
		re.kind=26;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch=='[')//左中括号，数组分界符
	{
		re.kind=27;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch==']')//右中括号
	{
		re.kind=28;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch==',')//逗号
	{
		re.kind=29;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch==';')//分号
	{
		re.kind=30;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch=='+')//加号
	{
		re.kind=31;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch=='-')//加号
	{
		re.kind=32;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch=='*')//乘号
	{
		re.kind=33;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch=='/')//除号
	{
		re.kind=34;
		re.symbol=ch;
		ch=' ';
		return re;
	}
	else if(ch=='<')
	{
		getch();
		if(ch=='=')//小于等于号
		{
			re.kind=36;
			re.symbol="<=";
			ch=' ';
			return re;
		}
		else if(ch=='>')//不等号
		{
			re.kind=40;
			re.symbol="<>";
			ch=' ';
			return re;
		}
		else //小于号
		{
			re.kind=35;
			re.symbol="<";
			return re;
		}
	}
	else if(ch=='>')//大于号
	{
		getch();
		if(ch!='=')
		{
			re.kind=37;
			re.symbol=">";
			return re;
		}
		else//大于等于号
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
			//没有ch=' ';
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
		//error不能识别的字符
		myErrorHandler.lexerError(NoOfLine,NowNo,"不能识别的字符");
		ch=' ';
		return getSym();
	}
}

//预读字符，压入symbolist的deque中
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

//测试程序
void Lexer::testLexer()
{
	LexerItem temp;
	Lexer myLexer;
	//freopen("E:\\ProgramData\\Compiler\\test_1.txt","w",stdout);
	do{
		temp=myLexer.getSym();
		cout<<temp.kind<<'\t'<<temp.symbol<<endl;
	}while(temp.kind!=-1);//当读到文件末尾时结束？？可不可以是读到句号时结束？？？中间出现句号？？突然endoffile
}




