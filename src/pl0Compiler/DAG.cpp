#include "DAG.h"
#include "GlobalData.h"
#include <iostream>
using namespace std;

int DAG::point;
int DAG::tail;
DAGCode DAG::Opt_Code;
deque<string> DAG::varName;
deque<int> DAG::varID;
deque<DAGItem> DAG::joint;
deque<DAGItem> DAG::line;
deque<int> DAG::lineNo;
int DAG::father[10000];
int DAG::procNum;

void DAG::addOptCode(int id)
{
	Opt_Code.addCode(myMiddleCode.codeList[id].opr,myMiddleCode.codeList[id].target1,myMiddleCode.codeList[id].target2,myMiddleCode.codeList[id].result);
}

int DAG::isCalc(string op)
{
	if(op=="+"||op=="-"||op=="*"||op=="/"||op==":="||op=="[]=") return 1;
	//if(op=="+"||op=="-"||op=="*"||op=="/"||op==":="||op==">"||op==">="||op=="<"||op=="<="||op=="="||op=="<>"||op=="[]=") return 1;
	else return 0;
}

void DAG::DAGinit()
{
	int pre;
	point=0;
	tail=myMiddleCode.codeList.size();
	while(point<tail)
	{
		while(point<tail&&isCalc(myMiddleCode.codeList[point].opr)==0)
		{
			if(myMiddleCode.codeList[point].opr=="Begin")
			{
				procNum=atoi(myMiddleCode.codeList[point].target1.c_str());
			}
			addOptCode(point);
			point++;
		}
		pre=point;
		while(point<tail&&isCalc(myMiddleCode.codeList[point].opr)==1)
		{
			point++;
		}
		if(point<tail) DAGdeal(pre,point);
	}
	freopen("MiddleCode(DAG).txt","w",stdout);
	//freopen("DAG优化四元式.txt","w",stdout);
	cout<<"DAGCode"<<endl;
	Opt_Code.show();
	updateMiddleCode();
}

int DAG::findInJoint(string name,int left,int right)
{
	int i;
	for(i=0;i<joint.size();i++)
	{
		if(joint[i].Name==name&&joint[i].Left==left&&joint[i].Right==right)
		{
			return i;
		}
	}
	return -1;
}

int DAG::findInVar(string name)
{
	int i;
	for(i=0;i<varName.size();i++)
	{
		if(varName[i]==name)
			return i;
	}
	return -1;
}

int DAG::isVar(string id)
{
	if(id[0]=='#') return 1;
	if(id[0]=='~') return 1;
	if(id[0]>='a'&&id[0]<='z') return 1;
	if(id[0]>='A'&&id[0]<='Z') return 1;
	return 0;
}

void DAG::linkVar(string id,int i)
{
	int temp;
	temp=findInVar(id);
	if(temp!=-1)//如果节点表里面已经有该变量
	{
		varID[temp]=i;
	}
	else//如果节点表里没有该变量
	{
		varName.push_back(id);
		varID.push_back(i);
	}
}

void DAG::arrayEqualAdd(int point)//,int head,int tail)//注意，这里要合并变量：将代码中完全一样的变量进行更新		//还不能是head和tail之间的更新，而是整个middlecode的更新！！！可能tail下一句是输出数组值writeexpression #t8,而在这里#t8已经被更新为#t2
{
	int i;
	for(i=0;i<arrayEqual.codeList.size();i++)
	{
		//有bug：如果是数组下标是以#开头，则可能将后面才能算出的值提前了！！！！

		if((myMiddleCode.codeList[point].opr==arrayEqual.codeList[i].opr)&&(myMiddleCode.codeList[point].target1==arrayEqual.codeList[i].target1)&&(myMiddleCode.codeList[point].target2==arrayEqual.codeList[i].target2))//&&(myMiddleCode.codeList[point].result==arrayEqual.codeList[i].result))
		{
			//更新中间代码
			string oldstr=myMiddleCode.codeList[point].result;
			string newstr=arrayEqual.codeList[i].result;
			//for(int j=head;j<tail;j++)
			for(int j=0;j<myMiddleCode.codeList.size();j++)
			{
				if(myMiddleCode.codeList[j].target1==oldstr)
				{
					myMiddleCode.codeList[j].target1=newstr;
				}
				if(myMiddleCode.codeList[j].target2==oldstr)
				{
					myMiddleCode.codeList[j].target2=newstr;
				}
				if(myMiddleCode.codeList[j].result==oldstr)
				{
					myMiddleCode.codeList[j].result=newstr;
				}
			}
			return;
		}
	}
	arrayEqual.addCode(myMiddleCode.codeList[point].opr,myMiddleCode.codeList[point].target1,myMiddleCode.codeList[point].target2,myMiddleCode.codeList[point].result);//错了。。。。。。。。。。。。。。对应的相等，赋入
}


