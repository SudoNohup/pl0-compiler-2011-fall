#include "ConflictGraph.h"
#include "GlobalData.h"
#include <iostream>

using namespace std;

deque <ConflictGraphItem> ConflictGraph::ConflictTable;
deque <ConflictGraphItem> ConflictGraph::ConflictTableCopy;
deque <int> ConflictGraph::colorRecord;

void ConflictGraph::addTableItem(int procIndex,int idNo)
{
	ConflictGraphItem temp;
	temp.No=ConflictTable.size();
	temp.procIndex=procIndex;
	temp.idNo=idNo;
	temp.regNum=-3;
	ConflictTable.push_back(temp);
	ConflictTableCopy.push_back(temp);
}

void ConflictGraph::addConflictRelation(int idProcNum1,int idIndex1,int idProcNum2,int idIndex2)
{
	int idTableIndex1=-1,idTableIndex2=-1;
	int i;
	if(!  (myBaseBlock.activeVarExists(mySymbolTable.tableContext[idProcNum1].itemList[idIndex1].name)
		&&mySymbolTable.tableContext[idProcNum1].itemList[idIndex1].isnotconst==true
		&&myBaseBlock.activeVarExists(mySymbolTable.tableContext[idProcNum2].itemList[idIndex2].name)
		&&mySymbolTable.tableContext[idProcNum2].itemList[idIndex2].isnotconst==true)
		)
	{
		return;
	}
	



	if(mySymbolTable.tableContext[idProcNum1].itemList[idIndex1].name[0]=='#'
		||mySymbolTable.tableContext[idProcNum1].itemList[idIndex1].name[0]=='~'
		||mySymbolTable.tableContext[idProcNum2].itemList[idIndex2].name[0]=='#'
		||mySymbolTable.tableContext[idProcNum2].itemList[idIndex2].name[0]=='~')//数组~存的是地址，但存入寄存器里多次调用的是值
	{
		return;
	}

	for(i=0;i<ConflictTable.size();i++)
	{
		if(ConflictTable[i].procIndex==idProcNum1&&ConflictTable[i].idNo==idIndex1)
		{
			idTableIndex1=i;
		}
		if(ConflictTable[i].procIndex==idProcNum2&&ConflictTable[i].idNo==idIndex2)
		{
			idTableIndex2=i;
		}
	}

	for(i=0;i<ConflictTable[idTableIndex1].itemNo.size();i++)
	{
		if(ConflictTable[idTableIndex1].itemNo[i]==idTableIndex2)
			return;
	}

	for(i=0;i<ConflictTable[idTableIndex2].itemNo.size();i++)
	{
		if(ConflictTable[idTableIndex2].itemNo[i]==idTableIndex1)
			return;
	}

	ConflictTable[idTableIndex1].itemNo.push_back(idTableIndex2);
	ConflictTable[idTableIndex2].itemNo.push_back(idTableIndex1);

	ConflictTableCopy[idTableIndex1].itemNo.push_back(idTableIndex2);
	ConflictTableCopy[idTableIndex2].itemNo.push_back(idTableIndex1);

}

void ConflictGraph::conflictGraphPrint()
{
	//freopen
	freopen("ConflictGraph.txt","w",stdout);
	cout<<"ID ProcIndex Id Name ConflictNo"<<endl;
	for(int i=0;i<ConflictTable.size();i++)
	{
		int tempProcIndex=ConflictTable[i].procIndex;
		int tempidNo=ConflictTable[i].idNo;
		string s=mySymbolTable.tableContext[tempProcIndex].itemList[tempidNo].name;
		
		cout<<i<<"\t"<<tempProcIndex<<"\t"<<tempidNo<<"\t"<<s<<":";
		for(int j=0;j<ConflictTable[i].itemNo.size();j++)
		{
			cout<<ConflictTable[i].itemNo[j]<<"\t";
		}
		cout<<endl;
	}
}


