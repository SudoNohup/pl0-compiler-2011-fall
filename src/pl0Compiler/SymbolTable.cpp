#include "SymbolTable.h"
#include "GlobalData.h"
#include <iostream>

using namespace std;

deque<SymTableContext>SymbolTable::tableContext;
deque <string> SymbolTable::printString;
//deque <int>SymbolTable::arrayList;
//int SymbolTable::arrayNum;

//int SymbolTable::number;

//��ʼ�����ű�
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

//��ָ����������Ŀ
void SymbolTable::addNewItem(int procNum,string name,kindType kind,int value)
{
	SymbolTableItem newItem;
	if(!isExists(procNum,name))
	{
		//�����������������ʱ�趨kind��Ϊint�����������ڴ�����ʱ�趨
		newItem.name=name;
		newItem.kind=kind;
		newItem.value=value;
		newItem.arraydim=1;//һ�������Ԫ���趨Ϊ1ά�������Ժ����ռ�ÿռ�
		newItem.isnotadr=true;//����������ڼ���仰		....�������仰�����������ǲ������е�ֵ��Ԥ�Ȱ����п��ܻ��������
		newItem.isnotconst=true;//Ĭ�ϲ��ǳ�����
		newItem.regnum=-1;

		//����id������������
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
		//	newItem.id=tableContext[procNum].itemList[tempSize-1].id+1;//(�������һ������Ϊ��ʱ���д��󣡣��趨һ��former id��Ϊ��������ڲ�����Ҳ���������һ�����)
		//}

		tableContext[procNum].itemList.push_back(newItem);
	}
	else
	{
		myErrorHandler.declareError(myLexer.x(),myLexer.y(),name+"�ظ�����");
	}

		//tableContext[procNum].addItem(string name,kindType kind,int value);
		//void SymTableContext::addItem(string name,kindType kind,int value);

}


//�Ƿ��Ѿ�����
int SymbolTable::isExists(int procNum,string name)
{
	for(int i=0;i<tableContext[procNum].itemList.size();i++)//ֻ��鱾��ģ�����ͬ���������������������ͬ			��㶨�帲�ǣ�����
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
	temptemp.procArgNum=0;//֮�����

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
		for(i=tableContext[j].itemList.size()-1;i>=0;i--)//���ű����ж��嵫����д���糣�����ͺ�procnone����
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
	return ERROR;//�ڷ��ű���û�ж�������

}



kindType SymbolTable::canRead(int procNum,string name)
{
	int i,j;
	j=procNum;
	while(j!=0)
	{
		for(i=tableContext[j].itemList.size()-1;i>=0;i--)//���ű����ж��嵫����д���糣�����ͺ�procnone����
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
	return ERROR;//�ڷ��ű���û�ж�������

}


kindType SymbolTable::isProcRead(int procNum,string name)
{
	int i,j;
	j=procNum;
	while(j!=0)
	{
		for(i=tableContext[j].itemList.size()-1;i>=0;i--)//���ű����ж��嵫����д���糣�����ͺ�procnone����
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
	return ERROR;//�ڷ��ű���û�ж�������
}


int SymbolTable::getProcIndexRead(int procNum,string name)
{
	int retnum=0;
	int i,j;
	j=procNum;
	while(j!=0)
	{
		for(i=tableContext[j].itemList.size()-1;i>=0;i--)//���ű����ж��嵫����д���糣�����ͺ�procnone����
		{
			if(tableContext[j].itemList[i].name==name)
			{
				if(tableContext[j].itemList[i].kind>ARRAYCHAR&&tableContext[j].itemList[i].kind<ERROR)
				{
					return tableContext[j].itemList[i].adr;
				}
				else
				{
					myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),name+"������/���̲���ȷ");
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
				myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),name+"������/���̲���ȷ");
				return retnum;
			}
		}
			//return (tableContext[j].itemList[i].kind>ARRAYCHAR&&tableContext[j].itemList[i].kind<ERROR)?tableContext[j].itemList[i].adr:retnum;
	}


	//parser::callfunction��tempIndex��������
	myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),name+"������/���̲���ȷ");
	return retnum;//�ڷ��ű���û�ж�������    ��Ҫ����ִ�б�������		�����Ƿ�Ҫ�ĳ�0,�����ܵ���0�㺯��		��һ�£����м���������	����-1���ڵ���	����ٿ�������
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
		for(i=tableContext[j].itemList.size()-1;i>=0;i--)//���ű����ж��嵫����д���糣�����ͺ�procnone����
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
	return ERROR;//�ڷ��ű���û�ж�������
}

/*
kindType SymbolTable::canBeReadVar(int procNum,string name)
{
	int i,j;
	j=procNum;
	while(j!=0)
	{
		for(i=tableContext[j].itemList.size()-1;i>=0;i--)//���ű����ж��嵫����д���糣�����ͺ�procnone����
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
	return ERROR;//�ڷ��ű���û�ж�������

}*/



/*
int SymbolTable::getProcIndex(string name)//���ܼ򵥵�����д�������ڿ�������Ҫ�����ڲ������������Һ����Ķ�ά����
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

int SymbolTable::getCurProcIndex(int ProcNum,string name)//���ܼ򵥵�����д�������ڿ�������Ҫ�����ڲ������������Һ����Ķ�ά����
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

//��arrayList������ά����¼
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
		for(i=tableContext[j].itemList.size()-1;i>=0;i--)//���ű����ж��嵫����д���糣�����ͺ�procnone����
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
	return;//�ڷ��ű���û�ж�������
}



//��ASM����ʱ����ƫ����
int SymbolTable::dimCount(int procNum,int idIndex)
{
	int i,dimTotal=0;
	for(i=0;i<idIndex;i++)
	{
		dimTotal+=tableContext[procNum].itemList[i].arraydim;
	}
	return dimTotal;
}

//�жϱ����Ƿ��ǵ�ַ
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