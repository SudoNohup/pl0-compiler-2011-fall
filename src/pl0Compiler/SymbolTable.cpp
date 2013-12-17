#include "SymbolTable.h"
#include "GlobalData.h"
#include <iostream>

using namespace std;

deque<SymTableContext>SymbolTable::tableContext;
deque <string> SymbolTable::printString;
//deque <int>SymbolTable::arrayList;
//int SymbolTable::arrayNum;

//int SymbolTable::number;

//初始化符号表
void SymbolTable::initSymbolTable()
{
	SymTableContext temp;
	temp.procName="Main";
	temp.procArgNum=0;
	temp.procKind=PROCNONE;
	temp.startProcId=0;
	temp.lev=0;
	temp.outproc=-1;

	tableContext.push_back(temp);
	//arrayNum=0;
	//number=0;
	//procName.push
}

//向指定表增加项目
void SymbolTable::addNewItem(int procNum,string name,kindType kind,int value)
{
	SymbolTableItem newItem;
	if(!isExists(procNum,name))
	{
		//可以在引用这个函数时设定kind都为int？？？或者在错误处理时设定
		newItem.name=name;
		newItem.kind=kind;
		newItem.value=value;
		newItem.arraydim=1;//一般非数组元素设定为1维，方便以后计算占用空间
		newItem.isnotadr=true;//测试无误后在加这句话		....必须加这句话，否则其他非参数表中的值在预先安排中可能会随机给定
		newItem.isnotconst=true;//默认不是常量。
		newItem.regnum=-1;

		//增加id计数？？？？
		//if(procNum==0&&tableContext[procNum].itemList.empty())
		//	newItem.id=0;
		//else if(procNum!=0&&tableContext[procNum].itemList.empty())
		//{
		//	int tempSize=tableContext[procNum-1].itemList.size();
		//	newItem.id=tableContext[procNum-1].itemList[tempSize-1].id+1;
		//}
		//else
		//{
		//	int tempSize=tableContext[procNum].itemList.size();
		//	newItem.id=tableContext[procNum].itemList[tempSize-1].id+1;//(当上面的一个函数为空时，有错误！！设定一个former id作为函数的入口参数；也可以最后再一起计算)
		//}

		tableContext[procNum].itemList.push_back(newItem);
	}
	else
	{
		myErrorHandler.declareError(myLexer.x(),myLexer.y(),name+"重复定义");
	}

		//tableContext[procNum].addItem(string name,kindType kind,int value);
		//void SymTableContext::addItem(string name,kindType kind,int value);

}


//是否已经定义
int SymbolTable::isExists(int procNum,string name)
{
	for(int i=0;i<tableContext[procNum].itemList.size();i++)//只会查本层的，允许不同层变量（甚至函数名）相同			外层定义覆盖？？？
	{
		if(tableContext[procNum].itemList[i].name==name)
			return 1;
	}
	return 0;
}

void SymbolTable::addNewContext(string name,kindType kind,int nowProcNum)
{
	SymTableContext temptemp;
	temptemp.outproc=nowProcNum;
	//temp.lev=lev+1;
	temptemp.lev=tableContext[nowProcNum].lev+1;
	temptemp.procKind=kind;
	temptemp.procName=name;
	temptemp.procArgNum=0;//之后回填

	//int tempSize=tableContext[nowProcNum].itemList.size();
	//temptemp.startProcId=tableContext[nowProcNum].itemList[tempSize-1].id+1;


	tableContext.push_back(temptemp);
}


kindType SymbolTable::canWrite(int procNum,string name)
{
	int i,j;

	j=procNum;
	while(j!=0)
	{
		for(i=tableContext[j].itemList.size()-1;i>=0;i--)//符号表中有定义但不可写：如常数类型和procnone类型
		{
			if(tableContext[j].itemList[i].name==name)
				return (tableContext[j].itemList[i].kind>CONSTCHAR&&tableContext[j].itemList[i].kind<PROCNONE)?tableContext[j].itemList[i].kind:ERROR;
		}
		j=tableContext[j].outproc;
	}
	for(i=tableContext[j].itemList.size()-1;i>=0;i--)
	{
		if(tableContext[j].itemList[i].name==name)
			return (tableContext[j].itemList[i].kind>CONSTCHAR&&tableContext[j].itemList[i].kind<PROCNONE)?tableContext[j].itemList[i].kind:ERROR;
	}
	return ERROR;//在符号表中没有定义的情况

}



