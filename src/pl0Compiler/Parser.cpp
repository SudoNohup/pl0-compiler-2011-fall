#include "Parser.h"
#include "GlobalData.h"
#include<stdlib.h>
#include<strstream>

#include "iostream"

using namespace std;

int Parser::nowProcNum;//现在所在的函数编号，开始为0
//要加一个层数的变量；
string Parser::idName;
int Parser::tempNum=0;
int Parser::tempAdrNum=0;
int Parser::flagNum=0;
int Parser::stringNum=0;
int Parser::level;//层数
bool Parser::functionFlag;//函数是否有返回值的标志位
LexerItem Parser::sym;
kindType Parser::t;	//语法制导 类型变量t

//构造函数
void Parser::initParser()
{
	sym=myLexer.getSym();
	nowProcNum=0;
	level=0;//最外面的一层是第1层、后面有level++
}

//是否是基本类型
int Parser::isbaseType(string s)
{
	if(s=="integer"||s=="char")
		return 1;
	else return 0;
}

/*
int Parser::isType(string s)
{
	if(isbaseType(s)||)
}*/

//是否是关系运算符
int Parser::isCompare(string s)
{
	if(s=="<"||s=="<="||s==">"||s==">="||s=="="||s=="<>")
		return 1;
	else return 0;
}

//是否是加减运算符
int Parser::isAdd(string s)
{
	if(s=="+"||s=="-")
		return 1;
	else return 0;
}

//是否是乘除运算符
int Parser::isMul(string s)
{
	if(s=="*"||s=="/")
		return 1;
	else return 0;
}


//程序
void Parser::program()
{
	block();
	if(sym.symbol!=".") 
	{
		//error没有句号结尾
		//cout<<"程序未结束\n";
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"程序中没有结尾.");
	}

	printf("程序语法分析结束\n");
}



//以下注意生成ProcBegin，ProcEnd的中间代码
//分程序
void Parser::block()//初始时lev=0？？
{
	//string lastFunc;
	int lastProcNum;
	lastProcNum=nowProcNum;
	level++;

	if(sym.symbol=="const")
	{
		//sym=myLexer.getSym();
		constDes();
	}
	
	if(sym.symbol=="var")//用预读字符的方法，这里有错		??????可能当时想的是类型回填
	{
		//sym=myLexer.getSym();
		varDes();
	}

	while(sym.symbol =="function"||sym.symbol =="procedure")
	{
		if(sym.symbol == "function")
		{
			//sym=myLexer.getSym();
			//语义动作
			function(lastProcNum);
			printf("声明了一个函数\n");
		}
		else if(sym.symbol == "procedure")
		{

			//以下部分也可以在进入procedure部分后预先执行
		//	idName=myLexer.getNextSym(1).symbol;//没有测试过，不知有没有bug？？？？？？？？？？
			//mySymbolTable.addNewItem(nowProcNum,idName,PROCNONE,0);
			//mySymbolTable.addNewContext(idName,PROCNONE,nowProcNum);
			
			//lastFunc=idName;
			//nowProcNum++;
			
			//sym=myLexer.getSym();不能加
			//语义动作
			procedure(lastProcNum);

			printf("声明了一个过程\n");

		}
	}

		//printf(end functions/procedure);


	if(sym.symbol=="begin")
	{

		mySymbolTable.tableContext[lastProcNum].functionReturnFlag=false;//记录执行中是否有返回值


		//myMiddleCode.addMiddleCode("Begin",mySymbolTable.tableContext[lastProcNum].procName,"","");

		//string(itoa(lastProcNum));

		strstream ss;
		string tempNumString;
		ss<<lastProcNum;
		ss>>tempNumString;

		myMiddleCode.addMiddleCode("Begin",tempNumString,"","");
		//sym=myLexer.getSym();
		complexSentense(lastProcNum);
		//myMiddleCode.addMiddleCode("End",mySymbolTable.tableContext[lastProcNum].procName,"","");

		myMiddleCode.addMiddleCode("End",tempNumString,"","");
		//sym=myLexer.getSym();


		if(mySymbolTable.tableContext[lastProcNum].functionReturnFlag==false&&(mySymbolTable.tableContext[lastProcNum].procKind==PROCINT||mySymbolTable.tableContext[lastProcNum].procKind==PROCCHAR))
		{
			//error
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"函数没有返回值");

			//error 函数无返回值
			//std::cout<<mySymbolTable.tableContext[lastProcNum].procKind<<endl;
			//printf("no return\n");
		}
	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"分程序中没有复合语句");
	}

	level--;

}

//常量说明部分
void Parser::constDes()
{
	if(sym.symbol!="const")//此句其实可以删掉
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"常量说明部分开始没有const");
		return;
	}
	sym=myLexer.getSym();
	constDefine();
	while(sym.symbol==",")//先有，后有标识符，注意与var先有标识符，后有；处理方式的异同
	{
		sym=myLexer.getSym();
		constDefine();
	}
	if(sym.symbol!=";")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"常量说明部分最后缺少;");
	}
	else
	{
		sym=myLexer.getSym();
	}
}


//常量定义
void Parser::constDefine()
{
	string constval;
	if(sym.kind!=21)
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"常量定义开始不是标识符");
		return;
	}
	else
	{
		idName=sym.symbol;
		sym=myLexer.getSym();
	}
	if(sym.symbol!="=")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"常量定义没有中间的=");
		return;
	}
	else
	{
		sym=myLexer.getSym();
	}
	constval=constValue();
	//if(const)

	//if(constval[0]=='\'')//这里是否会有问题？？？？？？？？？？？怕数字和字符都是\'的ASCII码值			t是全局变量，这样就可以在词法分析得到的sym中直接判断。//这里没有类型不匹配的问题
	//	t=CONSTCHAR;
	//else// if(sym.symbol=="char")
	//	t=CONSTINT;
	//idName=sym.symbol;
	if(t==CONSTCHAR)
		mySymbolTable.addNewItem(nowProcNum,idName,t,constval[1]);
	else
		mySymbolTable.addNewItem(nowProcNum,idName,t,atoi(constval.c_str()));//也可以在constValue中没有多读一个字符，但这样怕常量在case中用到时不一致。
	int tempSize=mySymbolTable.tableContext[nowProcNum].itemList.size();
	mySymbolTable.tableContext[nowProcNum].itemList[tempSize-1].isnotconst=false;
	//myMiddleCode.addMiddleCode("ConstDefine",idName,"","");
	//sym=myLexer.getSym();


	//constant();
}

