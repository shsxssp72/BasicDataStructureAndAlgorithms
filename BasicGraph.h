//
// Created by 星落_月残 on 2017/6/6.
//
#ifndef BASICDATASTRUCTUREANDALGORITHMS_BASICGRAPH_H
#define BASICDATASTRUCTUREANDALGORITHMS_BASICGRAPH_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "MultipurposeSequence.h"

//TODO 修订Graph，酌情增加更多实用的函数，完善LoadConfig

using namespace std;

constexpr long long DefaultNumberOfVertices=10;

template<class T>
struct Vertex
{
	Vertex();
	Vertex(const int inIndex);
	Vertex(const int inIndex,const T inData);
	Vertex(const Vertex<T> &another);
	long long index;
	T data;
};

template<class T>
Vertex<T>::Vertex()
{
	index=0;
}
template<class T>
Vertex<T>::Vertex(const int inIndex)
{
	index=inIndex;
}
template<class T>
Vertex<T>::Vertex(const int inIndex,const T inData)
{
	index=inIndex;
	data=inData;
}
template<class T>
Vertex<T>::Vertex(const Vertex<T> &another)
{
	index=another.index;
	data=another.data;
}

template<class T>
struct Edge
{
	Edge();
	Edge(const long long inMultiplicity);
	Edge(const long long inMultiplicity,const long long inWeight);
	Edge(const Edge &another);
	long long multiplicity;
	long long weight;
};

template<class T>
Edge<T>::Edge()
{
	multiplicity=0;
	weight=1;
}
template<class T>
Edge<T>::Edge(const long long inMultiplicity)
{
	multiplicity=inMultiplicity;
	weight=1;
}
template<class T>
Edge<T>::Edge(const long long inMultiplicity,const long long inWeight)
{
	multiplicity=inMultiplicity;
	weight=inWeight;
}
template<class T>
Edge<T>::Edge(const Edge &another)
{
	multiplicity=another.multiplicity;
	weight=another.weight;
}

template<class T>
class Graph
{
public:
	Graph();
	Graph(const long long int inSize);
	Graph(Edge<T> **matrix,int inSize,Vertex<T> **inVerticesList);
	Graph(const Graph &another);
	~Graph();
	long long int getSize() const;
	Edge<T> &getEdgeFromPosition(const int From,const int To) const;
	Vertex<T> *getVertexPointerFromPosition(const int position) const;
	long long getMultiplicity(const int From,const int To) const;
	long long getWeight(const int From,const int To) const;
	ErrorCode_List setEdgeFromPosition(const int From,const int To,const Edge<T> &toWrite);//set函数较麻烦,除了symmetrical
	ErrorCode_List setVertexPointerFromPosition(const int position,const Vertex<T> *&toWrite);
	ErrorCode_List setEdgeSymmetrical(const int From,const int To,const int inMultiplicity,const int inWeight);
	bool isMultiplicity(const int From,const int To,const int toCompare);
	bool isWeight(const int From,const int To,const int toCompare);
	void LoadConfig();
	void LoadConfig(const string &FileName);
	void LoadConfig(Edge<T> **matrix,int inSize,Vertex<T> **inVerticesList);
	void DepthFirstTraverse(Vertex<T> *Home,void (*visit)(Vertex<T> *));
	void BreadthFirstTraverse(Vertex<T> *Home,void (*visit)(Vertex<T> *));//都是给定入口开始遍历，未必能遍历到
	void DepthFirstSort(Vertex<T> *Home,Sequence<Vertex<T> *> &output);
	void BreadthFirstSort(Sequence<Vertex<T> *> &output);

private:
	long long size;
	Edge<T> **AdjacencyMatrix;//[DefaultNumberOfVertices][DefaultNumberOfVertices];
	Vertex<T> **VerticesList;//[DefaultNumberOfVertices];
};

