#include "stdio.h"
#include "OptASMGenerator.h"
#include "GlobalData.h"
#include "SymbolTable.h"
#include <iostream>
#include <strstream>
#include <string>

using namespace std;

int OptASMGenerator::GlobalOK;//ȫ�ּĴ����Ż�����
string OptASMGenerator::prefix="C:\\masm32\\lib\\";
//string OptASMGenerator::prefix="";
int OptASMGenerator::codePoint1=0;
int OptASMGenerator::codePoint2=0;
int OptASMGenerator::judgeFlagNum=0;
int OptASMGenerator::judgeArrayFlagNum=0;
deque <int> OptASMGenerator::varFlag;
//int OptASMGenerator::addr;
int OptASMGenerator::RegNum;
deque <string> OptASMGenerator::Register;
deque <string> OptASMGenerator::registerToMem;

deque <string> OptASMGenerator::GlobalRegister;

void OptASMGenerator::setObjectfileName(char s[])
{
	strcpy(objectfileName,s);
}

void OptASMGenerator::setGlobalOK(int ok)
{
	GlobalOK=ok;
}

void OptASMGenerator::init()
{

	Register.push_back("eax");   
	Register.push_back("ecx");
	Register.push_back("edx");
	//Register.push_back("ebx");
	//Register.push_back("esi");
	//Register.push_back("edi");

	registerToMem.push_back("");
	registerToMem.push_back("");
	registerToMem.push_back("");
	//registerToMem.push_back("");
	//registerToMem.push_back("");
	//registerToMem.push_back("");


	GlobalRegister.push_back("edi");   
	GlobalRegister.push_back("ebx");
	//GlobalRegister.push_back("esi");

	varFlag.push_back(0);
	varFlag.push_back(0);


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



void OptASMGenerator::generate()
{
	init();
	data();
	code();

}

void OptASMGenerator::data()
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

void OptASMGenerator::procStart(int index)
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
	printf("sub\tesp,%d\n",mySymbolTable.dimCount(index,mySymbolTable.tableContext[index].itemList.size())*4+4);//���Ȳ��ܺ������ǹ��̣�һ����1���ռ䱣�淵��ֵ��֮��Ҫ�Ż���������ֹ��char����


	if(GlobalOK==1)
	{
		if(varFlag[1]==0)	printf("push\tebx\n");
		//printf("push\tesi\n");
		if(varFlag[0]==0)	printf("push\tedi\n");
	}



	for(i=1;i<=mySymbolTable.tableContext[index].procArgNum;i++)				//�����ˣ�Ҫ�ж��Ǵ��λ��Ǵ���ַ��������
	{
		//if(mySymbolTable.)���ܴ��λ��Ǵ�ֵ���������С������Ĺ��̵ģ���

		
		if(GlobalOK==0)
		{
			printf("mov\teax,dword ptr [ebp+%d]\n",(mySymbolTable.tableContext[index].procArgNum-i)*4+8);//���Ĳ���ֻ���ǻ������ͣ����������飬����û��dimCount������ά��
			printf("mov\tdword ptr ");
			cout<<"[ebp-"<<4*i<<"]"<<",eax"<<endl;
		}
		else
		{
			string s3;

			int RegNum=GotWholeReg(index,mySymbolTable.tableContext[index].itemList[i-1].name);
			if(RegNum>=0)//���Ƿ�ռ��ȫ�ּĴ���
			{
				s3=GlobalRegister[RegNum];
				cout<<"mov\t"<<s3<<","<<"dword ptr [ebp+"<<(mySymbolTable.tableContext[index].procArgNum-i)*4+8<<"]"<<endl;
			}
			else
			{
				printf("mov\teax,dword ptr [ebp+%d]\n",(mySymbolTable.tableContext[index].procArgNum-i)*4+8);//���Ĳ���ֻ���ǻ������ͣ����������飬����û��dimCount������ά��
				printf("mov\tdword ptr ");
				cout<<"[ebp-"<<4*i<<"]"<<",eax"<<endl;
			}

		}


		
	}

	
	for(i=0;i<mySymbolTable.tableContext[index].itemList.size();i++)
	{
		if(mySymbolTable.tableContext[index].itemList[i].isnotconst==false)
		{

			if(GlobalOK==0)
			{
				printf("mov\tdword ptr [ebp-%d],%d\n",4*(i+1),mySymbolTable.tableContext[index].itemList[i].value);//ֻ�����Ǳ��㣬�ǵ�ַ��
			}
			else
			{
				string s3;

				int RegNum=GotWholeReg(index,mySymbolTable.tableContext[index].itemList[i].name);
				if(RegNum>=0)//���Ƿ�ռ��ȫ�ּĴ���
				{
					s3=GlobalRegister[RegNum];
					cout<<"mov\t"<<s3<<","<<mySymbolTable.tableContext[index].itemList[i].value<<endl;
				}
				else
				{
					printf("mov\tdword ptr [ebp-%d],%d\n",4*(i+1),mySymbolTable.tableContext[index].itemList[i].value);//ֻ�����Ǳ��㣬�ǵ�ַ��
				}
			}
			


		}
	}
}