//<常量>
string Parser::constValue()
{
	string temp;
	strstream ss;
	if (sym.kind==23)
	{
		t=CONSTCHAR;
		//ss << sym.symbol;
		//ss >> temp;
		temp=sym.symbol;
		sym=myLexer.getSym();
		return temp;
	}
	else
	{
		string op;
		if(isAdd(sym.symbol))
		{
			op=sym.symbol;
			sym=myLexer.getSym();
		}
		else op="";
		if(sym.kind==22)//要分类吗？？？？？？？？？？？？？？？？？
		{
			t=CONSTINT;
			//sym.symbol=op+sym.symbol;
			//ss<<sym.symbol;
			//ss>>temp;
			temp=op+sym.symbol;
			sym=myLexer.getSym();
			return temp;
		}
		else
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"常量出现不符合定义的符号");
			return "";
			//return what??wrong
		}
	}
}


/*
//常量
void Parser::constant()
{
	SymbolTableItem newItem;
	if(sym.kind==23)
	{
		newItem.kind=CONSTCHAR;
		newItem.value=0;
		
		//语义处理，符号表中加上这一项种类是constchar;						，对应值为，
	}
	else
	{
		string op;
		if(isAdd(sym.symbol))
		{
			op=sym.symbol;
		}
		else op="";

		sym=myLexer.getSym();
		if(sym.kind==22)
		{
			sym.symbol=op+sym.symbol;
		}
		else
		{
			//error
		}
		//语义处理,数字
		sym=myLexer.getSym();
	}
}*/

//变量说明部分
void Parser::varDes()//类型不匹配的问题??????????????在赋值时考虑
{
	if(sym.symbol!="var")//可以删掉
	{
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"变量说明部分开始没有var");
		//error
	}
	sym=myLexer.getSym();
	varDefine();
	if(sym.symbol!=";")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"变量说明部分缺少;");
	}
	else
	{
		sym=myLexer.getSym();
	}

	while(sym.kind==21)
	{
		varDefine();
		if(sym.symbol!=";")
		{
			//error return
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"变量说明部分缺少;");
		}
		else
		{
			sym=myLexer.getSym();
		}
	}
}

//变量说明
void Parser::varDefine()
{
	int start,end;//用作回填类型时的计数
	int tempdim;//用来记录数组变量的维数
	start=mySymbolTable.tableContext[nowProcNum].itemList.size();
	if(sym.kind==21)
	{
		idName=sym.symbol;
		mySymbolTable.addNewItem(nowProcNum,idName,ERROR,0);
		//myMiddleCode.addMiddleCode("VarDefine",idName,"","");
		sym=myLexer.getSym();
		//填入符号表
	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"变量说明开始不是标识符");
		return;
	}
	//sym=myLexer.getSym();
	while(sym.symbol==",")//先有，后有标识符，注意与var先有标识符，后有；处理方式的异同
	{
		sym=myLexer.getSym();
		//constDefine();
		if(sym.kind==21)
		{
			idName=sym.symbol;
			mySymbolTable.addNewItem(nowProcNum,idName,ERROR,0);
			//myMiddleCode.addMiddleCode("VarDefine",idName,"","");
			sym=myLexer.getSym();
			//填入符号表
		}
		else
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"变量说明开始不是标识符");
			return;
		}
	}
	if(sym.symbol!=":")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"变量说明缺少:");

	}
	else
	{
		sym=myLexer.getSym();
	}
	if(isbaseType(sym.symbol))
	{
		if(sym.symbol=="integer")
			t=INT;
		else// if(sym.symbol=="char")
			t=CHAR;
		end=mySymbolTable.tableContext[nowProcNum].itemList.size()-1;
		for(int i=start;i<=end;i++)
		{
			mySymbolTable.tableContext[nowProcNum].itemList[i].kind=t;
		}
		//回填类型
		sym=myLexer.getSym();
	}
	else if(sym.symbol=="array")
	{
		sym=myLexer.getSym();
		if(sym.symbol!="[")
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"变量说明定义数组缺少[");
		}
		sym=myLexer.getSym();
		if(sym.kind==22)
		{
			//mySymbolTable.arrayList.push_back(atoi(sym.symbol.c_str()));
			tempdim=atoi(sym.symbol.c_str());
			//定义数组
			sym=myLexer.getSym();
		}
		else
		{
			//未定义维数
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"变量说明定义数组没有说明维数");
		}
		if(sym.symbol!="]")
		{
			//error//没有]
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"变量说明定义数组缺少]");
		}
		else
		{
			sym=myLexer.getSym();
		}
		if(sym.symbol!="of")
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"变量说明定义数组缺少of");
		}
		else
		{
			sym=myLexer.getSym();
		}
		if(isbaseType(sym.symbol))
		{
			//语义处理，回填类型

			if(sym.symbol=="integer")
				t=ARRAYINT;
			else// if(sym.symbol=="char")
				t=ARRAYCHAR;
			end=mySymbolTable.tableContext[nowProcNum].itemList.size()-1;
			for(int i=start;i<=end;i++)
			{
				mySymbolTable.tableContext[nowProcNum].itemList[i].kind=t;
				//mySymbolTable.tableContext[nowProcNum].itemList[i].arrayref=mySymbolTable.arrayList[mySymbolTable.arrayList.size()-1];
				mySymbolTable.tableContext[nowProcNum].itemList[i].arraydim=tempdim;
			}
			//回填类型
			sym=myLexer.getSym();//是否放在这里？？
		}
		else
		{
			//error没有类型
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"变量说明数组定义没有基本类型");
		}


	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"变量说明没有类型");

	}
}

//函数说明部分
void Parser::function(int lastProcNum)
{
	//functionFlag=false;
	functionStart(lastProcNum);
	//mySymbolTable.tableContext[lastProcNum].functionReturnFlag=false;
	block();
	//if(mySymbolTable.tableContext[lastProcNum].functionReturnFlag==false)
	//{
	//	//error 函数无返回值
	//	std::cout<<mySymbolTable.tableContext[lastProcNum].procKind<<endl;
	//	printf("no return\n");
	//}
	//sym=myLexer.getSym();
	if(sym.symbol!=";")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"函数说明部分缺少;");
	}
	else
	{
		sym=myLexer.getSym();
	}
	/*
	while(sym.symbol=="function")
	{
		//functionStart(lastProcNum);
		function(lastProcNum);
		//sym=myLexer.getSym();
		if(sym.symbol!=";")
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"函数说明部分缺少;");
		}
		//else情况下不能再读下一个，可能仅仅是漏写,处理方式，凡是有error的地方马上要修改！！！！！！！！！！！！！！！！
		else
		{
			sym=myLexer.getSym();
		}
	}*/
}