template<class T>
Graph<T>::Graph()
{
	size=DefaultNumberOfVertices;
	AdjacencyMatrix=new Edge<T> *[DefaultNumberOfVertices];
	VerticesList=new Vertex<T> *[DefaultNumberOfVertices];
	for(long long i=0;i<DefaultNumberOfVertices;i++)
	{
		AdjacencyMatrix[i]=new Edge<T>[DefaultNumberOfVertices];
		for(long long j=0;j<DefaultNumberOfVertices;j++)
		{
			AdjacencyMatrix[i][j].multiplicity=0;
			AdjacencyMatrix[i][j].weight=0;
		}
		VerticesList[i]=new Vertex<T>;
		VerticesList[i]->index=i;
	}
}

template<class T>
Graph<T>::Graph(const long long int inSize)
{
	size=inSize;
	AdjacencyMatrix=new Edge<T> *[inSize];
	VerticesList=new Vertex<T> *[inSize];
	for(long long i=0;i<inSize;i++)
	{
		AdjacencyMatrix[i]=new Edge<T>[inSize];
		for(long long j=0;j<inSize;j++)
		{
			AdjacencyMatrix[i][j].multiplicity=0;
			AdjacencyMatrix[i][j].weight=0;
		}
		VerticesList[i]=new Vertex<T>;
		VerticesList[i]->index=i;
	}
}
template<class T>
Graph<T>::Graph(Edge<T> **matrix,int inSize,Vertex<T> **inVerticesList)
{
	LoadConfig(matrix,inSize,inVerticesList);
}
template<class T>
Graph<T>::Graph(const Graph &another)
{
	size=another.size;
	AdjacencyMatrix=new Edge<T> *[size];
	VerticesList=new Vertex<T> *[size];
	for(long long i=0;i<size;i++)
	{
		AdjacencyMatrix[i]=new Edge<T>[size];
		for(long long j=0;j<size;j++)
		{
			AdjacencyMatrix[i][j].multiplicity=another.AdjacencyMatrix[i][j].multiplicity;
			AdjacencyMatrix[i][j].weight=another.AdjacencyMatrix[i][j].weight;
		}
		VerticesList[i]=new Vertex<T>;
		VerticesList[i]->index=another.VerticesList[i]->index;
	}
}

template<class T>
Graph<T>::~Graph()
{
	for(long long i=0;i<size;i++)
	{
		delete[] AdjacencyMatrix[i];
		delete VerticesList[i];
	}
	delete[] AdjacencyMatrix;
	delete[] VerticesList;
}

template<class T>
long long int Graph<T>::getSize() const
{
	return size;
}
template<class T>
Edge<T> &Graph<T>::getEdgeFromPosition(const int From,const int To) const
{
	if(From>=size||To>=size)
		return AdjacencyMatrix[size-1][size-1];
	else if(From<0||To<0)
		return AdjacencyMatrix[0][0];
	else
		return AdjacencyMatrix[From][To];
}

template<class T>
Vertex<T> *Graph<T>::getVertexPointerFromPosition(const int position) const
{
	if(position>=size)
		return VerticesList[size-1];
	else if(position<0)
		return VerticesList[0];
	else
		return VerticesList[position];
}

template<class T>
long long Graph<T>::getMultiplicity(const int From,const int To) const
{
	if(From>=size||To>=size||From<0||To<0)
		return -1;
	else
		return AdjacencyMatrix[From][To].multiplicity;
}

template<class T>
long long Graph<T>::getWeight(const int From,const int To) const
{
	if(From>=size||To>=size||From<0||To<0)
		return -1;
	else
		return AdjacencyMatrix[From][To].weight;
}

template<class T>
ErrorCode_List Graph<T>::setEdgeFromPosition(const int From,const int To,const Edge<T> &toWrite)
{
	if(From>=size||To>=size||From<0||To<0)
		return ErrorCode_List::range_error;
	else
	{
		AdjacencyMatrix[From][To]=toWrite;
		return success;
	}

}

