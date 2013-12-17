#include "ErrorHandler.h"
#include "GlobalData.h"
#include <iostream>

using namespace std;

int ErrorHandler::errorTot=0;
//int Error

//ErrorHandler::ErrorHandler()
//{
//	errorTot=0;
//}


int ErrorHandler::isType(string s)
{
	if(s=="int"||s=="char")
		return 1;
	return 0;
}

int ErrorHandler::isAllStartMark(string s)
{
	if(s=="const"||s=="var"||s=="function"||s=="procedure"||s=="begin"||s==".")
		return 1;
	return 0;
}


int ErrorHandler::isStartMark1(string s)
{
	if(s=="var"||s=="function"||s=="procedure"||s=="begin"||s==".")
		return 1;
	return 0;
}

int ErrorHandler::isStartMark2(string s)
{
	if(s=="function"||s=="procedure"||s=="begin"||s==".")
		return 1;
	return 0;
}

int ErrorHandler::isbaseType(string s)
{
	if(s=="integer"||s=="char")
		return 1;
	else return 0;
}

void ErrorHandler::lexerError(int x,int y, string message)
{
	cout<<"(�кţ��к�):";
	if(message=="�ַ��к��зǷ��ַ�")
	{
		cout<<"("<<x<<","<<y<<"):�ʷ��������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�ַ����ȱ��ƥ��\'")
	{
		cout<<"("<<x<<","<<y<<"):�ʷ��������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�ַ������ȱ��ƥ��\"")
	{
		cout<<"("<<x<<","<<y<<"):�ʷ��������ִ���"<<message<<endl;

		//����������ֱ����/"��ֹͣ�ķ����вŴ��ڣ�����ɾ����
		cout<<"compile exit!"<<endl;
		exit(0);

		return;
	}

	else if(message=="�ַ����ﺬ�з����֡�����ĸ���ǿո���ַ�")
	{
		cout<<"("<<x<<","<<y<<"):�ʷ��������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����ʶ����ַ�")
	{
		cout<<"("<<x<<","<<y<<"):�ʷ��������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�ַ��к��зǷ��ַ�")
	{
		cout<<"("<<x<<","<<y<<"):�ʷ��������ִ���"<<message<<endl;
		return;
	}
	else
	{
		cout<<"wrong error!"<<endl;
	}
}