void ConflictGraph::conflictGraphBuilt()
{
	int ProcNo=-1;
	for(int j=0;j<mySymbolTable.tableContext.size();j++)
	{
		for(int i=0;i<mySymbolTable.tableContext[j].itemList.size();i++)
		{

			//(mySymbolTable.tableContext[j].itemList[i].isnotconst==true)
			//cout<<j<<" "<<i<<endl;
			if(myBaseBlock.activeVarExists(mySymbolTable.tableContext[j].itemList[i].name)	&&mySymbolTable.tableContext[j].itemList[i].isnotconst==true)
			{
				if(mySymbolTable.tableContext[j].itemList[i].name[0]!='#'
					&&mySymbolTable.tableContext[j].itemList[i].name[0]!='~'
					)//数组~存的是地址，但存入寄存器里多次调用的是值				//_0不加入？？？？？？？？？？？？
					addTableItem(j,i);
			}
			
		}
	}
	

	//cout<<"hello"<<endl;

	for(int t=0;t<myMiddleCode.codeList.size();t++)
	{
		
		if(myMiddleCode.codeList[t].opr=="Begin")
		{
			ProcNo=atoi(myMiddleCode.codeList[t].target1.c_str());
			
		}

		if(myBaseBlock.flag[t]!=myBaseBlock.flag[t+1])//处理前一个基本块
		{
			int i,j,k;
			i=myBaseBlock.flag[t];
			for(j=0;j<myBaseBlock.varIn[i].size();j++)
			{
				for(k=j+1;k<myBaseBlock.varIn[i].size();k++)
				{
					int idProcNum1=-1,idIndex1=-1;
					int idProcNum2=-1,idIndex2=-1;
					mySymbolTable.getIdNameAdr(ProcNo,myBaseBlock.varIn[i][j],idProcNum1,idIndex1);
					

					
					//cout<<myBaseBlock.varIn[i][j]<<endl;
					mySymbolTable.getIdNameAdr(ProcNo,myBaseBlock.varIn[i][k],idProcNum2,idIndex2);
					
					//cout<<myBaseBlock.varIn[i][k]<<endl;


					//exit(0);


					if(idProcNum1==-1||idIndex1==-1||idProcNum2==-1||idIndex2==-1)
					{
						//这种情况不会出现
					}
					else
					{
						addConflictRelation(idProcNum1,idIndex1,idProcNum2,idIndex2);
					}
				}
			}
			for(j=0;j<myBaseBlock.varOut[i].size();j++)
			{
				for(int k=j+1;k<myBaseBlock.varOut[i].size();k++)
				{
					int idProcNum1=-1,idIndex1=-1;
					int idProcNum2=-1,idIndex2=-1;
					mySymbolTable.getIdNameAdr(ProcNo,myBaseBlock.varOut[i][j],idProcNum1,idIndex1);
					mySymbolTable.getIdNameAdr(ProcNo,myBaseBlock.varOut[i][k],idProcNum2,idIndex2);
					if(idProcNum1==-1||idIndex1==-1||idProcNum2==-1||idIndex2==-1)
					{
						//这种情况不会出现
					}
					else
					{
						addConflictRelation(idProcNum1,idIndex1,idProcNum2,idIndex2);
					}
				}
			}
		}
	}

	conflictGraphPrint();

}


//int ConflictGraph::degreesum(deque<int> itemNo)
//{
	
//}

void ConflictGraph::deletePoint(int PointNum)
{
	for(int i=0;i<ConflictTable[PointNum].itemNo.size();i++)
	{
		int LinkPointNum=ConflictTable[PointNum].itemNo[i];

		for(int k=0;k<ConflictTable.size();k++)
		{
			if(ConflictTable[k].No==LinkPointNum)
			{
				for(int j=0;j<ConflictTable[k].itemNo.size();j++)
				{
					if(ConflictTable[k].itemNo[j]==ConflictTable[PointNum].No)
					{
						ConflictTable[k].itemNo.erase(ConflictTable[k].itemNo.begin()+j);
					}
				}
			}
		}

	}
	ConflictTable.erase(ConflictTable.begin()+PointNum);
}