//函数首部
void Parser::functionStart(int lastProcNum)
{
	if(sym.symbol!="function")//此句其实可以删掉
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"函数首部缺少function");
	}
	sym=myLexer.getSym();
	if(sym.kind==21)
	{
		//以下部分也可以在进入procedure部分前执行
		idName=sym.symbol;
		mySymbolTable.addNewItem(lastProcNum,idName,ERROR,0);//error表示未确定类型。也可以先设定默认类型int？？？？错误处理时改？？？
		mySymbolTable.addNewContext(idName,ERROR,lastProcNum);
		//lastFunc=idName;
		nowProcNum++;
		int tempSize=mySymbolTable.tableContext[lastProcNum].itemList.size();
		mySymbolTable.tableContext[lastProcNum].itemList[tempSize-1].adr=nowProcNum;//增加入口地址？？？
		//填入符号表,ProcNone
		sym=myLexer.getSym();
		//填入符号表
	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"函数首部开始缺少标识符");
	}
	if(sym.symbol!="(")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"函数首部缺少(");
	}
	else
	{
		sym=myLexer.getSym();
	}
	if(sym.symbol=="var"||sym.kind==21)//可有可无的形式参数表
	{
		argTable();//形式参数表
	}
	//sym=myLexer.getSym();
	if(sym.symbol!=")")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"函数首部缺少)");
	}
	else
	{
		sym=myLexer.getSym();//是否要放在else里面写？？if(sym.symbol==")"则跳读下一个	最后再改！！！！！！！！！！！！！！！！！！！！！！！
	}
	if(sym.symbol!=":")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"函数首部缺少:");
		//return;
	}
	else
	{
		sym=myLexer.getSym();//是否要放在else里面写？？if(sym.symbol==")"则跳读下一个	最后再改！！！！！！！！！！！！！！！！！！！！！！！
	}
	if(isbaseType(sym.symbol))//到上一层中改类型啊啊啊！！！！
	{
		if(sym.symbol=="integer")
			t=PROCINT;
		else// if(sym.symbol=="char")
			t=PROCCHAR;
		mySymbolTable.tableContext[nowProcNum].procKind=t;

		//这里用外层的最后一个，还是设定一个函数进行搜索？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？*****************有问题啊》》》》
		//两种都要试一下!!!!!!!!!!!!!!


		//SymTableContext tempContext;

		//int tempProc=mySymbolTable.tableContext[nowProcNum].outproc;
		//int changeProcNum=mySymbolTable.getCurProcIndex(tempProc,mySymbolTable[nowProcNum].ProcName);
		//mySymbolTable.tableContext[tempProc].itemList[changeProcNum].kind=t;


		int tempProc=mySymbolTable.tableContext[nowProcNum].outproc;
		int tempSize=mySymbolTable.tableContext[tempProc].itemList.size();
		mySymbolTable.tableContext[tempProc].itemList[tempSize-1].kind=t;
		
		//回填符号
		sym=myLexer.getSym();
	}
	else
	{
		//error并设置默认类型
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"函数首部没有设置基本类型");
	}
	if(sym.symbol!=";")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"函数首部缺少;");
	}
	else
	{
		sym=myLexer.getSym();
	}
	
}


//过程说明部分
void Parser::procedure(int lastProcNum)
{
	procedureStart(lastProcNum);
	block();
	//sym=myLexer.getSym();
	if(sym.symbol!=";")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"过程说明部分缺少;");
	}
	else
	{
		sym=myLexer.getSym();
	}
	/*while(sym.symbol=="procedure")
	{
		//procedureStart(lastProcNum);
		procedure(lastProcNum);
		//sym=myLexer.getSym();
		if(sym.symbol!=";")
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"过程说明部分缺少;");
		}
		//else情况下不能再读下一个，可能仅仅是漏写,处理方式，凡是有error的地方马上要修改！！！！！！！！！！！！！！！！
		else
		{
			sym=myLexer.getSym();
		}
	}*/
}


//过程首部
void Parser::procedureStart(int lastProcNum)
{
	if(sym.symbol!="procedure")//此句其实可以删掉
	{
		//error函数首部缺少function
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"过程首部缺少function");
	}
	sym=myLexer.getSym();
	if(sym.kind==21)
	{
		//以下部分也可以在进入procedure部分前执行
		idName=sym.symbol;
		mySymbolTable.addNewItem(lastProcNum,idName,PROCNONE,0);

		mySymbolTable.addNewContext(idName,PROCNONE,lastProcNum);
		//lastFunc=idName;
		nowProcNum++;
		int tempSize=mySymbolTable.tableContext[lastProcNum].itemList.size();
		mySymbolTable.tableContext[lastProcNum].itemList[tempSize-1].adr=nowProcNum;//增加入口地址？？？
		//填入符号表,ProcNone
		sym=myLexer.getSym();
	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"过程首部开始缺少标识符");

	}
	if(sym.symbol!="(")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"过程首部缺少(");
	}
	else
	{
		sym=myLexer.getSym();
	}
	if(sym.symbol=="var"||sym.kind==21)//可有可无的形式参数表
	{
		argTable();//形式参数表
	}
	//sym=myLexer.getSym();
	if(sym.symbol!=")")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"过程首部缺少)");
	}
	else
	{
		sym=myLexer.getSym();//是否要放在else里面写？？if(sym.symbol==")"则跳读下一个	最后再改！！！！！！！！！！！！！！！！！！！！！！！
	}
	if(sym.symbol!=";")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"过程首部缺少;");

	}
	else
	{
		sym=myLexer.getSym();
	}
}

//形式参数表
void Parser::argTable()
{
	bool tempflag=true;
	arg(tempflag);
	//sym=myLexer.getSym();
	while(sym.symbol==";")
	{
		tempflag=true;
		sym=myLexer.getSym();
		arg(tempflag);
	}
}

//形式参数段
void Parser::arg(bool tempflag)
{
	int start,end;//用作回填类型时的计数
	start=mySymbolTable.tableContext[nowProcNum].itemList.size();
	if(sym.symbol=="var")//传地址，后面的几个参数相应的isnotadr属性为false，可以在前面设置一个bool值，默认为true，然后这里可以改变为false，不行啊，var开头影响到后面
	{
		tempflag=false;
		sym=myLexer.getSym();
	}
	if(sym.kind==21)
	{
		idName=sym.symbol;
		mySymbolTable.tableContext[nowProcNum].procArgNum++;//已经初始化为0
		mySymbolTable.addNewItem(nowProcNum,idName,ERROR,0);
		sym=myLexer.getSym();
		//语义处理，填符号表
	}
	else
	{
		//error 缺少标识符
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"形式参数段缺少标志符");
	}
	while(sym.symbol==",")
	{
		sym=myLexer.getSym();
		if(sym.kind==21)
		{
			idName=sym.symbol;
			mySymbolTable.tableContext[nowProcNum].procArgNum++;//已经初始化为0
			mySymbolTable.addNewItem(nowProcNum,idName,ERROR,0);
			sym=myLexer.getSym();
			//语义处理，填符号表
			//语义处理
		}
		else
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"形式参数段开始不是标识符");
		}
		//sym=myLexer.getSym();						这句隐患啊！！！！！！
	}
	if(sym.symbol!=":")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"形式参数段缺少:");
	}
	else
	{
		sym=myLexer.getSym();
	}
	if(isbaseType(sym.symbol))
	{
		//回填类型
		if(sym.symbol=="integer")
			t=INT;
		else// if(sym.symbol=="char")
			t=CHAR;
		end=mySymbolTable.tableContext[nowProcNum].itemList.size()-1;
		for(int i=start;i<=end;i++)
		{
			mySymbolTable.tableContext[nowProcNum].itemList[i].kind=t;
			mySymbolTable.tableContext[nowProcNum].itemList[i].isnotadr=tempflag;
		}
		sym=myLexer.getSym();
		//回填
	}
	else
	{
		//error，可以设置默认属性为integer
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"形式参数段缺少基本类型定义");
	}
	//sym=myLexer.getSym();
}

