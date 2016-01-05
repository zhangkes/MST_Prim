/*
*
*  V0.2015112721	  ����FLoyd�㷨���������һ�Զ���֮������·��
*
*/
#include<iostream>
#include <malloc.h>
#include <conio.h>
using namespace std;

#define ERROR 0
/* ͼ���ڽӾ����ʾ�� */

#define MaxVertexNum 100        /* ��󶥵�����Ϊ100 */
#define INFINITY 99              /* ����Ϊ˫�ֽ��޷������������ֵ65535*/
typedef int Vertex;             /* �ö����±��ʾ����,Ϊ���� */
typedef int WeightType;         /* �ߵ�Ȩֵ��Ϊ���� */
typedef char DataType;          /* ����洢������������Ϊ�ַ��� */
bool  Visited[MaxVertexNum];

/* �ߵĶ��� */
typedef struct ENode *PtrToENode;
struct ENode{
	Vertex V1, V2;      /* �����<V1, V2> */
	WeightType Weight;  /* Ȩ�� */
};
typedef PtrToENode Edge;

/* ͼ���Ķ��� */
typedef struct GNode *PtrToGNode;
struct GNode{
	int Nv;  /* ������ */
	int Ne;  /* ����   */
	WeightType G[MaxVertexNum][MaxVertexNum];  /* �ڽӾ��� */
	DataType Data[MaxVertexNum];               /* �涥������� */
	/* ע�⣺�ܶ�����£����������ݣ���ʱData[]���Բ��ó��� */
};
typedef PtrToGNode MGraph; /* ���ڽӾ���洢��ͼ���� */


MGraph CreateMGraph(int VertexNum)
{ /* ��ʼ��һ����VertexNum�����㵫û�бߵ�ͼ */
	Vertex V, W;
	MGraph Graph;

	Graph = (MGraph)malloc(sizeof(struct GNode)); /* ����ͼ */
	Graph->Nv = VertexNum;
	Graph->Ne = 0;
	/* ��ʼ���ڽӾ��� */
	/* ע�⣺����Ĭ�϶����Ŵ�0��ʼ����(Graph->Nv - 1) */
	for (V = 0; V<Graph->Nv; V++)
		for (W = 0; W<Graph->Nv; W++)
			Graph->G[V][W] = INFINITY;

	return Graph;
}

void InsertEdge(MGraph Graph, Edge E)
{
	/* ����� <V1, V2> */
	Graph->G[E->V1][E->V2] = E->Weight;
	/* ��������ͼ����Ҫ�����<V2, V1> */
	Graph->G[E->V2][E->V1] = E->Weight;
}

MGraph BuildMGraph()
{
	MGraph Graph;
	Edge E;
	Vertex V;
	int Nv, i;

	cout << "���붥��ĸ����ͱ���" << endl;
	cin >> Nv;   /* ���붥����� */
	Graph = CreateMGraph(Nv); /* ��ʼ����Nv�����㵫û�бߵ�ͼ */

	cin >> Graph->Ne; /* ������� */
	if (Graph->Ne != 0) { /* ����б� */
		E = (Edge)malloc(sizeof(struct ENode)); /* �����߽�� */
		/* ����ߣ���ʽΪ"��� �յ� Ȩ��"�������ڽӾ��� */
		for (i = 0; i<Graph->Ne; i++) {
			cout << "����ߣ���ʽΪ ��� �յ� Ȩ�� " << endl;
			cin >> E->V1 >> E->V2 >> E->Weight;
			/* ע�⣺���Ȩ�ز������ͣ�Weight�Ķ����ʽҪ�� */
			InsertEdge(Graph, E);
		}
	}

	/* ������������ݵĻ����������� */
	/*for (V = 0; V < Graph->Nv; V++)
	{
	cout << "��" << V << "����������� " << endl;
	cin >> Graph->Data[V];
	}*/

	return Graph;
}

