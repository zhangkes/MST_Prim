// MST_Prim.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"MGraph.h"
int _tmain(int argc, _TCHAR* argv[])
{
	//MSTPrim_Test();
	cout << "邻接矩阵存储 - Prim最小生成树算法" << endl;
	MGraph Graph = BuildMGraph();
	MGraph MST=NULL;
	ShowGraph(Graph);
	cout << endl << "--------------------" << endl;
	MiniSpanTree_Prim(Graph, MST);
	return 0;
}