void ErrorHandler::parserError(int x,int y,string message)
{
	cout<<"(�кţ��к�):";
	//if(message=="����û�з���ֵ")
	//{
	//	cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<",Ĭ�Ϸ���ֵΪ0"<<endl;//Ĭ�Ϸ���ֵΪ0
	//	return;
	//}
	//else 
	if(message=="�ֳ�����û�и������")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		if(myParser.sym.symbol==";")
		{
			cout<<"\t���ֶ�ηֺ�;"<<endl;
			myParser.sym=myLexer.getSym();
		}

		while(!isStartMark2(myParser.sym.symbol)&&myParser.sym.kind!=-1)
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}
	else if(message=="����˵�����ֿ�ʼû��const")//һ�㲻���õ�
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����˵���������ȱ��;")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;

		while(myParser.sym.kind!=21&&!isStartMark1(myParser.sym.symbol)&&myParser.sym.kind!=-1)
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		//������Ҫ����������
		return;
	}
	else if(message=="�������忪ʼ���Ǳ�ʶ��")//��ʼʱconst�ظ�����Ĵ������ͨ����һ��ͬ�����˵�
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		//if(myParser.sym.symbol=="const")
		//{
		//	cout<<"\tconst�ظ�����"<<endl;
		//	myParser.sym=myLexer.getSym();
		//	return;
		//}
		while(myParser.sym.symbol!=";"&&myParser.sym.symbol!=","&&myParser.sym.kind!=-1&&!isStartMark1(myParser.sym.symbol))
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}
	else if(message=="��������û���м��=")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;

		if(myParser.sym.symbol==":=")
		{
			cout<<"\t��Ӧ��:=,��Ӧ����="<<endl;
			myParser.sym=myLexer.getSym();
			return;
		}
		while(myParser.sym.symbol!=";"&&myParser.sym.symbol!=","&&myParser.sym.kind!=-1&&!isStartMark1(myParser.sym.symbol))
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		//�Ƿ�Ҫ����ʲô��
		//ֱ����һ����ʶ����������
		return;
	}
	else if(message=="�������ֲ����϶���ķ���")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		while(myParser.sym.symbol!=";"&&myParser.sym.symbol!=","&&!isStartMark1(myParser.sym.symbol)&&myParser.sym.kind!=-1)
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}
	else if(message=="����˵�����ֿ�ʼû��var")//һ�㲻���õ�
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����˵������ȱ��;")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;


		while(myParser.sym.kind!=21&&!isStartMark2(myParser.sym.symbol)&&myParser.sym.kind!=-1)
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}


	else if(message=="����˵����ʼ���Ǳ�ʶ��")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		//if(myParser.sym.symbol=="var")
		//{
		//	cout<<"\tvar�ظ�����"<<endl;
		//	myParser.sym=myLexer.getSym();
		//	return;
		//}
		while(myParser.sym.symbol!=";"&&myParser.sym.symbol!=","&&myParser.sym.kind!=-1&&!isStartMark2(myParser.sym.symbol))
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}


	else if(message=="����˵��ȱ��:")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		
		while(myParser.sym.kind!=-1&&!isStartMark2(myParser.sym.symbol)&&!isbaseType(myParser.sym.symbol))
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}


	else if(message=="����˵��û������")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����˵����������ȱ��[")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����˵����������ȱ��]")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����˵����������ȱ��of")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����˵�����鶨��û�л�������")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����˵������ȱ��;")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		while(!isStartMark2(myParser.sym.symbol)&&myParser.sym.kind!=-1)
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}
	else if(message=="�����ײ�ȱ��function")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����ײ���ʼȱ�ٱ�ʶ��")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����ײ�ȱ��(")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����ײ�ȱ��)")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����ײ�ȱ��:")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		while(myParser.sym.symbol!=";"&&myParser.sym.kind!=-1&&!isStartMark2(myParser.sym.symbol)&&!isbaseType(myParser.sym.symbol))
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}
	else if(message=="�����ײ�û�����û�������")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}

	else if(message=="�����ײ�ȱ��;")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����˵������ȱ��;")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����ײ�ȱ��function")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����ײ���ʼȱ�ٱ�ʶ��")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����ײ�ȱ��(")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����ײ�ȱ��)")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����ײ�ȱ��;")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}


	else if(message=="��ʽ������ȱ�ٱ�־��")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="��ʽ�����ο�ʼ���Ǳ�ʶ��")
	{
		//���������/,���ߡ�����sym=nextsym
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="��ʽ������ȱ��:")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		while(myParser.sym.symbol!=";"&&myParser.sym.kind!=-1&&!isStartMark2(myParser.sym.symbol)&&!isbaseType(myParser.sym.symbol))
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}
	else if(message=="��ʽ������ȱ�ٻ������Ͷ���")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="������俪ʼû��begin")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="��������βû��end���߸�������м�û�зֺ�")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����Ա�ʶ����ͷ��ȴ���Ǹ�ֵ�������̵������")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="��ֵ��俪ʼ���Ǳ�ʶ��")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����������ȱ��]")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="��ֵ����forѭ�����ȱ��:=")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		if(myParser.sym.symbol=="=")
		{
			cout<<"\t\t\t:=��д��=,�������������"<<endl;
			myParser.sym=myLexer.getSym();
			return;
		}
		return;
	}
	else if(message=="��ֵ���ȱ��:=��������û��[]���")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;

		return;
	}

	else if(message=="����(<���ʽ>)ȱ��)")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����ʶ�������")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����в�Ӧ���ַ�")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}

	else if(message=="��������ȱ�ٱ�ʶ��")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="��������ȱ�٣�")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="���̵���ȱ�ٱ�ʶ��")
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="���̵���ȱ�٣�")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="���̵���ȱ��)")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="���������������(")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="��������������ˣ�")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�������ȱ��then")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="������û�й�ϵ�����")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����俪ʼȱ��case")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����俪ʼȱ��of")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="������ȱ��end")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="��������ȱ��:")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		while(myParser.sym.symbol!=";"&&myParser.sym.kind!=-1&&!isStartMark2(myParser.sym.symbol)&&!isbaseType(myParser.sym.symbol))
		{
			myParser.sym=myLexer.getSym();
		}
		if(myParser.sym.kind==-1) exit(0);
		return;
	}
	else if(message=="forѭ����俪ʼȱ��for")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="forѭ����俪ʼȱ�ٱ�ʶ��")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="forѭ�����ȱ��to/downto")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="forѭ�����ȱ��do")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����俪ʼȱ��read")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����ȱ��(")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}

	else if(message=="δ�������ǳ����������鵫���ܸ�ֵ")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else if(message=="�����ȱ�ٱ�ʶ��")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}

	else if(message=="�����ȱ��)")
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="д��俪ʼȱ��write")
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="д���ȱ��(")
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="д���ȱ��)")
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="������û�н�β.")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else 
	{
		cout<<"wrong error"<<endl;
	}

}


/*
void ErrorHandler::sentenceError(int x,int y, string message)
{
	if(message=="��ֵ����б�ʶ��������]����������Ԫ��")
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="��ֵ����е�ʽ��߱���δ����|���Ǳ���|�ǹ��̱���|������Ԫ�ص�����û��[")
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����ʹ��û�ж��塢���ܶ�ȡ�ı�ʶ��")
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����ʹ�ô���")
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����ʹ��û�ж��������")
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����������ȷ|Ӧ���Ǻ�����������ȴ�ǹ���")
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="������/���̲���ȷ")//ǰ�滹��fun1��������Ϣ������������������
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="������Ŀ����ȷ")
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="��������ȱ��)")
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="����������ȷ|Ӧ���ǹ��̣�������ȴ�Ǻ���")
	{
		cout<<"("<<x<<","<<y<<"):����������ִ���"<<message<<endl;
		return;
	}
	else if(message=="forѭ������б�ʶ��δ������߲��Ǳ�������������Ԫ��")
	{
		cout<<"("<<x<<","<<y<<"):�﷨�������ִ���"<<message<<endl;
		return;
	}
	else
	{
	}
}*/


void ErrorHandler::declareError(int x,int y,string message)
{
	cout<<"(�кţ��к�):";
	cout<<"("<<x<<","<<y<<"):�������ִ���:"<<message<<endl;
}


void ErrorHandler::sentenceError(int x,int y ,string message)
{
	cout<<"(�кţ��к�):";
	cout << "(" << x << "," << y << "):���巢�ִ���:" << message << endl;
}