//复合语句
void Parser::complexSentense(int lastProcNum)
{
	if(sym.symbol!="begin")//此句其实可以删掉
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"复合语句开始没有begin");
	}
	sym=myLexer.getSym();
	{
		statement(lastProcNum);
	}
	while(sym.symbol==";")
	{
		sym=myLexer.getSym();
		statement(lastProcNum);
	}
	if(sym.symbol!="end")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"复合语句结尾没有end或者复合语句中间没有分号");
	}
	else
	{
		sym=myLexer.getSym();
	}
}


//语句
void Parser::statement(int lastProcNum)
{
	if(sym.kind==21)
	{

		//符号表判断，但是可能没有声明啊！！过程调用或者赋值语句、区别：1.符号表，2.后面有：=或者（）


		//查符号表时如果有重复定义？？外层与内层：一个是过程，一个是变量

		//倾向于预读处理！！！！

		//预读两个字符，看是:=还是(
		string tempsym;
		tempsym=myLexer.getNextSym(1).symbol;
		if(tempsym==":="||tempsym=="["||tempsym=="=")
			becomesState(lastProcNum);
		else if(tempsym=="(")
		//callProcedure();
			callProcedure(lastProcNum);//函数调用与过程调用合并
		else
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"语句以标识符开头，却不是赋值语句与过程调用语句");
		}

	}
	else if(sym.symbol=="if")
	{
		ifState(lastProcNum);
	}
	else if(sym.symbol=="case")
	{
		caseState(lastProcNum);
	}
	else if(sym.symbol=="begin")
	{
		complexSentense(lastProcNum);
	}
	else if(sym.symbol=="read")
	{
		readState(lastProcNum);
	}
	else if(sym.symbol=="write")
	{
		writeState(lastProcNum);
	}
	else if(sym.symbol=="for")
	{
		forState(lastProcNum);
	}
	else
	{
		//不处理，空语句
	}
}

//赋值语句
void Parser::becomesState(int lastProcNum)
{
	//bool leftArrayFlag=false;
	string newTemp;
	if(sym.kind!=21)//可以去掉
	{
		//error

	}
	//语义处理
	idName=sym.symbol;							//是否需要另外定义一个局部变量，因为idName会在expression中改变
	sym=myLexer.getSym();
	if(sym.symbol==":="||sym.symbol=="=")//是否要加入关于=的错误处理？？？
	{
		//普通变量或者函数符
		//检查
		kindType temptype=mySymbolTable.canWrite(lastProcNum,idName);
		if(temptype==ERROR||temptype==ARRAYCHAR||temptype==ARRAYCHAR)
		{
			//error
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"赋值语句中等式左边变量未定义|不是变量|是过程变量|是数组元素但后面没有[");
		}
		
	//	else
		{
			if(sym.symbol=="=")
			{
				myErrorHandler.parserError(myLexer.x(),myLexer.y(),"赋值语句或for循环语句缺少:=");//错误处理，试图将:=写成=的错误纠正
				myMiddleCode.addMiddleCode(":=",expression(lastProcNum),"",idName);
			}
			else
			{
				sym=myLexer.getSym();
				myMiddleCode.addMiddleCode(":=",expression(lastProcNum),"",idName);
			}
		}

		if(temptype==PROCINT||temptype==PROCCHAR)
		{
			mySymbolTable.tableContext[lastProcNum].functionReturnFlag=true;
		}

		//expression();
		//语义处理时千万注意，如果是函数类型的赋值，要作为返回值来处理！！！或者在生成代码中才这样写：中间代码一样，生成时加判断!!!!!!!!!!!!!!!生成时处理，便于优化。。。。。！这样对变量的可以读写就要多加判断
	}
	else if(sym.symbol=="[")
	{
		//数组变量
		//检查符号表
		kindType temptype=mySymbolTable.canWrite(lastProcNum,idName);
		if(!(temptype==ARRAYINT||temptype==ARRAYCHAR))
		{
			int i;
			i=1;
			//error
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"赋值语句中标识符后面有]但不是数组元素");
		}
		//[]=	B	k	t1
		//	=	2	t1

			//leftArrayFlag=true;
			sym=myLexer.getSym();
			newTemp=getNewTemp(lastProcNum,false);			//不知这里是否有效？？



			string tempExpression=expression(lastProcNum);

			if(tempExpression[0]>='0'&&tempExpression[0]<='9')
			{
				int idProcNum=-1,idIndex=-1;
				mySymbolTable.getIdNameAdr(lastProcNum,idName,idProcNum,idIndex);
				if(idProcNum==-1||idIndex==-1)
				{

				}
				else
				{
					int dim=mySymbolTable.tableContext[idProcNum].itemList[idIndex].arraydim;
					int tempdim=atoi( tempExpression.c_str() );
					if(tempdim<0||tempdim>=dim)
						myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"数组下标越界");
				}
			}

			myMiddleCode.addMiddleCode("[]=",idName,tempExpression,newTemp);				//从右向左调用参数！！！！！！！！！！！！！！！！！！
			//myMiddleCode.addMiddleCode("+",preTemp,nowTemp,getNewTemp());

		//在符号表中需标记新加的newTemp为isnotadr为flase;

		if(sym.symbol!="]")
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"数组变量后面缺少]");
		}
		else
		{
			sym=myLexer.getSym();
		}
		if(sym.symbol!=":=")
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"赋值语句缺少:=");
		}
		else
		{
			sym=myLexer.getSym();
			myMiddleCode.addMiddleCode(":=",expression(lastProcNum),"",newTemp);
			//expression();
			//if(leftArrayFlag==true)
			//{
			//myMiddleCode.addMiddleCode("=",expression(),newTemp,"");
			//}
		}

	}
	else
	{
		//myError.parserError(myLexer.x(),myLexer(),"赋值语句缺少:=或者数组没有[]标号");
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"赋值语句缺少:=或者数组没有[]标号");
	}
}

