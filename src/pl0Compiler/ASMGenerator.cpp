#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ASMGenerator.h"
#include "GlobalData.h"
#include "SymbolTable.h"
#include <iostream>
#include <strstream>
#include <string>

using namespace std;

string ASMGenerator::prefix="C:\\masm32\\lib\\";
//string ASMGenerator::prefix="";
int ASMGenerator::codePoint1=0;
int ASMGenerator::codePoint2=0;
int ASMGenerator::judgeFlagNum=0;
int ASMGenerator::judgeArrayFlagNum=0;

void ASMGenerator::setObjectfileName(char s[])
{
	strcpy(objectfileName,s);
}

void ASMGenerator::init()
{
	//freopen("E:\\ProgramData\\Compiler\\test10_ASM.asm","w",stdout);
	freopen(objectfileName,"w",stdout);
	printf("\t.386\n");
	printf("\t.model flat,stdcall\n\n");
	printf("option\tcasemap:none\n");
	cout<<"includelib\t"<<prefix<<"msvcrt.lib"<<endl;
	//printf("includelib\tmsvcrt.lib\n");
	printf("printf\tproto C:dword,:vararg\n");
	printf("scanf\tproto C:dword,:vararg\n");
	printf("\t.STACK\t4096*16\n\n");
	while(myMiddleCode.codeList[codePoint2].opr!="End"||myMiddleCode.codeList[codePoint2].target1!="0")
		++codePoint2;
}



void ASMGenerator::generate()
{
	init();
	data();
	code();

}

void ASMGenerator::data()
{
	int i;

	printf("\t.DATA\n");
	printf("inint \tDB \"%%d\",0\n");
	printf("outint \tDB \"%%d\",0DH,0AH,0\n");
	//printf("inchar \tDB \" %%c\",0\n");
	printf("inchar \tDB \" %%c\",0\n");
	printf("outchar \tDB \"%%c\",0DH,0AH,0\n");
	printf("arrayerror \tDB \"array out of range\",0DH,0AH,0\n");
	//��ӡ�ַ���
	for (i=0;i<mySymbolTable.printString.size();i++)
	{
		printf("string%d\tDB ",i+1);
		cout << mySymbolTable.printString[i] << ",0DH,0AH,0" << endl;
	}
	cout << endl;
}

void ASMGenerator::procStart(int index)
{
	int i;
	if(index==0)
	{
		printf("\nstart:\n");
	}
	else
	{
		printf("\nid%d_%s\tPROC near32\n",index,mySymbolTable.tableContext[index].procName.c_str());
	}


	printf("push\tebp\n");
	printf("mov\tebp,esp\n");
	//printf("sub\tesp,%d\n",mySymbolTable.dimCount(index,mySymbolTable.tableContext[index].itemList.size())*4+4);//���Ȳ��ܺ������ǹ��̣�һ����1���ռ䱣�淵��ֵ��֮��Ҫ�Ż�������

	//printf("sub\tesp,%d\n",mySymbolTable.dimCount(index,mySymbolTable.tableContext[index].itemList.size())*4);//���Ȳ��ܺ������ǹ��̣�һ����1���ռ䱣�淵��ֵ��֮��Ҫ�Ż�������
	printf("sub\tesp,%d\n",mySymbolTable.dimCount(index,mySymbolTable.tableContext[index].itemList.size())*4+4);//���Ȳ��ܺ������ǹ��̣�һ����1���ռ䱣�淵��ֵ��֮��Ҫ�Ż���������ֹ��char����
	//printf("push\tebx\n");
	//printf("push\tesi\n");
	//printf("push\tedi\n");
	for(i=1;i<=mySymbolTable.tableContext[index].procArgNum;i++)				//�����ˣ�Ҫ�ж��Ǵ��λ��Ǵ���ַ��������
	{
		//if(mySymbolTable.)���ܴ��λ��Ǵ�ֵ���������С������Ĺ��̵ģ���


		printf("mov\teax,dword ptr [ebp+%d]\n",(mySymbolTable.tableContext[index].procArgNum-i)*4+8);//���Ĳ���ֻ���ǻ������ͣ����������飬����û��dimCount������ά��
		printf("mov\tdword ptr ");
		cout<<"[ebp-"<<4*i<<"]"<<",eax"<<endl;
	}

	//constDefine��α��氡
	
	for(i=0;i<mySymbolTable.tableContext[index].itemList.size();i++)
	{
		if(mySymbolTable.tableContext[index].itemList[i].isnotconst==false)
		{
			printf("mov\tdword ptr [ebp-%d],%d\n",4*(i+1),mySymbolTable.tableContext[index].itemList[i].value);//ֻ�����Ǳ��㣬�ǵ�ַ��
			//string temp;
			//strstream ss;
			//ss<<mySymbolTable.tableContext[index].itemList[i].value;
			//ss>>temp;

			//symbol(index,temp,"eax");
			//symbolAdrEqual(index,mySymbolTable.tableContext[index].itemList[i].name,"esi","eax");

		}
	}
}