void OptASMGenerator::procEnd(int index)
{

	if(GlobalOK==1)
	{
		//printf("push\tesi\n");
		if(varFlag[0]==0)	printf("pop\tedi\n");
		//printf("pop\tesi\n");
		if(varFlag[1]==0)	printf("pop\tebx\n");
	}

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

void OptASMGenerator::WriteString(int procIndex,string s)
{
	//if(registerToMem[0]!="")
	//{
	//	movToMem(procIndex,0);
	//	registerToMem[0]="";
	//}
	//if(registerToMem[2]!="")
	//{
	//	movToMem(procIndex,2);
	//	registerToMem[2]="";
	//}
	logOutReg(procIndex);
	//��������eax��ڴ����Ϊ��ı�eax��ֵ
	cout<<"lea\tesi,"<<s<<endl;
	cout<<"push\tesi"<<endl; 
	cout<<"call\tprintf"<<endl;
	cout<<"add\tesp,4"<<endl;
}

bool OptASMGenerator::isInReg(string s)
{
	if(s=="eax"||s=="ecx"||s=="edx"||s=="ebx"||s=="esi"||s=="edi")
		return true;
	return false;
}



void OptASMGenerator::WriteExpression(int procIndex,string s)
{
	string temps;
	
	
	//int addr=GetSymbol(procIndex,s,temps);
	//if(addr==-1||addr==-2)
	//{
	//	cout<<"push\t"<<temps<<endl;
	//}
	//else
	//{
	//	cout<<"mov\teax,"<<temps<<";"<<endl;//��ʱ��֤�԰ɣ��������Ż���
	//	cout<<"push\teax"<<endl;
	//}

	GetSymbol(procIndex,s,temps);
	cout<<"push\t"<<temps<<endl;

	kindType tempType=mySymbolTable.canUse(procIndex,s);
	if(tempType==CONSTCHAR||tempType==CHAR||tempType==PROCCHAR||tempType==ARRAYCHAR)//���治������Ԫ�ص���Ԫʽ��ת��
	{
		//if(registerToMem[0]!="")
		//{
		//	movToMem(procIndex,0);
		//	registerToMem[0]="";
		//}
		//if(registerToMem[2]!="")
		//{
		//	movToMem(procIndex,2);
		//	registerToMem[2]="";
		//}
		logOutReg(procIndex);

		//cout<<"push\teax"<<endl;
		cout<<"lea\tesi,outchar\n";
		cout<<"push\tesi"<<endl;
		cout<<"call\tprintf"<<endl;
		cout<<"add\tesp,8"<<endl;
	}
	else
	{
		//if(registerToMem[0]!="")
		//{
		//	movToMem(procIndex,0);
		//	registerToMem[0]="";
		//}
		//if(registerToMem[2]!="")
		//{
		//	movToMem(procIndex,2);
		//	registerToMem[2]="";
		//}
		logOutReg(procIndex);
		//cout<<"push\teax"<<endl;
		cout<<"lea\tesi,outint\n";
		cout<<"push\tesi"<<endl;
		cout<<"call\tprintf"<<endl;
		cout<<"add\tesp,8"<<endl;
	}
}

void OptASMGenerator::Read(int procIndex,string s)
{
	kindType tempType=mySymbolTable.canUse(procIndex,s);
	if(tempType==CONSTCHAR||tempType==CHAR||tempType==PROCCHAR||tempType==ARRAYCHAR)//���治������Ԫ�ص���Ԫʽ��ת��
	{
		string s3;
		int i;
		//GetSymbol(procIndex,s,temps);

		for(i=1;i<registerToMem.size();i++)
		{
			if(registerToMem[i]==""){
				registerToMem[i]="";//���Ϊ��ʱʹ�õļĴ�����֮�������ͷŲ��ã����Ի���""
				//movToReg(i,c);
				break;
			}
		}
		if(i<registerToMem.size())
		{
			s3=Register[i];
		}
		else if(i==registerToMem.size())//�Ĵ���������
		{
			//ע�⣺eax�Ѿ������ã�����
			for(i=1;i<=2;i++)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!���ǰ��������
			{
				if(registerToMem[i]!=""&&registerToMem[i]!="")
					break;
			}
			movToMem(procIndex,i);
			registerToMem[i]="";
			s3=Register[i];
		}

		symbolAdr(procIndex,s,s3);		//����esi�Ĵ���
		//ʹ��eax�������ɲ������룡������������������������������������������������������������


		//getSymbolAdr???

		//if(registerToMem[0]!="")
		//{
		//	movToMem(procIndex,0);
		//	registerToMem[0]="";
		//}
		//if(registerToMem[2]!="")
		//{
		//	movToMem(procIndex,2);
		//	registerToMem[2]="";
		//}
		
		cout<<"xor\teax,eax"<<endl;

		cout<<"mov dword ptr ["<<s3<<"],eax"<<endl;//��0

		symbolAdr(procIndex,s,"esi");//esi����ʹ�ã���ȻҲ����ʹ��s3
		cout<<"push\tesi"<<endl;
		cout<<"lea\tesi,inchar\n";
		cout<<"push\tesi"<<endl;

		logOutReg(procIndex);
		cout<<"call\tscanf"<<endl;
		cout<<"add\tesp,8"<<endl;
	}
	else
	{
		//if(registerToMem[0]!="")
		//{
		//	movToMem(procIndex,0);
		//	registerToMem[0]="";
		//}
		//if(registerToMem[2]!="")
		//{
		//	movToMem(procIndex,2);
		//	registerToMem[2]="";
		//}
		logOutReg(procIndex);
		symbolAdr(procIndex,s,"esi");

		cout<<"push\tesi"<<endl;
		cout<<"lea\tesi,inint\n";
		cout<<"push\tesi"<<endl;
		cout<<"call\tscanf"<<endl;
		cout<<"add\tesp,8"<<endl;
	}
}

void OptASMGenerator::arrayEqual(int procIndex,string a,string b,string c)
{
	//[]=	B	3	t1
	//lea	eax,[ebp-4-4*3]
	//ʹ��eax������������������ʱ������esi
	//lea eax,[ebp-4];
	//mov	esi,3
	//imul	esi,4
	//sub	eax,esi
	//mov [ebp-48],eax

	//string reg="eax";
	//cout<<"lea\t"<<reg<<",[ebp"

	string s3,s33;
	//s3��b��	s33��a��	s333��c	ע�⣺c�д���ǵ�ַ�����ǲ�Ҫ����ʱ�Ĵ�����ȡ

	int i,tempi;
	//GetSymbol(procIndex,s,temps);

	for(i=0;i<registerToMem.size();i++)
	{
		if(registerToMem[i]==""){
			registerToMem[i]="";//���Ϊ��ʱʹ�õļĴ�����֮�������ͷŲ��ã����Ի���""
			//movToReg(i,c);
			break;
		}
	}
	if(i<registerToMem.size())
	{
		s33=Register[i];
	}
	else if(i==registerToMem.size())//�Ĵ���������
	{
		for(i=0;i<=2;i++)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!���ǰ��������
		{
			if(registerToMem[i]!=b&&registerToMem[i]!=c)
				break;
		}
		movToMem(procIndex,i);
		registerToMem[i]="";

		s33=Register[i];
	}
	tempi=i;
	//cout<<endl<<tempi<<endl<<endl<<endl;

	symbolAdr(procIndex,a,s33);//����a�ĵ�ַ��

	for(i=0;i<registerToMem.size();i++)
	{
		if(registerToMem[i]==""&&i!=tempi){
			registerToMem[i]="";//���Ϊ��ʱʹ�õļĴ�����֮�������ͷŲ��ã����Ի���""
			//movToReg(i,c);
			break;
		}
	}
	if(i<registerToMem.size())
	{
		s3=Register[i];
	}
	else if(i==registerToMem.size())//�Ĵ���������
	{
		for(i=0;i<=2;i++)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!���ǰ��������
		{
			if(registerToMem[i]!=a&&registerToMem[i]!=c&&i!=tempi)
				break;
		}
		if(i!=3)
		{
			movToMem(procIndex,i);
			registerToMem[i]="";
			s3=Register[i];
		}
		else//��ʵ����Ҫ���ࣺ��ʱ�Ĵ����ﲻ���ܴ�ȡa��ֵ����һ����ַ��
		{
			for(i=0;i<=1;i++)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!���ǰ��������
			{
				if(i!=tempi)
					break;
			}
			movToMem(procIndex,i);
			registerToMem[i]="";
			s3=Register[i];
		}

	}

	//s3=AssignReg(procIndex,a,c,b);//���ܸı�esi��ֵ������������!!!!
	//symbol(procIndex,b,);
	string tempb;
	GetSymbol(procIndex,b,tempb);
	cout<<"mov\t"<<s3<<","<<tempb<<endl;

	//cmp esi,10
	//jl @array_flag1
	//lea	esi,arrayerror
	//push	esi
	//call	printf
	//add	esp,4
	//mov	esp,ebp
	//pop	ebp
	//ret
	//@array_flag1:
	//cmp esi,0
	//jge @array_flag2
	//lea	esi,arrayerror
	//push	esi
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


	cout<<"cmp\t"<<s3<<","<<mySymbolTable.tableContext[idProcNum].itemList[idIndex].arraydim<<endl;
	cout<<"jl\t"<<tempArrayFlag1<<endl;
	WriteString(procIndex,"arrayerror");
	printf("mov\tesp,ebp\n");
	printf("pop\tebp\n");
	printf("ret\n");
	cout<<tempArrayFlag1<<":"<<endl;

	cout<<"cmp\t"<<s3<<","<<"0"<<endl;
	cout<<"jge\t"<<tempArrayFlag2<<endl;
	WriteString(procIndex,"arrayerror");
	printf("mov\tesp,ebp\n");
	printf("pop\tebp\n");
	printf("ret\n");
	cout<<tempArrayFlag2<<":"<<endl;


	cout<<"imul\t"<<s3<<",4"<<endl;
	cout<<"sub\t"<<s33<<","<<s3<<endl;


	kindType tempType;
	int tempArgNum;
	int level;
	string tempreg,resultreg;
	tempreg=s3;
	resultreg=s33;
	mySymbolTable.getIdNameAdr(procIndex,c,idProcNum,idIndex);
	if(procIndex==idProcNum)//����	ע�⣺[]=middlecode����result���Ǳ���ľֲ�����
	{
		cout<<"mov\tdword ptr[ebp-"<<mySymbolTable.dimCount(procIndex,idIndex)*4+4<<"]"<<","<<s33<<endl;	//attention	��������Ҫ�޸ģ����������0��ʼ������			
	}
	else//���Ż���Ӧ�ò����õ�
	{
		//��������				//����������ΪmovToMem�������õ�esi��ֵ�ı���
		if(registerToMem[0]!="")
		{
			movToMem(procIndex,0);
			registerToMem[0]="";
		}
		
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




void OptASMGenerator::equalArray(int procIndex,string a,string b,string c)
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
	WriteString(procIndex,"arrayerror");
	printf("mov\tesp,ebp\n");
	printf("pop\tebp\n");
	printf("ret\n");
	cout<<tempArrayFlag1<<":"<<endl;

	cout<<"cmp\tecx,"<<"0"<<endl;
	cout<<"jge\t"<<tempArrayFlag2<<endl;
	WriteString(procIndex,"arrayerror");
	printf("mov\tesp,ebp\n");
	printf("pop\tebp\n");
	printf("ret\n");
	cout<<tempArrayFlag2<<":"<<endl;





	cout<<"imul\tecx,4"<<endl;
	cout<<"sub\tebx,ecx"<<endl;
	cout<<"mov\teax,dword ptr [ebx]"<<endl;
	symbolAdrEqual(procIndex,c,"esi","eax");
}


void OptASMGenerator::Becomes(int procIndex,string a,string b)
{
	//����Ǻ�������ֵ������
	//kindType tempType=mySymbolTable.canUse(procIndex,b);
	//if(tempType)
	//��Ȼ��������˵����̫�����ˣ�������������������������������������
	string s1,s3;
	s3=AssignReg(procIndex,"",a,b);

	GetSymbol(procIndex,a,s1);
	cout<<"mov\t"<<s3<<","<<s1<<endl;

}

/*
int OptASMGenerator::askRegister(int procIndex,string c)
{
	for(int i=0;i<RegisterToMem.size();i++){
		if(registerToMem[i]==""){
			registerToMem[i]=c;
			//movToReg(i,c);
			return i;
		}
	}

	
	if(registerToMem[0]!=""){
		movToMem(0);
		registerToMem[0]=c;
	}
	return 0;
}*/


void OptASMGenerator::movToMem(int procIndex,int i)
{
	//ǰ���Ƿ�Ҫ���esi�ѱ�ռ�ã���������������������������symbolAdrEqual��ǣ�������
	if(registerToMem[i]!="")
	{
		symbolAdrEqual(procIndex,registerToMem[i],"esi",Register[i]);
	}
}


string OptASMGenerator::AssignReg(int procIndex,string a,string b,string c)
{
	string s3;
	int i,addr;
	//�ȸ�c����Ĵ���������������
	addr=GotWholeReg(procIndex,c);
	if(addr>=0)//���Ƿ�ռ��ȫ�ּĴ���
	{
		s3=GlobalRegister[addr];
		//s3=//ȫ�ּĴ���ebx�ȵ�
	}
	else
	{
		for(i=0;i<registerToMem.size();i++){
			if(registerToMem[i]==""){
				registerToMem[i]=c;
				//movToReg(i,c);
				break;
			}
		}
		if(i<registerToMem.size())
		{
			s3=Register[i];
		}
		else if(i==registerToMem.size())//�Ĵ���������
		{
			for(i=0;i<=2;i++)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!���ǰ��������
			{
				if(registerToMem[i]!=a&&registerToMem[i]!=b)
					break;
			}
			movToMem(procIndex,i);
			registerToMem[i]=c;
			s3=Register[i];
		}
	}
	return s3;
}

void OptASMGenerator::Add(int procIndex,string a,string b,string c)
{
	string s1,s2,s3;
	s3=AssignReg(procIndex,a,b,c);

	GetSymbol(procIndex,a,s1);
	cout<<"mov\t"<<s3<<","<<s1<<endl;
	GetSymbol(procIndex,b,s2);
	cout<<"add\t"<<s3<<","<<s2<<endl;

}

void OptASMGenerator::Sub(int procIndex,string a,string b,string c)
{
	string s1,s2,s3;
	s3=AssignReg(procIndex,a,b,c);

	GetSymbol(procIndex,a,s1);
	cout<<"mov\t"<<s3<<","<<s1<<endl;
	GetSymbol(procIndex,b,s2);
	cout<<"sub\t"<<s3<<","<<s2<<endl;
}

void OptASMGenerator::Mul(int procIndex,string a ,string b,string c)
{
	string s1,s2,s3;
	s3=AssignReg(procIndex,a,b,c);

	GetSymbol(procIndex,a,s1);
	cout<<"mov\t"<<s3<<","<<s1<<endl;
	GetSymbol(procIndex,b,s2);
	cout<<"imul\t"<<s3<<","<<s2<<endl;
}

void OptASMGenerator::Div(int procIndex,string a,string b,string c)
{
	string s1,s2,s3;
	int i,addr;

	int quanjuflag=0,eaxflag=0;
	//�ȸ�c����Ĵ���������������
	addr=GotWholeReg(procIndex,c);
	if(addr>=0)//���Ƿ�ռ��ȫ�ּĴ���
	{
		//s3=//ȫ�ּĴ���ebx�ȵ�			//ȫ�ּĴ���Ҳ���ô�����������������ǵ�ת��eax����������������������

		quanjuflag=1;
		movToMem(procIndex,0);//����ʹ�üĴ���eax��������������������������������������������������������
		registerToMem[0]="";
		//s3=Register[0];
	}
	else
	{
		if(registerToMem[0]==a)
		{
			eaxflag=1;
			movToMem(procIndex,0);
			registerToMem[0]=c;
			s3=Register[0];
		}
		else
		{
			movToMem(procIndex,0);//����ʹ�üĴ���eax��������������������������������������������������������
			registerToMem[0]=c;
			s3=Register[0];
		}
		
	}

	GetSymbol(procIndex,a,s1);
	//cout<<"mov\t"<<s3<<","<<s1<<endl;
	if(eaxflag==0)
	{
		cout<<"mov\t"<<"eax"<<","<<s1<<endl;
	}
	else
	{
		//donothing,��Ϊeax�����ľ���a��
	}
	
	if(registerToMem[2]!="")
	{
		movToMem(procIndex,2);
		registerToMem[2]="";
	}


	GetSymbol(procIndex,b,s2);
	cout<<"cdq"<<endl;
	cout<<"idiv\t"<<s2<<endl;

	if(quanjuflag==1)
	{
		cout<<"mov\t"<<s3<<",eax"<<endl;
		registerToMem[0]="";
	}

}


void OptASMGenerator::Compare(int procIndex,string op,string a,string b,string c)
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

void OptASMGenerator::IfFalse(int procIndex,string a,string b)
{
	//cmp	dword ptr [ebp-8],0
	//	je	@flag_flag2
	symbol(procIndex,a,"eax");//����ֱ��ȡ��ֵ�Ƚϵģ�ֻ����һ����ʱ���������Ż�ʱ��ȥ����������������������������
	cout<<"cmp\teax,0"<<endl;
	cout<<"je\t"<<b<<endl;
}


void OptASMGenerator::CompareFalse(int procIndex,string op,string a,string b,string c)
{
	string s1,s2,s3;

	int i;
	for(i=0;i<registerToMem.size();i++)
	{
		if(registerToMem[i]==""){
			registerToMem[i]="";//���Ϊ��ʱʹ�õļĴ�����֮�������ͷŲ��ã����Ի���""
			//movToReg(i,c);
			break;
		}
	}
	if(i<registerToMem.size())
	{
		s3=Register[i];
	}
	else if(i==registerToMem.size())//�Ĵ���������
	{
		for(i=0;i<=2;i++)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!���ǰ��������
		{
			if(registerToMem[i]!=a&&registerToMem[i]!=b)
				break;
		}
		movToMem(procIndex,i);
		registerToMem[i]="";
		s3=Register[i];
	}

	GetSymbol(procIndex,a,s1);
	cout<<"mov\t"<<s3<<","<<s1<<endl;
	GetSymbol(procIndex,b,s2);
	cout<<"cmp\t"<<s3<<","<<s2<<endl;



	//symbol(procIndex,a,"eax");
	//symbol(procIndex,b,"ebx");//Ҫ�Ż�������2����������������

	//cout<<"cmp\teax,ebx"<<endl;

	//��ռĴ�����
	//д����Ӧ���ڴ�ռ�
	logOutReg(procIndex);

	string opcmp;//��֪�����з������Ƚϻ����޷������Ƚϣ�������+��-�������氡�����з�����������������������������
	opcmp=opCompareReverse(op);
	cout<<opcmp<<"\t"<<c<<endl;
}
//CompareFalse(temp,myMiddleCode.codeList[codePoint1].opr,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].result);




string  OptASMGenerator::opCMP(string op)//op�Ƚ������һ���������������
{
	if(op=="<")	return "jl";
	else if(op=="<=")	return "jle";
	else if(op==">")	return "jg";
	else if(op==">=")	return "jge";
	else if(op=="=")	return "je";
	else//�����ڣ�<>
		return "jne";
}

string  OptASMGenerator::opCompareReverse(string op)
{
	if(op=="<false")	return "jge";
	else if(op=="<=false")	return "jg";
	else if(op==">false")	return "jle";
	else if(op==">=false")	return "jl";
	else if(op=="=false")	return "jne";
	else//�����ڣ�<>
		return "je";
}

void OptASMGenerator::SetFlag(int procIndex,string a)
{
	cout<<a<<":"<<endl;
}

void OptASMGenerator::JmpTo(int procIndex,string a)
{
	cout<<"jmp\t"<<a<<endl;
}


void OptASMGenerator::Display(int procIndex,string a,string b)
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


void OptASMGenerator::ValueEqual(int procIndex,string a,string b,string c)
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
			//symbol(procIndex,a,"eax");//����ڱ���Ĵ洢���ڡ�b�ǵ�ַ��ֱ��ѹ��ջ�м���
			//cout<<"push\teax"<<endl;

			string tempa;
			GetSymbol(procIndex,a,tempa);
			cout<<"push\t"<<tempa<<endl;
		}
		else
		{
			if(GlobalOK==0)
			{
				symbolAdr(procIndex,a,"eax");//b�ǵ�ַ������Ҫ����ַ����eax����ѹջ
				cout<<"push\teax"<<endl;
			}
			else
			{
				string s3;

				//int tempidProcNum=-1,tempidIndex=-1;
				//int tempc=atoi(c.c_str());
				//mySymbolTable.getIdNameAdr(procIndex,a,tempidProcNum,tempidIndex);

				int RegNum=GotWholeReg(procIndex,a);
				if(RegNum>=0)//���Ƿ�ռ��ȫ�ּĴ���
				{
					varFlag[RegNum]=1;//			ע�⣺�������ú���/���̿�ͷ�Ƿ�Ĵ�������ԭֵ������������
					mySymbolTable.tableContext[idProcNum].itemList[idIndex].regnum=RegNum;//�������ڲ㴫��ַ�ı��������ı���ͬһ��ȫ�ּĴ���  bug:�ȶ��庯����Ȼ��ŵ��ð���������
				}
				else
				{
					//�����Ⱑ��
					symbolAdr(procIndex,a,"eax");//b�ǵ�ַ������Ҫ����ַ����eax����ѹջ
					cout<<"push\teax"<<endl;
				}
				
			}

			
		}
	}
	
	//if(mySymbolTable.canUse)
}


