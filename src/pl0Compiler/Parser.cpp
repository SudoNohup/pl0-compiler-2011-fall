#include "Parser.h"
#include "GlobalData.h"
#include<stdlib.h>
#include<strstream>

#include "iostream"

using namespace std;

int Parser::nowProcNum;//�������ڵĺ�����ţ���ʼΪ0
//Ҫ��һ�������ı�����
string Parser::idName;
int Parser::tempNum=0;
int Parser::tempAdrNum=0;
int Parser::flagNum=0;
int Parser::stringNum=0;
int Parser::level;//����
bool Parser::functionFlag;//�����Ƿ��з���ֵ�ı�־λ
LexerItem Parser::sym;
kindType Parser::t;	//�﷨�Ƶ� ���ͱ���t

//���캯��
void Parser::initParser()
{
	sym=myLexer.getSym();
	nowProcNum=0;
	level=0;//�������һ���ǵ�1�㡢������level++
}

//�Ƿ��ǻ�������
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

//�Ƿ��ǹ�ϵ�����
int Parser::isCompare(string s)
{
	if(s=="<"||s=="<="||s==">"||s==">="||s=="="||s=="<>")
		return 1;
	else return 0;
}

//�Ƿ��ǼӼ������
int Parser::isAdd(string s)
{
	if(s=="+"||s=="-")
		return 1;
	else return 0;
}

//�Ƿ��ǳ˳������
int Parser::isMul(string s)
{
	if(s=="*"||s=="/")
		return 1;
	else return 0;
}


//����
void Parser::program()
{
	block();
	if(sym.symbol!=".") 
	{
		//errorû�о�Ž�β
		//cout<<"����δ����\n";
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"������û�н�β.");
	}

	printf("�����﷨��������\n");
}



//����ע������ProcBegin��ProcEnd���м����
//�ֳ���
void Parser::block()//��ʼʱlev=0����
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
	
	if(sym.symbol=="var")//��Ԥ���ַ��ķ����������д�		??????���ܵ�ʱ��������ͻ���
	{
		//sym=myLexer.getSym();
		varDes();
	}

	while(sym.symbol =="function"||sym.symbol =="procedure")
	{
		if(sym.symbol == "function")
		{
			//sym=myLexer.getSym();
			//���嶯��
			function(lastProcNum);
			printf("������һ������\n");
		}
		else if(sym.symbol == "procedure")
		{

			//���²���Ҳ�����ڽ���procedure���ֺ�Ԥ��ִ��
		//	idName=myLexer.getNextSym(1).symbol;//û�в��Թ�����֪��û��bug��������������������
			//mySymbolTable.addNewItem(nowProcNum,idName,PROCNONE,0);
			//mySymbolTable.addNewContext(idName,PROCNONE,nowProcNum);
			
			//lastFunc=idName;
			//nowProcNum++;
			
			//sym=myLexer.getSym();���ܼ�
			//���嶯��
			procedure(lastProcNum);

			printf("������һ������\n");

		}
	}

		//printf(end functions/procedure);


	if(sym.symbol=="begin")
	{

		mySymbolTable.tableContext[lastProcNum].functionReturnFlag=false;//��¼ִ�����Ƿ��з���ֵ


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
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"����û�з���ֵ");

			//error �����޷���ֵ
			//std::cout<<mySymbolTable.tableContext[lastProcNum].procKind<<endl;
			//printf("no return\n");
		}
	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�ֳ�����û�и������");
	}

	level--;

}

//����˵������
void Parser::constDes()
{
	if(sym.symbol!="const")//�˾���ʵ����ɾ��
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵�����ֿ�ʼû��const");
		return;
	}
	sym=myLexer.getSym();
	constDefine();
	while(sym.symbol==",")//���У����б�ʶ����ע����var���б�ʶ�������У�����ʽ����ͬ
	{
		sym=myLexer.getSym();
		constDefine();
	}
	if(sym.symbol!=";")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵���������ȱ��;");
	}
	else
	{
		sym=myLexer.getSym();
	}
}


//��������
void Parser::constDefine()
{
	string constval;
	if(sym.kind!=21)
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�������忪ʼ���Ǳ�ʶ��");
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
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��������û���м��=");
		return;
	}
	else
	{
		sym=myLexer.getSym();
	}
	constval=constValue();
	//if(const)

	//if(constval[0]=='\'')//�����Ƿ�������⣿�������������������������ֺ��ַ�����\'��ASCII��ֵ			t��ȫ�ֱ����������Ϳ����ڴʷ������õ���sym��ֱ���жϡ�//����û�����Ͳ�ƥ�������
	//	t=CONSTCHAR;
	//else// if(sym.symbol=="char")
	//	t=CONSTINT;
	//idName=sym.symbol;
	if(t==CONSTCHAR)
		mySymbolTable.addNewItem(nowProcNum,idName,t,constval[1]);
	else
		mySymbolTable.addNewItem(nowProcNum,idName,t,atoi(constval.c_str()));//Ҳ������constValue��û�ж��һ���ַ����������³�����case���õ�ʱ��һ�¡�
	int tempSize=mySymbolTable.tableContext[nowProcNum].itemList.size();
	mySymbolTable.tableContext[nowProcNum].itemList[tempSize-1].isnotconst=false;
	//myMiddleCode.addMiddleCode("ConstDefine",idName,"","");
	//sym=myLexer.getSym();


	//constant();
}