void ASMGenerator::procEnd(int index)
{
	//printf("pop\tedi\n");
	//printf("pop\tesi\n");
	//printf("pop\tebx\n");
	printf("mov\tesp,ebp\n");
	printf("pop\tebp\n");
	printf("ret\n");

	if(index==0)
	{
		printf("\nend\tstart\n");
	}
	else
	{
		printf("\nid%d_%s\tENDP\n",index,mySymbolTable.tableContext[index].procName.c_str());
	}

	//mov eax,Ҫ�з���ֵ������������������������������������������������������������������������������������������
}

void ASMGenerator::WriteString(string s)
{
	cout<<"lea\tecx,"<<s<<endl;
	cout<<"push\tecx"<<endl; 
	cout<<"call\tprintf"<<endl;
	cout<<"add\tesp,4"<<endl;
}



void ASMGenerator::WriteExpression(int procIndex,string s)
{
	symbol(procIndex,s,"eax");
	kindType tempType=mySymbolTable.canUse(procIndex,s);
	if(tempType==CONSTCHAR||tempType==CHAR||tempType==PROCCHAR||tempType==ARRAYCHAR)//���治������Ԫ�ص���Ԫʽ��ת��
	{
		cout<<"push\teax"<<endl;
		cout<<"lea\teax,outchar\n";
		cout<<"push\teax"<<endl;
		cout<<"call\tprintf"<<endl;
		cout<<"add\tesp,8"<<endl;
	}
	else
	{
		cout<<"push\teax"<<endl;
		cout<<"lea\teax,outint\n";
		cout<<"push\teax"<<endl;
		cout<<"call\tprintf"<<endl;
		cout<<"add\tesp,8"<<endl;
	}
}

void ASMGenerator::Read(int procIndex,string s)
{
	//symbol(procIndex,s,"eax");
	//cout<<"xor\teax,eax"<<endl;


	//test����֮��
	//symbolAdr(procIndex,s,"eax");

	symbolAdr(procIndex,s,"esi");		//����esi�Ĵ���
	cout<<"xor\teax,eax"<<endl;

	cout<<"mov dword ptr [esi],eax"<<endl;//��0
	
	symbolAdr(procIndex,s,"eax");

	//symbolAdrByte(procIndex,s,"eax");						��ѡ��������

	kindType tempType=mySymbolTable.canUse(procIndex,s);
	if(tempType==CONSTCHAR||tempType==CHAR||tempType==PROCCHAR||tempType==ARRAYCHAR)//���治������Ԫ�ص���Ԫʽ��ת��
	{
		cout<<"push\teax"<<endl;
		cout<<"lea\teax,inchar\n";
		cout<<"push\teax"<<endl;
		cout<<"call\tscanf"<<endl;
		cout<<"add\tesp,8"<<endl;
	}
	else
	{
		cout<<"push\teax"<<endl;
		cout<<"lea\teax,inint\n";
		cout<<"push\teax"<<endl;
		cout<<"call\tscanf"<<endl;
		cout<<"add\tesp,8"<<endl;
	}
}

