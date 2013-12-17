#include "ErrorHandler.h"
#include "GlobalData.h"
#include <iostream>

using namespace std;

int ErrorHandler::errorTot=0;
//int Error

//ErrorHandler::ErrorHandler()
//{
//	errorTot=0;
//}


int ErrorHandler::isType(string s)
{
	if(s=="int"||s=="char")
		return 1;
	return 0;
}

int ErrorHandler::isAllStartMark(string s)
{
	if(s=="const"||s=="var"||s=="function"||s=="procedure"||s=="begin"||s==".")
		return 1;
	return 0;
}


int ErrorHandler::isStartMark1(string s)
{
	if(s=="var"||s=="function"||s=="procedure"||s=="begin"||s==".")
		return 1;
	return 0;
}

int ErrorHandler::isStartMark2(string s)
{
	if(s=="function"||s=="procedure"||s=="begin"||s==".")
		return 1;
	return 0;
}

int ErrorHandler::isbaseType(string s)
{
	if(s=="integer"||s=="char")
		return 1;
	else return 0;
}

void ErrorHandler::lexerError(int x,int y, string message)
{
	cout<<"(行号，列号):";
	if(message=="字符中含有非法字符")
	{
		cout<<"("<<x<<","<<y<<"):词法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="字符最后缺少匹配\'")
	{
		cout<<"("<<x<<","<<y<<"):词法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="字符串最后缺少匹配\"")
	{
		cout<<"("<<x<<","<<y<<"):词法分析发现错误："<<message<<endl;

		//下面两句在直到有/"才停止的方案中才存在，否则删除。
		cout<<"compile exit!"<<endl;
		exit(0);

		return;
	}

	else if(message=="字符串里含有非数字、非字母、非空格的字符")
	{
		cout<<"("<<x<<","<<y<<"):词法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="不能识别的字符")
	{
		cout<<"("<<x<<","<<y<<"):词法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="字符中含有非法字符")
	{
		cout<<"("<<x<<","<<y<<"):词法分析发现错误："<<message<<endl;
		return;
	}
	else
	{
		cout<<"wrong error!"<<endl;
	}
}



void ErrorHandler::parserError(int x,int y,string message)
{
	cout<<"(行号，列号):";
	//if(message=="函数没有返回值")
	//{
	//	cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<",默认返回值为0"<<endl;//默认返回值为0
	//	return;
	//}
	//else 
	if(message=="分程序中没有复合语句")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		if(myParser.sym.symbol==";")
		{
			cout<<"\t出现多次分号;"<<endl;
			myParser.sym=myLexer.getSym();
		}

		while(!isStartMark2(myParser.sym.symbol)&&myParser.sym.kind!=-1)
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}
	else if(message=="常量说明部分开始没有const")//一般不会用到
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="常量说明部分最后缺少;")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;

		while(myParser.sym.kind!=21&&!isStartMark1(myParser.sym.symbol)&&myParser.sym.kind!=-1)
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		//可能需要跳读？？？
		return;
	}
	else if(message=="常量定义开始不是标识符")//开始时const重复定义的错误可以通过这一句同样过滤掉
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		//if(myParser.sym.symbol=="const")
		//{
		//	cout<<"\tconst重复出现"<<endl;
		//	myParser.sym=myLexer.getSym();
		//	return;
		//}
		while(myParser.sym.symbol!=";"&&myParser.sym.symbol!=","&&myParser.sym.kind!=-1&&!isStartMark1(myParser.sym.symbol))
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}
	else if(message=="常量定义没有中间的=")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;

		if(myParser.sym.symbol==":=")
		{
			cout<<"\t不应是:=,而应该是="<<endl;
			myParser.sym=myLexer.getSym();
			return;
		}
		while(myParser.sym.symbol!=";"&&myParser.sym.symbol!=","&&myParser.sym.kind!=-1&&!isStartMark1(myParser.sym.symbol))
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		//是否要跳读什么的
		//直到下一个标识符？？或者
		return;
	}
	else if(message=="常量出现不符合定义的符号")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		while(myParser.sym.symbol!=";"&&myParser.sym.symbol!=","&&!isStartMark1(myParser.sym.symbol)&&myParser.sym.kind!=-1)
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}
	else if(message=="变量说明部分开始没有var")//一般不会用到
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="变量说明部分缺少;")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;


		while(myParser.sym.kind!=21&&!isStartMark2(myParser.sym.symbol)&&myParser.sym.kind!=-1)
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}


	else if(message=="变量说明开始不是标识符")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		//if(myParser.sym.symbol=="var")
		//{
		//	cout<<"\tvar重复出现"<<endl;
		//	myParser.sym=myLexer.getSym();
		//	return;
		//}
		while(myParser.sym.symbol!=";"&&myParser.sym.symbol!=","&&myParser.sym.kind!=-1&&!isStartMark2(myParser.sym.symbol))
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}


	else if(message=="变量说明缺少:")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		
		while(myParser.sym.kind!=-1&&!isStartMark2(myParser.sym.symbol)&&!isbaseType(myParser.sym.symbol))
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}


	else if(message=="变量说明没有类型")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="变量说明定义数组缺少[")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="变量说明定义数组缺少]")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="变量说明定义数组缺少of")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="变量说明数组定义没有基本类型")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="函数说明部分缺少;")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		while(!isStartMark2(myParser.sym.symbol)&&myParser.sym.kind!=-1)
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}
	else if(message=="函数首部缺少function")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="函数首部开始缺少标识符")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="函数首部缺少(")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="函数首部缺少)")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="函数首部缺少:")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		while(myParser.sym.symbol!=";"&&myParser.sym.kind!=-1&&!isStartMark2(myParser.sym.symbol)&&!isbaseType(myParser.sym.symbol))
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}
	else if(message=="函数首部没有设置基本类型")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}

	else if(message=="函数首部缺少;")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="过程说明部分缺少;")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="过程首部缺少function")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="过程首部开始缺少标识符")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="过程首部缺少(")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="过程首部缺少)")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="过程首部缺少;")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}


	else if(message=="形式参数段缺少标志符")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="形式参数段开始不是标识符")
	{
		//如果不是是/,或者。。。sym=nextsym
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="形式参数段缺少:")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		while(myParser.sym.symbol!=";"&&myParser.sym.kind!=-1&&!isStartMark2(myParser.sym.symbol)&&!isbaseType(myParser.sym.symbol))
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}
	else if(message=="形式参数段缺少基本类型定义")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="复合语句开始没有begin")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="复合语句结尾没有end或者复合语句中间没有分号")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="语句以标识符开头，却不是赋值语句与过程调用语句")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="赋值语句开始不是标识符")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="数组变量后面缺少]")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="赋值语句或for循环语句缺少:=")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		if(myParser.sym.symbol=="=")
		{
			cout<<"\t\t\t:=错写成=,汇编中已做更正"<<endl;
			myParser.sym=myLexer.getSym();
			return;
		}
		return;
	}
	else if(message=="赋值语句缺少:=或者数组没有[]标号")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;

		return;
	}

	else if(message=="因子(<表达式>)缺少)")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="不能识别的因子")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="因子中不应有字符")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}

	else if(message=="函数调用缺少标识符")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="函数调用缺少（")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="过程调用缺少标识符")
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="过程调用缺少（")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="过程调用缺少)")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="条件语句条件多了(")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="条件语句条件多了）")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="条件语句缺少then")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="条件中没有关系运算符")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="情况语句开始缺少case")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="情况语句开始缺少of")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="情况语句缺少end")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="情况子语句缺少:")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		while(myParser.sym.symbol!=";"&&myParser.sym.kind!=-1&&!isStartMark2(myParser.sym.symbol)&&!isbaseType(myParser.sym.symbol))
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}
	else if(message=="for循环语句开始缺少for")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="for循环语句开始缺少标识符")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="for循环语句缺少to/downto")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="for循环语句缺少do")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="读语句开始缺少read")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="读语句缺少(")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}

	else if(message=="未声明或是常量或是数组但不能赋值")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="读语句缺少标识符")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}

	else if(message=="读语句缺少)")
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="写语句开始缺少write")
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="写语句缺少(")
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="写语句缺少)")
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="程序中没有结尾.")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else 
	{
		cout<<"wrong error"<<endl;
	}

}


