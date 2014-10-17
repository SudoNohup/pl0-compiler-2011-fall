#pragma once

#include <deque>
#include <string>


using namespace std;

class ConflictGraphItem
{
public:
	int No;
	int procIndex;
	//string s;
	int idNo;
	deque<int> itemNo;
	int regNum;
};

class ConflictGraph
{
public:
	static deque <ConflictGraphItem> ConflictTable;
	static deque <ConflictGraphItem> ConflictTableCopy;
	static deque <int> colorRecord;
	void addTableItem(int procIndex,int idNo);
	void conflictGraphBuilt();
	void addConflictRelation(int idProcNum1,int idIndex1,int idProcNum2,int idIndex2);
	void conflictGraphPrint();
	void color();
	void deletePoint(int PointNum);
	void assignColor(int PointNum,int regTotal);
	void color(int regtotal);
	bool Duplicate(int PointNum,int ColorNum);
	void colorPrint();
	void RegisterToSymbolTable();
};