void ASMGenerator::arrayEqual(int procIndex,string a,string b,string c)
{
	//[]=	B	3	t1
	//lea	eax,[ebp-4-4*3]
	//ʹ��eax������������������ʱ������ecx
	//lea eax,[ebp-4];
	//mov	ecx,3
	//imul	ecx,4
	//sub	eax,ecx
	//mov [ebp-48],eax

	//string reg="eax";
	//cout<<"lea\t"<<reg<<",[ebp"
	symbolAdr(procIndex,a,"eax");//����a�ĵ�ַ��
	symbol(procIndex,b,"ecx");

	//cmp ecx,10
	//jl @array_flag1
	//lea	ecx,arrayerror
	//push	ecx
	//call	printf
	//add	esp,4
	//mov	esp,ebp
	//pop	ebp
	//ret
	//@array_flag1:
	//cmp ecx,0
	//jge @array_flag2
	//lea	ecx,arrayerror
	//push	ecx
	//call	printf
	//add	esp,4
	//mov	esp,ebp
	//pop	ebp
	//ret
	//@array_flag2:


	string tempArrayFlag1=getNewArrayJudge();
	string tempArrayFlag2=getNewArrayJudge();
	//cout<<opcmp<<"\t"<<tempFlag<<endl;

	int idProcNum=-1,idIndex=-1;
	mySymbolTable.getIdNameAdr(procIndex,a,idProcNum,idIndex);


	cout<<"cmp\tecx,"<<mySymbolTable.tableContext[idProcNum].itemList[idIndex].arraydim<<endl;
	cout<<"jl\t"<<tempArrayFlag1<<endl;
	WriteString("arrayerror");
	printf("mov\tesp,ebp\n");
	printf("pop\tebp\n");
	printf("ret\n");
	cout<<tempArrayFlag1<<":"<<endl;

	cout<<"cmp\tecx,"<<"0"<<endl;
	cout<<"jge\t"<<tempArrayFlag2<<endl;
	WriteString("arrayerror");
	printf("mov\tesp,ebp\n");
	printf("pop\tebp\n");
	printf("ret\n");
	cout<<tempArrayFlag2<<":"<<endl;


	//getNewArrayJudge()


	cout<<"imul\tecx,4"<<endl;
	cout<<"sub\teax,ecx"<<endl;

	//��������ֻ���Ǳ��㡢�ǵ�ַ������ֹ�Ż����������㡣
	/*int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,c,idProcNum,idIndex);
	if(idProcNum==procIndex&&mySymbolTable.tableContext[procIndex].itemList[idIndex].isnotadr==true)//����ǵ�ַ	��಻����Ϊ��ֵ
	{
		cout<<"mov\tdword ptr[ebp-"<<mySymbolTable.dimCount(procIndex,idIndex)*4+4<<"]"<<",eax"<<endl;//���Ż�����������Ч
	}
	else//�����ߣ���ȡ��ַ��ע��:ȡ��ַʱ�������õ�eax
	{
		symbolAdr(procIndex,c,"esi");
		cout<<"mov\tdword ptr [esi],eax"<<endl;
	}*/

	//����1����ʱ��Ϊ�ǵ�ַ������ٸ�Ϊ��ַ��
	//����2�������һֱʹ�á���t1���������Ż�ʱ���ϲ�����Ϊ��[]=�����.�������Ժ������У�t1������cʲô�ĺϲ����������úϲ�������ʼ�ձ���t1��


	//symbolAdrEqual(procIndex,c,"esi","eax");

	//int idProcNum=-1,idIndex=-1;//ǰ���Ѿ�����
	kindType tempType;
	int tempArgNum;
	int level;
	string tempreg,resultreg;
	tempreg="esi";resultreg="eax";
	mySymbolTable.getIdNameAdr(procIndex,c,idProcNum,idIndex);
	if(procIndex==idProcNum)//����	ע�⣺[]=middlecode����result���Ǳ���ľֲ�����
	{
		cout<<"mov\tdword ptr[ebp-"<<mySymbolTable.dimCount(procIndex,idIndex)*4+4<<"]"<<",eax"<<endl;	//attention	��������Ҫ�޸ģ����������1��ʼ������			
	}
	else//���Ż���Ӧ�ò����õ�
	{

		level=mySymbolTable.tableContext[idProcNum].lev;
		tempArgNum=mySymbolTable.tableContext[procIndex].procArgNum;//ע�⣺�Ǳ��㺯�����βθ�������������

		cout<<"mov\t"<<tempreg<<",dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//������eax����esi��??
		cout<<"sub\t"<<tempreg<<","<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
		cout<<"mov\tdword ptr ["<<tempreg<<"],"<<resultreg<<endl;

	}

	//cout<<"lea"
	//ע�⣺�����ǡ�ȡ��ַ��������˵c��ԭ����ŵľ��ǵ�ַ����c:=[ebp-32](c���ǵ�ַ���岻ͬ)
	//:= eax,c��ָc�д�ŵ�ĳһ�����ĵ�ַ����ʱҪ��mov esi,dword ptr[c]; mov dword ptr [esi],eax
	//����ȡ��ַ������ȡĳһ�����ĵ�ַ������eax���Ѿ��ǵ�ַ��������һ������ֱ�ӵ��ڼ��ɡ����൱��ȡ�����ַ���ڣ�mov dword ptr[c] eax

}