kindType SymbolTable::canRead(int procNum,string name)
{
	int i,j;
	j=procNum;
	while(j!=0)
	{
		for(i=tableContext[j].itemList.size()-1;i>=0;i--)//符号表中有定义但不可写：如常数类型和procnone类型
		{
			if(tableContext[j].itemList[i].name==name)
				return (tableContext[j].itemList[i].kind<PROCNONE)?tableContext[j].itemList[i].kind:ERROR;
		}
		j=tableContext[j].outproc;
	}
	for(i=tableContext[j].itemList.size()-1;i>=0;i--)
	{
		if(tableContext[j].itemList[i].name==name)
			return (tableContext[j].itemList[i].kind<PROCNONE)?tableContext[j].itemList[i].kind:ERROR;
	}
	return ERROR;//在符号表中没有定义的情况

}


kindType SymbolTable::isProcRead(int procNum,string name)
{
	int i,j;
	j=procNum;
	while(j!=0)
	{
		for(i=tableContext[j].itemList.size()-1;i>=0;i--)//符号表中有定义但不可写：如常数类型和procnone类型
		{
			if(tableContext[j].itemList[i].name==name)
				return (tableContext[j].itemList[i].kind>ARRAYCHAR&&tableContext[j].itemList[i].kind<ERROR)?tableContext[j].itemList[i].kind:ERROR;
		}
		j=tableContext[j].outproc;
	}
	for(i=tableContext[j].itemList.size()-1;i>=0;i--)
	{
		if(tableContext[j].itemList[i].name==name)
			return (tableContext[j].itemList[i].kind>ARRAYCHAR&&tableContext[j].itemList[i].kind<ERROR)?tableContext[j].itemList[i].kind:ERROR;
	}
	return ERROR;//在符号表中没有定义的情况
}


int SymbolTable::getProcIndexRead(int procNum,string name)
{
	int retnum=0;
	int i,j;
	j=procNum;
	while(j!=0)
	{
		for(i=tableContext[j].itemList.size()-1;i>=0;i--)//符号表中有定义但不可写：如常数类型和procnone类型
		{
			if(tableContext[j].itemList[i].name==name)
			{
				if(tableContext[j].itemList[i].kind>ARRAYCHAR&&tableContext[j].itemList[i].kind<ERROR)
				{
					return tableContext[j].itemList[i].adr;
				}
				else
				{
					myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),name+"函数名/过程不正确");
					return retnum;
				}
			}
				//return (tableContext[j].itemList[i].kind>ARRAYCHAR&&tableContext[j].itemList[i].kind<ERROR)?tableContext[j].itemList[i].adr:retnum;
		}
		j=tableContext[j].outproc;
	}
	for(i=tableContext[j].itemList.size()-1;i>=0;i--)
	{
		if(tableContext[j].itemList[i].name==name)
		{
			if(tableContext[j].itemList[i].kind>ARRAYCHAR&&tableContext[j].itemList[i].kind<ERROR)
			{
				return tableContext[j].itemList[i].adr;
			}
			else
			{
				myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),name+"函数名/过程不正确");
				return retnum;
			}
		}
			//return (tableContext[j].itemList[i].kind>ARRAYCHAR&&tableContext[j].itemList[i].kind<ERROR)?tableContext[j].itemList[i].adr:retnum;
	}


	//parser::callfunction中tempIndex崩溃啊！
	myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),name+"函数名/过程不正确");
	return retnum;//在符号表中没有定义的情况    需要继续执行报错！！！		这里是否要改成0,不可能调用0层函数		改一下，怕中间程序崩溃。	还是-1便于调试	最后再看。。。
}

/*
string SymbolTable::getProcArg(int procNum,string name,int index)
{
	int tempIndex=mySymbolTable.getProcIndexRead(procNum,name);
	if(index>tableContext[tempIndex].procArgNum)
		return "";
	return tableContext[tempIndex].itemList[index].name;
}*/


string SymbolTable::getProcArg(int procIndex,string name,int index)
{
	if(index>tableContext[procIndex].procArgNum)
		return "";
	return tableContext[procIndex].itemList[index].name;
}



kindType SymbolTable::canUse(int procNum,string name)
{
	int i,j;
	j=procNum;
	while(j!=0)
	{
		for(i=tableContext[j].itemList.size()-1;i>=0;i--)//符号表中有定义但不可写：如常数类型和procnone类型
		{
			if(tableContext[j].itemList[i].name==name)
				return (tableContext[j].itemList[i].kind<ERROR)?tableContext[j].itemList[i].kind:ERROR;
		}
		j=tableContext[j].outproc;
	}
	for(i=tableContext[j].itemList.size()-1;i>=0;i--)
	{
		if(tableContext[j].itemList[i].name==name)
			return (tableContext[j].itemList[i].kind<ERROR)?tableContext[j].itemList[i].kind:ERROR;
	}
	return ERROR;//在符号表中没有定义的情况
}