void OptASMGenerator::Call(int procIndex,string a)
{
	int tempCalledProcIndex=atoi(a.c_str());
	string tempCalledProcName=mySymbolTable.tableContext[tempCalledProcIndex].procName;
	cout<<"call\t"<<"id"<<a<<"_"<<tempCalledProcName<<endl;
	cout<<"add\tesp,"<<4*(mySymbolTable.tableContext[tempCalledProcIndex].procArgNum+mySymbolTable.tableContext[tempCalledProcIndex].lev)<<endl;//lev�Ƿ�Ҫ-1��������
}

void OptASMGenerator::CallValue(int procIndex,string a,string b)
{
	int tempCalledProcIndex=atoi(a.c_str());
	string tempCalledProcName=mySymbolTable.tableContext[tempCalledProcIndex].procName;
	cout<<"call\t"<<"id"<<a<<"_"<<tempCalledProcName<<endl;
	cout<<"add\tesp,"<<4*(mySymbolTable.tableContext[tempCalledProcIndex].procArgNum+mySymbolTable.tableContext[tempCalledProcIndex].lev)<<endl;//lev�Ƿ�Ҫ-1��������
	//symbol(procIndex,)

	//cout<<"mov\teax,"
	//symbol(procIndex,tempCalledProcName,"eax");//symbol���������ã���procIndex��s����������ֵ����eax��
	//symbolAdrEqual(procIndex,b,"esi","eax");



	string temps;
	GetSymbol(procIndex,tempCalledProcName,temps);
	cout<<"mov\teax,"<<temps<<endl;
	symbolAdrEqual(procIndex,b,"esi","eax");


}



