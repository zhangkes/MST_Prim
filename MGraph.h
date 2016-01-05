/*
*
*  V0.2015112721	  新增FLoyd算法：求得任意一对顶点之间的最短路径
*
*/
#include<iostream>
#include <malloc.h>
#include <conio.h>
using namespace std;

#define ERROR 0
/* 图的邻接矩阵表示法 */

#define MaxVertexNum 100        /* 最大顶点数设为100 */
#define INFINITY 99              /* ∞设为双字节无符号整数的最大值65535*/
typedef int Vertex;             /* 用顶点下标表示顶点,为整型 */
typedef int WeightType;         /* 边的权值设为整型 */
typedef char DataType;          /* 顶点存储的数据类型设为字符型 */
bool  Visited[MaxVertexNum];

/* 边的定义 */
typedef struct ENode *PtrToENode;
struct ENode{
	Vertex V1, V2;      /* 有向边<V1, V2> */
	WeightType Weight;  /* 权重 */
};
typedef PtrToENode Edge;

/* 图结点的定义 */
typedef struct GNode *PtrToGNode;
struct GNode{
	int Nv;  /* 顶点数 */
	int Ne;  /* 边数   */
	WeightType G[MaxVertexNum][MaxVertexNum];  /* 邻接矩阵 */
	DataType Data[MaxVertexNum];               /* 存顶点的数据 */
	/* 注意：很多情况下，顶点无数据，此时Data[]可以不用出现 */
};
typedef PtrToGNode MGraph; /* 以邻接矩阵存储的图类型 */


MGraph CreateMGraph(int VertexNum)
{ /* 初始化一个有VertexNum个顶点但没有边的图 */
	Vertex V, W;
	MGraph Graph;

	Graph = (MGraph)malloc(sizeof(struct GNode)); /* 建立图 */
	Graph->Nv = VertexNum;
	Graph->Ne = 0;
	/* 初始化邻接矩阵 */
	/* 注意：这里默认顶点编号从0开始，到(Graph->Nv - 1) */
	for (V = 0; V<Graph->Nv; V++)
		for (W = 0; W<Graph->Nv; W++)
			Graph->G[V][W] = INFINITY;

	return Graph;
}

void InsertEdge(MGraph Graph, Edge E)
{
	/* 插入边 <V1, V2> */
	Graph->G[E->V1][E->V2] = E->Weight;
	/* 若是无向图，还要插入边<V2, V1> */
	Graph->G[E->V2][E->V1] = E->Weight;
}

MGraph BuildMGraph()
{
	MGraph Graph;
	Edge E;
	Vertex V;
	int Nv, i;

	cout << "输入顶点的个数和边数" << endl;
	cin >> Nv;   /* 读入顶点个数 */
	Graph = CreateMGraph(Nv); /* 初始化有Nv个顶点但没有边的图 */

	cin >> Graph->Ne; /* 读入边数 */
	if (Graph->Ne != 0) { /* 如果有边 */
		E = (Edge)malloc(sizeof(struct ENode)); /* 建立边结点 */
		/* 读入边，格式为"起点 终点 权重"，插入邻接矩阵 */
		for (i = 0; i<Graph->Ne; i++) {
			cout << "读入边，格式为 起点 终点 权重 " << endl;
			cin >> E->V1 >> E->V2 >> E->Weight;
			/* 注意：如果权重不是整型，Weight的读入格式要改 */
			InsertEdge(Graph, E);
		}
	}

	/* 如果顶点有数据的话，读入数据 */
	/*for (V = 0; V < Graph->Nv; V++)
	{
	cout << "第" << V << "个顶点的数据 " << endl;
	cin >> Graph->Data[V];
	}*/

	return Graph;
}

void ShowGraph(MGraph Graph){

	/*邻接矩阵显示*/
	cout << endl << "邻接矩阵" << endl;
	for (int i = 0; i < Graph->Nv; ++i)
	{
		cout << "  " << i;
	}
	cout << endl;
	for (int i = 0; i < Graph->Nv; ++i)
	{
		cout << i << "|";
		for (int j = 0; j<Graph->Nv; ++j)
		{
			cout << Graph->G[i][j] << "  ";
		}
		cout << endl;
	}
}