//<����>
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
		if(sym.kind==22)//Ҫ�����𣿣�������������������������������
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
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�������ֲ����϶���ķ���");
			return "";
			//return what??wrong
		}
	}
}


/*
//����
void Parser::constant()
{
	SymbolTableItem newItem;
	if(sym.kind==23)
	{
		newItem.kind=CONSTCHAR;
		newItem.value=0;
		
		//���崦�����ű��м�����һ��������constchar;						����ӦֵΪ��
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
		//���崦��,����
		sym=myLexer.getSym();
	}
}*/

//����˵������
void Parser::varDes()//���Ͳ�ƥ�������??????????????�ڸ�ֵʱ����
{
	if(sym.symbol!="var")//����ɾ��
	{
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵�����ֿ�ʼû��var");
		//error
	}
	sym=myLexer.getSym();
	varDefine();
	if(sym.symbol!=";")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵������ȱ��;");
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
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵������ȱ��;");
		}
		else
		{
			sym=myLexer.getSym();
		}
	}
}

//����˵��
void Parser::varDefine()
{
	int start,end;//������������ʱ�ļ���
	int tempdim;//������¼���������ά��
	start=mySymbolTable.tableContext[nowProcNum].itemList.size();
	if(sym.kind==21)
	{
		idName=sym.symbol;
		mySymbolTable.addNewItem(nowProcNum,idName,ERROR,0);
		//myMiddleCode.addMiddleCode("VarDefine",idName,"","");
		sym=myLexer.getSym();
		//������ű�
	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵����ʼ���Ǳ�ʶ��");
		return;
	}
	//sym=myLexer.getSym();
	while(sym.symbol==",")//���У����б�ʶ����ע����var���б�ʶ�������У�����ʽ����ͬ
	{
		sym=myLexer.getSym();
		//constDefine();
		if(sym.kind==21)
		{
			idName=sym.symbol;
			mySymbolTable.addNewItem(nowProcNum,idName,ERROR,0);
			//myMiddleCode.addMiddleCode("VarDefine",idName,"","");
			sym=myLexer.getSym();
			//������ű�
		}
		else
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵����ʼ���Ǳ�ʶ��");
			return;
		}
	}
	if(sym.symbol!=":")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵��ȱ��:");

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
		//��������
		sym=myLexer.getSym();
	}
	else if(sym.symbol=="array")
	{
		sym=myLexer.getSym();
		if(sym.symbol!="[")
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵����������ȱ��[");
		}
		sym=myLexer.getSym();
		if(sym.kind==22)
		{
			//mySymbolTable.arrayList.push_back(atoi(sym.symbol.c_str()));
			tempdim=atoi(sym.symbol.c_str());
			//��������
			sym=myLexer.getSym();
		}
		else
		{
			//δ����ά��
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵����������û��˵��ά��");
		}
		if(sym.symbol!="]")
		{
			//error//û��]
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵����������ȱ��]");
		}
		else
		{
			sym=myLexer.getSym();
		}
		if(sym.symbol!="of")
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵����������ȱ��of");
		}
		else
		{
			sym=myLexer.getSym();
		}
		if(isbaseType(sym.symbol))
		{
			//���崦����������

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
			//��������
			sym=myLexer.getSym();//�Ƿ���������
		}
		else
		{
			//errorû������
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵�����鶨��û�л�������");
		}


	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵��û������");

	}
}

//����˵������
void Parser::function(int lastProcNum)
{
	//functionFlag=false;
	functionStart(lastProcNum);
	//mySymbolTable.tableContext[lastProcNum].functionReturnFlag=false;
	block();
	//if(mySymbolTable.tableContext[lastProcNum].functionReturnFlag==false)
	//{
	//	//error �����޷���ֵ
	//	std::cout<<mySymbolTable.tableContext[lastProcNum].procKind<<endl;
	//	printf("no return\n");
	//}
	//sym=myLexer.getSym();
	if(sym.symbol!=";")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵������ȱ��;");
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
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵������ȱ��;");
		}
		//else����²����ٶ���һ�������ܽ�����©д,����ʽ��������error�ĵط�����Ҫ�޸ģ�������������������������������
		else
		{
			sym=myLexer.getSym();
		}
	}*/
}