void DAG::fatherAdd(int child,int father)
{
	int i;
	for(i=0;i<joint[child].FatherNode.size();i++)
	{
		if(father==joint[child].FatherNode[i])
			return;
	}
	joint[child].FatherNode.push_back(father);
}


void DAG::DAGdeal(int head,int tail)
{
	int i;
	int temp1;
	int temp2;
	int temp3;
	DAGItem t;
	joint.clear();
	varName.clear();
	varID.clear();
	arrayEqual.codeList.clear();
	for(i=head;i<tail;i++)//先将数组取地址的输出，后面不做处理了
	{
		if(myMiddleCode.codeList[i].opr=="[]=")
		{
			arrayEqualAdd(i);
		}
	}
	for(int j=0;j<arrayEqual.codeList.size();j++)
	{
		Opt_Code.addCode(arrayEqual.codeList[j].opr,arrayEqual.codeList[j].target1,arrayEqual.codeList[j].target2,arrayEqual.codeList[j].result);
	}
	for(i=head;i<tail;i++)
	{
		if(myMiddleCode.codeList[i].opr=="[]=")
		{
			continue;
			//有bug：如果数组下标是以#开头，则可能把后面才能计算的值提到前面了
		}
		if(myMiddleCode.codeList[i].opr==":=")
		{
			temp1=findInVar(myMiddleCode.codeList[i].target1);//看在节点表的第几行
			if(temp1==-1)//如果在节点表中不能找到该变量	//没找到就说明是叶子节点
			{
				if(isVar(myMiddleCode.codeList[i].target1))//如果是临时变量或者一般变量
				{
					if(!mySymbolTable.isExists(procNum,myMiddleCode.codeList[i].target1+"_0"))
					{
						mySymbolTable.addNewItem(procNum,myMiddleCode.codeList[i].target1+"_0",INT,0);
					}
					
					//_0作用是保存初始“值”
					
					//最后还是这样写了。。。。。。。。。。。。。。。。。。。。。。
					//int idProcNum=-1,idIndex=-1;
					//mySymbolTable.getIdNameAdr(procNum,myMiddleCode.codeList[i].target1,idProcNum,idIndex);

					//if(idProcNum==-1||idIndex==-1)
					//{
					//	//nothingtodo
					//}
					//else
					//{
					//	int tempSize=mySymbolTable.tableContext[procNum].itemList.size();
					//	mySymbolTable.tableContext[procNum].itemList[tempSize-1].isnotadr=mySymbolTable.tableContext[idProcNum].itemList[idIndex].isnotadr;
					//}

					//如果是地址的话，则不需要这一句？？？？？因为地址只会简单的复制：甚至两边都是地址,删去此中间代码：不行，不能删：a[3]=a[2]


					//算法中_0的作用是什么？保存初值，因为其值可能后来会改变	:所以_0一律不是地址？？？

					

					t.Name=myMiddleCode.codeList[i].target1+"_0";//这样~开头的数组元素可能也会有：=加_0的处理。
					Opt_Code.addCode(":=",myMiddleCode.codeList[i].target1,"",myMiddleCode.codeList[i].target1+"_0");//这样做的目的是什么？保留初值？？？？？//可能会改变的量！！！！！局部非临时变量或者是数组地址的临时变量
				}
				else
				{
					t.Name=myMiddleCode.codeList[i].target1;//如果是立即数？？
				}
				t.Left=-1;
				t.Right=-1;
				t.isleaf=true;
				joint.push_back(t);
				linkVar(myMiddleCode.codeList[i].target1,joint.size()-1);
				temp1=varName.size()-1;
			}
			linkVar(myMiddleCode.codeList[i].result,varID[temp1]);//等号右边的结果与左边绑定在一起，一个节点上
		}
		//else if(myMiddleCode.codeList[i].opr=="[]=")
		//{
		//}
		else
		{
			temp1=findInVar(myMiddleCode.codeList[i].target1);//看在节点表的第几行
			if(temp1==-1)//没找到就说明是叶子节点
			{
				if(isVar(myMiddleCode.codeList[i].target1))
				{
					if(!mySymbolTable.isExists(procNum,myMiddleCode.codeList[i].target1+"_0"))
					{
						mySymbolTable.addNewItem(procNum,myMiddleCode.codeList[i].target1+"_0",INT,0);
					}


					//_0作用是保存初始“值”

					//最后还是这样写了。。。。。。。。。。。。。。。。。。。。。。
					//int idProcNum=-1,idIndex=-1;
					//mySymbolTable.getIdNameAdr(procNum,myMiddleCode.codeList[i].target1,idProcNum,idIndex);

					//if(idProcNum==-1||idIndex==-1)
					//{
					//	//nothingtodo
					//}
					//else
					//{
					//	int tempSize=mySymbolTable.tableContext[procNum].itemList.size();
					//	mySymbolTable.tableContext[procNum].itemList[tempSize-1].isnotadr=mySymbolTable.tableContext[idProcNum].itemList[idIndex].isnotadr;
					//}

					t.Name=myMiddleCode.codeList[i].target1+"_0";
					Opt_Code.addCode(":=",myMiddleCode.codeList[i].target1,"",myMiddleCode.codeList[i].target1+"_0");//可能会改变的量！！！！！局部非临时变量或者是数组地址的临时变量
				}
				else
				{
					t.Name=myMiddleCode.codeList[i].target1;//如果是立即数？？
				}
				t.Left=-1;
				t.Right=-1;
				t.isleaf=true;
				joint.push_back(t);
				linkVar(myMiddleCode.codeList[i].target1,joint.size()-1);
				temp1=varName.size()-1;
			}


			temp2=findInVar(myMiddleCode.codeList[i].target2);//看在节点表的第几行
			if(temp2==-1)//没找到就说明是叶子节点
			{
				if(isVar(myMiddleCode.codeList[i].target2))
				{
					if(!mySymbolTable.isExists(procNum,myMiddleCode.codeList[i].target2+"_0"))
					{
						mySymbolTable.addNewItem(procNum,myMiddleCode.codeList[i].target2+"_0",INT,0);
					}


					//_0作用是保存初始“值”

					//最后还是这样写了。。。。。。。。。。。。。。。。。。。。。。
					//int idProcNum=-1,idIndex=-1;
					//mySymbolTable.getIdNameAdr(procNum,myMiddleCode.codeList[i].target2,idProcNum,idIndex);

					//if(idProcNum==-1||idIndex==-1)
					//{
					//	//nothingtodo
					//}
					//else
					//{
					//	int tempSize=mySymbolTable.tableContext[procNum].itemList.size();
					//	mySymbolTable.tableContext[procNum].itemList[tempSize-1].isnotadr=mySymbolTable.tableContext[idProcNum].itemList[idIndex].isnotadr;
					//}

					t.Name=myMiddleCode.codeList[i].target2+"_0";
					Opt_Code.addCode(":=",myMiddleCode.codeList[i].target2,"",myMiddleCode.codeList[i].target2+"_0");//可能会改变的量！！！！！局部非临时变量或者是数组地址的临时变量
				}
				else
				{
					t.Name=myMiddleCode.codeList[i].target2;//如果是立即数？？
				}
				t.Left=-1;
				t.Right=-1;
				t.isleaf=true;
				joint.push_back(t);
				linkVar(myMiddleCode.codeList[i].target2,joint.size()-1);




				temp2=varName.size()-1;
			}


			temp3=findInJoint(myMiddleCode.codeList[i].opr,varID[temp1],varID[temp2]);
			if(temp3==-1)
			{
				t.Name=myMiddleCode.codeList[i].opr;
				t.Left=varID[temp1];
				t.Right=varID[temp2];
				t.isleaf=false;
				joint.push_back(t);
				temp3=joint.size()-1;
				//joint[joint[temp3].Left].FatherNode.push_back(temp3);
				//joint[joint[temp3].Right].FatherNode.push_back(temp3);

				fatherAdd(joint[temp3].Left,temp3);
				fatherAdd(joint[temp3].Right,temp3);
			}
			linkVar(myMiddleCode.codeList[i].result,temp3);
		}
	}

	freopen("DAG.txt","w",stdout);
	cout<<endl;

	cout<<"DAG图"<<endl;
	cout<<"Name"<<"\t"<<"Left"<<"\t"<<"Right"<<endl;
	for(i=0;i<joint.size();i++)
	{
		cout<<joint[i].Name<<"\t"<<joint[i].Left<<"\t"<<joint[i].Right<<endl;
	}
	cout<<endl<<endl<<endl;

	cout<<"节点表"<<endl;
	cout<<"Name"<<"\t"<<"ID"<<endl;
	for(i=0;i<varName.size();i++)
	{
		//if(isVar(varName[i]))
		cout<<varName[i]<<"\t"<<varID[i]<<endl;
	}
	cout<<endl;


	DAGline();
}