void OptASMGenerator::code()
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
			if(myBaseBlock.flag[codePoint1]!=myBaseBlock.flag[codePoint1-1])//����һ���µĻ�����
			{
				//��ռĴ�����
				logOutReg(temp);
			}

			
			cout<<endl;
			cout<<";"<<myMiddleCode.codeList[codePoint1].opr << "\t" << myMiddleCode.codeList[codePoint1].target1 << "\t" << myMiddleCode.codeList[codePoint1].target2 << "\t" << myMiddleCode.codeList[codePoint1].result << endl;

			if(myMiddleCode.codeList[codePoint1].opr=="WriteString")
			{
				WriteString(temp,myMiddleCode.codeList[codePoint1].target1);
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
				//��ռĴ�����
				//д����Ӧ���ڴ�ռ�
				logOutReg(temp);
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
				//��ռĴ�����
				//logOutReg(temp);

				Call(temp,myMiddleCode.codeList[codePoint1].target1);

			}
			else if (myMiddleCode.codeList[codePoint1].opr=="CallValue")
			{
				//��ռĴ�����
				//logOutReg(temp);

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
				//��ռĴ�����
				logOutReg(temp);
				Display(temp,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2);
			}

			if(myBaseBlock.flag[codePoint1]!=myBaseBlock.flag[codePoint1+1])//����һ��������
			{
				//��ռĴ�����
				//д����Ӧ���ڴ�ռ�
				logOutReg(temp);
			}

			++codePoint1;
		}
		cout<<endl;
		cout<<";"<<myMiddleCode.codeList[codePoint1].opr << "\t" << myMiddleCode.codeList[codePoint1].target1 << "\t" << myMiddleCode.codeList[codePoint1].target2 << "\t" << myMiddleCode.codeList[codePoint1].result << endl;
		if(myBaseBlock.flag[codePoint1]!=myBaseBlock.flag[codePoint1+1])//����һ��������	//ע�⣺����flag����0��������
		{
			//��ռĴ�����
			//д����Ӧ���ڴ�ռ�
			logOutReg(temp);
		}
		procEnd(temp);
		++codePoint1;
		//if(opr=="End")???
		
	}

}