//�����ײ�
void Parser::functionStart(int lastProcNum)
{
	if(sym.symbol!="function")//�˾���ʵ����ɾ��
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ײ�ȱ��function");
	}
	sym=myLexer.getSym();
	if(sym.kind==21)
	{
		//���²���Ҳ�����ڽ���procedure����ǰִ��
		idName=sym.symbol;
		mySymbolTable.addNewItem(lastProcNum,idName,ERROR,0);//error��ʾδȷ�����͡�Ҳ�������趨Ĭ������int��������������ʱ�ģ�����
		mySymbolTable.addNewContext(idName,ERROR,lastProcNum);
		//lastFunc=idName;
		nowProcNum++;
		int tempSize=mySymbolTable.tableContext[lastProcNum].itemList.size();
		mySymbolTable.tableContext[lastProcNum].itemList[tempSize-1].adr=nowProcNum;//������ڵ�ַ������
		//������ű�,ProcNone
		sym=myLexer.getSym();
		//������ű�
	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ײ���ʼȱ�ٱ�ʶ��");
	}
	if(sym.symbol!="(")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ײ�ȱ��(");
	}
	else
	{
		sym=myLexer.getSym();
	}
	if(sym.symbol=="var"||sym.kind==21)//���п��޵���ʽ������
	{
		argTable();//��ʽ������
	}
	//sym=myLexer.getSym();
	if(sym.symbol!=")")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ײ�ȱ��)");
	}
	else
	{
		sym=myLexer.getSym();//�Ƿ�Ҫ����else����д����if(sym.symbol==")"��������һ��	����ٸģ���������������������������������������������
	}
	if(sym.symbol!=":")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ײ�ȱ��:");
		//return;
	}
	else
	{
		sym=myLexer.getSym();//�Ƿ�Ҫ����else����д����if(sym.symbol==")"��������һ��	����ٸģ���������������������������������������������
	}
	if(isbaseType(sym.symbol))//����һ���и����Ͱ�������������
	{
		if(sym.symbol=="integer")
			t=PROCINT;
		else// if(sym.symbol=="char")
			t=PROCCHAR;
		mySymbolTable.tableContext[nowProcNum].procKind=t;

		//�������������һ���������趨һ������������������������������������������������������������������������������������������������������������*****************�����Ⱑ��������
		//���ֶ�Ҫ��һ��!!!!!!!!!!!!!!


		//SymTableContext tempContext;

		//int tempProc=mySymbolTable.tableContext[nowProcNum].outproc;
		//int changeProcNum=mySymbolTable.getCurProcIndex(tempProc,mySymbolTable[nowProcNum].ProcName);
		//mySymbolTable.tableContext[tempProc].itemList[changeProcNum].kind=t;


		int tempProc=mySymbolTable.tableContext[nowProcNum].outproc;
		int tempSize=mySymbolTable.tableContext[tempProc].itemList.size();
		mySymbolTable.tableContext[tempProc].itemList[tempSize-1].kind=t;
		
		//�������
		sym=myLexer.getSym();
	}
	else
	{
		//error������Ĭ������
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ײ�û�����û�������");
	}
	if(sym.symbol!=";")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ײ�ȱ��;");
	}
	else
	{
		sym=myLexer.getSym();
	}
	
}


//����˵������
void Parser::procedure(int lastProcNum)
{
	procedureStart(lastProcNum);
	block();
	//sym=myLexer.getSym();
	if(sym.symbol!=";")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵������ȱ��;");
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
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����˵������ȱ��;");
		}
		//else����²����ٶ���һ�������ܽ�����©д,����ʽ��������error�ĵط�����Ҫ�޸ģ�������������������������������
		else
		{
			sym=myLexer.getSym();
		}
	}*/
}


//�����ײ�
void Parser::procedureStart(int lastProcNum)
{
	if(sym.symbol!="procedure")//�˾���ʵ����ɾ��
	{
		//error�����ײ�ȱ��function
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ײ�ȱ��function");
	}
	sym=myLexer.getSym();
	if(sym.kind==21)
	{
		//���²���Ҳ�����ڽ���procedure����ǰִ��
		idName=sym.symbol;
		mySymbolTable.addNewItem(lastProcNum,idName,PROCNONE,0);

		mySymbolTable.addNewContext(idName,PROCNONE,lastProcNum);
		//lastFunc=idName;
		nowProcNum++;
		int tempSize=mySymbolTable.tableContext[lastProcNum].itemList.size();
		mySymbolTable.tableContext[lastProcNum].itemList[tempSize-1].adr=nowProcNum;//������ڵ�ַ������
		//������ű�,ProcNone
		sym=myLexer.getSym();
	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ײ���ʼȱ�ٱ�ʶ��");

	}
	if(sym.symbol!="(")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ײ�ȱ��(");
	}
	else
	{
		sym=myLexer.getSym();
	}
	if(sym.symbol=="var"||sym.kind==21)//���п��޵���ʽ������
	{
		argTable();//��ʽ������
	}
	//sym=myLexer.getSym();
	if(sym.symbol!=")")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ײ�ȱ��)");
	}
	else
	{
		sym=myLexer.getSym();//�Ƿ�Ҫ����else����д����if(sym.symbol==")"��������һ��	����ٸģ���������������������������������������������
	}
	if(sym.symbol!=";")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ײ�ȱ��;");

	}
	else
	{
		sym=myLexer.getSym();
	}
}