void ASMGenerator::equalArray(int procIndex,string a,string b,string c)
{

	//lea ebx,[ebp-4];
	//mov	ecx,3
	//imul	ecx,4
	//sub	ebx,ecx
	//;ȡ��ַ
	//mov	eax,dword ptr[ebx]
	//mov dword ptr[ebp-4],eax

	symbolAdr(procIndex,a,"ebx");//����a�ĵ�ַ��
	symbol(procIndex,b,"ecx");

	string tempArrayFlag1=getNewArrayJudge();
	string tempArrayFlag2=getNewArrayJudge();
	//cout<<opcmp<<"\t"<<tempFlag<<endl;
	int idProcNum=-1,idIndex=-1;
	mySymbolTable.getIdNameAdr(procIndex,a,idProcNum,idIndex);
	cout<<"cmp\tecx,"<<mySymbolTable.tableContext[idProcNum].itemList[idIndex].arraydim<<endl;
	cout<<"jl\t"<<tempArrayFlag1<<endl;
	WriteString("arrayerror");
	printf("mov\tesp,ebp\n");
	printf("pop\tebp\n");
	printf("ret\n");
	cout<<tempArrayFlag1<<":"<<endl;

	cout<<"cmp\tecx,"<<"0"<<endl;
	cout<<"jge\t"<<tempArrayFlag2<<endl;
	WriteString("arrayerror");
	printf("mov\tesp,ebp\n");
	printf("pop\tebp\n");
	printf("ret\n");
	cout<<tempArrayFlag2<<":"<<endl;





	cout<<"imul\tecx,4"<<endl;
	cout<<"sub\tebx,ecx"<<endl;
	cout<<"mov\teax,dword ptr [ebx]"<<endl;
	symbolAdrEqual(procIndex,c,"esi","eax");
}


void ASMGenerator::Becomes(int procIndex,string a,string b)
{
	//����Ǻ�������ֵ������
	//kindType tempType=mySymbolTable.canUse(procIndex,b);
	//if(tempType)
	//��Ȼ��������˵����̫�����ˣ�������������������������������������
	symbol(procIndex,a,"eax");
	symbolAdrEqual(procIndex,b,"esi","eax");
}


void ASMGenerator::Add(int procIndex,string a,string b,string c)
{
	//mov eax,dword ptr [ebp-4]
	//add eax,2
	//mov dword ptr[ebp-8],eax

	//mov eax,dword ptr [ebp-4]
	//add eax,dword ptr [ebp-8]
	//mov dword ptr[ebp-12],eax


	symbol(procIndex,a,"eax");
	symbol(procIndex,b,"ebx");//�Ż�ʱ���Ա�Ǿ���������
	cout<<"add\teax,ebx"<<endl;

	/*int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,c,idProcNum,idIndex);

	if(idProcNum==procIndex&&mySymbolTable.tableContext[procIndex].itemList[idIndex].isnotadr==true)//����ǵ�ַ	��಻����Ϊ��ֵ		//���Է�װ���������棿����ԭ������д��̫�߳���
	{
		cout<<"mov\t[ebp-"<<mySymbolTable.dimCount(procIndex,idIndex)*4+4<<"]"<<",eax"<<endl;
	}
	else//�����ߣ���ȡ��ַ��ע��:ȡ��ַʱ�������õ�eax
	{
		symbolAdr(procIndex,c,"esi");
		cout<<"mov\tdword ptr [esi],eax"<<endl;
	}*/
	symbolAdrEqual(procIndex,c,"esi","eax");
}

void ASMGenerator::Sub(int procIndex,string a,string b,string c)
{
	symbol(procIndex,a,"eax");
	symbol(procIndex,b,"ebx");
	cout<<"sub\teax,ebx"<<endl;
	symbolAdrEqual(procIndex,c,"esi","eax");
}

void ASMGenerator::Mul(int procIndex,string a ,string b,string c)
{
	symbol(procIndex,a,"eax");
	symbol(procIndex,b,"ebx");
	cout<<"imul\teax,ebx"<<endl;
	symbolAdrEqual(procIndex,c,"esi","eax");
}

void ASMGenerator::Div(int procIndex,string a,string b,string c)
{
	symbol(procIndex,a,"eax");
	symbol(procIndex,b,"ebx");
	cout<<"cdq"<<endl;
	cout<<"idiv\tebx"<<endl;
	symbolAdrEqual(procIndex,c,"esi","eax");
}


void ASMGenerator::Compare(int procIndex,string op,string a,string b,string c)
{
	//mov dword ptr [ebp-8],1
	//cmp dword ptr [ebp-4],0
	//jne @judge_flag1
	//mov dword ptr [ebp-8],0
	//@judge_flag1:

	symbol(procIndex,a,"eax");
	symbol(procIndex,b,"ebx");//Ҫ�Ż�������2����������������
	
	//c�Ǳ�־λ��ֻ���Ǳ��㡣
	//����϶�Ҫ�Ż�������
	//cout<<"mov\tdword ptr [ebp-"<<mySymbolTable.dimCount(procIndex,idIndex)*4+4<<"]"<<",1"<<endl;


	symbolAdrEqual(procIndex,c,"esi","1");//�൱�ڸ�ֵΪ1��������������������������
	cout<<"cmp\teax,ebx"<<endl;
	
	string opcmp;//��֪�����з������Ƚϻ����޷������Ƚϣ�������+��-�������氡�����з�����������������������������
	string tempFlag;
	opcmp=opCMP(op);
	tempFlag=getNewJudge();
	cout<<opcmp<<"\t"<<tempFlag<<endl;
	symbolAdrEqual(procIndex,c,"esi","0");
	cout<<tempFlag<<":"<<endl;
}

