//
// Created by 星落_月残 on 2017/5/25.
//

#ifndef ERRORCODE_LIST
#define ERRORCODE_LIST
enum ErrorCode_List
{
	underflow,overflow,success,range_error,non_existence,overwrite,duplicate,new_entry
};
#endif

#ifndef BASICDATASTRUCTUREANDALGORITHMS_BASICBTREE_H
#define BASICDATASTRUCTUREANDALGORITHMS_BASICBTREE_H

constexpr long long MaxDegreeOfBTree=4;
constexpr long long MaxDataNumber=MaxDegreeOfBTree-1;//此数据量不能达到

//TODO debug
template<class T>
struct BClientData
{
	long long index;
	T data;
	BClientData();
};

template<class T>
BClientData<T>::BClientData()
{
	index=-1;
}

template<class T>
struct BClient
{
	BClientData<T> DataList[MaxDataNumber];
	BClient<T> *SubList[MaxDegreeOfBTree];
	BClient<T> *Senior;
	bool isLeaf;
	long long currentLength;
	BClient();
};

//数据与指针的对应：第0指针指向小于第1数据的键，第n指针指向大于第n-1数据的键
template<class T>
BClient<T>::BClient()
{
	for(int i=0;i<MaxDegreeOfBTree;i++)
		SubList[i]=nullptr;
	currentLength=0;
	isLeaf=false;
	Senior=nullptr;
}

//存储T类型，可以用其他类型，如int作为索引
template<class T>
class BTree
{
public:
	BTree();
	~BTree();
	void Clear(BClient<T> *Entrance);
	ErrorCode_List InsertData(const long long toWriteIndex,const T toWrite);
	ErrorCode_List DeleteData(const long long toDeleteIndex);
	ErrorCode_List RetrieveData(const long long toFindIndex,T &output);
	ErrorCode_List PreorderTraverse(BClient<T> *Entrance,void (*visit)(BClient<T> *));
	ErrorCode_List merge(BClient<T> *Entrance,long long SeniorSubscript);//入口为父节点，保证左子节点不变
	ErrorCode_List split(BClient<T> *Entrance);//入口为左子节点
	long long binarySearchForIndex(BClientData<T> *List,long long listLength,long long toFindIndex);
	//private:
	BClient<T> *Root;
};

template<class T>
BTree<T>::BTree()
{
	Root=new BClient<T>;
	Root->isLeaf=true;
}

template<class T>
void BTree<T>::Clear(BClient<T> *Entrance)
{
	if(Entrance!=nullptr)
	{
		for(int i=0;i<MaxDegreeOfBTree;i++)
		{
			if(Entrance->SubList[i]!=nullptr)
			{
				Clear(Entrance->SubList[i]);
			}
		}
		delete Entrance;
	}
}

template<class T>
BTree<T>::~BTree()
{
	Clear(Root);
}

template<class T>
ErrorCode_List BTree<T>::InsertData(const long long toWriteIndex,const T toWrite)
{
	//寻找插入的节点
	BClient<T> *InsertPointer=Root;
	for(;!(InsertPointer->isLeaf);)
	{
		long long i=0,length=InsertPointer->currentLength;
		if(toWriteIndex<InsertPointer->DataList[0].index)
		{
			InsertPointer=InsertPointer->SubList[0];
			continue;
		}
		else if(toWriteIndex>InsertPointer->DataList[InsertPointer->currentLength-1].index)
		{
			InsertPointer=InsertPointer->SubList[InsertPointer->currentLength];
			continue;
		}
		for(;i<length-1;i++)
		{
			if(InsertPointer->DataList[i].index==toWriteIndex)
				return duplicate;
			else if(InsertPointer->DataList[i].index<toWriteIndex)
				if(InsertPointer->DataList[i+1].index<toWriteIndex)
					continue;
				else
				{
					InsertPointer=InsertPointer->SubList[i+1];
					break;
				}
		}
	}
	//此处是找到新插入位置节点后的操作，如确定数据位置（变动原有数据位置），累加器加一
	long long i=0;
	for(;InsertPointer->DataList[i].index<toWriteIndex&&InsertPointer->DataList[i].index!=-1;i++);
	if(InsertPointer->DataList[i].index==-1)
	{
		InsertPointer->DataList[i].index=toWriteIndex;
		InsertPointer->DataList[i].data=toWrite;
		InsertPointer->currentLength++;
	}
	else if(InsertPointer->DataList[i].index==toWriteIndex)
		return duplicate;
	else if(InsertPointer->DataList[i].index>toWriteIndex)
	{
		for(long long j=InsertPointer->currentLength-1;j>=i;j--)
			InsertPointer->DataList[j+1]=InsertPointer->DataList[j];
		InsertPointer->DataList[i].index=toWriteIndex;
		InsertPointer->DataList[i].data=toWrite;
		InsertPointer->currentLength++;
	}
	//然后是满节点拆分，此时才开始申请节点空间，逐级向上判断是否满节点
	BClient<T> *TraversePointer=InsertPointer;
	for(;TraversePointer!=nullptr;TraversePointer=TraversePointer->Senior)
		split(TraversePointer);
}