//��ʽ������
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

//��ʽ������
void Parser::arg(bool tempflag)
{
	int start,end;//������������ʱ�ļ���
	start=mySymbolTable.tableContext[nowProcNum].itemList.size();
	if(sym.symbol=="var")//����ַ������ļ���������Ӧ��isnotadr����Ϊfalse��������ǰ������һ��boolֵ��Ĭ��Ϊtrue��Ȼ��������Ըı�Ϊfalse�����а���var��ͷӰ�쵽����
	{
		tempflag=false;
		sym=myLexer.getSym();
	}
	if(sym.kind==21)
	{
		idName=sym.symbol;
		mySymbolTable.tableContext[nowProcNum].procArgNum++;//�Ѿ���ʼ��Ϊ0
		mySymbolTable.addNewItem(nowProcNum,idName,ERROR,0);
		sym=myLexer.getSym();
		//���崦������ű�
	}
	else
	{
		//error ȱ�ٱ�ʶ��
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��ʽ������ȱ�ٱ�־��");
	}
	while(sym.symbol==",")
	{
		sym=myLexer.getSym();
		if(sym.kind==21)
		{
			idName=sym.symbol;
			mySymbolTable.tableContext[nowProcNum].procArgNum++;//�Ѿ���ʼ��Ϊ0
			mySymbolTable.addNewItem(nowProcNum,idName,ERROR,0);
			sym=myLexer.getSym();
			//���崦������ű�
			//���崦��
		}
		else
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��ʽ�����ο�ʼ���Ǳ�ʶ��");
		}
		//sym=myLexer.getSym();						���������������������
	}
	if(sym.symbol!=":")
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��ʽ������ȱ��:");
	}
	else
	{
		sym=myLexer.getSym();
	}
	if(isbaseType(sym.symbol))
	{
		//��������
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
		//����
	}
	else
	{
		//error����������Ĭ������Ϊinteger
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��ʽ������ȱ�ٻ������Ͷ���");
	}
	//sym=myLexer.getSym();
}

//�������
void Parser::complexSentense(int lastProcNum)
{
	if(sym.symbol!="begin")//�˾���ʵ����ɾ��
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"������俪ʼû��begin");
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
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��������βû��end���߸�������м�û�зֺ�");
	}
	else
	{
		sym=myLexer.getSym();
	}
}


//���
void Parser::statement(int lastProcNum)
{
	if(sym.kind==21)
	{

		//���ű��жϣ����ǿ���û���������������̵��û��߸�ֵ��䡢����1.���ű�2.�����У�=���ߣ���


		//����ű�ʱ������ظ����壿��������ڲ㣺һ���ǹ��̣�һ���Ǳ���

		//������Ԥ������������

		//Ԥ�������ַ�������:=����(
		string tempsym;
		tempsym=myLexer.getNextSym(1).symbol;
		if(tempsym==":="||tempsym=="["||tempsym=="=")
			becomesState(lastProcNum);
		else if(tempsym=="(")
		//callProcedure();
			callProcedure(lastProcNum);//������������̵��úϲ�
		else
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����Ա�ʶ����ͷ��ȴ���Ǹ�ֵ�������̵������");
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
		//�����������
	}
}

//��ֵ���
void Parser::becomesState(int lastProcNum)
{
	//bool leftArrayFlag=false;
	string newTemp;
	if(sym.kind!=21)//����ȥ��
	{
		//error

	}
	//���崦��
	idName=sym.symbol;							//�Ƿ���Ҫ���ⶨ��һ���ֲ���������ΪidName����expression�иı�
	sym=myLexer.getSym();
	if(sym.symbol==":="||sym.symbol=="=")//�Ƿ�Ҫ�������=�Ĵ���������
	{
		//��ͨ�������ߺ�����
		//���
		kindType temptype=mySymbolTable.canWrite(lastProcNum,idName);
		if(temptype==ERROR||temptype==ARRAYCHAR||temptype==ARRAYCHAR)
		{
			//error
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"��ֵ����е�ʽ��߱���δ����|���Ǳ���|�ǹ��̱���|������Ԫ�ص�����û��[");
		}
		
	//	else
		{
			if(sym.symbol=="=")
			{
				myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��ֵ����forѭ�����ȱ��:=");//��������ͼ��:=д��=�Ĵ������
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
		//���崦��ʱǧ��ע�⣬����Ǻ������͵ĸ�ֵ��Ҫ��Ϊ����ֵ�������������������ɴ����в�����д���м����һ��������ʱ���ж�!!!!!!!!!!!!!!!����ʱ���������Ż������������������Ա����Ŀ��Զ�д��Ҫ����ж�
	}
	else if(sym.symbol=="[")
	{
		//�������
		//�����ű�
		kindType temptype=mySymbolTable.canWrite(lastProcNum,idName);
		if(!(temptype==ARRAYINT||temptype==ARRAYCHAR))
		{
			int i;
			i=1;
			//error
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"��ֵ����б�ʶ��������]����������Ԫ��");
		}
		//[]=	B	k	t1
		//	=	2	t1

			//leftArrayFlag=true;
			sym=myLexer.getSym();
			newTemp=getNewTemp(lastProcNum,false);			//��֪�����Ƿ���Ч����



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
						myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"�����±�Խ��");
				}
			}

			myMiddleCode.addMiddleCode("[]=",idName,tempExpression,newTemp);				//����������ò���������������������������������������
			//myMiddleCode.addMiddleCode("+",preTemp,nowTemp,getNewTemp());

		//�ڷ��ű��������¼ӵ�newTempΪisnotadrΪflase;

		if(sym.symbol!="]")
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����������ȱ��]");
		}
		else
		{
			sym=myLexer.getSym();
		}
		if(sym.symbol!=":=")
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��ֵ���ȱ��:=");
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
		//myError.parserError(myLexer.x(),myLexer(),"��ֵ���ȱ��:=��������û��[]���");
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��ֵ���ȱ��:=��������û��[]���");
	}
}