//表达式
string Parser::expression(int lastProcNum)
{
	int flag=0;
	string preTemp="";
	string nowTemp="";
	if(isAdd(sym.symbol))
	{
		flag=sym.symbol=="-"?1:0;
		sym=myLexer.getSym();
	}
	nowTemp=term(lastProcNum);
	if(flag)
	{
		preTemp=getNewTemp(lastProcNum);
		myMiddleCode.addMiddleCode("-","0",nowTemp,preTemp);
		//preTemp=getNowTemp();
	}
	//getnowTemp在哪里？？？ 只能是临时寄存的数啊！！
	else preTemp=nowTemp;
	while(isAdd(sym.symbol))
	{
		flag=sym.symbol=="-"?1:0;
		sym=myLexer.getSym();
		nowTemp=term(lastProcNum);

		//preTemp=geNew
		if(flag)
			myMiddleCode.addMiddleCode("-",preTemp,nowTemp,getNewTemp(lastProcNum));
		else
			myMiddleCode.addMiddleCode("+",preTemp,nowTemp,getNewTemp(lastProcNum));
		preTemp=getNowTemp();
	}
	return preTemp;
}


//项
string Parser::term(int lastProcNum)
{
	string preTemp;//改为全局变量可否？？？？？？？？？？？？？？？？？？？？？？？
	string nowTemp;
	string fh;
	preTemp=factor(lastProcNum);
	while(isMul(sym.symbol))
	{
		fh=sym.symbol;
		sym=myLexer.getSym();

		if(fh=="/"&&sym.symbol=="0")//词法分析已经分割//&&myLexer.getNextSym(1).kind!=22)
		{
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"除数不能是0");
		}

		nowTemp=factor(lastProcNum);
		myMiddleCode.addMiddleCode(fh,preTemp,nowTemp,getNewTemp(lastProcNum));
		preTemp=getNowTemp();
	}
	return preTemp;
}

//因子,这里处理感觉有些难度
string Parser::factor(int lastProcNum)
{
	string preTemp="";
	if(sym.kind==21)
	{
		string tempsym;
		kindType tempType=mySymbolTable.canRead(lastProcNum,sym.symbol);
		if(tempType==ERROR)//符号表应该对整个进行检查，这里有bug
		{
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"因子使用没有定义、不能读取的标识符");
		}
		//else
		//{
			//是否应该将下面的if elseif else加在这个里面？？？？？？？？？？？？？？？或者就是报一下错，但整个程序能继续执行！！！
		//}
		//目前倾向于就报一下错

		//	myError.sentenceError(myLexer.x(),myLexer.y(),"因子未定义而使用");
		tempsym=myLexer.getNextSym(1).symbol;//预读一个字符*****************************************
		if(tempsym=="(")//函数调用语句
		{
			if(mySymbolTable.isProcRead(lastProcNum,sym.symbol)==ERROR)
			{
				myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"函数使用错误");

			}
			//else
			{
				preTemp=callFunction(lastProcNum);
			}
		}
		else if(tempsym=="[")//数组
		{
			string tempidName;
			kindType tempType=mySymbolTable.canUse(lastProcNum,sym.symbol);
			if(tempType==ARRAYCHAR||tempType==ARRAYINT)
			{
				tempidName=sym.symbol;		
				sym=myLexer.getSym();//此时sym中是[
				sym=myLexer.getSym();//此时sym中才是表达式的第一项
				//t1=A[k]	=[]	B	k a
				//BB string newTemp=getNewTemp(lastProcNum);

				//BB int tempSize=mySymbolTable.tableContext[lastProcNum].itemList.size();
				//BB mySymbolTable.tableContext[lastProcNum].itemList[tempSize-1].isnotadr=false;
				//AA  myMiddleCode.addMiddleCode("=[]",tempidName,expression(lastProcNum),newTemp);//是否可以不用临时变量，直接用sym.symbol,不用newtemp，tempidName？？？？？？

				//BB myMiddleCode.addMiddleCode("[]=",tempidName,expression(lastProcNum),newTemp);//是否可以不用临时变量，直接用sym.symbol,不用newtemp，tempidName？？？？？？
				

				string newTemp=getNewTemp(lastProcNum,false);			//不知这里是否有效？？
				string tempExpression=expression(lastProcNum);

				if(tempExpression[0]>='0'&&tempExpression[0]<='9')
				{
					int idProcNum=-1,idIndex=-1;
					mySymbolTable.getIdNameAdr(lastProcNum,tempidName,idProcNum,idIndex);
					if(idProcNum==-1||idIndex==-1)
					{

					}
					else
					{
						int dim=mySymbolTable.tableContext[idProcNum].itemList[idIndex].arraydim;
						int tempdim=atoi( tempExpression.c_str() );
						if(tempdim<0||tempdim>=dim)
							myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"数组下标越界");
					}
				}

				myMiddleCode.addMiddleCode("[]=",tempidName,tempExpression,newTemp);				//从右向左调用参数！！！！！！！！！！！！！！！！！！

				//是否是在call函数时传参数时才取以上3行特殊处理？？？？？？？？？？？？？？？？？？？？
				
				preTemp=newTemp;

				if(sym.symbol!="]")
				{
					//error数组变量后面缺少]
					myErrorHandler.parserError(myLexer.x(),myLexer.y(),"数组变量后面缺少]");
				}
				else
				{
					sym=myLexer.getSym();
				}
			}
			else
			{
				//error没有数组的定义使用错误
				myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"因子使用没有定义的数组");
			}
		}
		else//最简单最赤裸裸的因子返回
		{
			//int,char,procint,procchar,constchar,constint都可以，上面已经排除得到这些情况
			preTemp=sym.symbol;
			sym=myLexer.getSym();
		}
	}
	else if(sym.symbol=="(")//(表达式)的情况
	{
		sym=myLexer.getSym();
		preTemp=expression(lastProcNum);
		if(sym.symbol==")")
		{
			sym=myLexer.getSym();
		}
		else
		{
			//error因子（<表达式>)缺少）
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"因子(<表达式>)缺少)");
		}
	}
	else if(sym.kind==22)//数字的处理方式不知是否正确
	{
		preTemp=sym.symbol;
		sym=myLexer.getSym();
	}



	else if(sym.kind==23)				//字符没有考虑？？？？？？？？？？？？？？？？？？？
	{
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"因子中不应有字符");
		preTemp=sym.symbol;
		sym=myLexer.getSym();
	}
	else
	{
		//error 不能识别的因子
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"不能识别的因子");
	}
	return preTemp;
}

