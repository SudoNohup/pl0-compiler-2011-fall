#include "BaseBlock.h"
#include "GlobalData.h"
#include <iostream>

using namespace std;

int BaseBlock::flag[10000]={0};
//int BaseBlock::flagToProc[10000]={0};
deque<int>BaseBlock::in[1000];
deque<int>BaseBlock::out[1000];
deque<string>BaseBlock::use[1000];
deque<string>BaseBlock::define[1000];
deque<string>BaseBlock::varIn[1000];
deque<string>BaseBlock::varOut[1000];
deque<string>BaseBlock::activeVar;

void BaseBlock::print()
{
	int i,j;
	//freopen()
	freopen("BlockDivide.txt","w",stdout);
	for(i=0;i<myMiddleCode.codeList.size();i++)
	{
		cout<<flag[i]<<"\t"<<myMiddleCode.codeList[i].opr<<"\t"<<myMiddleCode.codeList[i].target1<<"\t"<<myMiddleCode.codeList[i].target2<<"\t"<<myMiddleCode.codeList[i].result<<endl;
	}
	//freopen
	freopen("BlockFlow.txt","w",stdout);
	for(i=1;i<=flag[myMiddleCode.codeList.size()-1];i++)//ע�⣬����i=1��ʼ��begin���ͱ���Ϊ1
	{
		cout<<"BaseBlock"<<i<<":"<<endl;
		cout<<"\tin\t";
		for(j=0;j<in[i].size();j++)
		{
			cout<<in[i][j]<<" ";
		}
		cout<<endl;
		cout<<"\tout\t";
		for(j=0;j<out[i].size();j++)
		{
			cout<<out[i][j]<<" ";
		}
		cout<<endl;
	}
}



int BaseBlock::isCompareFalse(string s)
{
	if (s=="<false"||s=="<=false"||s==">false"||s==">=false"||s=="<>false"||s=="=false")
		return 1;
	else return 0;
}

void BaseBlock::init()
{
	int i;
	int temp;
	int ProcNo=0;

	memset(flag,0,sizeof(flag));
	//̫�����ˡ��ָ����Ϊ1��Ȼ�������ۼ�
	for(i=0;i<myMiddleCode.codeList.size();i++)
	{
		if(myMiddleCode.codeList[i].opr=="Begin"||myMiddleCode.codeList[i].opr=="SetFlag")//||myMiddleCode.codeList[i].opr=="End")
			flag[i]=1;
		if(myMiddleCode.codeList[i].opr=="JmpTo"||isCompareFalse(myMiddleCode.codeList[i].opr)||myMiddleCode.codeList[i].opr=="IfFalse")
			flag[i+1]=1;
	}
	for(i=1;i<myMiddleCode.codeList.size();i++)
	{
		flag[i]=flag[i]+flag[i-1];
	}

	for(i=0;i<myMiddleCode.codeList.size()-1;i++)//ע������С��size����-1
	{
		if(flag[i]>0&&flag[i]!=flag[i+1])//ֱ��ǰ���ϵ����in out��ϵ
		{
			if(myMiddleCode.codeList[i].opr=="IfFalse"||isCompareFalse(myMiddleCode.codeList[i].opr))
			{
				if(existsOut(flag[i],flag[i+1])==0)
				{
					in[flag[i+1]].push_back(flag[i]);
					out[flag[i]].push_back(flag[i+1]);
				}
				if(myMiddleCode.codeList[i].opr=="IfFalse")
					temp=findFlag(myMiddleCode.codeList[i].target2);
				else
					temp=findFlag(myMiddleCode.codeList[i].result);

				if(existsOut(flag[i],temp)==0)
				{
					in[temp].push_back(flag[i]);
					out[flag[i]].push_back(temp);
				}
			}
			//else if(myMiddleCode.codeList[i+1].opr=="SetFlag")//��ʵҲ���Լ�JmpTo��IfFalse��isCompareFalse		���ǰ����jmp�����ﲻ������������
			//{
			//	//temp=findFlag(myMiddleCode.codeList[i].target1);
			//	if(existsOut(flag[i+1],flag[i])==0)
			//	{
			//		in[flag[i+1]].push_back(flag[i]);
			//		out[flag[i]].push_back(flag[i+1]);
			//	}
			//}
			else if(myMiddleCode.codeList[i].opr=="JmpTo")
			{
				temp=findFlag(myMiddleCode.codeList[i].target1);
				if(existsOut(flag[i],temp)==0)
				{
					in[temp].push_back(flag[i]);
					out[flag[i]].push_back(temp);
				}
			}
			else if(myMiddleCode.codeList[i].opr!="End")		//!!!����End����ֱ�Ӵ��й�ϵ				//End�Ƿ����������䣿��������		ע�⣺���ﻹ��Ҫ����ģ�û�ж�begin������Ϊ���һ���Ȼ��end
			{														//�����Ƕ�1~size()-1��begin���д���
				if(existsOut(flag[i],flag[i+1])==0)
				{
					in[flag[i+1]].push_back(flag[i]);
					out[flag[i]].push_back(flag[i+1]);
				}
			}
		}

	}

}