//��ռĴ�����
//�����мĴ����е�ֵд��Ӧ���ڴ���
void OptASMGenerator::logOutReg(int procIndex)
{
	for(int i=0;i<registerToMem.size();i++)
	{
		if(registerToMem[i]!=""){
			movToMem(procIndex,i);
		}
		registerToMem[i]="";
	}
}


int OptASMGenerator::isCompare(string s)
{
	if (s=="<"||s=="<="||s==">"||s==">="||s=="<>"||s=="=")
		return 1;
	else return 0;
}

int OptASMGenerator::isCompareFalse(string s)
{
	if (s=="<false"||s=="<=false"||s==">false"||s==">=false"||s=="<>false"||s=="=false")
		return 1;
	else return 0;
}

string OptASMGenerator::getNewJudge()
{
	string temp;
	strstream ss;
	ss<<++judgeFlagNum;
	ss>>temp;
	return "@judge_flag"+temp;
}

string OptASMGenerator::getNewArrayJudge()
{
	string temp;
	strstream ss;
	ss<<++judgeArrayFlagNum;
	ss>>temp;
	return "@array_flag"+temp;
}


int OptASMGenerator::GotWholeReg(int procIndex,string s)
{
	if(GlobalOK==1)
	{
		int idProcNum=-1,idIndex=-1;
		mySymbolTable.getIdNameAdr(procIndex,s,idProcNum,idIndex);
		if(idProcNum==-1&&idIndex==-1)
		{
			//��������������
			return -1;
		}
		else
		{
			int regNum=mySymbolTable.tableContext[idProcNum].itemList[idIndex].regnum;
			return regNum;
		}

	}
	return -1;
}