//���ʽ
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
	//getnowTemp��������� ֻ������ʱ�Ĵ����������
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


//��
string Parser::term(int lastProcNum)
{
	string preTemp;//��Ϊȫ�ֱ����ɷ񣿣�������������������������������������������
	string nowTemp;
	string fh;
	preTemp=factor(lastProcNum);
	while(isMul(sym.symbol))
	{
		fh=sym.symbol;
		sym=myLexer.getSym();

		if(fh=="/"&&sym.symbol=="0")//�ʷ������Ѿ��ָ�//&&myLexer.getNextSym(1).kind!=22)
		{
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"����������0");
		}

		nowTemp=factor(lastProcNum);
		myMiddleCode.addMiddleCode(fh,preTemp,nowTemp,getNewTemp(lastProcNum));
		preTemp=getNowTemp();
	}
	return preTemp;
}

//����,���ﴦ��о���Щ�Ѷ�
string Parser::factor(int lastProcNum)
{
	string preTemp="";
	if(sym.kind==21)
	{
		string tempsym;
		kindType tempType=mySymbolTable.canRead(lastProcNum,sym.symbol);
		if(tempType==ERROR)//���ű�Ӧ�ö��������м�飬������bug
		{
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"����ʹ��û�ж��塢���ܶ�ȡ�ı�ʶ��");
		}
		//else
		//{
			//�Ƿ�Ӧ�ý������if elseif else����������棿�������������������������������߾��Ǳ�һ�´������������ܼ���ִ�У�����
		//}
		//Ŀǰ�����ھͱ�һ�´�

		//	myError.sentenceError(myLexer.x(),myLexer.y(),"����δ�����ʹ��");
		tempsym=myLexer.getNextSym(1).symbol;//Ԥ��һ���ַ�*****************************************
		if(tempsym=="(")//�����������
		{
			if(mySymbolTable.isProcRead(lastProcNum,sym.symbol)==ERROR)
			{
				myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"����ʹ�ô���");

			}
			//else
			{
				preTemp=callFunction(lastProcNum);
			}
		}
		else if(tempsym=="[")//����
		{
			string tempidName;
			kindType tempType=mySymbolTable.canUse(lastProcNum,sym.symbol);
			if(tempType==ARRAYCHAR||tempType==ARRAYINT)
			{
				tempidName=sym.symbol;		
				sym=myLexer.getSym();//��ʱsym����[
				sym=myLexer.getSym();//��ʱsym�в��Ǳ��ʽ�ĵ�һ��
				//t1=A[k]	=[]	B	k a
				//BB string newTemp=getNewTemp(lastProcNum);

				//BB int tempSize=mySymbolTable.tableContext[lastProcNum].itemList.size();
				//BB mySymbolTable.tableContext[lastProcNum].itemList[tempSize-1].isnotadr=false;
				//AA  myMiddleCode.addMiddleCode("=[]",tempidName,expression(lastProcNum),newTemp);//�Ƿ���Բ�����ʱ������ֱ����sym.symbol,����newtemp��tempidName������������

				//BB myMiddleCode.addMiddleCode("[]=",tempidName,expression(lastProcNum),newTemp);//�Ƿ���Բ�����ʱ������ֱ����sym.symbol,����newtemp��tempidName������������
				

				string newTemp=getNewTemp(lastProcNum,false);			//��֪�����Ƿ���Ч����
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
							myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"�����±�Խ��");
					}
				}

				myMiddleCode.addMiddleCode("[]=",tempidName,tempExpression,newTemp);				//����������ò���������������������������������������

				//�Ƿ�����call����ʱ������ʱ��ȡ����3�����⴦����������������������������������������
				
				preTemp=newTemp;

				if(sym.symbol!="]")
				{
					//error�����������ȱ��]
					myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����������ȱ��]");
				}
				else
				{
					sym=myLexer.getSym();
				}
			}
			else
			{
				//errorû������Ķ���ʹ�ô���
				myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"����ʹ��û�ж��������");
			}
		}
		else//��������������ӷ���
		{
			//int,char,procint,procchar,constchar,constint�����ԣ������Ѿ��ų��õ���Щ���
			preTemp=sym.symbol;
			sym=myLexer.getSym();
		}
	}
	else if(sym.symbol=="(")//(���ʽ)�����
	{
		sym=myLexer.getSym();
		preTemp=expression(lastProcNum);
		if(sym.symbol==")")
		{
			sym=myLexer.getSym();
		}
		else
		{
			//error���ӣ�<���ʽ>)ȱ�٣�
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����(<���ʽ>)ȱ��)");
		}
	}
	else if(sym.kind==22)//���ֵĴ���ʽ��֪�Ƿ���ȷ
	{
		preTemp=sym.symbol;
		sym=myLexer.getSym();
	}



	else if(sym.kind==23)				//�ַ�û�п��ǣ�������������������������������������
	{
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����в�Ӧ���ַ�");
		preTemp=sym.symbol;
		sym=myLexer.getSym();
	}
	else
	{
		//error ����ʶ�������
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����ʶ�������");
	}
	return preTemp;
}