int BaseBlock::findFlag(string s)
{
	int i;
	for(i=0;i<myMiddleCode.codeList.size();i++)
	{
		if(myMiddleCode.codeList[i].opr=="SetFlag"&&myMiddleCode.codeList[i].target1==s)
			return flag[i];
	}
	return 0;
}

int BaseBlock::existsOut(int blockID,int id)
{
	int i;
	for(i=0;i<out[blockID].size();i++)
	{
		if(out[blockID][i]==id)
			return 1;
	}
	return 0;
}

int BaseBlock::isCalc(string op)
{
	if(op=="+"||op=="-"||op=="*"||op=="/") return 1;
	//if(op=="+"||op=="-"||op=="*"||op=="/"||op==":="||op==">"||op==">="||op=="<"||op=="<="||op=="="||op=="<>"||op=="[]=") return 1;
	else return 0;
}


void  BaseBlock::addUse(int blockID,string id)//������δ�����
{
	int i;
	if(id[0]=='#'||id[0]=='~') return;
	if(!(id[0]=='-'||(id[0]>='A'&&id[0]<='Z')||(id[0]>='a'&&id[0]<='z'))) return;//���ſ�ͷ�������ſ�ͷ����
	for(i=0;i<define[blockID].size();i++)//??????//defineɱ��use������define  wrong!!!
	{
		if(define[blockID][i]==id)
		{
			return;
		}
	}
	for(i=0;i<use[blockID].size();i++)
	{
		if(use[blockID][i]==id)
		{
			return;
		}
	}
	use[blockID].push_back(id);
}



void BaseBlock::addDefine(int blockID,string id)//������ų�һ��flag_flag�����
{
	int i;
	if(id[0]=='#'||id[0]=='~') return;
	if(!(id[0]=='-'||(id[0]>='A'&&id[0]<='Z')||(id[0]>='a'&&id[0]<='z'))) return;//���ſ�ͷ�������ſ�ͷ����
	for(i=0;i<use[blockID].size();i++)
	{
		if(use[blockID][i]==id)
		{
			return;
		}
	}
	for(i=0;i<define[blockID].size();i++)
	{
		if(define[blockID][i]==id)
		{
			return;
		}
	}
	define[blockID].push_back(id);
}

int BaseBlock::isCompare(string s)
{
	if (s=="<"||s=="<="||s==">"||s==">="||s=="<>"||s=="=")
		return 1;
	else return 0;
}


void BaseBlock::activeVarInit()
{
	int point=0;
	int now=0;
	int i,j;
	while(flag[point]==0) point++;
	for(i=point;i<myMiddleCode.codeList.size();i++)
	{
		if(flag[i]!=flag[i-1]) now++;
		if(myMiddleCode.codeList[i].opr=="WriteExpression")
		{
			addUse(flag[i],myMiddleCode.codeList[i].target1);
		}
		else if(myMiddleCode.codeList[i].opr=="Read")
		{
			addDefine(flag[i],myMiddleCode.codeList[i].target1);
		}
		else if(isCalc(myMiddleCode.codeList[i].opr))
		{
			addUse(flag[i],myMiddleCode.codeList[i].target1);
			addUse(flag[i],myMiddleCode.codeList[i].target2);//��addUse����addDefine:Use����
			addDefine(flag[i],myMiddleCode.codeList[i].result);
		}
		else if(isCompare(myMiddleCode.codeList[i].opr))//�����Ż���Ĳ����õ�
		{
			addUse(flag[i],myMiddleCode.codeList[i].target1);
			addUse(flag[i],myMiddleCode.codeList[i].target2);//��addUse����addDefine:Use����
			addDefine(flag[i],myMiddleCode.codeList[i].result);
		}
		else if(isCompareFalse(myMiddleCode.codeList[i].opr))//�����Ż���Ҫ�õ�
		{
			addUse(flag[i],myMiddleCode.codeList[i].target1);
			addUse(flag[i],myMiddleCode.codeList[i].target2);
		}
		else if(myMiddleCode.codeList[i].opr=="IfFalse")//�����Ż���Ĳ����õ�
		{
			addUse(flag[i],myMiddleCode.codeList[i].target1);
		}
		else if(myMiddleCode.codeList[i].opr=="value=")
		{
			addUse(flag[i],myMiddleCode.codeList[i].target1);
		}
		else if(myMiddleCode.codeList[i].opr==":=")
		{
			addUse(flag[i],myMiddleCode.codeList[i].target1);
			addDefine(flag[i],myMiddleCode.codeList[i].result);
		}
		else if(myMiddleCode.codeList[i].opr=="[]=")
		{
			addUse(flag[i],myMiddleCode.codeList[i].target1);
			addUse(flag[i],myMiddleCode.codeList[i].target2);//��addUse����addDefine:Use����
			addDefine(flag[i],myMiddleCode.codeList[i].result);//û�����ã���
		}
	}


	//freopen
	freopen("DefineUse.txt","w",stdout);

	for(i=1;i<=flag[myMiddleCode.codeList.size()-1];i++)//ע�⣬�������1��ʼ���
	{
		cout<<"Block"<<i<<":"<<endl;
		cout<<"\tuse\t";
		for(j=0;j<use[i].size();j++)
		{
			cout<<use[i][j]<<" ";
		}
		cout<<endl;
		cout<<"\tdefine\t";
		for(j=0;j<define[i].size();j++)
		{
			cout<<define[i][j]<<" ";
		}
		cout<<endl;
	}
}