void ASMGenerator::IfFalse(int procIndex,string a,string b)
{
	//cmp	dword ptr [ebp-8],0
	//	je	@flag_flag2
	symbol(procIndex,a,"eax");//����ֱ��ȡ��ֵ�Ƚϵģ�ֻ����һ����ʱ���������Ż�ʱ��ȥ����������������������������
	cout<<"cmp\teax,0"<<endl;
	cout<<"je\t"<<b<<endl;
}


void ASMGenerator::CompareFalse(int procIndex,string op,string a,string b,string c)
{
	symbol(procIndex,a,"eax");
	symbol(procIndex,b,"ebx");//Ҫ�Ż�������2����������������

	cout<<"cmp\teax,ebx"<<endl;

	string opcmp;//��֪�����з������Ƚϻ����޷������Ƚϣ�������+��-�������氡�����з�����������������������������
	opcmp=opCompareReverse(op);
	cout<<opcmp<<"\t"<<c<<endl;
}
//CompareFalse(temp,myMiddleCode.codeList[codePoint1].opr,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].result);




string  ASMGenerator::opCMP(string op)//op�Ƚ������һ���������������
{
	if(op=="<")	return "jl";
	else if(op=="<=")	return "jle";
	else if(op==">")	return "jg";
	else if(op==">=")	return "jge";
	else if(op=="=")	return "je";
	else//�����ڣ�<>
		return "jne";
}

string  ASMGenerator::opCompareReverse(string op)
{
	if(op=="<false")	return "jge";
	else if(op=="<=false")	return "jg";
	else if(op==">false")	return "jle";
	else if(op==">=false")	return "jl";
	else if(op=="=false")	return "jne";
	else//�����ڣ�<>
		return "je";
}

void ASMGenerator::SetFlag(int procIndex,string a)
{
	cout<<a<<":"<<endl;
}

void ASMGenerator::JmpTo(int procIndex,string a)
{
	cout<<"jmp\t"<<a<<endl;
}


void ASMGenerator::Display(int procIndex,string a,string b)
{
	//cout<<"mov\tesi,[ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//������eax����esi��??
	int tempCalledProcIndex=atoi(b.c_str());
	int levelofCaller=mySymbolTable.tableContext[procIndex].lev;
	int templevelofCallee=mySymbolTable.tableContext[tempCalledProcIndex].lev;
	//tempArgNum=mySymbolTable.tableContext[procIndex].procArgNum;
	int tempArgNum=mySymbolTable.tableContext[procIndex].procArgNum;//ע�⣺�Ǳ��㺯�����βθ�������������??????
	if(levelofCaller<tempCalledProcIndex)//j=i+1(i:caller;j:callee)
	{
		int i;
		cout<<"push\tebp"<<endl;
		for(i=0;i<levelofCaller;i++)
		{
			cout<<"push\t[ebp+"<<8+i*4+tempArgNum*4<<"]"<<endl;
		}
	}
	else
	{
		int i;
		for(i=0;i<tempCalledProcIndex;i++)
		{
			cout<<"push\t[ebp+"<<8+i*4+tempArgNum*4<<"]"<<endl;
		}
	}
}


void ASMGenerator::ValueEqual(int procIndex,string a,string b,string c)
{
	//push	dword ptr function2_id
	//if(mySymbolTable[])

	int idProcNum=-1,idIndex=-1;
	int tempc=atoi(c.c_str());
	mySymbolTable.getIdNameAdr(tempc,b,idProcNum,idIndex);
	if(idProcNum==-1||idIndex==-1)//��������������
	{

	}
	else
	{
		//bool tempadrflag=mySymbolTable.tableContext[idProcNum].itemList[idIndex].adr;//������������ﲻ�����tempadrflag�������뵽if��else������
		bool tempadrflag=mySymbolTable.tableContext[idProcNum].itemList[idIndex].isnotadr;
		if(tempadrflag)
		{
			symbol(procIndex,a,"eax");//����ڱ���Ĵ洢���ڡ�b�ǵ�ַ��ֱ��ѹ��ջ�м���
		}
		else
		{
			symbolAdr(procIndex,a,"eax");//b�ǵ�ַ������Ҫ����ַ����eax����ѹջ
		}
	}
	cout<<"push\teax"<<endl;
	//if(mySymbolTable.canUse)
}