deque<string> DAG::getJointSymbol(int id)//获取节点表中某节点对应的重合的变量名。
{
	deque<string> temp;
	int i;
	for(i=0;i<varName.size();i++)
	{
		if(varID[i]==id)//在节点表中有这个节点，找出所有对应名字。
		{
			if(varName[i][0]=='#')//临时变量，且存取的不是数组地址
			{
				temp.push_back(varName[i]);
			}
			else//一般变量或者以~开头，存取的是数组地址
			{
				temp.push_front(varName[i]);
			}
		}
	}
	if(temp.size()==0)//如果在节点表中所有行没有这个节点：这个在哪里会用到？？				//这句太精彩了！！是指b被另外赋值的情况下（即b标记在了其他节点，而该节点（叶？？）不再有对应符号）：直接返回节点现有名字	//或者是数字的情况
		temp.push_back(joint[id].Name);//直接将该节点对应的名字放里去：这里可能是+-* 或者 /之类的符号		？？？或者赋值号？？：=
	return temp;
}


int DAG::getNoFather()
{
	int i;
	for(i=0;i<joint.size();i++)
	{
		if(father[i]==0)//&&//isCalc(joint[i].Name)||(!(isVar(joint[i].Name))))//这里需要特别注意：是否需要区别[]=	//由于可能会有赋值的情况：使某个变量值与根结点值一样。
		{
			return i;
		}
	}
	return -1;
}


