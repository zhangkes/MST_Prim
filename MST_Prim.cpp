// MST_Prim.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include"MGraph.h"
int _tmain(int argc, _TCHAR* argv[])
{
	//MSTPrim_Test();
	cout << "�ڽӾ���洢 - Prim��С�������㷨" << endl;
	MGraph Graph = BuildMGraph();
	MGraph MST=NULL;
	ShowGraph(Graph);
	cout << endl << "--------------------" << endl;
	MiniSpanTree_Prim(Graph, MST);
	return 0;
}