void ASMGenerator::Call(int procIndex,string a)
{
	int tempCalledProcIndex=atoi(a.c_str());
	string tempCalledProcName=mySymbolTable.tableContext[tempCalledProcIndex].procName;
	cout<<"call\t"<<"id"<<a<<"_"<<tempCalledProcName<<endl;
	cout<<"add\tesp,"<<4*(mySymbolTable.tableContext[tempCalledProcIndex].procArgNum+mySymbolTable.tableContext[tempCalledProcIndex].lev)<<endl;//lev�Ƿ�Ҫ-1��������
}

void ASMGenerator::CallValue(int procIndex,string a,string b)
{
	int tempCalledProcIndex=atoi(a.c_str());
	string tempCalledProcName=mySymbolTable.tableContext[tempCalledProcIndex].procName;
	cout<<"call\t"<<"id"<<a<<"_"<<tempCalledProcName<<endl;
	cout<<"add\tesp,"<<4*(mySymbolTable.tableContext[tempCalledProcIndex].procArgNum+mySymbolTable.tableContext[tempCalledProcIndex].lev)<<endl;//lev�Ƿ�Ҫ-1��������
	//symbol(procIndex,)

	//cout<<"mov\teax,"
	symbol(procIndex,tempCalledProcName,"eax");//symbol���������ã���procIndex��s����������ֵ����eax��
	symbolAdrEqual(procIndex,b,"esi","eax");
}



void ASMGenerator::code()
{
	cout<<"\t.CODE"<<endl;

	//int temp=0;
	int temp;
	while(codePoint1<codePoint2)
	{
		//temp++;
		
		cout<<endl;
		cout<<";"<<myMiddleCode.codeList[codePoint1].opr << "\t" << myMiddleCode.codeList[codePoint1].target1 << "\t" << myMiddleCode.codeList[codePoint1].target2 << "\t" << myMiddleCode.codeList[codePoint1].result << endl;
		

		if(myMiddleCode.codeList[codePoint1].opr=="Begin")
		{
			temp=atoi(myMiddleCode.codeList[codePoint1].target1.c_str());
			procStart(temp);
		}

		codePoint1++;

		while(myMiddleCode.codeList[codePoint1].opr!="End")
		{
			cout<<endl;
			cout<<";"<<myMiddleCode.codeList[codePoint1].opr << "\t" << myMiddleCode.codeList[codePoint1].target1 << "\t" << myMiddleCode.codeList[codePoint1].target2 << "\t" << myMiddleCode.codeList[codePoint1].result << endl;

			if(myMiddleCode.codeList[codePoint1].opr=="WriteString")
			{
				WriteString(myMiddleCode.codeList[codePoint1].target1);
			}
			else if(myMiddleCode.codeList[codePoint1].opr=="WriteExpression")
			{
				WriteExpression(temp,myMiddleCode.codeList[codePoint1].target1);
			}
			else if(myMiddleCode.codeList[codePoint1].opr=="Read")
			{
				Read(temp,myMiddleCode.codeList[codePoint1].target1);
			}
			else if(myMiddleCode.codeList[codePoint1].opr=="+")
			{
				Add(temp,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].result);
			}
			else if(myMiddleCode.codeList[codePoint1].opr=="-")
			{
				Sub(temp,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].result);
			}
			else if(myMiddleCode.codeList[codePoint1].opr=="*")
			{
				Mul(temp,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].result);
			}
			else if(myMiddleCode.codeList[codePoint1].opr=="/")
			{
				Div(temp,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].result);
			}
			else if (myMiddleCode.codeList[codePoint1].opr==":=")//��������������ô����ģ�������������������������������������������������result
			{
				Becomes(temp,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].result);
			}
			else if (isCompare(myMiddleCode.codeList[codePoint1].opr))
			{
				Compare(temp,myMiddleCode.codeList[codePoint1].opr,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].result);
			}
			else if (isCompareFalse(myMiddleCode.codeList[codePoint1].opr))
			{
				CompareFalse(temp,myMiddleCode.codeList[codePoint1].opr,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].result);
			}
			else if (myMiddleCode.codeList[codePoint1].opr=="IfFalse")
			{
				IfFalse(temp,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2);
			}
			else if (myMiddleCode.codeList[codePoint1].opr=="SetFlag")
			{
				SetFlag(temp,myMiddleCode.codeList[codePoint1].target1);
			}
			else if (myMiddleCode.codeList[codePoint1].opr=="JmpTo")
			{
				JmpTo(temp,myMiddleCode.codeList[codePoint1].target1);
			}
			//else if (myMiddleCode.codeList[codePoint1].opr=="ConstDefine")//����ط�Ҫע�⣿���������������������������Ҵ������𣿣�������������
			//{
			//	Equal(temp,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].target1);
			//}
			else if (myMiddleCode.codeList[codePoint1].opr=="value=")
			{
				ValueEqual(temp,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].result);
			}
			else if (myMiddleCode.codeList[codePoint1].opr=="Call")
			{
				Call(temp,myMiddleCode.codeList[codePoint1].target1);
			}
			else if (myMiddleCode.codeList[codePoint1].opr=="CallValue")
			{
				CallValue(temp,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].result);
			}
			//else if (myMiddleCode.codeList[codePoint1].opr=="Return")
			//{
			//	Return(temp,myMiddleCode.codeList[codePoint1].target1);
			//}

			else if(myMiddleCode.codeList[codePoint1].opr=="[]=")
			{
				arrayEqual(temp,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].result);
			}
			else if(myMiddleCode.codeList[codePoint1].opr=="=[]")
			{
				equalArray(temp,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].result);
			}
			else if(myMiddleCode.codeList[codePoint1].opr=="Display")
			{
				Display(temp,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2);
			}
			++codePoint1;
		}
		cout<<endl;
		cout<<";"<<myMiddleCode.codeList[codePoint1].opr << "\t" << myMiddleCode.codeList[codePoint1].target1 << "\t" << myMiddleCode.codeList[codePoint1].target2 << "\t" << myMiddleCode.codeList[codePoint1].result << endl;
		++codePoint1;
		//if(opr=="End")???
		procEnd(temp);
	}

}

