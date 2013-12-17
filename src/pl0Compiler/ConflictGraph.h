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
	void ConflictGraph::addTableItem(int procIndex,int idNo);
	void ConflictGraph::conflictGraphBuilt();
	void ConflictGraph::addConflictRelation(int idProcNum1,int idIndex1,int idProcNum2,int idIndex2);
	void ConflictGraph::conflictGraphPrint();
	void ConflictGraph::color();
	void ConflictGraph::deletePoint(int PointNum);
	void ConflictGraph::assignColor(int PointNum,int regTotal);
	void ConflictGraph::color(int regtotal);
	bool ConflictGraph::Duplicate(int PointNum,int ColorNum);
	void ConflictGraph::colorPrint();
	void ConflictGraph::RegisterToSymbolTable();
};
