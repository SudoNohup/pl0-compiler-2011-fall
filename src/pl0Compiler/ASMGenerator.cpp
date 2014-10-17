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
	//打印字符串
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
	//printf("sub\tesp,%d\n",mySymbolTable.dimCount(index,mySymbolTable.tableContext[index].itemList.size())*4+4);//首先不管函数还是过程，一律留1个空间保存返回值。之后要优化！！！

	//printf("sub\tesp,%d\n",mySymbolTable.dimCount(index,mySymbolTable.tableContext[index].itemList.size())*4);//首先不管函数还是过程，一律留1个空间保存返回值。之后要优化！！！
	printf("sub\tesp,%d\n",mySymbolTable.dimCount(index,mySymbolTable.tableContext[index].itemList.size())*4+4);//首先不管函数还是过程，一律留1个空间保存返回值。之后要优化！！！防止读char错误
	//printf("push\tebx\n");
	//printf("push\tesi\n");
	//printf("push\tedi\n");
	for(i=1;i<=mySymbolTable.tableContext[index].procArgNum;i++)				//复杂了，要判断是传参还是传地址！！！！
	{
		//if(mySymbolTable.)不管传参还是传值，都还是有“传”的过程的！！


		printf("mov\teax,dword ptr [ebp+%d]\n",(mySymbolTable.tableContext[index].procArgNum-i)*4+8);//传的参数只能是基本类型，不能是数组，所以没用dimCount函数求维数
		printf("mov\tdword ptr ");
		cout<<"[ebp-"<<4*i<<"]"<<",eax"<<endl;
	}

	//constDefine如何保存啊
	
	for(i=0;i<mySymbolTable.tableContext[index].itemList.size();i++)
	{
		if(mySymbolTable.tableContext[index].itemList[i].isnotconst==false)
		{
			printf("mov\tdword ptr [ebp-%d],%d\n",4*(i+1),mySymbolTable.tableContext[index].itemList[i].value);//只可能是本层，非地址。
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

	//mov eax,要有返回值？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
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
	if(tempType==CONSTCHAR||tempType==CHAR||tempType==PROCCHAR||tempType==ARRAYCHAR)//后面不会数组元素的四元式已转换
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


	//test错误之处
	//symbolAdr(procIndex,s,"eax");

	symbolAdr(procIndex,s,"esi");		//用了esi寄存器
	cout<<"xor\teax,eax"<<endl;

	cout<<"mov dword ptr [esi],eax"<<endl;//清0
	
	symbolAdr(procIndex,s,"eax");

	//symbolAdrByte(procIndex,s,"eax");						待选，待调试

	kindType tempType=mySymbolTable.canUse(procIndex,s);
	if(tempType==CONSTCHAR||tempType==CHAR||tempType==PROCCHAR||tempType==ARRAYCHAR)//后面不会数组元素的四元式已转换
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
	//使用eax？？？？？？？？临时增加用ecx
	//lea eax,[ebp-4];
	//mov	ecx,3
	//imul	ecx,4
	//sub	eax,ecx
	//mov [ebp-48],eax

	//string reg="eax";
	//cout<<"lea\t"<<reg<<",[ebp"
	symbolAdr(procIndex,a,"eax");//数组a的地址。
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

	//尽管这里只能是本层、非地址，但防止优化后可能是外层。
	/*int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,c,idProcNum,idIndex);
	if(idProcNum==procIndex&&mySymbolTable.tableContext[procIndex].itemList[idIndex].isnotadr==true)//本层非地址	左侧不可能为常值
	{
		cout<<"mov\tdword ptr[ebp-"<<mySymbolTable.dimCount(procIndex,idIndex)*4+4<<"]"<<",eax"<<endl;//不优化都是这条有效
	}
	else//外层或者：均取地址；注意:取地址时不能再用到eax
	{
		symbolAdr(procIndex,c,"esi");
		cout<<"mov\tdword ptr [esi],eax"<<endl;
	}*/

	//方案1：临时改为非地址。最后再改为地址。
	//方案2：这里就一直使用。（t1不可能在优化时被合并，因为是[]=运算符.错！！在以后运算中，t1可能与c什么的合并啊？？不让合并？？？始终保留t1）


	//symbolAdrEqual(procIndex,c,"esi","eax");

	//int idProcNum=-1,idIndex=-1;//前面已经定义
	kindType tempType;
	int tempArgNum;
	int level;
	string tempreg,resultreg;
	tempreg="esi";resultreg="eax";
	mySymbolTable.getIdNameAdr(procIndex,c,idProcNum,idIndex);
	if(procIndex==idProcNum)//本层	注意：[]=middlecode最后的result都是本层的局部变量
	{
		cout<<"mov\tdword ptr[ebp-"<<mySymbolTable.dimCount(procIndex,idIndex)*4+4<<"]"<<",eax"<<endl;	//attention	这里马上要修改，由于数组从1开始计数。			
	}
	else//不优化，应该不会用到
	{

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

void ASMGenerator::equalArray(int procIndex,string a,string b,string c)
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
	//如果是函数返回值！！！
	//kindType tempType=mySymbolTable.canUse(procIndex,b);
	//if(tempType)
	//竟然不用特殊说明，太神奇了！！！！！！！！！！！！！！！！！！！
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
	symbol(procIndex,b,"ebx");//优化时可以标记具体在哪里
	cout<<"add\teax,ebx"<<endl;

	/*int idProcNum=-1,idIndex=-1;
	kindType tempType;
	int tempArgNum;
	int level;
	mySymbolTable.getIdNameAdr(procIndex,c,idProcNum,idIndex);

	if(idProcNum==procIndex&&mySymbolTable.tableContext[procIndex].itemList[idIndex].isnotadr==true)//本层非地址	左侧不可能为常值		//可以封装到函数里面？？？原来函数写的太冗长。
	{
		cout<<"mov\t[ebp-"<<mySymbolTable.dimCount(procIndex,idIndex)*4+4<<"]"<<",eax"<<endl;
	}
	else//外层或者：均取地址；注意:取地址时不能再用到eax
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

void ASMGenerator::IfFalse(int procIndex,string a,string b)
{
	//cmp	dword ptr [ebp-8],0
	//	je	@flag_flag2
	symbol(procIndex,a,"eax");//可以直接取出值比较的，只能是一个零时变量，怕优化时会去掉？？？？？？？？？？？？？
	cout<<"cmp\teax,0"<<endl;
	cout<<"je\t"<<b<<endl;
}


void ASMGenerator::CompareFalse(int procIndex,string op,string a,string b,string c)
{
	symbol(procIndex,a,"eax");
	symbol(procIndex,b,"ebx");//要优化啊。第2个操作数是立即数

	cout<<"cmp\teax,ebx"<<endl;

	string opcmp;//不知道用有符号数比较还是无符号数比较？？？？+、-算在里面啊。用有符号整数！！！！！！！！！！！
	opcmp=opCompareReverse(op);
	cout<<opcmp<<"\t"<<c<<endl;
}
//CompareFalse(temp,myMiddleCode.codeList[codePoint1].opr,myMiddleCode.codeList[codePoint1].target1,myMiddleCode.codeList[codePoint1].target2,myMiddleCode.codeList[codePoint1].result);




string  ASMGenerator::opCMP(string op)//op比较运算符一定在下述情况中吗？
{
	if(op=="<")	return "jl";
	else if(op=="<=")	return "jle";
	else if(op==">")	return "jg";
	else if(op==">=")	return "jge";
	else if(op=="=")	return "je";
	else//不等于：<>
		return "jne";
}

string  ASMGenerator::opCompareReverse(string op)
{
	if(op=="<false")	return "jge";
	else if(op=="<=false")	return "jg";
	else if(op==">false")	return "jle";
	else if(op==">=false")	return "jl";
	else if(op=="=false")	return "jne";
	else//不等于：<>
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


void ASMGenerator::ValueEqual(int procIndex,string a,string b,string c)
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
			symbol(procIndex,a,"eax");//如果在本层的存储器内、b非地址；直接压入栈中即可
		}
		else
		{
			symbolAdr(procIndex,a,"eax");//b是地址，则需要将地址传入eax，再压栈
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
	cout<<"add\tesp,"<<4*(mySymbolTable.tableContext[tempCalledProcIndex].procArgNum+mySymbolTable.tableContext[tempCalledProcIndex].lev)<<endl;//lev是否要-1？？？？
}

void ASMGenerator::CallValue(int procIndex,string a,string b)
{
	int tempCalledProcIndex=atoi(a.c_str());
	string tempCalledProcName=mySymbolTable.tableContext[tempCalledProcIndex].procName;
	cout<<"call\t"<<"id"<<a<<"_"<<tempCalledProcName<<endl;
	cout<<"add\tesp,"<<4*(mySymbolTable.tableContext[tempCalledProcIndex].procArgNum+mySymbolTable.tableContext[tempCalledProcIndex].lev)<<endl;//lev是否要-1？？？？
	//symbol(procIndex,)

	//cout<<"mov\teax,"
	symbol(procIndex,tempCalledProcName,"eax");//symbol函数的作用：从procIndex找s变量，将其值传到eax中
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

void ASMGenerator::symbol(int procIndex,string s,string reg)//结果在reg寄存器里面				//是地址或者非地址、外层用到esi！！！		最后是否要改成edi？？？	// 地址：间接取值；非地址：直接取值
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
				cout<<"mov\t"<<reg<<",dword ptr [esi]"<<endl;
			}
			else
			{
				cout<<"mov\tesi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//可以用eax代替esi的??
				cout<<"sub\tesi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				cout<<"mov\t"<<"edi"<<",dword ptr [esi]"<<endl;//对edi可以换为eax？？？？？？？？
				cout<<"mov\t"<<reg<<",dword ptr [edi]"<<endl;
			}
		}
		else//不是地址
		{
			if(idProcNum==procIndex)//其实应判断层数相等与否？？？？？？
			{
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


void ASMGenerator::symbolAdr(int procIndex,string s,string reg)//地址、外层用到edi！！				//symbolAdr并非绝对取地址，如果s本身存放地址（已存放），则传入s本身。
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
				cout<<"mov\t"<<reg<<",dword ptr [ebp-"<< mySymbolTable.dimCount(idProcNum,idIndex)*4+4 <<"]"<<endl;
			}
			else
			{
				cout<<"mov\tedi,dword ptr [ebp+"<<8+level*4+tempArgNum*4<<"]"<<endl;//可以用eax代替esi的??
				cout<<"sub\tedi,"<<4*mySymbolTable.dimCount(idProcNum,idIndex)+4<<endl;
				cout<<"mov\t"<<reg<<",dword ptr [edi]"<<endl;//对edi可以换为eax？？？？？？？？
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


void ASMGenerator::symbolAdrByte(int procIndex,string s,string reg)//读语句用到	地址、外层用到edi！！				//symbolAdr并非绝对取地址，如果s本身存放地址（已存放），则传入s本身。
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




void ASMGenerator::symbolAdrEqual(int procIndex,string s,string tempreg,string resultreg)//s是左侧的不知是地址还是值的变量,		//注意：外层或者：均取地址调用symbolAdr；不能用edi了
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