int ASMGenerator::isCompare(string s)
{
	if (s=="<"||s=="<="||s==">"||s==">="||s=="<>"||s=="=")
		return 1;
	else return 0;
}

int ASMGenerator::isCompareFalse(string s)
{
	if (s=="<false"||s=="<=false"||s==">false"||s==">=false"||s=="<>false"||s=="=false")
		return 1;
	else return 0;
}

string ASMGenerator::getNewJudge()
{
	string temp;
	strstream ss;
	ss<<++judgeFlagNum;
	ss>>temp;
	return "@judge_flag"+temp;
}

string ASMGenerator::getNewArrayJudge()
{
	string temp;
	strstream ss;
	ss<<++judgeArrayFlagNum;
	ss>>temp;
	return "@array_flag"+temp;
}

void ASMGenerator::symbol(int procIndex,string s,string reg)//�����reg�Ĵ�������				//�ǵ�ַ���߷ǵ�ַ������õ�esi������		����Ƿ�Ҫ�ĳ�edi������	// ��ַ�����ȡֵ���ǵ�ַ��ֱ��ȡֵ
{
	int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,s,idProcNum,idIndex);

	//cout<<procIndex<<idProcNum<<endl;

	if(idProcNum==-1&&idIndex==-1)//��ӡһ�������������֣���						//�Ƿ������ʱ�ַ�����������������char������
	{
		cout<<"mov\t"<<reg<<","<<s<<endl;
	}
	else
	{
		//idIndex++;
		tempType=mySymbolTable.tableContext[idProcNum].itemList[idIndex].kind;
		level=mySymbolTable.tableContext[idProcNum].lev;
		tempArgNum=mySymbolTable.tableContext[procIndex].procArgNum;//ע�⣺�Ǳ��㺯�����βθ�������������
		if(mySymbolTable.tableContext[idProcNum].itemList[idIndex].isnotadr==false)//�ǵ�ַ
		{
			if(idProcNum==procIndex)//��ʵӦ�жϲ��������񣿣���������
			{
				cout<<"mov\tesi,"<<"dword ptr [ebp-"<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4<<"]"<<endl;//mov eax,[eax]����񣿣���������������Ļ�����������Ż����ͷ���һ���Ĵ�������
				cout<<"mov\t"<<reg<<",dword ptr [esi]"<<endl;
			}
			else
			{
				cout<<"mov\tesi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//������eax����esi��??
				cout<<"sub\tesi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				cout<<"mov\t"<<"edi"<<",dword ptr [esi]"<<endl;//��edi���Ի�Ϊeax����������������
				cout<<"mov\t"<<reg<<",dword ptr [edi]"<<endl;
			}
		}
		else//���ǵ�ַ
		{
			if(idProcNum==procIndex)//��ʵӦ�жϲ��������񣿣���������
			{
				cout<<"mov\t"<<reg<<",dword ptr [ebp-"<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4<<"]"<<endl;//mov eax,[eax]����񣿣���
			}
			else
			{
				cout<<"mov\tesi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//������eax����esi��??
				cout<<"sub\tesi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				cout<<"mov\t"<<reg<<",dword ptr [esi]"<<endl;
			}
		}
	}
}