void DAG::DAGline()
{
	deque<string>symbols;
	int i,j,temp;

	line.clear();
	lineNo.clear();
	for(i=0;i<joint.size();i++)
	{
		father[i]=0;
	}
	for(i=0;i<joint.size();i++)
	{
		if(father[joint[i].Left]!=-1)
		{
			father[joint[i].Left]++;
			father[joint[i].Right]++;
		}
	}

	while((temp=getNoFather())!=-1)//启发式算法，实质是拓扑图
	{
		do 
		{
			line.push_front(joint[temp]);//line用来存取joint节点（具体的，包括Left，Right等等）
			lineNo.push_front(temp);//lineNo用来存取joint的编号
			if(father[joint[temp].Left]!=-1)
			{
				father[joint[temp].Left]--;
				father[joint[temp].Right]--;
				father[temp]=-1;

				temp=joint[temp].Left;
				if(temp==-1||father[temp]!=0) break;
			}
			else
			{
				break;
			}			
		} while (1);
	}


	for(i=0;i<line.size();i++)
	{
		if(line[i].Left==-1)
		{
			symbols=getJointSymbol(lineNo[i]);
			for(j=0;j<symbols.size();j++)
			{
				if(symbols[j][0]=='#')
					break;																					//会出现A:b_0:=b_0; B:b=b_0但不会出现b_0=b;
				if(line[i].Name!=symbols[j]+"_0"&&isVar(symbols[j])    &&line[i].Name!=symbols[j])		//数组怎么办？？b_0=b不会出现？？
					Opt_Code.addCode(":=",line[i].Name,"",symbols[j]);									//b:=b_0就没必要再写了，说明b没有变化，还在原来节点上(如果是b改变了b=c;b=b0;显然不可能)	b_0:=b_0;说明symbols[j]是b_0，b_0出现在叶节点(左右节点是-1,-1)，找不到其他与之相等的数了的情况下
			}
		}
		else
		{
			symbols=getJointSymbol(lineNo[i]);
			deque<string>symbolsleft=getJointSymbol(line[i].Left);
			deque<string>symbolsright=getJointSymbol(line[i].Right);
			Opt_Code.addCode(line[i].Name,symbolsleft[0],symbolsright[0],symbols[0]);
			for(j=1;j<symbols.size();j++)
			{
				if(symbols[j][0]=='#')
					break;
				Opt_Code.addCode(":=",symbols[0],"",symbols[j]);
			}

		}
	}





	/*
	for (i=0;i<joint.size();i++){
		if (joint[i].Left==-1){
			symbols=getJointSymbol(i);
			for (j=0;j<symbols.size();j++){
				if (symbols[j][0]=='#')
					break;
				if (joint[i].Name!=symbols[j]+"_0"&&isVar(symbols[j]) &&  joint[i].Name!=symbols[j])
					Opt_Code.addCode(":=",joint[i].Name,"",symbols[j]);
			}
		}
		else{
			symbols=getJointSymbol(i);
			Opt_Code.addCode(joint[i].Name,getJointSymbol(joint[i].Left)[0],getJointSymbol(joint[i].Right)[0],symbols[0]);
			for (j=1;j<symbols.size();j++){
				if (symbols[j][0]=='#')
					break;
				Opt_Code.addCode(":=",symbols[0],"",symbols[j]);
			}
		}
	}*/










	//string temptemp=myMiddleCode.codeList[point].op;



	//下面的代码是否需要？？？
	if((myMiddleCode.codeList[point].opr=="WriteExpression"||
		myMiddleCode.codeList[point].opr=="IfFalse"||		//窥孔优化后不会有这个四元式
		myMiddleCode.codeList[point].opr=="value=")&&myMiddleCode.codeList[point].target1[0]=='#')//有下面的语句会用到的局部变量
	{
			symbols=getJointSymbol(getPoint(myMiddleCode.codeList[point].target1));
			if (myMiddleCode.codeList[point].target1!=symbols[0]&&isVar(myMiddleCode.codeList[point].target1))		//isVar应该是确定的，因为以#开头。这是怕优化过多，使得#局部变量成为叶节点？？	非叶节点都可以生成
				Opt_Code.addCode(":=",symbols[0],"",myMiddleCode.codeList[point].target1);
	}

}


int DAG::getPoint(string id)
{
	int i;
	for (i=0;i<varName.size();i++)
		if(varName[i]==id)
			return varID[i];
	return -1;
}

void DAG::updateMiddleCode()
{
	int i;
	myMiddleCode.codeList.clear();
	for (i=0;i<Opt_Code.codeList.size();i++){
		myMiddleCode.addMiddleCode(Opt_Code.codeList[i].opr,
			Opt_Code.codeList[i].target1,
			Opt_Code.codeList[i].target2,
			Opt_Code.codeList[i].result);
	}
}