//�����������
string Parser::callFunction(int lastProcNum)
{
	string re="";//����ɾ�����践�صĵط�ֱ�ӷ��ؼ��ɡ�����ɾ
	string funcName;
	if(sym.kind==21)//����Ҫ�ɣ�����������������������������������ǰ���Ѿ������жϣ���������
	{
		kindType tempType;
		tempType=mySymbolTable.isProcRead(lastProcNum,sym.symbol);
		if(tempType==ERROR||tempType==PROCNONE)
		{
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),sym.symbol+"����������ȷ|Ӧ���Ǻ�����������ȴ�ǹ���");
		}
		//else
		{
			funcName=sym.symbol;
			sym=myLexer.getSym();
		}
	}
	else
	{
		//error	����ȱ�ٱ�ʶ��
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��������ȱ�ٱ�ʶ��");
	}
	if(sym.symbol=="(")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//error	��������ȱ�٣�
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��������ȱ�٣�");//ע������Ĵ�����û�м������¶��ַ�����������������������
		return "";
	}
				//ע�⣺�����ķ����������ʵ�ڲ��������������Ҵ���!!����Ϊ�յģ�ע�⣺[]��������ָ���п��ޡ�
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
		if(valueTable(lastProcNum,tempIndex,funcName)!=mySymbolTable.tableContext[tempIndex].procArgNum)//������ʽ������ֻȡǰ����������ɾȥ�󼸸�value=��䣿��������զ�죿��
		{
				myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"������Ŀ����ȷ");
		}
		
		//if(mySymbolTable.isProc(funcName)==PROCNONE)				//�Ƿ�Ҫ�ֿ�д����������������������������������������
			//myMiddleCode.addMiddleCode("Call",funcName,"","");
		//else
		//{
			//myMiddleCode.addMiddleCode("CallValue",funcName,"",getNewTemp(lastProcNum));
			myMiddleCode.addMiddleCode("CallValue",strtempIndex,"",getNewTemp(lastProcNum));
			re=getNowTemp();											//��ʱ�����ܷ��أ����У�Ҫ�ж�
		//}
		if(sym.symbol==")")
		{
			sym=myLexer.getSym();
		}
		else
		{
			//error	��������ȱ��)
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��������ȱ��)");
			return "";
		}
	}
	return re;
}


//���̵������
string Parser::callProcedure(int lastProcNum)//�õ��ú����ĵط����ù��̣���Ҫ����
{
	string re="";//����ɾ�����践�صĵط�ֱ�ӷ��ؼ��ɡ�
	string funcName;
	if(sym.kind==21)//����Ҫ�ɣ�����������������������������������ǰ���Ѿ������жϣ���������
	{
		kindType tempType;
		tempType=mySymbolTable.isProcRead(lastProcNum,sym.symbol);
		if(!(tempType==PROCNONE))
		{
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),sym.symbol+"����������ȷ|Ӧ���ǹ��̣�������ȴ�Ǻ���");			//��ʵ������֮ǰ�ĵ���ʱ�жϣ�������һ������ʵ��
		}
		else
		{
			funcName=sym.symbol;
			sym=myLexer.getSym();
		}

	}
	else
	{
		//error	����ȱ�ٱ�ʶ��
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"���̵���ȱ�ٱ�ʶ��");
	}
	if(sym.symbol=="(")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//error	��������ȱ�٣�
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"���̵���ȱ�٣�");//ע������Ĵ�����û�м������¶��ַ�����������������������
		return "";
	}
					//ע�⣺�����ķ����������ʵ�ڲ�������������
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

		if(valueTable(lastProcNum,tempIndex,funcName)!=mySymbolTable.tableContext[tempIndex].procArgNum)//������ʽ������ֻȡǰ����������ɾȥ�󼸸�value=��䣿��������զ�죿��
		{
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"������Ŀ����ȷ");
		}

		//if(mySymbolTable.isProc(funcName)==PROCNONE)				//�Ƿ�Ҫ�ֿ�д����������������������������������������
		//myMiddleCode.addMiddleCode("Call",funcName,"","");
		myMiddleCode.addMiddleCode("Call",strtempIndex,"","");
		//else
		//{
		//myMiddleCode.addMiddleCode("CallValue",funcName,"",getNewTemp());
		//re=getNowTemp();											//��ʱ�����ܷ��أ����У�Ҫ�ж�
		//}
		if(sym.symbol==")")
		{
			sym=myLexer.getSym();
		}
		else
		{
			//error	��������ȱ��)
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"���̵���ȱ��)");
			return "";
		}
	}
	return re;
}