template<class T>
ErrorCode_List Graph<T>::setVertexPointerFromPosition(const int position,const Vertex<T> *&toWrite)
{
	if(position>=size||position<0)
		return ErrorCode_List::range_error;
	else
	{
		VerticesList[position]=const_cast<Vertex<T> *>(toWrite);
		return success;
	}

}
template<class T>
ErrorCode_List Graph<T>::setEdgeSymmetrical(const int From,const int To,const int inMultiplicity,const int inWeight)
{
	if(From>=getSize()||To>=getSize()||From<0||To<0)
		return ErrorCode_List::range_error;
	else if(From!=To)
	{
		getEdgeFromPosition(From,To).multiplicity=inMultiplicity;
		getEdgeFromPosition(From,To).weight=inWeight;
		getEdgeFromPosition(To,From).multiplicity=inMultiplicity;
		getEdgeFromPosition(To,From).weight=inWeight;
	}
	else
	{
		getEdgeFromPosition(From,To).multiplicity=inMultiplicity;
		getEdgeFromPosition(From,To).weight=inWeight;
	}
}
template<class T>
bool Graph<T>::isMultiplicity(const int From,const int To,const int toCompare)
{
	return AdjacencyMatrix[From][To].multiplicity==toCompare;
}
template<class T>
bool Graph<T>::isWeight(const int From,const int To,const int toCompare)
{
	return AdjacencyMatrix[From][To].weight==toCompare;
}
template<class T>
void Graph<T>::LoadConfig()
{
	for(long long i=0;i<DefaultNumberOfVertices;i++)
		cin>>VerticesList[i]->data;

	for(long long i=0;i<DefaultNumberOfVertices;i++)
		for(long long j=0;j<DefaultNumberOfVertices;j++)
			cin>>AdjacencyMatrix[i][j].multiplicity;
}

template<class T>
void Graph<T>::LoadConfig(const string &FileName)
{
	ifstream fin(FileName);
	if(!fin.is_open())
		cerr<<"Input Error-Can not open file "<<FileName<<"."<<endl;
	else
	{
		for(long long i=0;i<DefaultNumberOfVertices;i++)
			fin>>VerticesList[i]->data;

		for(long long i=0;i<DefaultNumberOfVertices;i++)
			for(long long j=0;j<DefaultNumberOfVertices;j++)
				fin>>AdjacencyMatrix[i][j].multiplicity;
	}
}
template<class T>
void Graph<T>::LoadConfig(Edge<T> **matrix,int inSize,Vertex<T> **inVerticesList)
{
	size=inSize;
	AdjacencyMatrix=new Edge<T> *[inSize];
	VerticesList=new Vertex<T> *[inSize];
	for(long long i=0;i<inSize;i++)
	{
		AdjacencyMatrix[i]=new Edge<T>[inSize];
		for(long long j=0;j<inSize;j++)
		{
			AdjacencyMatrix[i][j].multiplicity=matrix[i][j].multiplicity;
			AdjacencyMatrix[i][j].weight=matrix[i][j].weight;
		}
		VerticesList[i]=new Vertex<T>;
		VerticesList[i]->index=inVerticesList[i]->index;
		VerticesList[i]->data=inVerticesList[i]->data;
	}
}
template<class T>
void Graph<T>::DepthFirstTraverse(Vertex<T> *Home,void (*visit)(Vertex<T> *))
{
	bool isVisited[DefaultNumberOfVertices]={0};//确保不会重复遍历
	//访问起始位置
	long long HomeIndex=Home->index;
	visit(Home);
	isVisited[HomeIndex]=true;
	Sequence<Vertex<T> *> TraverseStack;
	for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
		if(i!=HomeIndex&&AdjacencyMatrix[HomeIndex][i].multiplicity>0&&!isVisited[i])
			TraverseStack.AddFinal(VerticesList[i]);

	//开始遍历
	Vertex<T> *current=nullptr;
	for(;!TraverseStack.isEmpty();)
	{
		TraverseStack.ExtractFinal(current);
		if(!isVisited[current->index])
		{
			long long currentIndex=current->index;
			visit(current);
			isVisited[currentIndex]=true;
			for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
				if(i!=currentIndex&&AdjacencyMatrix[currentIndex][i].multiplicity>0&&!isVisited[i])
					TraverseStack.AddFinal(VerticesList[i]);
		}
	}

	for(long long j=0;j<DefaultNumberOfVertices;j++)
	{
		if(!isVisited[j])
		{
			visit(VerticesList[j]);
			isVisited[j]=true;
			TraverseStack.Clear();
			for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
				if(i!=j&&AdjacencyMatrix[j][i].multiplicity>0&&!isVisited[i])
					TraverseStack.AddFinal(VerticesList[i]);

			Vertex<T> *current2=nullptr;
			for(;!TraverseStack.isEmpty();)
			{
				TraverseStack.ExtractFinal(current2);
				if(!isVisited[current2->index])
				{
					long long currentIndex=current2->index;
					visit(current2);
					isVisited[currentIndex]=true;
					for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
						if(i!=currentIndex&&AdjacencyMatrix[currentIndex][i].multiplicity>0&&!isVisited[i])
							TraverseStack.AddFinal(VerticesList[i]);
				}
			}
		}
	}
}