//函数调用语句
string Parser::callFunction(int lastProcNum)
{
	string re="";//可以删，在需返回的地方直接返回即可。不能删
	string funcName;
	if(sym.kind==21)//不必要吧？？？？？？？？？？？？？？？？？，前面已经做出判断，在因子中
	{
		kindType tempType;
		tempType=mySymbolTable.isProcRead(lastProcNum,sym.symbol);
		if(tempType==ERROR||tempType==PROCNONE)
		{
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),sym.symbol+"函数名不正确|应该是函数，而这里却是过程");
		}
		//else
		{
			funcName=sym.symbol;
			sym=myLexer.getSym();
		}
	}
	else
	{
		//error	函数缺少标识符
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"函数调用缺少标识符");
	}
	if(sym.symbol=="(")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//error	函数调用缺少（
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"函数调用缺少（");//注意这里的错误处理，没有继续向下读字符！！！！！！！！！！！
		return "";
	}
				//注意：给的文法里面必须有实在参数？？？？？我错了!!可以为空的：注意：[]的作用是指可有可无。
	int tempIndex=mySymbolTable.getProcIndexRead(lastProcNum,funcName);
	string strlastProcNum,strtempIndex;
	strstream ss1,ss2;
	ss1<<lastProcNum;
	ss1>>strlastProcNum;
	ss2<<tempIndex;
	ss2>>strtempIndex;

	if(sym.symbol==")")
	{
		sym=myLexer.getSym();
		//if(mySymbolTable.isProc(funcName)==PROCNONE)
		//	myMiddleCode.addMiddleCode("Call",funcName,"","");
		//else
		//{

			myMiddleCode.addMiddleCode("Display",strlastProcNum,strtempIndex,"");
			//myMiddleCode.addMiddleCode("CallValue",funcName,"",getNewTemp(lastProcNum));
			myMiddleCode.addMiddleCode("CallValue",strtempIndex,"",getNewTemp(lastProcNum));
			re=getNowTemp();
		//}
	}
	else
	{
		//myMiddleCode.addMiddleCode("Display",strlastProcNum,strtempIndex,"");
		if(valueTable(lastProcNum,tempIndex,funcName)!=mySymbolTable.tableContext[tempIndex].procArgNum)//错误处理方式，可以只取前几个参数，删去后几个value=语句？？但少了咋办？？
		{
				myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"参数数目不正确");
		}
		
		//if(mySymbolTable.isProc(funcName)==PROCNONE)				//是否要分开写？？？？？？？？？？？？？？？？？？？？
			//myMiddleCode.addMiddleCode("Call",funcName,"","");
		//else
		//{
			//myMiddleCode.addMiddleCode("CallValue",funcName,"",getNewTemp(lastProcNum));
			myMiddleCode.addMiddleCode("CallValue",strtempIndex,"",getNewTemp(lastProcNum));
			re=getNowTemp();											//此时还不能返回，还有）要判断
		//}
		if(sym.symbol==")")
		{
			sym=myLexer.getSym();
		}
		else
		{
			//error	函数调用缺少)
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"函数调用缺少)");
			return "";
		}
	}
	return re;
}


//过程调用语句
string Parser::callProcedure(int lastProcNum)//该调用函数的地方调用过程，需要报错啊
{
	string re="";//可以删，在需返回的地方直接返回即可。
	string funcName;
	if(sym.kind==21)//不必要吧？？？？？？？？？？？？？？？？？，前面已经做出判断，在因子中
	{
		kindType tempType;
		tempType=mySymbolTable.isProcRead(lastProcNum,sym.symbol);
		if(!(tempType==PROCNONE))
		{
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),sym.symbol+"过程名不正确|应该是过程，而这里却是函数");			//其实可以在之前的调用时判断，可以用一个函数实现
		}
		else
		{
			funcName=sym.symbol;
			sym=myLexer.getSym();
		}

	}
	else
	{
		//error	函数缺少标识符
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"过程调用缺少标识符");
	}
	if(sym.symbol=="(")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//error	函数调用缺少（
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"过程调用缺少（");//注意这里的错误处理，没有继续向下读字符！！！！！！！！！！！
		return "";
	}
					//注意：给的文法里面必须有实在参数？？？？？
	int tempIndex=mySymbolTable.getProcIndexRead(lastProcNum,funcName);
	string strlastProcNum,strtempIndex;
	strstream ss1,ss2;
	ss1<<lastProcNum;
	ss1>>strlastProcNum;
	ss2<<tempIndex;
	ss2>>strtempIndex;

	if(sym.symbol==")")
	{
		sym=myLexer.getSym();
		//if(mySymbolTable.isProc(funcName)==PROCNONE)
		myMiddleCode.addMiddleCode("Display",strlastProcNum,strtempIndex,"");

			//myMiddleCode.addMiddleCode("Call",funcName,"","");
			myMiddleCode.addMiddleCode("Call",strtempIndex,"","");
		//else
		//{
		//	myMiddleCode.addMiddleCode("CallValue",funcName,"",getNewTemp());
		//	re=getNowTemp();
		//}
	}
	else
	{
		//myMiddleCode.addMiddleCode("Display",strlastProcNum,strtempIndex,"");

		if(valueTable(lastProcNum,tempIndex,funcName)!=mySymbolTable.tableContext[tempIndex].procArgNum)//错误处理方式，可以只取前几个参数，删去后几个value=语句？？但少了咋办？？
		{
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"参数数目不正确");
		}

		//if(mySymbolTable.isProc(funcName)==PROCNONE)				//是否要分开写？？？？？？？？？？？？？？？？？？？？
		//myMiddleCode.addMiddleCode("Call",funcName,"","");
		myMiddleCode.addMiddleCode("Call",strtempIndex,"","");
		//else
		//{
		//myMiddleCode.addMiddleCode("CallValue",funcName,"",getNewTemp());
		//re=getNowTemp();											//此时还不能返回，还有）要判断
		//}
		if(sym.symbol==")")
		{
			sym=myLexer.getSym();
		}
		else
		{
			//error	函数调用缺少)
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"过程调用缺少)");
			return "";
		}
	}
	return re;
}




//实在参数表
int Parser::valueTable(int lastProcNum,int ProcIndex,string procName)
{
	//int temp=0;

	string strProcIndex;
	strstream ss1;
	ss1<<ProcIndex;
	ss1>>strProcIndex;

	string strlastProcNum;
	strstream ss2;
	ss2<<lastProcNum;
	ss2>>strlastProcNum;

	string tempstring;
	int temp=-1;//这样不知是否正确？？？不同编译器，可能有问题？？？
	int flagflag;
	do{																//这是一种对A{，A}的文法的处理方式
		tempstring = expression(lastProcNum);
		if(temp==-1)
		{
			myMiddleCode.addMiddleCode("Display",strlastProcNum,strProcIndex,"");
		}
		myMiddleCode.addMiddleCode("value=",tempstring,mySymbolTable.getProcArg(ProcIndex,procName,++temp),strProcIndex);


		//后来加上，正确性待测试	优化后可能不会报错？？			//注意：数组已经用~代替
		if(temp<=mySymbolTable.tableContext[ProcIndex].procArgNum&&mySymbolTable.tableContext[ProcIndex].itemList[temp].isnotadr==false&&tempstring[0]=='#')
		{
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"传地址形参不能是复杂表达式");
		}


		

		if(sym.symbol==",")
		{
			sym=myLexer.getSym();
			flagflag=1;
		}
		else flagflag=0;
	}while(flagflag);
	return (temp+1);
}