void ShowGraph(MGraph Graph){

	/*�ڽӾ�����ʾ*/
	cout << endl << "�ڽӾ���" << endl;
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


/* �ڽӾ���洢 - ��Դ���·�㷨 */
bool Floyd(MGraph Graph, WeightType Distance[][MaxVertexNum], Vertex path[][MaxVertexNum])
{
	Vertex i, j, k;

	/* ��ʼ�� */
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
					if (i == j && Distance[i][j]<0) /* �����ָ�ֵȦ */
						return false; /* ������ȷ��������ش����� */
					path[i][j] = k;
				}
	return true; /* �㷨ִ����ϣ�������ȷ��� */
}
/*����2�����ݽṹ��
lowcost[i]:��ʾ��iΪ�յ�ıߵ���СȨֵ,��lowcost[i]=0˵����iΪ�յ�ıߵ���СȨֵ=0,Ҳ���Ǳ�ʾi�������MST
parent[i]:��ʾ��Ӧlowcost[i]����㣬��˵����<parent[i],i>��MST��һ���ߣ���parent[i]=0��ʾ���i����MST
*/
/* �ڽӾ���洢 - Prim��С�������㷨 */

Vertex FindMinDist(MGraph Graph, WeightType lowcost[])
{ /* ����δ����¼������dist��С�� */
	Vertex MinIndex, V;
	WeightType MinLowcost = INFINITY;     /*MinLowcost:��ʾ��СȨ��*/

	for (V = 0; V<Graph->Nv; V++) {
		if (lowcost[V] != 0 && lowcost[V]<MinLowcost) {
			/* ��Vδ����¼����dist[V]��С */
			MinLowcost = lowcost[V]; /* ������С���� */
			MinIndex = V; /* ���¶�Ӧ���� */
		}
	}
	if (MinLowcost < INFINITY) /* ���ҵ���Сdist */
		return MinIndex; /* ���ض�Ӧ�Ķ����±� */
	else return ERROR;  /* �������Ķ��㲻���ڣ�����-1��Ϊ��� */
}
int MiniSpanTree_Prim(MGraph Graph, MGraph MST, Vertex original_index = 0)
{  /* ����С����������Ϊ�ڽӱ�洢��ͼMST��������СȨ�غ� */
	WeightType lowcost[MaxVertexNum], TotalWeight;
	Vertex parent[MaxVertexNum], V, W;
	//Vertex original_index;   /*Ĭ����ʼ�ص�*/
	Vertex MinIndex;
	/* ��ʼ����Ĭ�ϳ�ʼ���±���0 */
	for (V = 0; V < Graph->Nv; V++) {
		/* ���������V��Wû��ֱ�ӵıߣ���Graph->G[V][W]����ΪINFINITY */
		lowcost[V] = Graph->G[0][V];
		parent[V] = 0; /* ���Ҷ������ж���ĸ���㶼�ǳ�ʼ��0 */
	}
	TotalWeight = 0; /* ��ʼ��Ȩ�غ�     */
	int VCount = 0;      /* ��ʼ����¼�Ķ����� */

	/* ����ʼ��0��¼��MST */
	lowcost[0] = 0;
	VCount++;
	parent[0] = -1; /* ��ǰ������0 */

	while (1) {
		MinIndex = FindMinDist(Graph, lowcost);
		/* MinIndex = δ����¼������lowcost��С�� */
		if (MinIndex == ERROR) /* ��������MinIndex������ */
			break;   /* �㷨���� */

		/* ��MinIndex����Ӧ�ı�<parent[MinIndex], MinIndex>��¼��MST */
		cout << "V" << parent[MinIndex] << "-V" << MinIndex << "=" << lowcost[MinIndex] << endl;
		TotalWeight += lowcost[MinIndex];
		/*������*/
		lowcost[MinIndex] = 0;
		VCount++;

		for (W = 0; W < Graph->Nv; W++) /* ��ͼ�е�ÿ������W */
			if (lowcost[W] != 0 && Graph->G[MinIndex][W] < INFINITY) {
				/* ��W��MinIndex���ڽӵ㲢��δ����¼ */
				if (Graph->G[MinIndex][W] < lowcost[W]) {
					/* ����¼MinIndexʹ��dist[W]��С */
					lowcost[W] = Graph->G[MinIndex][W]; /* ����dist[W] */
					parent[W] = MinIndex; /* ������ */
				}
			}
	} /* while����*/
	if (VCount < Graph->Nv) /* MST���յĶ��㲻��|V|�� */
		TotalWeight = ERROR;
	return TotalWeight;   /* �㷨ִ����ϣ�������СȨ�غͻ������ */
}