int OptASMGenerator::OccupyReg(int procIndex,string s)
{
	for(int i=0;i<registerToMem.size();i++)//Ҳ�����ڷ��ű��н������
	{
		if(registerToMem[i]==s)
		{
			return i;
		}
	}
	return -1;
}


int OptASMGenerator::GetSymbol(int procIndex,string s,string &temps)//�����reg�Ĵ�������				//�ǵ�ַ���߷ǵ�ַ������õ�esi������		����Ƿ�Ҫ�ĳ�edi������	// ��ַ�����ȡֵ���ǵ�ַ��ֱ��ȡֵ
{
	int addr;
	addr=-1;
	RegNum=GotWholeReg(procIndex,s);
	if(RegNum>=0)
	{
		addr=-2;//��־Ϊ����ȫ�ּĴ�����
		temps=GlobalRegister[RegNum];
	}
	else
	{
		RegNum=OccupyReg(procIndex,s);
		if(RegNum>=0)//ռ����ʱ�Ĵ�����
		{
			addr=-1;
			temps=Register[RegNum];//string����
		}
		else
		{
			temps=GetMemSymbol(procIndex,s);
			addr=0;//��־���ڴ�����
		}
	}
	return addr;
}


string OptASMGenerator::GetMemSymbol(int procIndex,string s)
{
	int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,s,idProcNum,idIndex);

	//cout<<procIndex<<idProcNum<<endl;

	if(idProcNum==-1&&idIndex==-1)//��ӡһ�������������֣���						//�Ƿ������ʱ�ַ�����������������char������
	{
		//cout<<"mov\t"<<reg<<","<<s<<endl;
		return s;
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
				//cout<<"mov\t"<<reg<<",dword ptr [eax]"<<endl;
				return "dword ptr [esi]";					//�󵨲²�Ӧ��û�й�ϵ����Ϊ��һ������
			}
			else
			{
				cout<<"mov\tesi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//������eax����esi��??
				cout<<"sub\tesi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				cout<<"mov\t"<<"esi"<<",dword ptr [esi]"<<endl;//��edi���Ի�Ϊeax����������������
				//cout<<"mov\t"<<reg<<",dword ptr [esi]"<<endl;//̫�����ˣ�˫��Ѱַ�����õ���esiһ���Ĵ�������
				return "dword ptr [esi]";
			}
		}
		else//���ǵ�ַ
		{
			if(idProcNum==procIndex)//��ʵӦ�жϲ��������񣿣���������
			{
				string tempStr;
				strstream ss;
				ss<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4;
				ss>>tempStr;

				//cout<<"mov\t"<<reg<<",dword ptr [ebp-"<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4<<"]"<<endl;//mov eax,[eax]����񣿣���
				return "dword ptr [ebp-"+tempStr+"]";
			}
			else
			{
				cout<<"mov\tesi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//������eax����esi��??
				cout<<"sub\tesi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				//cout<<"mov\t"<<reg<<",dword ptr [esi]"<<endl;
				return "dword ptr [esi]";
			}
		}
	}
}

