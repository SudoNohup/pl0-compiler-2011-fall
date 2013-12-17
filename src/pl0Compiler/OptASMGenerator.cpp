#include "stdio.h"
#include "OptASMGenerator.h"
#include "GlobalData.h"
#include "SymbolTable.h"
#include <iostream>
#include <strstream>
#include <string>

using namespace std;

int OptASMGenerator::GlobalOK;//全局寄存器优化开关
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
	//打印字符串
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
	printf("sub\tesp,%d\n",mySymbolTable.dimCount(index,mySymbolTable.tableContext[index].itemList.size())*4+4);//首先不管函数还是过程，一律留1个空间保存返回值。之后要优化！！！防止读char错误


	if(GlobalOK==1)
	{
		if(varFlag[1]==0)	printf("push\tebx\n");
		//printf("push\tesi\n");
		if(varFlag[0]==0)	printf("push\tedi\n");
	}



	for(i=1;i<=mySymbolTable.tableContext[index].procArgNum;i++)				//复杂了，要判断是传参还是传地址！！！！
	{
		//if(mySymbolTable.)不管传参还是传值，都还是有“传”的过程的！！

		
		if(GlobalOK==0)
		{
			printf("mov\teax,dword ptr [ebp+%d]\n",(mySymbolTable.tableContext[index].procArgNum-i)*4+8);//传的参数只能是基本类型，不能是数组，所以没用dimCount函数求维数
			printf("mov\tdword ptr ");
			cout<<"[ebp-"<<4*i<<"]"<<",eax"<<endl;
		}
		else
		{
			string s3;

			int RegNum=GotWholeReg(index,mySymbolTable.tableContext[index].itemList[i-1].name);
			if(RegNum>=0)//看是否占用全局寄存器
			{
				s3=GlobalRegister[RegNum];
				cout<<"mov\t"<<s3<<","<<"dword ptr [ebp+"<<(mySymbolTable.tableContext[index].procArgNum-i)*4+8<<"]"<<endl;
			}
			else
			{
				printf("mov\teax,dword ptr [ebp+%d]\n",(mySymbolTable.tableContext[index].procArgNum-i)*4+8);//传的参数只能是基本类型，不能是数组，所以没用dimCount函数求维数
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
				printf("mov\tdword ptr [ebp-%d],%d\n",4*(i+1),mySymbolTable.tableContext[index].itemList[i].value);//只可能是本层，非地址。
			}
			else
			{
				string s3;

				int RegNum=GotWholeReg(index,mySymbolTable.tableContext[index].itemList[i].name);
				if(RegNum>=0)//看是否占用全局寄存器
				{
					s3=GlobalRegister[RegNum];
					cout<<"mov\t"<<s3<<","<<mySymbolTable.tableContext[index].itemList[i].value<<endl;
				}
				else
				{
					printf("mov\tdword ptr [ebp-%d],%d\n",4*(i+1),mySymbolTable.tableContext[index].itemList[i].value);//只可能是本层，非地址。
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

	//mov eax,要有返回值？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
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
	//无条件将eax填到内存里：因为会改变eax的值
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
	//	cout<<"mov\teax,"<<temps<<";"<<endl;//暂时保证对吧，可能需优化啊
	//	cout<<"push\teax"<<endl;
	//}

	GetSymbol(procIndex,s,temps);
	cout<<"push\t"<<temps<<endl;

	kindType tempType=mySymbolTable.canUse(procIndex,s);
	if(tempType==CONSTCHAR||tempType==CHAR||tempType==PROCCHAR||tempType==ARRAYCHAR)//后面不会数组元素的四元式已转换
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
	if(tempType==CONSTCHAR||tempType==CHAR||tempType==PROCCHAR||tempType==ARRAYCHAR)//后面不会数组元素的四元式已转换
	{
		string s3;
		int i;
		//GetSymbol(procIndex,s,temps);

		for(i=1;i<registerToMem.size();i++)
		{
			if(registerToMem[i]==""){
				registerToMem[i]="";//标记为暂时使用的寄存器，之后立即释放不用，所以还是""
				//movToReg(i,c);
				break;
			}
		}
		if(i<registerToMem.size())
		{
			s3=Register[i];
		}
		else if(i==registerToMem.size())//寄存器池已满
		{
			//注意：eax已经被征用！！！
			for(i=1;i<=2;i++)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!检查前两个即可
			{
				if(registerToMem[i]!=""&&registerToMem[i]!="")
					break;
			}
			movToMem(procIndex,i);
			registerToMem[i]="";
			s3=Register[i];
		}

		symbolAdr(procIndex,s,s3);		//用了esi寄存器
		//使用eax，需生成补偿代码！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！


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

		cout<<"mov dword ptr ["<<s3<<"],eax"<<endl;//清0

		symbolAdr(procIndex,s,"esi");//esi可以使用，当然也可以使用s3
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
	//使用eax？？？？？？？？临时增加用esi
	//lea eax,[ebp-4];
	//mov	esi,3
	//imul	esi,4
	//sub	eax,esi
	//mov [ebp-48],eax

	//string reg="eax";
	//cout<<"lea\t"<<reg<<",[ebp"

	string s3,s33;
	//s3：b；	s33：a；	s333：c	注意：c中存的是地址：还是不要用临时寄存器存取

	int i,tempi;
	//GetSymbol(procIndex,s,temps);

	for(i=0;i<registerToMem.size();i++)
	{
		if(registerToMem[i]==""){
			registerToMem[i]="";//标记为暂时使用的寄存器，之后立即释放不用，所以还是""
			//movToReg(i,c);
			break;
		}
	}
	if(i<registerToMem.size())
	{
		s33=Register[i];
	}
	else if(i==registerToMem.size())//寄存器池已满
	{
		for(i=0;i<=2;i++)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!检查前两个即可
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

	symbolAdr(procIndex,a,s33);//数组a的地址。

	for(i=0;i<registerToMem.size();i++)
	{
		if(registerToMem[i]==""&&i!=tempi){
			registerToMem[i]="";//标记为暂时使用的寄存器，之后立即释放不用，所以还是""
			//movToReg(i,c);
			break;
		}
	}
	if(i<registerToMem.size())
	{
		s3=Register[i];
	}
	else if(i==registerToMem.size())//寄存器池已满
	{
		for(i=0;i<=2;i++)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!检查前两个即可
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
		else//其实不必要分类：临时寄存器里不肯能存取a的值（是一个地址）
		{
			for(i=0;i<=1;i++)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!检查前两个即可
			{
				if(i!=tempi)
					break;
			}
			movToMem(procIndex,i);
			registerToMem[i]="";
			s3=Register[i];
		}

	}

	//s3=AssignReg(procIndex,a,c,b);//可能改变esi的值！！！！！！!!!!
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
	if(procIndex==idProcNum)//本层	注意：[]=middlecode最后的result都是本层的局部变量
	{
		cout<<"mov\tdword ptr[ebp-"<<mySymbolTable.dimCount(procIndex,idIndex)*4+4<<"]"<<","<<s33<<endl;	//attention	这里马上要修改，由于数组从0开始计数。			
	}
	else//不优化，应该不会用到
	{
		//补偿代码				//可能有误：因为movToMem中外层会用到esi：值改变了
		if(registerToMem[0]!="")
		{
			movToMem(procIndex,0);
			registerToMem[0]="";
		}
		
		level=mySymbolTable.tableContext[idProcNum].lev;
		tempArgNum=mySymbolTable.tableContext[procIndex].procArgNum;//注意：是本层函数的形参个数！！！！！

		cout<<"mov\t"<<tempreg<<",dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//可以用eax代替esi的??
		cout<<"sub\t"<<tempreg<<","<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
		cout<<"mov\tdword ptr ["<<tempreg<<"],"<<resultreg<<endl;

	}

	//cout<<"lea"
	//注意：这里是“取地址”，不是说c里原来存放的就是地址，与c:=[ebp-32](c中是地址语义不同)
	//:= eax,c是指c中存放的某一变量的地址，此时要求mov esi,dword ptr[c]; mov dword ptr [esi],eax
	//这里取地址：就是取某一变量的地址。由于eax中已经是地址，这样下一步就是直接等于即可。（相当于取本身地址等于）mov dword ptr[c] eax

}




void OptASMGenerator::equalArray(int procIndex,string a,string b,string c)
{

	//lea ebx,[ebp-4];
	//mov	ecx,3
	//imul	ecx,4
	//sub	ebx,ecx
	//;取地址
	//mov	eax,dword ptr[ebx]
	//mov dword ptr[ebp-4],eax

	symbolAdr(procIndex,a,"ebx");//数组a的地址。
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
	//如果是函数返回值！！！
	//kindType tempType=mySymbolTable.canUse(procIndex,b);
	//if(tempType)
	//竟然不用特殊说明，太神奇了！！！！！！！！！！！！！！！！！！！
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
	//前面是否要标记esi已被占用？？？？？？？？？？？或者在symbolAdrEqual标记？？？？
	if(registerToMem[i]!="")
	{
		symbolAdrEqual(procIndex,registerToMem[i],"esi",Register[i]);
	}
}


string OptASMGenerator::AssignReg(int procIndex,string a,string b,string c)
{
	string s3;
	int i,addr;
	//先给c分配寄存器？？？？？？
	addr=GotWholeReg(procIndex,c);
	if(addr>=0)//看是否占用全局寄存器
	{
		s3=GlobalRegister[addr];
		//s3=//全局寄存器ebx等等
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
		else if(i==registerToMem.size())//寄存器池已满
		{
			for(i=0;i<=2;i++)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!检查前两个即可
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
	//先给c分配寄存器？？？？？？
	addr=GotWholeReg(procIndex,c);
	if(addr>=0)//看是否占用全局寄存器
	{
		//s3=//全局寄存器ebx等等			//全局寄存器也不好处理啊！！！！！最后还是得转给eax！！！！！！！！！！！

		quanjuflag=1;
		movToMem(procIndex,0);//必须使用寄存器eax！！！！！！！！！！！！！！！！！！！！！！！！！！！！
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
			movToMem(procIndex,0);//必须使用寄存器eax！！！！！！！！！！！！！！！！！！！！！！！！！！！！
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
		//donothing,因为eax里面存的就是a啊
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
	symbol(procIndex,b,"ebx");//要优化啊。第2个操作数是立即数
	
	//c是标志位，只能是本层。
	//这里肯定要优化啊！！
	//cout<<"mov\tdword ptr [ebp-"<<mySymbolTable.dimCount(procIndex,idIndex)*4+4<<"]"<<",1"<<endl;


	symbolAdrEqual(procIndex,c,"esi","1");//相当于赋值为1？？？？？？？？？？？？？
	cout<<"cmp\teax,ebx"<<endl;
	
	string opcmp;//不知道用有符号数比较还是无符号数比较？？？？+、-算在里面啊。用有符号整数！！！！！！！！！！！
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
	symbol(procIndex,a,"eax");//可以直接取出值比较的，只能是一个临时变量，怕优化时会去掉？？？？？？？？？？？？？
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
			registerToMem[i]="";//标记为暂时使用的寄存器，之后立即释放不用，所以还是""
			//movToReg(i,c);
			break;
		}
	}
	if(i<registerToMem.size())
	{
		s3=Register[i];
	}
	else if(i==registerToMem.size())//寄存器池已满
	{
		for(i=0;i<=2;i++)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!检查前两个即可
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
	//symbol(procIndex,b,"ebx");//要优化啊。第2个操作数是立即数

	//cout<<"cmp\teax,ebx"<<endl;

	//清空寄存器池
	//写回相应的内存空间
	logOutReg(procIndex);

	string opcmp;//不知道用有符号数比较还是无符号数比较？？？？+、-算在里面啊。用有符号整数！！！！！！！！！！！
	opcmp=opCompareReverse(op);
	cout<<opcmp<<"\t"<<c<<endl;
}
//CompareFalse(temp,myMiddleCode.codeList[codePoint1].opr,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].result);




string  OptASMGenerator::opCMP(string op)//op比较运算符一定在下述情况中吗？
{
	if(op=="<")	return "jl";
	else if(op=="<=")	return "jle";
	else if(op==">")	return "jg";
	else if(op==">=")	return "jge";
	else if(op=="=")	return "je";
	else//不等于：<>
		return "jne";
}

string  OptASMGenerator::opCompareReverse(string op)
{
	if(op=="<false")	return "jge";
	else if(op=="<=false")	return "jg";
	else if(op==">false")	return "jle";
	else if(op==">=false")	return "jl";
	else if(op=="=false")	return "jne";
	else//不等于：<>
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
	//cout<<"mov\tesi,[ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//可以用eax代替esi的??
	int tempCalledProcIndex=atoi(b.c_str());
	int levelofCaller=mySymbolTable.tableContext[procIndex].lev;
	int templevelofCallee=mySymbolTable.tableContext[tempCalledProcIndex].lev;
	//tempArgNum=mySymbolTable.tableContext[procIndex].procArgNum;
	int tempArgNum=mySymbolTable.tableContext[procIndex].procArgNum;//注意：是本层函数的形参个数！！！！！??????
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
	if(idProcNum==-1||idIndex==-1)//这种情况不会出现
	{

	}
	else
	{
		//bool tempadrflag=mySymbolTable.tableContext[idProcNum].itemList[idIndex].adr;//最奇葩的是这里不用这个tempadrflag，就跳入到if的else里面了
		bool tempadrflag=mySymbolTable.tableContext[idProcNum].itemList[idIndex].isnotadr;
		if(tempadrflag)
		{
			//symbol(procIndex,a,"eax");//如果在本层的存储器内、b非地址；直接压入栈中即可
			//cout<<"push\teax"<<endl;

			string tempa;
			GetSymbol(procIndex,a,tempa);
			cout<<"push\t"<<tempa<<endl;
		}
		else
		{
			if(GlobalOK==0)
			{
				symbolAdr(procIndex,a,"eax");//b是地址，则需要将地址传入eax，再压栈
				cout<<"push\teax"<<endl;
			}
			else
			{
				string s3;

				//int tempidProcNum=-1,tempidIndex=-1;
				//int tempc=atoi(c.c_str());
				//mySymbolTable.getIdNameAdr(procIndex,a,tempidProcNum,tempidIndex);

				int RegNum=GotWholeReg(procIndex,a);
				if(RegNum>=0)//看是否占用全局寄存器
				{
					varFlag[RegNum]=1;//			注意：这里设置函数/过程开头是否寄存器保留原值！！！！！！
					mySymbolTable.tableContext[idProcNum].itemList[idIndex].regnum=RegNum;//设置其内层传地址的变量与外层的变量同一个全局寄存器  bug:先定义函数，然后才调用啊！！！！
				}
				else
				{
					//有问题啊。
					symbolAdr(procIndex,a,"eax");//b是地址，则需要将地址传入eax，再压栈
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
	cout<<"add\tesp,"<<4*(mySymbolTable.tableContext[tempCalledProcIndex].procArgNum+mySymbolTable.tableContext[tempCalledProcIndex].lev)<<endl;//lev是否要-1？？？？
}

void OptASMGenerator::CallValue(int procIndex,string a,string b)
{
	int tempCalledProcIndex=atoi(a.c_str());
	string tempCalledProcName=mySymbolTable.tableContext[tempCalledProcIndex].procName;
	cout<<"call\t"<<"id"<<a<<"_"<<tempCalledProcName<<endl;
	cout<<"add\tesp,"<<4*(mySymbolTable.tableContext[tempCalledProcIndex].procArgNum+mySymbolTable.tableContext[tempCalledProcIndex].lev)<<endl;//lev是否要-1？？？？
	//symbol(procIndex,)

	//cout<<"mov\teax,"
	//symbol(procIndex,tempCalledProcName,"eax");//symbol函数的作用：从procIndex找s变量，将其值传到eax中
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
			if(myBaseBlock.flag[codePoint1]!=myBaseBlock.flag[codePoint1-1])//进入一个新的基本块
			{
				//清空寄存器池
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
			else if (myMiddleCode.codeList[codePoint1].opr==":=")//我忘了我这里怎么定义的？？？？？？？？？？？？？？？？？？？？？？？？？result
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
				//清空寄存器池
				//写回相应的内存空间
				logOutReg(temp);
				JmpTo(temp,myMiddleCode.codeList[codePoint1].target1);
			}
			//else if (myMiddleCode.codeList[codePoint1].opr=="ConstDefine")//这个地方要注意？？？？？？？？？？？？？？我处理了吗？？？？？？？？
			//{
			//	Equal(temp,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].target1);
			//}
			else if (myMiddleCode.codeList[codePoint1].opr=="value=")
			{
				ValueEqual(temp,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].result);
			}
			else if (myMiddleCode.codeList[codePoint1].opr=="Call")
			{
				//清空寄存器池
				//logOutReg(temp);

				Call(temp,myMiddleCode.codeList[codePoint1].target1);

			}
			else if (myMiddleCode.codeList[codePoint1].opr=="CallValue")
			{
				//清空寄存器池
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
				//清空寄存器池
				logOutReg(temp);
				Display(temp,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2);
			}

			if(myBaseBlock.flag[codePoint1]!=myBaseBlock.flag[codePoint1+1])//结束一个基本块
			{
				//清空寄存器池
				//写回相应的内存空间
				logOutReg(temp);
			}

			++codePoint1;
		}
		cout<<endl;
		cout<<";"<<myMiddleCode.codeList[codePoint1].opr << "\t" << myMiddleCode.codeList[codePoint1].target1 << "\t" << myMiddleCode.codeList[codePoint1].target2 << "\t" << myMiddleCode.codeList[codePoint1].result << endl;
		if(myBaseBlock.flag[codePoint1]!=myBaseBlock.flag[codePoint1+1])//结束一个基本块	//注意：最后的flag都是0？？？？
		{
			//清空寄存器池
			//写回相应的内存空间
			logOutReg(temp);
		}
		procEnd(temp);
		++codePoint1;
		//if(opr=="End")???
		
	}

}

//清空寄存器池
//将所有寄存器中的值写相应的内存中
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
			//不会出现这种情况
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
	for(int i=0;i<registerToMem.size();i++)//也可以在符号表中建立标记
	{
		if(registerToMem[i]==s)
		{
			return i;
		}
	}
	return -1;
}


int OptASMGenerator::GetSymbol(int procIndex,string s,string &temps)//结果在reg寄存器里面				//是地址或者非地址、外层用到esi！！！		最后是否要改成edi？？？	// 地址：间接取值；非地址：直接取值
{
	int addr;
	addr=-1;
	RegNum=GotWholeReg(procIndex,s);
	if(RegNum>=0)
	{
		addr=-2;//标志为存在全局寄存器中
		temps=GlobalRegister[RegNum];
	}
	else
	{
		RegNum=OccupyReg(procIndex,s);
		if(RegNum>=0)//占用临时寄存器了
		{
			addr=-1;
			temps=Register[RegNum];//string数组
		}
		else
		{
			temps=GetMemSymbol(procIndex,s);
			addr=0;//标志在内存里面
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

	if(idProcNum==-1&&idIndex==-1)//打印一个常数？？数字？？						//是否可能随时字符？？？？？？？？char？？？
	{
		//cout<<"mov\t"<<reg<<","<<s<<endl;
		return s;
	}
	else
	{
		//idIndex++;
		tempType=mySymbolTable.tableContext[idProcNum].itemList[idIndex].kind;
		level=mySymbolTable.tableContext[idProcNum].lev;
		tempArgNum=mySymbolTable.tableContext[procIndex].procArgNum;//注意：是本层函数的形参个数！！！！！
		if(mySymbolTable.tableContext[idProcNum].itemList[idIndex].isnotadr==false)//是地址
		{
			if(idProcNum==procIndex)//其实应判断层数相等与否？？？？？？
			{
				cout<<"mov\tesi,"<<"dword ptr [ebp-"<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4<<"]"<<endl;//mov eax,[eax]允许否？？？？？？？允许的话这里可以做优化，释放了一个寄存器？？
				//cout<<"mov\t"<<reg<<",dword ptr [eax]"<<endl;
				return "dword ptr [esi]";					//大胆猜测应该没有关系，因为下一步就是
			}
			else
			{
				cout<<"mov\tesi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//可以用eax代替esi的??
				cout<<"sub\tesi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				cout<<"mov\t"<<"esi"<<",dword ptr [esi]"<<endl;//对edi可以换为eax？？？？？？？？
				//cout<<"mov\t"<<reg<<",dword ptr [esi]"<<endl;//太霸气了！双重寻址，都用的是esi一个寄存器！！
				return "dword ptr [esi]";
			}
		}
		else//不是地址
		{
			if(idProcNum==procIndex)//其实应判断层数相等与否？？？？？？
			{
				string tempStr;
				strstream ss;
				ss<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4;
				ss>>tempStr;

				//cout<<"mov\t"<<reg<<",dword ptr [ebp-"<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4<<"]"<<endl;//mov eax,[eax]允许否？？？
				return "dword ptr [ebp-"+tempStr+"]";
			}
			else
			{
				cout<<"mov\tesi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//可以用eax代替esi的??
				cout<<"sub\tesi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				//cout<<"mov\t"<<reg<<",dword ptr [esi]"<<endl;
				return "dword ptr [esi]";
			}
		}
	}
}

string OptASMGenerator::GetSymbolAdr(int procIndex,string s)//地址、外层用到edi！！				//symbolAdr并非绝对取地址，如果s本身存放地址（已存放），则传入s本身。
{
	int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,s,idProcNum,idIndex);

	//cout<<procIndex<<idProcNum<<endl;

	if(idProcNum==-1&&idIndex==-1)//不会进入这个条件的
	{
		//cout<<"mov\t"<<reg<<","<<s<<endl;
		return s;
	}
	else
	{
		//idIndex++;
		tempType=mySymbolTable.tableContext[idProcNum].itemList[idIndex].kind;
		level=mySymbolTable.tableContext[idProcNum].lev;
		tempArgNum=mySymbolTable.tableContext[procIndex].procArgNum;//注意：是本层函数的形参个数！！！！！
		if(mySymbolTable.tableContext[idProcNum].itemList[idIndex].isnotadr==false)//是地址
		{
			if(idProcNum==procIndex)//其实应判断层数相等与否？？？？？？
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
				cout<<"mov\tesi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//可以用eax代替esi的??
				cout<<"sub\tesi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				//cout<<"mov\t"<<reg<<",dword ptr [esi]"<<endl;//对edi可以换为eax？？？？？？？？
				return "dword ptr [esi]";
			}
		}
		else//不是地址
		{
			if(idProcNum==procIndex)//其实应判断层数相等与否？？？？？？
			{
				//本层时一般不用？？？？



				 //cout<<"lea\t"<<reg<<",dword ptr [ebp-"<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4<<"]"<<endl;//mov eax,[eax]允许否？？？
			}
			else//外层
			{
				//cout<<"mov\t"<<reg<<",dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//可以用eax代替esi的??
				//cout<<"sub\t"<<reg<<","<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;

				//外层取地址？？？？？？？？？？？？？？？？？？？


			}
		}
	}
}

void OptASMGenerator::symbol(int procIndex,string s,string reg)//结果在reg寄存器里面				//是地址或者非地址、外层用到esi！！！		最后是否要改成edi？？？	// 地址：间接取值；非地址：直接取值
{
	int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,s,idProcNum,idIndex);

	//cout<<procIndex<<idProcNum<<endl;

	if(idProcNum==-1&&idIndex==-1)//打印一个常数？？数字？？						//是否可能随时字符？？？？？？？？char？？？
	{
		cout<<"mov\t"<<reg<<","<<s<<endl;
	}
	else
	{
		//idIndex++;
		tempType=mySymbolTable.tableContext[idProcNum].itemList[idIndex].kind;
		level=mySymbolTable.tableContext[idProcNum].lev;
		tempArgNum=mySymbolTable.tableContext[procIndex].procArgNum;//注意：是本层函数的形参个数！！！！！
		if(mySymbolTable.tableContext[idProcNum].itemList[idIndex].isnotadr==false)//是地址
		{
			if(idProcNum==procIndex)//其实应判断层数相等与否？？？？？？
			{
				cout<<"mov\tesi,"<<"dword ptr [ebp-"<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4<<"]"<<endl;//mov eax,[eax]允许否？？？？？？？允许的话这里可以做优化，释放了一个寄存器？？
				cout<<"mov\t"<<reg<<",dword ptr [eax]"<<endl;
			}
			else
			{
				cout<<"mov\tesi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//可以用eax代替esi的??
				cout<<"sub\tesi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				cout<<"mov\t"<<"esi"<<",dword ptr [esi]"<<endl;//对edi可以换为eax？？？？？？？？
				cout<<"mov\t"<<reg<<",dword ptr [esi]"<<endl;//太霸气了！双重寻址，都用的是esi一个寄存器！！
			}
		}
		else//不是地址
		{
			if(idProcNum==procIndex)//其实应判断层数相等与否？？？？？？
			{
				string tempStr;
				strstream ss;
				ss<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4;
				ss>>tempStr;

				cout<<"mov\t"<<reg<<",dword ptr [ebp-"<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4<<"]"<<endl;//mov eax,[eax]允许否？？？
			}
			else
			{
				cout<<"mov\tesi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//可以用eax代替esi的??
				cout<<"sub\tesi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				cout<<"mov\t"<<reg<<",dword ptr [esi]"<<endl;
			}
		}
	}
}





void OptASMGenerator::symbolAdr(int procIndex,string s,string reg)//地址、外层用到edi！！				//symbolAdr并非绝对取地址，如果s本身存放地址（已存放），则传入s本身。
{
	int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,s,idProcNum,idIndex);

	//cout<<procIndex<<idProcNum<<endl;

	if(idProcNum==-1&&idIndex==-1)//不会进入这个条件的
	{
		cout<<"mov\t"<<reg<<","<<s<<endl;
	}
	else
	{
		//idIndex++;
		tempType=mySymbolTable.tableContext[idProcNum].itemList[idIndex].kind;
		level=mySymbolTable.tableContext[idProcNum].lev;
		tempArgNum=mySymbolTable.tableContext[procIndex].procArgNum;//注意：是本层函数的形参个数！！！！！
		if(mySymbolTable.tableContext[idProcNum].itemList[idIndex].isnotadr==false)//是地址
		{
			if(idProcNum==procIndex)//其实应判断层数相等与否？？？？？？
			{
				string tempStr;
				strstream ss;
				ss<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4;
				ss>>tempStr;

				cout<<"mov\t"<<reg<<",dword ptr [ebp-"<< mySymbolTable.dimCount(idProcNum,idIndex)*4+4 <<"]"<<endl;
			}
			else
			{
				cout<<"mov\tesi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//可以用eax代替esi的??
				cout<<"sub\tesi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				cout<<"mov\t"<<reg<<",dword ptr [esi]"<<endl;//对edi可以换为eax？？？？？？？？
			}
		}
		else//不是地址
		{
			if(idProcNum==procIndex)//其实应判断层数相等与否？？？？？？
			{
				cout<<"lea\t"<<reg<<",dword ptr [ebp-"<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4<<"]"<<endl;//mov eax,[eax]允许否？？？
			}
			else//外层
			{
				cout<<"mov\t"<<reg<<",dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//可以用eax代替esi的??
				cout<<"sub\t"<<reg<<","<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
			}
		}
	}
}


void OptASMGenerator::symbolAdrByte(int procIndex,string s,string reg)//读语句用到	地址、外层用到edi！！				//symbolAdr并非绝对取地址，如果s本身存放地址（已存放），则传入s本身。
{
	int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,s,idProcNum,idIndex);

	//cout<<procIndex<<idProcNum<<endl;

	if(idProcNum==-1&&idIndex==-1)//不会进入这个条件的
	{
		cout<<"mov\t"<<reg<<","<<s<<endl;
	}
	else
	{
		//idIndex++;
		tempType=mySymbolTable.tableContext[idProcNum].itemList[idIndex].kind;
		level=mySymbolTable.tableContext[idProcNum].lev;
		tempArgNum=mySymbolTable.tableContext[procIndex].procArgNum;//注意：是本层函数的形参个数！！！！！
		if(mySymbolTable.tableContext[idProcNum].itemList[idIndex].isnotadr==false)//是地址
		{
			if(idProcNum==procIndex)//其实应判断层数相等与否？？？？？？
			{
				cout<<"mov\t"<<reg<<",byte ptr [ebp-"<< mySymbolTable.dimCount(idProcNum,idIndex)*4+4 <<"]"<<endl;
			}
			else
			{
				cout<<"mov\tedi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//可以用eax代替esi的??
				cout<<"sub\tedi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				cout<<"mov\t"<<reg<<",byte ptr [edi]"<<endl;//对edi可以换为eax？？？？？？？？
			}
		}
		else//不是地址
		{
			if(idProcNum==procIndex)//其实应判断层数相等与否？？？？？？
			{
				cout<<"lea\t"<<reg<<",byte ptr [ebp-"<<mySymbolTable.dimCount(idProcNum,idIndex)*4+4<<"]"<<endl;//mov eax,[eax]允许否？？？
			}
			else//外层
			{
				cout<<"mov\t"<<reg<<",byte ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//可以用eax代替esi的??
				cout<<"sub\t"<<reg<<","<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
			}
		}
	}
}




void OptASMGenerator::symbolAdrEqual(int procIndex,string s,string tempreg,string resultreg)//s是左侧的不知是地址还是值的变量,		//注意：外层或者：均取地址调用symbolAdr；不能用edi了
{
	int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,s,idProcNum,idIndex);
	if(idProcNum==procIndex&&mySymbolTable.tableContext[procIndex].itemList[idIndex].isnotadr==true)//本层非地址	左侧不可能为常值
	{
		cout<<"mov\tdword ptr [ebp-"<<mySymbolTable.dimCount(procIndex,idIndex)*4+4<<"]"<<","<<resultreg<<endl;//不优化都是这条有效
	}
	else//外层或者：均取地址；注意:取地址时不能再用到eax
	{
		symbolAdr(procIndex,s,tempreg);
		cout<<"mov\tdword ptr ["<<tempreg<<"],"<<resultreg<<endl;
	}
}