template<class T>
void Graph<T>::BreadthFirstTraverse(Vertex<T> *Home,void (*visit)(Vertex<T> *))
{
	bool isVisited[DefaultNumberOfVertices]={0};//确保不会重复遍历
	//访问起始位置
	long long HomeIndex=Home->index;
	visit(Home);
	isVisited[HomeIndex]=true;
	Sequence<Vertex<T> *> TraverseQueue;
	for(long long i=0;i<DefaultNumberOfVertices;i++)
		if(i!=HomeIndex&&AdjacencyMatrix[HomeIndex][i].multiplicity>0)
			TraverseQueue.AddFinal(VerticesList[i]);

	//开始遍历
	Vertex<T> *current=nullptr;
	for(;!TraverseQueue.isEmpty();)
	{
		TraverseQueue.Extract(0,current);
		if(!isVisited[current->index])
		{
			long long currentIndex=current->index;
			visit(current);
			isVisited[currentIndex]=true;
			for(long long i=0;i<DefaultNumberOfVertices;i++)
				if(i!=currentIndex&&AdjacencyMatrix[currentIndex][i].multiplicity>0)
					TraverseQueue.AddFinal(VerticesList[i]);
		}
	}

	for(long long j=0;j<DefaultNumberOfVertices;j++)
	{
		if(!isVisited[j])
		{
			visit(VerticesList[j]);
			isVisited[j]=true;
			TraverseQueue.Clear();
			for(long long i=0;i<DefaultNumberOfVertices;i++)
				if(i!=HomeIndex&&AdjacencyMatrix[HomeIndex][i].multiplicity>0)
					TraverseQueue.AddFinal(VerticesList[i]);

			Vertex<T> *current2=nullptr;
			for(;!TraverseQueue.isEmpty();)
			{
				TraverseQueue.Extract(0,current2);
				if(!isVisited[current2->index])
				{
					long long currentIndex=current2->index;
					visit(current2);
					isVisited[currentIndex]=true;
					for(long long i=0;i<DefaultNumberOfVertices;i++)
						if(i!=currentIndex&&AdjacencyMatrix[currentIndex][i].multiplicity>0)
							TraverseQueue.AddFinal(VerticesList[i]);
				}
			}
		}
	}
}

