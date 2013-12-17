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
	//freopen("DAG�Ż���Ԫʽ.txt","w",stdout);
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
	if(temp!=-1)//����ڵ�������Ѿ��иñ���
	{
		varID[temp]=i;
	}
	else//����ڵ����û�иñ���
	{
		varName.push_back(id);
		varID.push_back(i);
	}
}

void DAG::arrayEqualAdd(int point)//,int head,int tail)//ע�⣬����Ҫ�ϲ�����������������ȫһ���ı������и���		//��������head��tail֮��ĸ��£���������middlecode�ĸ��£���������tail��һ�����������ֵwriteexpression #t8,��������#t8�Ѿ�������Ϊ#t2
{
	int i;
	for(i=0;i<arrayEqual.codeList.size();i++)
	{
		//��bug������������±�����#��ͷ������ܽ�������������ֵ��ǰ�ˣ�������

		if((myMiddleCode.codeList[point].opr==arrayEqual.codeList[i].opr)&&(myMiddleCode.codeList[point].target1==arrayEqual.codeList[i].target1)&&(myMiddleCode.codeList[point].target2==arrayEqual.codeList[i].target2))//&&(myMiddleCode.codeList[point].result==arrayEqual.codeList[i].result))
		{
			//�����м����
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
	arrayEqual.addCode(myMiddleCode.codeList[point].opr,myMiddleCode.codeList[point].target1,myMiddleCode.codeList[point].target2,myMiddleCode.codeList[point].result);//���ˡ�����������������������������Ӧ����ȣ�����
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
	for(i=head;i<tail;i++)//�Ƚ�����ȡ��ַ����������治��������
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
			//��bug����������±�����#��ͷ������ܰѺ�����ܼ����ֵ�ᵽǰ����
		}
		if(myMiddleCode.codeList[i].opr==":=")
		{
			temp1=findInVar(myMiddleCode.codeList[i].target1);//���ڽڵ��ĵڼ���
			if(temp1==-1)//����ڽڵ���в����ҵ��ñ���	//û�ҵ���˵����Ҷ�ӽڵ�
			{
				if(isVar(myMiddleCode.codeList[i].target1))//�������ʱ��������һ�����
				{
					if(!mySymbolTable.isExists(procNum,myMiddleCode.codeList[i].target1+"_0"))
					{
						mySymbolTable.addNewItem(procNum,myMiddleCode.codeList[i].target1+"_0",INT,0);
					}
					
					//_0�����Ǳ����ʼ��ֵ��
					
					//���������д�ˡ�������������������������������������������
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

					//����ǵ�ַ�Ļ�������Ҫ��һ�䣿����������Ϊ��ַֻ��򵥵ĸ��ƣ��������߶��ǵ�ַ,ɾȥ���м���룺���У�����ɾ��a[3]=a[2]


					//�㷨��_0��������ʲô�������ֵ����Ϊ��ֵ���ܺ�����ı�	:����_0һ�ɲ��ǵ�ַ������

					

					t.Name=myMiddleCode.codeList[i].target1+"_0";//����~��ͷ������Ԫ�ؿ���Ҳ���У�=��_0�Ĵ���
					Opt_Code.addCode(":=",myMiddleCode.codeList[i].target1,"",myMiddleCode.codeList[i].target1+"_0");//��������Ŀ����ʲô��������ֵ����������//���ܻ�ı���������������ֲ�����ʱ���������������ַ����ʱ����
				}
				else
				{
					t.Name=myMiddleCode.codeList[i].target1;//���������������
				}
				t.Left=-1;
				t.Right=-1;
				t.isleaf=true;
				joint.push_back(t);
				linkVar(myMiddleCode.codeList[i].target1,joint.size()-1);
				temp1=varName.size()-1;
			}
			linkVar(myMiddleCode.codeList[i].result,varID[temp1]);//�Ⱥ��ұߵĽ������߰���һ��һ���ڵ���
		}
		//else if(myMiddleCode.codeList[i].opr=="[]=")
		//{
		//}
		else
		{
			temp1=findInVar(myMiddleCode.codeList[i].target1);//���ڽڵ��ĵڼ���
			if(temp1==-1)//û�ҵ���˵����Ҷ�ӽڵ�
			{
				if(isVar(myMiddleCode.codeList[i].target1))
				{
					if(!mySymbolTable.isExists(procNum,myMiddleCode.codeList[i].target1+"_0"))
					{
						mySymbolTable.addNewItem(procNum,myMiddleCode.codeList[i].target1+"_0",INT,0);
					}


					//_0�����Ǳ����ʼ��ֵ��

					//���������д�ˡ�������������������������������������������
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
					Opt_Code.addCode(":=",myMiddleCode.codeList[i].target1,"",myMiddleCode.codeList[i].target1+"_0");//���ܻ�ı���������������ֲ�����ʱ���������������ַ����ʱ����
				}
				else
				{
					t.Name=myMiddleCode.codeList[i].target1;//���������������
				}
				t.Left=-1;
				t.Right=-1;
				t.isleaf=true;
				joint.push_back(t);
				linkVar(myMiddleCode.codeList[i].target1,joint.size()-1);
				temp1=varName.size()-1;
			}


			temp2=findInVar(myMiddleCode.codeList[i].target2);//���ڽڵ��ĵڼ���
			if(temp2==-1)//û�ҵ���˵����Ҷ�ӽڵ�
			{
				if(isVar(myMiddleCode.codeList[i].target2))
				{
					if(!mySymbolTable.isExists(procNum,myMiddleCode.codeList[i].target2+"_0"))
					{
						mySymbolTable.addNewItem(procNum,myMiddleCode.codeList[i].target2+"_0",INT,0);
					}


					//_0�����Ǳ����ʼ��ֵ��

					//���������д�ˡ�������������������������������������������
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
					Opt_Code.addCode(":=",myMiddleCode.codeList[i].target2,"",myMiddleCode.codeList[i].target2+"_0");//���ܻ�ı���������������ֲ�����ʱ���������������ַ����ʱ����
				}
				else
				{
					t.Name=myMiddleCode.codeList[i].target2;//���������������
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

	cout<<"DAGͼ"<<endl;
	cout<<"Name"<<"\t"<<"Left"<<"\t"<<"Right"<<endl;
	for(i=0;i<joint.size();i++)
	{
		cout<<joint[i].Name<<"\t"<<joint[i].Left<<"\t"<<joint[i].Right<<endl;
	}
	cout<<endl<<endl<<endl;

	cout<<"�ڵ��"<<endl;
	cout<<"Name"<<"\t"<<"ID"<<endl;
	for(i=0;i<varName.size();i++)
	{
		//if(isVar(varName[i]))
		cout<<varName[i]<<"\t"<<varID[i]<<endl;
	}
	cout<<endl;


	DAGline();
}




deque<string> DAG::getJointSymbol(int id)//��ȡ�ڵ����ĳ�ڵ��Ӧ���غϵı�������
{
	deque<string> temp;
	int i;
	for(i=0;i<varName.size();i++)
	{
		if(varID[i]==id)//�ڽڵ����������ڵ㣬�ҳ����ж�Ӧ���֡�
		{
			if(varName[i][0]=='#')//��ʱ�������Ҵ�ȡ�Ĳ��������ַ
			{
				temp.push_back(varName[i]);
			}
			else//һ�����������~��ͷ����ȡ���������ַ
			{
				temp.push_front(varName[i]);
			}
		}
	}
	if(temp.size()==0)//����ڽڵ����������û������ڵ㣺�����������õ�����				//���̫�����ˣ�����ָb�����⸳ֵ������£���b������������ڵ㣬���ýڵ㣨Ҷ�����������ж�Ӧ���ţ���ֱ�ӷ��ؽڵ���������	//���������ֵ����
		temp.push_back(joint[id].Name);//ֱ�ӽ��ýڵ��Ӧ�����ַ���ȥ�����������+-* ���� /֮��ķ���		���������߸�ֵ�ţ�����=
	return temp;
}


int DAG::getNoFather()
{
	int i;
	for(i=0;i<joint.size();i++)
	{
		if(father[i]==0)//&&//isCalc(joint[i].Name)||(!(isVar(joint[i].Name))))//������Ҫ�ر�ע�⣺�Ƿ���Ҫ����[]=	//���ڿ��ܻ��и�ֵ�������ʹĳ������ֵ������ֵһ����
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

	while((temp=getNoFather())!=-1)//����ʽ�㷨��ʵ��������ͼ
	{
		do 
		{
			line.push_front(joint[temp]);//line������ȡjoint�ڵ㣨����ģ�����Left��Right�ȵȣ�
			lineNo.push_front(temp);//lineNo������ȡjoint�ı��
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
					break;																					//�����A:b_0:=b_0; B:b=b_0���������b_0=b;
				if(line[i].Name!=symbols[j]+"_0"&&isVar(symbols[j])    &&line[i].Name!=symbols[j])		//������ô�죿��b_0=b������֣���
					Opt_Code.addCode(":=",line[i].Name,"",symbols[j]);									//b:=b_0��û��Ҫ��д�ˣ�˵��bû�б仯������ԭ���ڵ���(�����b�ı���b=c;b=b0;��Ȼ������)	b_0:=b_0;˵��symbols[j]��b_0��b_0������Ҷ�ڵ�(���ҽڵ���-1,-1)���Ҳ���������֮��ȵ����˵������
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



	//����Ĵ����Ƿ���Ҫ������
	if((myMiddleCode.codeList[point].opr=="WriteExpression"||
		myMiddleCode.codeList[point].opr=="IfFalse"||		//�����Ż��󲻻��������Ԫʽ
		myMiddleCode.codeList[point].opr=="value=")&&myMiddleCode.codeList[point].target1[0]=='#')//������������õ��ľֲ�����
	{
			symbols=getJointSymbol(getPoint(myMiddleCode.codeList[point].target1));
			if (myMiddleCode.codeList[point].target1!=symbols[0]&&isVar(myMiddleCode.codeList[point].target1))		//isVarӦ����ȷ���ģ���Ϊ��#��ͷ���������Ż����࣬ʹ��#�ֲ�������ΪҶ�ڵ㣿��	��Ҷ�ڵ㶼��������
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