/*
	>	k	1	#t1
	IfFalse	#t1	@flag_flag1
	doIfexpress
	JmpTo @flag_flag2
	SetFlag @flag_flag1
	doElseexpress
	SetFlag @flag_flag2
*/





//条件语句
void Parser::ifState(int lastProcNum)
{
	string preFlag1,preFlag2;
	if(sym.symbol!="if")//可以去掉
	{
		//error
	}
	sym=myLexer.getSym();


	if(sym.symbol=="(")
	{
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"条件语句条件多了(");//这一句不知能不能放在代码里面？？
		sym=myLexer.getSym();
	}
	//else
	//{
		//error	条件语句缺少(
		//myError.parserError(myLexer.x(),myLexer.y(),"条件语句缺少(");
	//}
	preFlag1=getNewFlag();
	myMiddleCode.addMiddleCode("IfFalse",condition(lastProcNum),preFlag1,"");
	//preFlag1=getNowFlag();
	//condition()??????????????
	//if(sym.symbol==")")
	//{
		//	myErrorHandler.parserError(myLexer.x(),myLexer.y(),"条件语句条件多了）");不敢放这一句？？？？
		//sym=myLexer.getSym();
	//}
	//else
	//{
		//error	条件语句缺少)
		//myError.parserError(myLexer.x(),myLexer.y(),"条件语句缺少)");
	//}
	if(sym.symbol=="then")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//error	if语句中缺少then
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"条件语句缺少then");
	}
	statement(lastProcNum);
	//这里可以做优化			读到else后面再决定加哪些代码
	//看最后是否可以优化掉？？可优化，则维持这样设计，否则初步生成的代码就要重新设计
	preFlag2=getNewFlag();
	myMiddleCode.addMiddleCode("JmpTo",preFlag2,"","");//优化时后面如果没有else语句这一句要删除！！！！！！！！！！！！和后面的紧靠在一起！！！！是一个优化，优化前后代码对比啊
	//preFlag2=getNowFlag();
	myMiddleCode.addMiddleCode("SetFlag",preFlag1,"","");
	if(sym.symbol=="else")
	{
		sym=myLexer.getSym();
		statement(lastProcNum);
	}//有无else皆可，不能报错
	myMiddleCode.addMiddleCode("SetFlag",preFlag2,"","");
}

//条件
string Parser::condition(int lastProcNum)
{
	string preTemp;
	string op;
	preTemp=expression(lastProcNum);
	if(isCompare(sym.symbol))				//这里是表达式后面可以有<关系运算符>，也可以没有？？？？？
	{
		string newTemp;
		op=sym.symbol;
		sym=myLexer.getSym();
		newTemp=getNewTemp(lastProcNum);			//不知这里是否有效？？
		myMiddleCode.addMiddleCode(op,preTemp,expression(lastProcNum),newTemp);
		preTemp=newTemp;
	}
	else//先不写？？？？？？？？？？？？？？？？？？？？
	{
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"条件中没有关系运算符");
		//error????????????????????????????????????????????????????????????/warning但不处理，允许？？？？这里实际的情况就是条件语句中只有表达式，没有关系运算符的情况。//需warning一下
	
	}
	return preTemp;							//不是1,0，非0的都当做1来处理，这里交给汇编处理
}



//情况语句
void Parser::caseState(int lastProcNum)
{
	if(sym.symbol!="case")//可以去掉
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"情况语句开始缺少case");
	}
	string target;
	string flag=getNewFlag();
	//string smallflag;
	sym=myLexer.getSym();
	target=expression(lastProcNum);
	if(sym.symbol!="of")
	{
		//error没有of
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"情况语句开始缺少of");
	}
	else
	{
		sym=myLexer.getSym();
	}
	int flagflag;
	do{
		//smallflag=getNewFlag();
		situationTable(lastProcNum,target,flag);//,smallflag);
		if(sym.symbol==";")
		{
			sym=myLexer.getSym();
			flagflag=1;
		}
		else flagflag=0;
	}
	while(flagflag);
	myMiddleCode.addMiddleCode("SetFlag",flag,"","");
	if(sym.symbol=="end")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//error 情况语句缺少end
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"情况语句缺少end");
		return;//这句是否可以删除？？？？？？？？？？？？？？？？？
	}
}


//情况表元素
void Parser::situationTable(int lastProcNum,string target,string flag)//,string smallflag)
{
	//太难写了！！！！！！！！！！！！！！！！！！！！语义啊
	string tempFlag;
	int flagflag;
	string smallflag;
	//smallflag=getNewFlag();
	tempFlag=getNewFlag();
	do{
		smallflag=getNewFlag();
		myMiddleCode.addMiddleCode("=",target,constValue(),getNewTemp(lastProcNum));
		myMiddleCode.addMiddleCode("IfFalse",getNowTemp(),smallflag,"");
		myMiddleCode.addMiddleCode("JmpTo",tempFlag,"","");
		if(sym.symbol==",")
		{
			myMiddleCode.addMiddleCode("SetFlag",smallflag,"","");
			sym=myLexer.getSym();
			flagflag=1;
		}
		else flagflag=0;
	}
	while(flagflag);
	if(sym.symbol==":")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//error 情况子语句缺少:
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"情况子语句缺少:");
	}
	myMiddleCode.addMiddleCode("SetFlag",tempFlag,"","");
	statement(lastProcNum);
	myMiddleCode.addMiddleCode("JmpTo",flag,"","");
	myMiddleCode.addMiddleCode("SetFlag",smallflag,"","");
}







/*
=    3    i 
SetFlag    @flag_flag1 
>=    i    1    #t0 
Ifflase    #t0    @flag_flag2 


JmpTo    @flag_flag1 
SetFlag    @flag_flag2
*/