template<class T>
ErrorCode_List BTree<T>::split(BClient<T> *Entrance)
{
	if(Entrance->currentLength>=MaxDataNumber)
	{
		BClient<T> *NewClient=new BClient<T>;
		BClient<T> *Senior=Entrance->Senior;
		long long middle=MaxDataNumber/2;
		long long length=Entrance->currentLength;
		long long j=0,k=0;
		for(j=middle+1,k=0;j<length;j++,k++)
		{
			NewClient->DataList[k]=Entrance->DataList[j];
			NewClient->currentLength++;
			NewClient->SubList[k]=Entrance->SubList[j];
			Entrance->SubList[j]=nullptr;
			Entrance->DataList[j].index=-1;
			Entrance->currentLength--;
		}
		NewClient->SubList[k]=Entrance->SubList[length];
		Entrance->SubList[length]=nullptr;

		NewClient->isLeaf=NewClient->SubList[0]==nullptr;//判定是否叶子节点

		if(Senior==nullptr)//产生新的根
		{
			BClient<T> *NewRoot=new BClient<T>;
			NewRoot->isLeaf=false;
			NewRoot->DataList[0]=Entrance->DataList[middle];
			Entrance->DataList[middle].index=-1;
			Entrance->currentLength--;
			Entrance->Senior=NewRoot;
			NewClient->Senior=NewRoot;
			NewRoot->SubList[0]=Entrance;
			NewRoot->SubList[1]=NewClient;
			NewRoot->currentLength=1;
			Root=NewRoot;
		}
		else
		{
			NewClient->Senior=Entrance->Senior;
			BClientData<T> tmpMiddle=Entrance->DataList[middle];
			Entrance->DataList[middle].index=-1;
			Entrance->currentLength--;
			long long toInsertIndex=tmpMiddle.index;
			long long order=0,seniorLength=Senior->currentLength;
			long long m=0;

			if(toInsertIndex<Senior->DataList[0].index)
			{
				for(m=seniorLength-1;m>=0;m--)
				{
					Senior->DataList[m+1]=Senior->DataList[m];
					Senior->SubList[m+2]=Senior->SubList[m+1];//第1指针空出
				}
				Senior->DataList[0]=tmpMiddle;
				Senior->SubList[1]=NewClient;
				Senior->currentLength++;
			}
			else if(toInsertIndex>Senior->DataList[Senior->currentLength-1].index)
			{
				Senior->DataList[seniorLength]=tmpMiddle;
				Senior->SubList[seniorLength+1]=NewClient;
				Senior->currentLength++;
			}
			else
			{
				for(;m<seniorLength-1;m++)
				{
					if(Senior->DataList[m].index==toInsertIndex)
						return duplicate;
					else if(Senior->DataList[m].index<toInsertIndex)
						if(Senior->DataList[m+1].index<toInsertIndex)
							continue;
						else
							break;
				}
				order=m+1;
				for(m=seniorLength-1;m>=order;m--)
				{
					Senior->DataList[m+1]=Senior->DataList[m];
					Senior->SubList[m+2]=Senior->SubList[m+1];//第order+1指针空出
				}
				Senior->DataList[order]=tmpMiddle;
				Senior->SubList[order+1]=NewClient;
				Senior->currentLength++;
			}
		}
	}
	return success;
}

