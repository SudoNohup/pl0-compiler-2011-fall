#include<stdio.h>
#include<iostream>
#include<string.h>
#include "Lexer.h"
#include "GlobalData.h"

using namespace std;

int main()
{
	int i,OptMode=0;
	cout<<"请输入源文件名："<<endl;

	//normal
	
	cin>>sourcefileName;
	cout<<"请选择模式(输入OptMode的值)："<<endl;
	cout<<"OptMode：\t\n-1	不做优化\t\n0	窥孔优化\t\n1	+DAG图（消除公共子表达式）";
	cout<<"\t\n2	+全局数据流分析,局部寄存器池优化\t\n3	+构建冲突图，图着色算法，全局寄存器池优化"<<endl;
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
		cout<<"窥孔优化后的中间代码如下："<<endl;
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