//for循环语句
void Parser::forState(int lastProcNum)
{
	if(sym.symbol!="for")//可以去掉
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"for循环语句开始缺少for");
	}
	string preFlag1;
	string preFlag2;
	string tempStart;
	string tempEnd;
	string temp;
	string tempCmp;
	int opFlag=0;
	sym=myLexer.getSym();
	if(sym.kind==21)
	{
		kindType temptype=mySymbolTable.canWrite(lastProcNum,sym.symbol);
		if(temptype==ERROR||temptype==ARRAYCHAR||temptype==ARRAYCHAR)//for语句中标识符可否是函数标识符？？？？？？？？？？？？？
		{
			//error
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"for循环语句中标识符未定义或者不是变量或者是数组元素");
		}
		else
		{
			temp=sym.symbol;
			sym=myLexer.getSym();//是否要放在else外面？？？？？？？？？？？？？？？？
			//myMiddleCode.addMiddleCode(":=",expression(lastProcNum),"",idName);
		}
		//检查符号表
		//temp=sym.symbol;
		//myMiddleCode.addMiddleCode(":=",,"","");
		//sym=myLexer.getSym();
	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"for循环语句开始缺少标识符");
	}
	if(sym.symbol==":=")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//errror
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"赋值语句或for循环语句缺少:=");
	}
	tempStart=expression(lastProcNum);
	myMiddleCode.addMiddleCode(":=",tempStart,"",temp);
	if(sym.symbol=="to")//可否通过tempStart和tempEnd的比较来与这个to/downto一起指出错误？？？？？？？？？？？？？？？？？
	{
		opFlag=1;
		sym=myLexer.getSym();
	}
	else if (sym.symbol=="downto")
	{
		opFlag=-1;
		sym=myLexer.getSym();
	}
	else
	{
		//errror
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"for循环语句缺少to/downto");
	}
	tempEnd=expression(lastProcNum);
	preFlag1=getNewFlag();
	myMiddleCode.addMiddleCode("SetFlag",preFlag1,"","");
	tempCmp=getNewTemp(lastProcNum);
	if(opFlag==1)
	{
		myMiddleCode.addMiddleCode("<=",temp,tempEnd,tempCmp);
	}
	else//opFlag=-1?????????????????????????????????????????????????????
	{
		myMiddleCode.addMiddleCode(">=",temp,tempEnd,tempCmp);
	}
	preFlag2=getNewFlag();
	myMiddleCode.addMiddleCode("IfFalse",tempCmp,preFlag2,"");
	if(sym.symbol=="do")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//errror	for中缺少do
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"for循环语句缺少do");
	}
	statement(lastProcNum);
	if(opFlag==1)
	{
		myMiddleCode.addMiddleCode("+",temp,"1",temp);
	}
	else//opFlag=-1?????????????????????????????????????????????????????
	{
		myMiddleCode.addMiddleCode("-",temp,"1",temp);
	}
	myMiddleCode.addMiddleCode("JmpTo",preFlag1,"","");
	myMiddleCode.addMiddleCode("SetFlag",preFlag2,"","");

}



//读语句
void Parser::readState(int lastProcNum)
{
	if(sym.symbol!="read")//可以去掉
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"读语句开始缺少read");

	}
	int flagflag;
	sym=myLexer.getSym();
	if(sym.symbol=="(")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//error	读语句缺少(
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"读语句缺少(");
		return;//这样就结束了？？？？？？？？？？？？？？？？？？？？？？？？？
	}
	do 
	{
		if(sym.kind==21)
		{
			kindType temptype=mySymbolTable.canWrite(lastProcNum,sym.symbol);
			if(!(temptype==INT||temptype==CHAR))
			{
				//error
				myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"未声明或是常量或是数组但不能赋值");
			}
			//else?????????????????????????????????????
			myMiddleCode.addMiddleCode("Read",sym.symbol,"","");

			//if(mySymbolTable.canWrite(nowProcNum,sym.symbol)==ERROR)
			//	myError.sentenceError(myLexer.x(),myLexer.y(),"未声明或是常量");
			sym=myLexer.getSym();
		}
		else
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"读语句缺少标识符");
		}
		if(sym.symbol==",")
		{
			sym=myLexer.getSym();
			flagflag=1;
		}
		else
			flagflag=0;
	} while (flagflag);
	if(sym.symbol==")")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"读语句缺少)");
		return;
	}
	//注意：结尾没有/;
}




//写语句
void Parser::writeState(int lastProcNum)
{
	if(sym.symbol!="write")//可以去掉
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"写语句开始缺少write");
	}
	sym=myLexer.getSym();
	if(sym.symbol=="(")
	{
		sym=myLexer.getSym();
		
	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"写语句缺少(");
		return;//是不是别的地方错误也要这样处理？？？
	}
	if(sym.kind==24)
	{
		mySymbolTable.addNewPrintString(sym.symbol);
		myMiddleCode.addMiddleCode("WriteString",getNewString(),"","");
		sym=myLexer.getSym();
		if(sym.symbol==",")
		{
			sym=myLexer.getSym();
			myMiddleCode.addMiddleCode("WriteExpression",expression(lastProcNum),"","");
		}
	}
	else
	{
		myMiddleCode.addMiddleCode("WriteExpression",expression(lastProcNum),"","");
	}
	if(sym.symbol==")")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//error	写语句 缺少)
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"写语句缺少)");
		return;//是否需要？？？？
	}
}


/*
//获取存取[]=的结果（数组元素地址）的临时变量
string Parser::getNewArrayAdrTemp(int lastProcNum,bool flag)
{
	string temp;
	strstream ss;
	ss<<++tempNum;
	ss>>temp;
	if(flag==true)
		mySymbolTable.addNewItem(lastProcNum,"#t"+temp,INT,0);
	else
	{
		mySymbolTable.addNewItem(lastProcNum,"#t"+temp,INT,0);
		int tempSize=mySymbolTable.tableContext[lastProcNum].itemList.size();
		mySymbolTable.tableContext[lastProcNum].itemList[tempSize-1].isnotadr=false;
	}
	return "#t"+temp;
}*/


//获取新的临时变量
string Parser::getNewTemp(int lastProcNum,bool flag)
{
	string temp;
	if(flag==true)
	{
		strstream ss;
		ss<<++tempNum;
		ss>>temp;
		mySymbolTable.addNewItem(lastProcNum,"#t"+temp,INT,0);
		return "#t"+temp;
	}
		
	else
	{
		strstream ss;
		//ss<<++tempAdrNum;
		ss<<++tempNum;
		ss>>temp;
		mySymbolTable.addNewItem(lastProcNum,"~t"+temp,INT,0);
		int tempSize=mySymbolTable.tableContext[lastProcNum].itemList.size();
		mySymbolTable.tableContext[lastProcNum].itemList[tempSize-1].isnotadr=false;
		return "~t"+temp;
	}
}

//得到当前临时变量
string Parser::getNowTemp()
{
	string temp;//可不可以写成temp=string(tempNum);
	strstream ss;
	ss<<tempNum;
	ss>>temp;
	return "#t"+temp;
}
//获取新的跳转点
string Parser::getNewFlag()
{
	string temp;
	strstream ss;
	ss<< ++flagNum;
	ss>>temp;
	return "@flag_flag"+temp;
}
//得到当前跳转点
string Parser::getNowFlag()
{
	string temp;
	strstream ss;
	ss<<flagNum;
	ss>>temp;
	return "@flag_flag"+temp;
}
//获取新的字符串标记
string Parser::getNewString()
{
	string temp;
	strstream ss;
	ss<<++stringNum;
	ss>>temp;
	return "string"+temp;
}
//得到当前字符串标记
string Parser::getNowString()
{
	string temp;
	strstream ss;
	ss<<stringNum;
	ss>>temp;
	return "string"+temp;
}


string numtoStr(int num)
{
	string temp;
	strstream ss;
	ss<<num;
	ss>>temp;
	return temp;
}


