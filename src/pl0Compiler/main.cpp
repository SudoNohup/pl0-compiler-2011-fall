#include<stdio.h>
#include<iostream>
#include<string.h>
#include "Lexer.h"
#include "GlobalData.h"

using namespace std;

int main()
{
	int i,OptMode=0;
	cout<<"Please input the source file name:"<<endl;

	//normal
	
	cin>>sourcefileName;
	cout<<"Please input the OptMode"<<endl;
	cout<<"OptMode:\t\n-1  No Optimization\t\n0	Peephole Optimization\t\n1	+DAG (Eliminate the common subexpression)";
	cout<<"\t\n2	+Local Register Allocation Optimization with Global Data Flow Analysis\t\n3	+Global Register Allocation Optimization with Constructing Conflict Graph and Heuristic Graph Coloring Algorithm"<<endl;
	cin>>OptMode;

	int len=strlen(sourcefileName);
	for(i=0;i<len-4;i++)
	{
		objectfileName[i]=sourcefileName[i];
	}
	strcpy(optobjectfileName,objectfileName);
	strcat(objectfileName,".asm");
	strcat(optobjectfileName,"Opt.asm");
	//strcat(optobjectfileName,objectfileName);

	
	//endnormal

	
	//debug
	/*
	char temp[20];
	cin>>temp;

	strcpy(sourcefileName,"E:\\compile\\test程序\\");
	//strcpy(sourcefileName,"E:\\ProgramData\\Compiler\\");
	strcat(sourcefileName,temp);
	strcat(sourcefileName,".txt");

	strcat(objectfileName,"C:\\Users\\huang\\Desktop\\hellojames\\1.asm");
	*/
	//enddebug

	

	if (freopen(sourcefileName,"r",stdin)==NULL){
		printf("Can not find the file!\n");
		return 0;
	}
	//myLexer.testLexer();



	mySymbolTable.initSymbolTable();
	myParser.initParser();
	myParser.program();
	cout<<myLexer.x()<<"\t"<<myLexer.y()<<endl;
	
	freopen("symbolTable(Origin).txt","w",stdout);
	for (i=0;i<=mySymbolTable.tableContext.size()-1;i++){
		mySymbolTable.showTable(i);
		cout << endl;
	}

	freopen("MiddleCode(Origin).txt","w",stdout);
	myMiddleCode.showMiddleCode();

	if(OptMode>=0)
	{
		freopen("MiddleCode(Kuikong).txt","w",stdout);
		cout<<"The intermediate code after the optimization is as the follows:"<<endl;
		myKuikong.Kuikonginit();
	}

	myASMGenerator.setObjectfileName(objectfileName);
	myASMGenerator.generate();

	if(OptMode>=1)
	{
		int GlobalOK=0;
		//myBaseBlock.init();//基本块划分也可以放在前面
		//myBaseBlock.print();
		//myBaseBlock.activeVarInit();
		//myBaseBlock.activeVarBegin();

		//cout<<"DAG优化后的中间代码如下："<<endl;//DAG图：消除局部公共子表达式
		myDAG.DAGinit();
		freopen("symbolTable(DAG).txt","w",stdout);
		for (i=0;i<=mySymbolTable.tableContext.size()-1;i++)
		{
			mySymbolTable.showTable(i);
			cout << endl;
		}

		if(OptMode>=2)
		{
			myBaseBlock.init();//基本块划分
			myBaseBlock.print();
			myBaseBlock.activeVarInit();
			myBaseBlock.activeVarBegin();
		}
		
		if(OptMode>=3)
		{
			myConflictGraph.conflictGraphBuilt();//冲突图构建
			myConflictGraph.color(2);//图着色算法分配全局寄存器
			GlobalOK=1;
		}

		//myASMGenerator.generate();
		myOptASMGenerator.setObjectfileName(optobjectfileName);//局部寄存器池优化
		myOptASMGenerator.setGlobalOK(GlobalOK);//设置是否进行全局寄存器优化
		myOptASMGenerator.generate();

	}
	return 0;
}
