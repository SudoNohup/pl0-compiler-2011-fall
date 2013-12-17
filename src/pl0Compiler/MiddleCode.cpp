#include "MiddleCode.h"
#include <iostream>
using namespace std;

deque<MiddleCodeItem>MiddleCode::codeList;

void MiddleCode::addMiddleCode(string opr,string target1,string target2,string result)
{
	MiddleCodeItem temp;
	temp.opr=opr;
	temp.target1=target1;
	temp.target2=target2;
	temp.result=result;
	codeList.push_back(temp);
}

void MiddleCode::showMiddleCode()
{
	for(int i=0;i<codeList.size();i++)
	{
		cout<<codeList[i].opr<<"\t"<<codeList[i].target1<<"\t"<<codeList[i].target2<<"\t"<<codeList[i].result<<endl;
	}
}