//ʵ�ڲ�����
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
	int temp=-1;//������֪�Ƿ���ȷ��������ͬ�����������������⣿����
	int flagflag;
	do{																//����һ�ֶ�A{��A}���ķ��Ĵ���ʽ
		tempstring = expression(lastProcNum);
		if(temp==-1)
		{
			myMiddleCode.addMiddleCode("Display",strlastProcNum,strProcIndex,"");
		}
		myMiddleCode.addMiddleCode("value=",tempstring,mySymbolTable.getProcArg(ProcIndex,procName,++temp),strProcIndex);


		//�������ϣ���ȷ�Դ�����	�Ż�����ܲ��ᱨ����			//ע�⣺�����Ѿ���~����
		if(temp<=mySymbolTable.tableContext[ProcIndex].procArgNum&&mySymbolTable.tableContext[ProcIndex].itemList[temp].isnotadr==false&&tempstring[0]=='#')
		{
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"����ַ�ββ����Ǹ��ӱ��ʽ");
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





//�������
void Parser::ifState(int lastProcNum)
{
	string preFlag1,preFlag2;
	if(sym.symbol!="if")//����ȥ��
	{
		//error
	}
	sym=myLexer.getSym();


	if(sym.symbol=="(")
	{
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"���������������(");//��һ�䲻֪�ܲ��ܷ��ڴ������棿��
		sym=myLexer.getSym();
	}
	//else
	//{
		//error	�������ȱ��(
		//myError.parserError(myLexer.x(),myLexer.y(),"�������ȱ��(");
	//}
	preFlag1=getNewFlag();
	myMiddleCode.addMiddleCode("IfFalse",condition(lastProcNum),preFlag1,"");
	//preFlag1=getNowFlag();
	//condition()??????????????
	//if(sym.symbol==")")
	//{
		//	myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��������������ˣ�");���ҷ���һ�䣿������
		//sym=myLexer.getSym();
	//}
	//else
	//{
		//error	�������ȱ��)
		//myError.parserError(myLexer.x(),myLexer.y(),"�������ȱ��)");
	//}
	if(sym.symbol=="then")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//error	if�����ȱ��then
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�������ȱ��then");
	}
	statement(lastProcNum);
	//����������Ż�			����else�����پ�������Щ����
	//������Ƿ�����Ż����������Ż�����ά��������ƣ�����������ɵĴ����Ҫ�������
	preFlag2=getNewFlag();
	myMiddleCode.addMiddleCode("JmpTo",preFlag2,"","");//�Ż�ʱ�������û��else�����һ��Ҫɾ���������������������������ͺ���Ľ�����һ�𣡣�������һ���Ż����Ż�ǰ�����ԱȰ�
	//preFlag2=getNowFlag();
	myMiddleCode.addMiddleCode("SetFlag",preFlag1,"","");
	if(sym.symbol=="else")
	{
		sym=myLexer.getSym();
		statement(lastProcNum);
	}//����else�Կɣ����ܱ���
	myMiddleCode.addMiddleCode("SetFlag",preFlag2,"","");
}

//����
string Parser::condition(int lastProcNum)
{
	string preTemp;
	string op;
	preTemp=expression(lastProcNum);
	if(isCompare(sym.symbol))				//�����Ǳ��ʽ���������<��ϵ�����>��Ҳ����û�У���������
	{
		string newTemp;
		op=sym.symbol;
		sym=myLexer.getSym();
		newTemp=getNewTemp(lastProcNum);			//��֪�����Ƿ���Ч����
		myMiddleCode.addMiddleCode(op,preTemp,expression(lastProcNum),newTemp);
		preTemp=newTemp;
	}
	else//�Ȳ�д����������������������������������������
	{
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"������û�й�ϵ�����");
		//error????????????????????????????????????????????????????????????/warning����������������������ʵ�ʵ�����������������ֻ�б��ʽ��û�й�ϵ������������//��warningһ��
	
	}
	return preTemp;							//����1,0����0�Ķ�����1���������ｻ����ദ��
}