template<class T>
ErrorCode_List BTree<T>::DeleteData(const long long toDeleteIndex)
{
	//寻找插入的节点
	BClient<T> *DeletePointer=Root;
	long long DeleteSubscript=0;
	long long SeniorSubscript=0;
	for(;!(DeletePointer->isLeaf);)
	{
		long long i=0,length=DeletePointer->currentLength;
		if(toDeleteIndex<DeletePointer->DataList[0].index)
		{
			SeniorSubscript=-1;
			DeletePointer=DeletePointer->SubList[0];
			continue;
		}
		else if(toDeleteIndex>DeletePointer->DataList[DeletePointer->currentLength-1].index)
		{
			SeniorSubscript=DeletePointer->currentLength-1;
			DeletePointer=DeletePointer->SubList[DeletePointer->currentLength];
			continue;
		}
		for(;i<length-1;i++)
		{
			if(DeletePointer->DataList[i].index==toDeleteIndex)
			{
				DeleteSubscript=i;
				break;
			}
			else if(DeletePointer->DataList[i].index<toDeleteIndex)
				if(DeletePointer->DataList[i+1].index<toDeleteIndex)
					continue;
				else
					return non_existence;
		}
	}
	//执行删除操作，首先进行分类，分为直接删除，删除与合并父节点的一项，用前驱代替并判定合并，平级借用（注意判定合并后是否需要分裂）
	if(DeletePointer->isLeaf)//考虑叶子节点情况
	{
		//先删除
		long long length=DeletePointer->currentLength;
		for(long long i=DeleteSubscript;i<length-1;i++)
			DeletePointer->DataList[i]=DeletePointer->DataList[i+1];
		DeletePointer->DataList[length-1].index=-1;
		DeletePointer->currentLength--;
		if(DeletePointer->currentLength<MaxDataNumber/2&&DeletePointer!=Root)
		{
			if(SeniorSubscript>=0&&DeletePointer->Senior->SubList[SeniorSubscript]->currentLength-1>MaxDataNumber/2)//向左平级借用
			{
				for(long long i=DeleteSubscript;i>0;i--)
					DeletePointer->DataList[i]=DeletePointer->DataList[i-1];
				DeletePointer->DataList[0]=DeletePointer->Senior->DataList[SeniorSubscript];
				DeletePointer->Senior->DataList[SeniorSubscript]=DeletePointer->Senior->SubList[SeniorSubscript]->DataList[DeletePointer->Senior->SubList[SeniorSubscript]->currentLength-1];
				DeletePointer->Senior->SubList[SeniorSubscript]->DataList[DeletePointer->Senior->SubList[SeniorSubscript]->currentLength-1].index=-1;
				DeletePointer->Senior->SubList[SeniorSubscript]->currentLength--;
			}
			else if(SeniorSubscript<DeletePointer->Senior->currentLength-1&&DeletePointer->Senior->SubList[SeniorSubscript+1]->currentLength-1>MaxDataNumber/2)//向右平级借用
			{
				DeletePointer->DataList[DeletePointer->currentLength]=DeletePointer->Senior->DataList[SeniorSubscript+1];
				DeletePointer->Senior->DataList[SeniorSubscript+1]=DeletePointer->Senior->SubList[SeniorSubscript+2]->DataList[0];
				long long tmplength=DeletePointer->Senior->SubList[SeniorSubscript+2]->currentLength;
				for(long long i=0;i<tmplength-1;i++)
					DeletePointer->Senior->SubList[SeniorSubscript+2]->DataList[i]=DeletePointer->Senior->SubList[SeniorSubscript+2]->DataList[i+1];
				DeletePointer->Senior->SubList[SeniorSubscript+2]->DataList[tmplength].index=-1;
				DeletePointer->Senior->SubList[SeniorSubscript+2]->currentLength--;
			}
			else//合并
			{
				if(merge(DeletePointer->Senior,SeniorSubscript)==success)
					split(DeletePointer);
				else
					return overflow;
			}
		}
	}
	else//考虑非叶子节点情况
	{
		BClient<T> *SearchPointer=DeletePointer->SubList[DeleteSubscript];
		for(;!(SearchPointer->isLeaf);SearchPointer=SearchPointer->SubList[SearchPointer->currentLength]);
		DeletePointer->DataList[DeleteSubscript]=SearchPointer->DataList[SearchPointer->currentLength-1];
		SearchPointer->DataList[SearchPointer->currentLength-1].index=-1;
		SearchPointer->currentLength--;

		if(merge(SearchPointer->Senior,SearchPointer->Senior->currentLength-1)==success)
			split(SearchPointer);
		else
			return overflow;
	}
	return success;
}

template<class T>
ErrorCode_List BTree<T>::merge(BClient<T> *Entrance,long long SeniorSubscript)
{
	BClient<T> *SubLeft=Entrance->SubList[SeniorSubscript];
	BClient<T> *SubRight=Entrance->SubList[SeniorSubscript+1];
	long long rightLength=SubRight->currentLength;
	if(rightLength+SubLeft->currentLength>MaxDataNumber)
		return overflow;
	else
	{
		SubLeft->DataList[SubLeft->currentLength]=Entrance->DataList[SeniorSubscript];
		SubLeft->currentLength++;
		for(long long i=0;i<rightLength;i++)
		{
			SubLeft->DataList[SubLeft->currentLength]=SubRight->DataList[i];
			SubLeft->currentLength++;
		}
		delete SubRight;
		for(long long i=SeniorSubscript;i<Entrance->currentLength-1;i++)
		{
			Entrance->DataList[i]=Entrance->DataList[i+1];
			Entrance->SubList[i+1]=Entrance->SubList[i+2];
		}
		Entrance->currentLength--;
	}
	return success;
}

template<class T>
ErrorCode_List BTree<T>::PreorderTraverse(BClient<T> *Entrance,void (*visit)(BClient<T> *))
{
	BClient<T> *TraversePointer=Entrance;
	visit(Entrance);
	if(!Entrance->isLeaf)
		for(long long i=0;i<=TraversePointer->currentLength;i++)
			PreorderTraverse(TraversePointer->SubList[i],visit);
	return success;
}

template<class T>
long long BTree<T>::binarySearchForIndex(BClientData<T> *List,long long listLength,long long toFindIndex)
{
	if(listLength==1)
		if(List[0].index==toFindIndex)
			return 0;
	long long middle=listLength/2;
	if(List[middle].index==toFindIndex)
		return middle;
	else if(List[middle].index>toFindIndex)
		return binarySearchForIndex(List,listLength/2,toFindIndex);
	else
		return binarySearchForIndex(List+middle+1,listLength/2,toFindIndex)+middle+1;
}


#endif //BASICDATASTRUCTUREANDALGORITHMS_BASICBTREE_H