//x=x-y	define:null	use:x,y

//����ģ�
//x=a+i;
//a=x+i;define��������use:???


int BaseBlock::existsVarIn(int blockID,string id)
{
	int i;
	for(i=0;i<varIn[blockID].size();i++)
	{
		if(varIn[blockID][i]==id)
			return 1;
	}
	return 0;
}

int BaseBlock::existsVarOut(int blockID,string id)
{
	int i;
	for(i=0;i<varOut[blockID].size();i++)
	{
		if(varOut[blockID][i]==id)
			return 1;
	}
	return 0;
}

int BaseBlock::existsDefine(int blockID,string id)
{
	int i;
	for(i=0;i<define[blockID].size();i++)
	{
		if(define[blockID][i]==id)
			return 1;
	}
	return 0;
}


bool BaseBlock::activeVarExists(string s)
{
	for(int i=0;i<activeVar.size();i++)
	{
		if(activeVar[i]==s) return true;
	}
	return false;

}


//out[B]=U(B�ĺ�̿�P)in[P]
//in[B]=use[B]U(out[B]-def[B])

void BaseBlock::activeVarBegin()
{
	int i,j,k;
	int f=1;
	for(i=flag[myMiddleCode.codeList.size()-1];i>0;i--)//�Ӻ���ǰɨ��
	{
		for(j=0;j<use[i].size();j++)
		{
			if(existsVarIn(i,use[i][j])==0)//���Բ��ж���ֱ�Ӹ��Ƶ�	//�����ǿ�ʼ
			{
				varIn[i].push_back(use[i][j]);			//������ÿ��in[B]�м���use[B]
				f=1;
			}
		}
	}

	while(f)
	{
		f=0;
		for(i=flag[myMiddleCode.codeList.size()-1];i>0;i--)//�Ӻ���ǰɨ�裬�Ի��������
		{
			for(j=0;j<out[i].size();j++)//�����г��ڱ���
			{
				for(k=0;k<varIn[out[i][j]].size();k++)//�Գ��ڴ���In[P]����Ԫ�ر���
				{
					int temp=existsVarOut(i,varIn[out[i][j]][k]);
					if(temp==0)//ʵ��out[B]=U(B�ĺ�̿�P)in[P]
					{
						varOut[i].push_back(varIn[out[i][j]][k]);
						f=1;
					}
				}
			}
			for(j=0;j<varOut[i].size();j++)//�����е�ǰ���out[B]��Ԫ�ر���
			{
				if(existsDefine(i,varOut[i][j])==0		//�ų�define[B],ʵ��out[B]-def[B])
					&&existsVarIn(i,varOut[i][j])==0)	//�ų��ظ���	�����in[B]���Ѿ�����use[B]
				{
					varIn[i].push_back(varOut[i][j]);
					f=1;
				}
			}
		}

	}
	//freopen
	freopen("BlockVarInOut.txt","w",stdout);
	for(i=1;i<=flag[myMiddleCode.codeList.size()-1];i++)
	{
		cout<<"Block"<<i<<":"<<endl;
		cout<<"\tIn\t";
		for(j=0;j<varIn[i].size();j++)
		{
			cout<<varIn[i][j]<<" ";
			if(!activeVarExists(varIn[i][j]))
			{
				activeVar.push_back(varIn[i][j]);
			}
		}
		cout<<endl;
		cout<<"\tOut\t";
		for(j=0;j<varOut[i].size();j++)
		{
			if(!activeVarExists(varOut[i][j]))
			{
				activeVar.push_back(varOut[i][j]);
			}
			cout<<varOut[i][j]<<" ";
		}
		cout<<endl;
	}

	cout<<"���п�Խ������Ļ�Ծ����"<<endl;
	for(i=0;i<activeVar.size();i++)
		cout<<activeVar[i]<<" ";
	cout<<endl;

	//conflictGraghBuilt();

}