/* 邻接矩阵存储 - 多源最短路算法 */
bool Floyd(MGraph Graph, WeightType Distance[][MaxVertexNum], Vertex path[][MaxVertexNum])
{
	Vertex i, j, k;

	/* 初始化 */
	for (i = 0; i<Graph->Nv; i++)
		for (j = 0; j<Graph->Nv; j++) {
			Distance[i][j] = Graph->G[i][j];
			path[i][j] = -1;
		}

	for (k = 0; k<Graph->Nv; k++)
		for (i = 0; i<Graph->Nv; i++)
			for (j = 0; j<Graph->Nv; j++)
				if (Distance[i][k]<INFINITY&&Distance[k][j]<INFINITY&&Distance[i][k] + Distance[k][j] < Distance[i][j]) {
					Distance[i][j] = Distance[i][k] + Distance[k][j];
					if (i == j && Distance[i][j]<0) /* 若发现负值圈 */
						return false; /* 不能正确解决，返回错误标记 */
					path[i][j] = k;
				}
	return true; /* 算法执行完毕，返回正确标记 */
}
/*设置2个数据结构：
lowcost[i]:表示以i为终点的边的最小权值,当lowcost[i]=0说明以i为终点的边的最小权值=0,也就是表示i点加入了MST
parent[i]:表示对应lowcost[i]的起点，即说明边<parent[i],i>是MST的一条边，当parent[i]=0表示起点i加入MST
*/
/* 邻接矩阵存储 - Prim最小生成树算法 */

Vertex FindMinDist(MGraph Graph, WeightType lowcost[])
{ /* 返回未被收录顶点中dist最小者 */
	Vertex MinIndex, V;
	WeightType MinLowcost = INFINITY;     /*MinLowcost:表示最小权重*/

	for (V = 0; V<Graph->Nv; V++) {
		if (lowcost[V] != 0 && lowcost[V]<MinLowcost) {
			/* 若V未被收录，且dist[V]更小 */
			MinLowcost = lowcost[V]; /* 更新最小距离 */
			MinIndex = V; /* 更新对应顶点 */
		}
	}
	if (MinLowcost < INFINITY) /* 若找到最小dist */
		return MinIndex; /* 返回对应的顶点下标 */
	else return ERROR;  /* 若这样的顶点不存在，返回-1作为标记 */
}
int MiniSpanTree_Prim(MGraph Graph, MGraph MST, Vertex original_index = 0)
{  /* 将最小生成树保存为邻接表存储的图MST，返回最小权重和 */
	WeightType lowcost[MaxVertexNum], TotalWeight;
	Vertex parent[MaxVertexNum], V, W;
	//Vertex original_index;   /*默认起始地点*/
	Vertex MinIndex;
	/* 初始化。默认初始点下标是0 */
	for (V = 0; V < Graph->Nv; V++) {
		/* 这里假设若V到W没有直接的边，则Graph->G[V][W]定义为INFINITY */
		lowcost[V] = Graph->G[0][V];
		parent[V] = 0; /* 暂且定义所有顶点的父结点都是初始点0 */
	}
	TotalWeight = 0; /* 初始化权重和     */
	int VCount = 0;      /* 初始化收录的顶点数 */

	/* 将初始点0收录进MST */
	lowcost[0] = 0;
	VCount++;
	parent[0] = -1; /* 当前树根是0 */

	while (1) {
		MinIndex = FindMinDist(Graph, lowcost);
		/* MinIndex = 未被收录顶点中lowcost最小者 */
		if (MinIndex == ERROR) /* 若这样的MinIndex不存在 */
			break;   /* 算法结束 */

		/* 将MinIndex及相应的边<parent[MinIndex], MinIndex>收录进MST */
		cout << "V" << parent[MinIndex] << "-V" << MinIndex << "=" << lowcost[MinIndex] << endl;
		TotalWeight += lowcost[MinIndex];
		/*更新树*/
		lowcost[MinIndex] = 0;
		VCount++;

		for (W = 0; W < Graph->Nv; W++) /* 对图中的每个顶点W */
			if (lowcost[W] != 0 && Graph->G[MinIndex][W] < INFINITY) {
				/* 若W是MinIndex的邻接点并且未被收录 */
				if (Graph->G[MinIndex][W] < lowcost[W]) {
					/* 若收录MinIndex使得dist[W]变小 */
					lowcost[W] = Graph->G[MinIndex][W]; /* 更新dist[W] */
					parent[W] = MinIndex; /* 更新树 */
				}
			}
	} /* while结束*/
	if (VCount < Graph->Nv) /* MST中收的顶点不到|V|个 */
		TotalWeight = ERROR;
	return TotalWeight;   /* 算法执行完毕，返回最小权重和或错误标记 */
}