/*
void ErrorHandler::sentenceError(int x,int y, string message)
{
	if(message=="赋值语句中标识符后面有]但不是数组元素")
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="赋值语句中等式左边变量未定义|不是变量|是过程变量|是数组元素但后面没有[")
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="因子使用没有定义、不能读取的标识符")
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="函数使用错误")
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="因子使用没有定义的数组")
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="函数名不正确|应该是函数，而这里却是过程")
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="函数名/过程不正确")//前面还有fun1这样的信息，所以整个函数换掉
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="参数数目不正确")
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="函数调用缺少)")
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="过程名不正确|应该是过程，而这里却是函数")
	{
		cout<<"("<<x<<","<<y<<"):语义分析发现错误："<<message<<endl;
		return;
	}
	else if(message=="for循环语句中标识符未定义或者不是变量或者是数组元素")
	{
		cout<<"("<<x<<","<<y<<"):语法分析发现错误："<<message<<endl;
		return;
	}
	else
	{
	}
}*/


void ErrorHandler::declareError(int x,int y,string message)
{
	cout<<"(行号，列号):";
	cout<<"("<<x<<","<<y<<"):声明发现错误:"<<message<<endl;
}


void ErrorHandler::sentenceError(int x,int y ,string message)
{
	cout<<"(行号，列号):";
	cout << "(" << x << "," << y << "):语义发现错误:" << message << endl;
}