/*
kindType SymbolTable::canBeReadVar(int procNum,string name)
{
	int i,j;
	j=procNum;
	while(j!=0)
	{
		for(i=tableContext[j].itemList.size()-1;i>=0;i--)//符号表中有定义但不可写：如常数类型和procnone类型
		{
			if(tableContext[j].itemList[i].name==name)
				return (tableContext[j].itemList[i].kind>CONSTCHAR&&tableContext[j].itemList[i].kind<ARRAYINT)?tableContext[j].itemList[i].kind:ERROR;
		}
		j=tableContext[j].outproc;
	}
	for(i=tableContext[j].itemList.size()-1;i>=0;i--)
	{
		if(tableContext[j].itemList[i].name==name)
			return (tableContext[j].itemList[i].kind>CONSTCHAR&&tableContext[j].itemList[i].kind<ARRAYINT)?tableContext[j].itemList[i].kind:ERROR;
	}
	return ERROR;//在符号表中没有定义的情况

}*/



/*
int SymbolTable::getProcIndex(string name)//不能简单的这样写啊，现在看来还得要从所在层向外搜索来找函数的二维坐标
{
	//return 
}*/

/*
int SymbolTable::getProcArgIndex(int index,string id)
{
	int i;
	//for()
}
*/

int SymbolTable::getCurProcIndex(int ProcNum,string name)//不能简单的这样写啊，现在看来还得要从所在层向外搜索来找函数的二维坐标
{
	for(int i=tableContext[ProcNum].itemList.size()-1;i>=0;i--)
		if(tableContext[ProcNum].itemList[i].name==name)
			return i;
	return 0;
	//return 
}


void SymbolTable::addNewPrintString(string s)
{
	printString.push_back(s);
}

//void SymbolTable::printString

/*
void SymbolTable::getCurProcArgIndex(int index,string id)
{
	int i;
	//for()
}
*/

//在arrayList中增加维数记录
//int SymbolTable::


/*
void SymbolTable::addNewArrayItem(int procNum,int name,kindType kind,int value)
{
	if(!isExists(procNum,name))
	{
		SymbolTableItem newItem;
		if(kind==ARRAYINT||ARRAYCHAR)
		{
			newItem.name=name;
			newItem.kind=kind;
			arrayList.push_back();

		}
		else
		{
			newItem.name=name;
			newItem.kind=kind;
			newItem.value=value;
		}

		tableContext[procNum].itemList.push_back(newItem);

		tableContext[procNum].addItem(string name,kindType kind,int value);
		void SymTableContext::addItem(string name,kindType kind,int value);
	}
}

*/


void SymbolTable::getIdNameAdr(int procNum,string name,int &BL,int &ON)
{
	int i,j;
	j=procNum;
	while(j!=0)
	{
		for(i=tableContext[j].itemList.size()-1;i>=0;i--)//符号表中有定义但不可写：如常数类型和procnone类型
		{
			if(tableContext[j].itemList[i].name==name)
			{
				if(tableContext[j].itemList[i].kind<PROCNONE)
				{
					BL=j;ON=i;
				}
				else
				{
					BL=-1;ON=-1;
				}
				return;
			}
		}
		j=tableContext[j].outproc;
	}
	for(i=tableContext[j].itemList.size()-1;i>=0;i--)
	{
		if(tableContext[j].itemList[i].name==name)
		{
			if(tableContext[j].itemList[i].kind<PROCNONE)
			{
				BL=j;ON=i;
			}
			else
			{
				BL=-1;ON=-1;
			}
			return;
		}
	}
	BL=-1;ON=-1;
	return;//在符号表中没有定义的情况
}



//在ASM生成时计算偏移量
int SymbolTable::dimCount(int procNum,int idIndex)
{
	int i,dimTotal=0;
	for(i=0;i<idIndex;i++)
	{
		dimTotal+=tableContext[procNum].itemList[i].arraydim;
	}
	return dimTotal;
}

//判断变量是否是地址
//bool SymbolTable::isAdr(int procNum,int )


void SymbolTable::showTable(int procNum)
{
	cout<<"procID:"<<procNum<<"\t"<<"procName:"<<tableContext[procNum].procName<<"\t"<<"procKind:"<<tableContext[procNum].procKind<<endl
		<<"ArgNum:"<<tableContext[procNum].procArgNum<<"\t"<<"lev:"<<tableContext[procNum].lev<<"\t"<<"Outfunc:"<<tableContext[procNum].outproc<<endl;
	cout<<"tableContext:"<<endl;
	cout<<"id"<<"\t"<<"name"<<"\t"<<"kind"<<"\t"<<"value"<<"\t"<<"isnotadr"<<"\t"<<"dim"<<"\t"<<endl;
	for(int i=0;i<tableContext[procNum].itemList.size();i++)
	{
		cout<<i<<"\t"<<tableContext[procNum].itemList[i].name<<"\t"<<tableContext[procNum].itemList[i].kind<<"\t"<<tableContext[procNum].itemList[i].value<<"\t"
			<<tableContext[procNum].itemList[i].isnotadr<<"\t"<<tableContext[procNum].itemList[i].arraydim<<endl;
	}
}