string OptASMGenerator::GetSymbolAdr(int procIndex,string s)//��ַ������õ�edi����				//symbolAdr���Ǿ���ȡ��ַ�����s�����ŵ�ַ���Ѵ�ţ�������s����
{
	int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,s,idProcNum,idIndex);

	//cout<<procIndex<<idProcNum<<endl;

	if(idProcNum==-1&&idIndex==-1)//����������������
	{
		//cout<<"mov\t"<<reg<<","<<s<<endl;
		return s;
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
				string tempStr;
				strstream ss;
				ss<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4;
				ss>>tempStr;
				
				//cout<<"mov\t"<<reg<<",dword ptr [ebp-"<< mySymbolTable.dimCount(idProcNum,idIndex)*4+4 <<"]"<<endl;
				return "dword ptr [ebp-"+tempStr+"]";
			}
			else
			{
				cout<<"mov\tesi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//������eax����esi��??
				cout<<"sub\tesi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				//cout<<"mov\t"<<reg<<",dword ptr [esi]"<<endl;//��edi���Ի�Ϊeax����������������
				return "dword ptr [esi]";
			}
		}
		else//���ǵ�ַ
		{
			if(idProcNum==procIndex)//��ʵӦ�жϲ��������񣿣���������
			{
				//����ʱһ�㲻�ã�������



				 //cout<<"lea\t"<<reg<<",dword ptr [ebp-"<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4<<"]"<<endl;//mov eax,[eax]����񣿣���
			}
			else//���
			{
				//cout<<"mov\t"<<reg<<",dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//������eax����esi��??
				//cout<<"sub\t"<<reg<<","<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;

				//���ȡ��ַ��������������������������������������


			}
		}
	}
}