template<class T>
void Graph<T>::DepthFirstSort(Vertex<T> *Home,Sequence<Vertex<T> *> &output)
{
	bool isVisited[DefaultNumberOfVertices]={0};//确保不会重复遍历
	Sequence<Vertex<T> *> TraverseStack;

	//起始位置
	long long HomeIndex=Home->index;
	TraverseStack.AddFinal(VerticesList[HomeIndex]);

	for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
		if(i!=HomeIndex&&AdjacencyMatrix[HomeIndex][i].multiplicity>0)
			TraverseStack.AddFinal(VerticesList[i]);
	int flag=0;
	Vertex<T> *current;
	for(;!TraverseStack.isEmpty();)
	{
		for(;;)
		{
			TraverseStack.RetrieveFinal(current);
			for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
				if(i!=current->index&&AdjacencyMatrix[current->index][i].multiplicity>0&&!isVisited[i])
				{
					TraverseStack.AddFinal(VerticesList[i]);
					flag=1;
				}

			if(flag==1)
				flag=0;
			else
				break;
		}

		Vertex<T> *transition;
		TraverseStack.ExtractFinal(transition);
		if(!isVisited[transition->index])
		{
			output.AddFinal(transition);
			isVisited[transition->index]=true;
		}
	}

	for(long long j=0;j<DefaultNumberOfVertices;j++)
	{
		if(!isVisited[j])
		{
			HomeIndex=j;
			TraverseStack.AddFinal(VerticesList[HomeIndex]);

			for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
				if(i!=HomeIndex&&AdjacencyMatrix[HomeIndex][i].multiplicity>0)
					TraverseStack.AddFinal(VerticesList[i]);
			int flag2=0;
			Vertex<T> *current2;
			for(;!TraverseStack.isEmpty();)
			{
				for(;;)
				{
					TraverseStack.RetrieveFinal(current2);
					for(long long i=DefaultNumberOfVertices-1;i>=0;i--)
						if(i!=current2->index&&AdjacencyMatrix[current2->index][i].multiplicity>0&&!isVisited[i])
						{
							TraverseStack.AddFinal(VerticesList[i]);
							flag2=1;
						}

					if(flag2==1)
						flag2=0;
					else
						break;
				}

				Vertex<T> *transition;
				TraverseStack.ExtractFinal(transition);
				if(!isVisited[transition->index])
				{
					output.AddFinal(transition);
					isVisited[transition->index]=true;
				}
			}
		}
	}
}

template<class T>
void Graph<T>::BreadthFirstSort(Sequence<Vertex<T> *> &output)
{
	//复制邻接矩阵
	Edge<T> CopyOfAdjacencyMatrix[DefaultNumberOfVertices][DefaultNumberOfVertices];
	for(long long i=0;i<DefaultNumberOfVertices;i++)
		for(long long j=0;j<DefaultNumberOfVertices;j++)
			CopyOfAdjacencyMatrix[i][j]=AdjacencyMatrix[i][j];

	Sequence<Vertex<T> *> TraverseQueue;
	bool isVisited[DefaultNumberOfVertices]={0};
	int flag=0;
	for(;;)
	{
		for(long long i=0;i<DefaultNumberOfVertices;i++)
		{
			for(long long j=0;j<DefaultNumberOfVertices;j++)
				if(CopyOfAdjacencyMatrix[j][i].multiplicity==1)
				{
					flag=1;
					break;
				}
			if(flag==0&&!isVisited[i])
			{
				TraverseQueue.AddFinal(VerticesList[i]);
				isVisited[i]=true;
			}
			else
				flag=0;
		}

		if(TraverseQueue.isEmpty())
			break;//放在这是为了防止当前队列和当前输出等长而提前退出循环

		Vertex<T> *transition;
		TraverseQueue.Extract(0,transition);
		long long currentIndex=transition->index;
		for(long long i=0;i<DefaultNumberOfVertices;i++)
			CopyOfAdjacencyMatrix[currentIndex][i].multiplicity=0;//清除后继
		output.AddFinal(transition);
	}

}


#endif //BASICDATASTRUCTUREANDALGORITHMS_BASICGRAPH_H