//������
void Parser::caseState(int lastProcNum)
{
	if(sym.symbol!="case")//����ȥ��
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����俪ʼȱ��case");
	}
	string target;
	string flag=getNewFlag();
	//string smallflag;
	sym=myLexer.getSym();
	target=expression(lastProcNum);
	if(sym.symbol!="of")
	{
		//errorû��of
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����俪ʼȱ��of");
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
		//error ������ȱ��end
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"������ȱ��end");
		return;//����Ƿ����ɾ������������������������������������
	}
}


//�����Ԫ��
void Parser::situationTable(int lastProcNum,string target,string flag)//,string smallflag)
{
	//̫��д�ˣ������������������������������������������尡
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
		//error ��������ȱ��:
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��������ȱ��:");
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

//forѭ�����
void Parser::forState(int lastProcNum)
{
	if(sym.symbol!="for")//����ȥ��
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"forѭ����俪ʼȱ��for");
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
		if(temptype==ERROR||temptype==ARRAYCHAR||temptype==ARRAYCHAR)//for����б�ʶ���ɷ��Ǻ�����ʶ����������������������������
		{
			//error
			myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"forѭ������б�ʶ��δ������߲��Ǳ�������������Ԫ��");
		}
		else
		{
			temp=sym.symbol;
			sym=myLexer.getSym();//�Ƿ�Ҫ����else���棿������������������������������
			//myMiddleCode.addMiddleCode(":=",expression(lastProcNum),"",idName);
		}
		//�����ű�
		//temp=sym.symbol;
		//myMiddleCode.addMiddleCode(":=",,"","");
		//sym=myLexer.getSym();
	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"forѭ����俪ʼȱ�ٱ�ʶ��");
	}
	if(sym.symbol==":=")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//errror
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"��ֵ����forѭ�����ȱ��:=");
	}
	tempStart=expression(lastProcNum);
	myMiddleCode.addMiddleCode(":=",tempStart,"",temp);
	if(sym.symbol=="to")//�ɷ�ͨ��tempStart��tempEnd�ıȽ��������to/downtoһ��ָ�����󣿣�������������������������������
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
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"forѭ�����ȱ��to/downto");
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
		//errror	for��ȱ��do
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"forѭ�����ȱ��do");
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



//�����
void Parser::readState(int lastProcNum)
{
	if(sym.symbol!="read")//����ȥ��
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"����俪ʼȱ��read");

	}
	int flagflag;
	sym=myLexer.getSym();
	if(sym.symbol=="(")
	{
		sym=myLexer.getSym();
	}
	else
	{
		//error	�����ȱ��(
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ȱ��(");
		return;//�����ͽ����ˣ�������������������������������������������������
	}
	do 
	{
		if(sym.kind==21)
		{
			kindType temptype=mySymbolTable.canWrite(lastProcNum,sym.symbol);
			if(!(temptype==INT||temptype==CHAR))
			{
				//error
				myErrorHandler.sentenceError(myLexer.x(),myLexer.y(),"δ�������ǳ����������鵫���ܸ�ֵ");
			}
			//else?????????????????????????????????????
			myMiddleCode.addMiddleCode("Read",sym.symbol,"","");

			//if(mySymbolTable.canWrite(nowProcNum,sym.symbol)==ERROR)
			//	myError.sentenceError(myLexer.x(),myLexer.y(),"δ�������ǳ���");
			sym=myLexer.getSym();
		}
		else
		{
			//error
			myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ȱ�ٱ�ʶ��");
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
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"�����ȱ��)");
		return;
	}
	//ע�⣺��βû��/;
}




//д���
void Parser::writeState(int lastProcNum)
{
	if(sym.symbol!="write")//����ȥ��
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"д��俪ʼȱ��write");
	}
	sym=myLexer.getSym();
	if(sym.symbol=="(")
	{
		sym=myLexer.getSym();
		
	}
	else
	{
		//error
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"д���ȱ��(");
		return;//�ǲ��Ǳ�ĵط�����ҲҪ������������
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
		//error	д��� ȱ��)
		myErrorHandler.parserError(myLexer.x(),myLexer.y(),"д���ȱ��)");
		return;//�Ƿ���Ҫ��������
	}
}


/*
//��ȡ��ȡ[]=�Ľ��������Ԫ�ص�ַ������ʱ����
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


//��ȡ�µ���ʱ����
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

//�õ���ǰ��ʱ����
string Parser::getNowTemp()
{
	string temp;//�ɲ�����д��temp=string(tempNum);
	strstream ss;
	ss<<tempNum;
	ss>>temp;
	return "#t"+temp;
}
//��ȡ�µ���ת��
string Parser::getNewFlag()
{
	string temp;
	strstream ss;
	ss<< ++flagNum;
	ss>>temp;
	return "@flag_flag"+temp;
}
//�õ���ǰ��ת��
string Parser::getNowFlag()
{
	string temp;
	strstream ss;
	ss<<flagNum;
	ss>>temp;
	return "@flag_flag"+temp;
}
//��ȡ�µ��ַ������
string Parser::getNewString()
{
	string temp;
	strstream ss;
	ss<<++stringNum;
	ss>>temp;
	return "string"+temp;
}
//�õ���ǰ�ַ������
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