void OptASMGenerator::symbol(int procIndex,string s,string reg)//�����reg�Ĵ�������				//�ǵ�ַ���߷ǵ�ַ������õ�esi������		����Ƿ�Ҫ�ĳ�edi������	// ��ַ�����ȡֵ���ǵ�ַ��ֱ��ȡֵ
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
				cout<<"mov\t"<<reg<<",dword ptr [eax]"<<endl;
			}
			else
			{
				cout<<"mov\tesi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//������eax����esi��??
				cout<<"sub\tesi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				cout<<"mov\t"<<"esi"<<",dword ptr [esi]"<<endl;//��edi���Ի�Ϊeax����������������
				cout<<"mov\t"<<reg<<",dword ptr [esi]"<<endl;//̫�����ˣ�˫��Ѱַ�����õ���esiһ���Ĵ�������
			}
		}
		else//���ǵ�ַ
		{
			if(idProcNum==procIndex)//��ʵӦ�жϲ��������񣿣���������
			{
				string tempStr;
				strstream ss;
				ss<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4;
				ss>>tempStr;

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





void OptASMGenerator::symbolAdr(int procIndex,string s,string reg)//��ַ������õ�edi����				//symbolAdr���Ǿ���ȡ��ַ�����s�����ŵ�ַ���Ѵ�ţ�������s����
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
				string tempStr;
				strstream ss;
				ss<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4;
				ss>>tempStr;

				cout<<"mov\t"<<reg<<",dword ptr [ebp-"<< mySymbolTable.dimCount(idProcNum,idIndex)*4+4 <<"]"<<endl;
			}
			else
			{
				cout<<"mov\tesi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//������eax����esi��??
				cout<<"sub\tesi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				cout<<"mov\t"<<reg<<",dword ptr [esi]"<<endl;//��edi���Ի�Ϊeax����������������
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


void OptASMGenerator::symbolAdrByte(int procIndex,string s,string reg)//������õ�	��ַ������õ�edi����				//symbolAdr���Ǿ���ȡ��ַ�����s�����ŵ�ַ���Ѵ�ţ�������s����
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




void OptASMGenerator::symbolAdrEqual(int procIndex,string s,string tempreg,string resultreg)//s�����Ĳ�֪�ǵ�ַ����ֵ�ı���,		//ע�⣺�����ߣ���ȡ��ַ����symbolAdr��������edi��
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