bool ConflictGraph::Duplicate(int PointNum,int ColorNum)
{
	for(int i=0;i<ConflictTableCopy[PointNum].itemNo.size();i++)
	{
		int No=ConflictTableCopy[PointNum].itemNo[i];
		if(ConflictTableCopy[No].regNum==ColorNum)
			return true;
	}
	return false;
}


void ConflictGraph::assignColor(int PointNum,int regTotal)//（反向后）依次给新加入的节点选取颜色
{
	if(ConflictTableCopy[PointNum].regNum==-2) return;

	for(int i=0;i<regTotal;i++)
	{
		if(!Duplicate(PointNum,i))
		{
			ConflictTableCopy[PointNum].regNum=i;
			return;
		}
	}

}


void ConflictGraph::color(int regTotal)
{
	int flag=1;
	while(!ConflictTable.empty())
	{
		while(flag)
		{
			flag=0;
			for(int i=0;i<ConflictTable.size();i++)
			{
				int degree=ConflictTable[i].itemNo.size();
				if(degree<regTotal)
				{
					//ConflictTable[i].regNum=-1;//标记为分配寄存器的节点

					int No=ConflictTable[i].No;
					ConflictTableCopy[No].regNum=-1;//标记为分配寄存器的节点
					colorRecord.push_front(No);
					deletePoint(i);
					flag=1;
				}
			}
		}


		//for(int i=0;i<ConflictTable.size();i++)
		//{

		//}
		if(!ConflictTable.empty())//在图中选取适当的节点，记录为不分配全局寄存器的节点，并从图中移走
		{
			int No=ConflictTable[0].No;
			ConflictTableCopy[No].regNum=-2;//标记为不分配寄存器的节点
			colorRecord.push_front(No);
			deletePoint(0);
			flag=1;
		}
		else
		{
			break;
		}

	}

	//print

	freopen("ReverseColor.txt","w",stdout);
	int i;
	for(i=0;i<colorRecord.size();i++)
	{
		cout<<colorRecord[i]<<"\t";
	}
	cout<<endl;

	for(i=0;i<colorRecord.size();i++)
	{
		assignColor(colorRecord[i],regTotal);
		//assign(i);
		
	}

	colorPrint();
	//填入符号表

	RegisterToSymbolTable();

	
}


void ConflictGraph::colorPrint()
{
	//freopen
	freopen("ColorPrint.txt","w",stdout);
	cout<<"ProcIndex Id Name RegNum"<<endl;
	for(int i=0;i<ConflictTableCopy.size();i++)
	{
		int tempProcIndex=ConflictTableCopy[i].procIndex;
		int tempidNo=ConflictTableCopy[i].idNo;
		string s=mySymbolTable.tableContext[tempProcIndex].itemList[tempidNo].name;

		cout<<tempProcIndex<<"\t"<<tempidNo<<"\t"<<s<<":";
		//for(int j=0;j<ConflictTableCopy[i].itemNo.size();j++)
		//{
			//cout<<ConflictTableCopy[i].itemNo[j]<<"\t";
		//}
		cout<<ConflictTableCopy[i].regNum;
		cout<<endl;
	}
}

void ConflictGraph::RegisterToSymbolTable()
{
	for(int i=0;i<ConflictTableCopy.size();i++)
	{
		if(ConflictTableCopy[i].regNum>=0)
		{
			 int procIndex=ConflictTableCopy[i].procIndex;
			 int idNo=ConflictTableCopy[i].idNo;
			 mySymbolTable.tableContext[procIndex].itemList[idNo].regnum=ConflictTableCopy[i].regNum;
		}
	}
}



