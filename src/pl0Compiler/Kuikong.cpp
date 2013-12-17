#include "Kuikong.h"
#include "GlobalData.h"
#include <iostream>

using namespace std;


//OptCode Kuikong::Opt_code;

int Kuikong::point;
int Kuikong::tail;
int Kuikong::flagofflag[10000];
OptCode Kuikong::Opt_code;


void Kuikong::addOptCode(int id)
{
	Opt_code.addCode(myMiddleCode.codeList[id].opr,myMiddleCode.codeList[id].target1,myMiddleCode.codeList[id].target2,myMiddleCode.codeList[id].result);
}

int Kuikong::isCompare(string s)
{
	if(s=="<"||s=="<="||s==">"||s==">="||s=="="||s=="<>")
		return 1;
	else return 0;
}

int Kuikong::isIfFalse(string s)
{
	if(s=="IfFalse")
		return 1;
	else return 0;
}

int Kuikong::isJmpTo(string s)
{
	if(s=="JmpTo")
		return 1;
	else return 0;
}

int Kuikong::isSetFlag(string s)
{
	if(s=="SetFlag")
		return 1;
	else return 0;
}

void Kuikong::deleteSetFlag()
{
	point=0;
	//tail=myMiddleCode.codeList.size();
	//Opt_code.codeList.clear();
	//while(point<tail)
	while(point<myMiddleCode.codeList.size())
	{
		if(isSetFlag(myMiddleCode.codeList[point].opr)&&isSetFlag(myMiddleCode.codeList[point+1].opr))//&&(myMiddleCode.codeList[point].target1==myMiddleCode.codeList[point+1].target1))
		{
			string tempflag=myMiddleCode.codeList[point].target1;
			string deleteflag=myMiddleCode.codeList[point+1].target1;
			
			for(int i=0;i<myMiddleCode.codeList.size();i++)
			{
				if(isIfFalse(myMiddleCode.codeList[i].opr)&&myMiddleCode.codeList[i].target2==deleteflag)
				{
					myMiddleCode.codeList[i].target2=tempflag;
				}
				else if(isJmpTo(myMiddleCode.codeList[i].opr)&&myMiddleCode.codeList[i].target1==deleteflag)
				{
					myMiddleCode.codeList[i].target1=tempflag;
				}
			}
			myMiddleCode.codeList.erase(myMiddleCode.codeList.begin()+point+1);
			//point不加1了，比较point之后的那个点。
		}
		else
		{
			point++;
		}
	}
	

}

int Kuikong::getNumberOfFlag(string s)
{
	char str[50];
	strcpy(str,s.c_str());
	for(int i=0;str[i]!='\0';i++)
	{
		if(str[i]>='0'&&str[i]<='9')
		{
			return (atoi(str+i));
		}
	}
	return 0;
}

void Kuikong::updateMiddleCode()
{
	int i;
	myMiddleCode.codeList.clear();
	for(i=0;i<Opt_code.codeList.size();i++)
	{
		myMiddleCode.addMiddleCode(Opt_code.codeList[i].opr,Opt_code.codeList[i].target1,Opt_code.codeList[i].target2,Opt_code.codeList[i].result);
	}
}

void Kuikong::getFlagSum()
{
	int flagnum;
	for(int i=0;i<myMiddleCode.codeList.size();i++)
	{
		if(isIfFalse(myMiddleCode.codeList[i].opr))
		{
			flagnum=getNumberOfFlag(myMiddleCode.codeList[i].target2);
			flagofflag[flagnum]++;
		}
		else if(isJmpTo(myMiddleCode.codeList[i].opr))
		{
			flagnum=getNumberOfFlag(myMiddleCode.codeList[i].target1);
			flagofflag[flagnum]++;
		}
	}
}


void Kuikong::Kuikonginit()
{
	int flagnum;
	memset(flagofflag,0,sizeof(flagofflag));//标志状态都为0
	deleteSetFlag();
	getFlagSum();


	point=0;
	tail=myMiddleCode.codeList.size();
	
	while(point<tail)
	{
		//cout<<point<<endl;
		
		if(isSetFlag(myMiddleCode.codeList[point].opr))//去掉无用的SetFlag，一般用不到这一条
		{
			flagnum=getNumberOfFlag(myMiddleCode.codeList[point].target1);
			if(flagofflag[flagnum]==0)
			{
				point++;
			}
			else
			{
				addOptCode(point);
				point++;
				continue;
			}
		}

		if((isCompare(myMiddleCode.codeList[point].opr)==0)&&(isJmpTo(myMiddleCode.codeList[point].opr)==0))
		{
			addOptCode(point);
			point++;
			continue;
		}
		else if(isCompare(myMiddleCode.codeList[point].opr)==1)
		{
			//flagnum=getNumberOfFlag(myMiddleCode.codeList[point].target1);
			//flagofflag[flagnum]=1;
			if(isIfFalse(myMiddleCode.codeList[point+1].opr))
			{
				Opt_code.addCode(myMiddleCode.codeList[point].opr+"false",myMiddleCode.codeList[point].target1,myMiddleCode.codeList[point].target2,myMiddleCode.codeList[point+1].target2);
				point++;point++;
				continue;
			}
			else
			{
				addOptCode(point);
				point++;
				continue;
			}
		}
		else if(isJmpTo(myMiddleCode.codeList[point].opr)==1)
		{
			flagnum=getNumberOfFlag(myMiddleCode.codeList[point].target1);
			if(isSetFlag(myMiddleCode.codeList[point+1].opr)&&(myMiddleCode.codeList[point].target1==myMiddleCode.codeList[point+1].target1))
			{
				if(flagofflag[flagnum]==1)
				{
					point++;point++;
				}
				else
				{
					point++;
				}
				//flagofflag[flagnum]=1;
			}
			else
			{
				addOptCode(point);
				point++;
				//flagofflag[flagnum]=1;
				continue;
			}
		}
		
	}
	Opt_code.show();
	updateMiddleCode();
	//cout<<"hello"<<endl;


}