void ASMGenerator::symbolAdr(int procIndex,string s,string reg)//��ַ������õ�edi����				//symbolAdr���Ǿ���ȡ��ַ�����s�����ŵ�ַ���Ѵ�ţ�������s����
{
	int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,s,idProcNum,idIndex);

	//cout<<procIndex<<idProcNum<<endl;

	if(idProcNum==-1&&idIndex==-1)//����������������
	{
		cout<<"mov\t"<<reg<<","<<s<<endl;
	}
	else
	{
		//idIndex++;
		tempType=mySymbolTable.tableContext[idProcNum].itemList[idIndex].kind;
		level=mySymbolTable.tableContext[idProcNum].lev;
		tempArgNum=mySymbolTable.tableContext[procIndex].procArgNum;//ע�⣺�Ǳ��㺯�����βθ�������������
		if(mySymbolTable.tableContext[idProcNum].itemList[idIndex].isnotadr==false)//�ǵ�ַ
		{
			if(idProcNum==procIndex)//��ʵӦ�жϲ��������񣿣���������
			{
				cout<<"mov\t"<<reg<<",dword ptr [ebp-"<< mySymbolTable.dimCount(idProcNum,idIndex)*4+4 <<"]"<<endl;
			}
			else
			{
				cout<<"mov\tedi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//������eax����esi��??
				cout<<"sub\tedi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				cout<<"mov\t"<<reg<<",dword ptr [edi]"<<endl;//��edi���Ի�Ϊeax����������������
			}
		}
		else//���ǵ�ַ
		{
			if(idProcNum==procIndex)//��ʵӦ�жϲ��������񣿣���������
			{
				cout<<"lea\t"<<reg<<",dword ptr [ebp-"<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4<<"]"<<endl;//mov eax,[eax]����񣿣���
			}
			else//���
			{
				cout<<"mov\t"<<reg<<",dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//������eax����esi��??
				cout<<"sub\t"<<reg<<","<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
			}
		}
	}
}


void ASMGenerator::symbolAdrByte(int procIndex,string s,string reg)//������õ�	��ַ������õ�edi����				//symbolAdr���Ǿ���ȡ��ַ�����s�����ŵ�ַ���Ѵ�ţ�������s����
{
	int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,s,idProcNum,idIndex);

	//cout<<procIndex<<idProcNum<<endl;

	if(idProcNum==-1&&idIndex==-1)//����������������
	{
		cout<<"mov\t"<<reg<<","<<s<<endl;
	}
	else
	{
		//idIndex++;
		tempType=mySymbolTable.tableContext[idProcNum].itemList[idIndex].kind;
		level=mySymbolTable.tableContext[idProcNum].lev;
		tempArgNum=mySymbolTable.tableContext[procIndex].procArgNum;//ע�⣺�Ǳ��㺯�����βθ�������������
		if(mySymbolTable.tableContext[idProcNum].itemList[idIndex].isnotadr==false)//�ǵ�ַ
		{
			if(idProcNum==procIndex)//��ʵӦ�жϲ��������񣿣���������
			{
				cout<<"mov\t"<<reg<<",byte ptr [ebp-"<< mySymbolTable.dimCount(idProcNum,idIndex)*4+4 <<"]"<<endl;
			}
			else
			{
				cout<<"mov\tedi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//������eax����esi��??
				cout<<"sub\tedi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				cout<<"mov\t"<<reg<<",byte ptr [edi]"<<endl;//��edi���Ի�Ϊeax����������������
			}
		}
		else//���ǵ�ַ
		{
			if(idProcNum==procIndex)//��ʵӦ�жϲ��������񣿣���������
			{
				cout<<"lea\t"<<reg<<",byte ptr [ebp-"<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4<<"]"<<endl;//mov eax,[eax]����񣿣���
			}
			else//���
			{
				cout<<"mov\t"<<reg<<",byte ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//������eax����esi��??
				cout<<"sub\t"<<reg<<","<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
			}
		}
	}
}




void ASMGenerator::symbolAdrEqual(int procIndex,string s,string tempreg,string resultreg)//s�����Ĳ�֪�ǵ�ַ����ֵ�ı���,		//ע�⣺�����ߣ���ȡ��ַ����symbolAdr��������edi��
{
	int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,s,idProcNum,idIndex);
	if(idProcNum==procIndex&&mySymbolTable.tableContext[procIndex].itemList[idIndex].isnotadr==true)//����ǵ�ַ	��಻����Ϊ��ֵ
	{
		cout<<"mov\tdword ptr [ebp-"<<mySymbolTable.dimCount(procIndex,idIndex)*4+4<<"]"<<","<<resultreg<<endl;//���Ż�����������Ч
	}
	else//�����ߣ���ȡ��ַ��ע��:ȡ��ַʱ�������õ�eax
	{
		symbolAdr(procIndex,s,tempreg);
		cout<<"mov\tdword ptr ["<<tempreg<<"],"<<resultreg<<endl;
	}
}

