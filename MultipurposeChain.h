//
// Created by 星落_月残 on 2017/4/13.
//

#ifndef ERRORCODE_LIST
#define ERRORCODE_LIST
enum ErrorCode_List
{
	underflow,overflow,success,range_error,non_existence,overwrite,duplicate,new_entry,unsuited
};
#endif


#ifndef BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSECHAIN_H
#define BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSECHAIN_H

#include <cstdlib>


constexpr int MaxChainLength=100;
constexpr int InsertionSortLimit_Chain=20;

template<class T>
struct Client
{
public:
	T Data;
	Client<T> *pLast;
	Client<T> *pNext;
	Client<T>();
	Client<T>(const T &toWrite,Client<T> *Last,Client<T> *Next);
};

template<class T>
Client<T>::Client()
{
	pLast=nullptr;
	pNext=nullptr;
}

template<class T>
Client<T>::Client(const T &toWrite,Client<T> *Last,Client<T> *Next)
{
	Data=toWrite;
	pLast=Last;
	pNext=Next;
}

template<class T>
class Chain
{
public:
	Chain();
	Chain(const Chain &original);//拆环
	~Chain();
	void operator =(const Chain &original);
	int getSize() const;
	bool isFull() const;
	bool isEmpty() const;
	bool isCircle();
	bool isLine();
	Client<T> *getHead() const;
	Client<T> *getArbitary() const;
	Client<T> *getAddressFromPosition(int position) const;
	Client<T> *getAddressFromData(const T &toFind) const;//顺序查找,假设无重复
	void Clear();
	void Traverse(void ( *visit)(T &));
	ErrorCode_List AddFirst(const T &toWrite);
	ErrorCode_List AddFinal(const T &toWrite);
	ErrorCode_List Delete(int position);
	ErrorCode_List DeleteAddress(Client<T> *DeletePointer);
	ErrorCode_List DeleteFinal();
	ErrorCode_List Extract(int position,T &output);//类似ServeAndRetrieve
	ErrorCode_List ExtractAddress(Client<T> *ExtractPointer,T &output);
	ErrorCode_List ExtractFinal(T &output);
	ErrorCode_List FormCircle();
	ErrorCode_List FormLine();
	ErrorCode_List Insert(int position,const T &toWrite);//postion位置之后插入
	ErrorCode_List Modify(int position,const T &toWrite);
	ErrorCode_List Retrieve(int position,T &output) const;
	ErrorCode_List SwapData(int position1,int position2);
	ErrorCode_List Sort(int (*cmp)(const T &a,const T &b));
	Client<T> *getTail() const;
	//ErrorCode_List TransformFromBinaryTree(BinaryTree<T> &original);
private:
	int count;
	Client<T> *pHead;
	Client<T> *pTail;
	Client<T> *ArbitraryPointer;//用于指向常用值,应用于Find
	ErrorCode_List sort(Client<T> *pFirst,Client<T> *pFinal,int (*cmp)(const T &a,const T &b));
};

template<class T>
Chain<T>::Chain()
{
	count=0;
	pHead=nullptr;
	pTail=nullptr;
	ArbitraryPointer=nullptr;
}

template<class T>
Chain<T>::Chain(const Chain &original)
{
	count=original.count;
	if(original.pHead==nullptr)
	{
		pHead=pTail=ArbitraryPointer=nullptr;
	}
	else
	{
		count=original.count;
		pHead=new Client<T>;
		pHead->Data=original.pHead->Data;
		Client<T> *tmpPointerOrigin=original.pHead;
		Client<T> *Final=original.pTail;
		Client<T> *tmpPointerNew=pHead;
		for(;tmpPointerOrigin!=Final;tmpPointerOrigin=tmpPointerOrigin->pNext)
		{
			tmpPointerNew->Data=tmpPointerOrigin->Data;
			tmpPointerNew->pNext=new Client<T>;
			tmpPointerNew->pNext->pLast=tmpPointerNew;
			tmpPointerNew=tmpPointerNew->pNext;
		}
		tmpPointerNew->Data=original.pTail->Data;
		tmpPointerNew->pNext=nullptr;
		pTail=tmpPointerNew;
	}
}

template<class T>
Chain<T>::~Chain()
{
	Clear();
}

template<class T>
void Chain<T>::operator =(const Chain &original)
{
	if(this==&original)
		return;
	else if(original.pHead==nullptr)
	{
		Clear();
		pHead=pTail=ArbitraryPointer=nullptr;
		count=0;
	}
	else
	{
		Clear();
		count=original.count;
		pHead=new Client<T>;
		pHead->Data=original.pHead->Data;
		Client<T> *tmpPointerOrigin=original.pHead;
		Client<T> *Final=original.pTail;
		Client<T> *tmpPointerNew=pHead;
		for(;tmpPointerOrigin!=Final;tmpPointerOrigin=tmpPointerOrigin->pNext)
		{
			tmpPointerNew->Data=tmpPointerOrigin->Data;
			tmpPointerNew->pNext=new Client<T>;
			tmpPointerNew->pNext->pLast=tmpPointerNew;
			tmpPointerNew=tmpPointerNew->pNext;
		}
		tmpPointerNew->Data=original.pTail->Data;
		tmpPointerNew->pNext=nullptr;
		pTail=tmpPointerNew;
	}
}

template<class T>
int Chain<T>::getSize() const
{
	return count;
}

template<class T>
bool Chain<T>::isFull() const
{
	Client<T> *tmpPointer=new Client<T>;
	if(tmpPointer==nullptr)
	{
		return true;
	}
	else
	{
		delete tmpPointer;
	}
	return count>=MaxChainLength;
}

template<class T>
bool Chain<T>::isEmpty() const
{
	return count==0;
}

template<class T>
bool Chain<T>::isCircle()
{

	return (pTail->pNext==pHead&&pHead->pLast==pTail);
}

template<class T>
bool Chain<T>::isLine()
{

	return (pTail->pNext==nullptr&&pHead->pLast==nullptr);
}

template<class T>
Client<T> *Chain<T>::getArbitary() const
{
	return ArbitraryPointer;
}

template<class T>
ErrorCode_List Chain<T>::DeleteFinal()
{
	if(isEmpty())
		return underflow;
	else if(count==1)
	{
		delete pTail;
		pHead=pTail=nullptr;
	}
	else
	{
		Client<T> *tmpPointer=pTail;
		bool Circle=false;
		if(isCircle())
			Circle=true;
		pTail=pTail->pLast;
		if(Circle)
			pTail->pNext=pHead;
		else
			pTail->pNext=nullptr;
		delete tmpPointer;
	}
	count--;
	return success;
}

template<class T>
void Chain<T>::Clear()
{
	for(;!isEmpty();)
	{
		DeleteFinal();
	}
}

template<class T>
Client<T> *Chain<T>::getAddressFromPosition(int position) const
{
	if(isEmpty())
		return nullptr;
	else if(position<0||position>=count)
		return nullptr;
	else
	{
		if(position<count/2)
		{
			int counter=0,target=position;
			Client<T> *returnPointer=pHead;
			for(;counter<target;counter++)
			{
				returnPointer=returnPointer->pNext;
			}
			return returnPointer;
		}
		else
		{
			int counter=0,target=count-position-1;
			Client<T> *returnPointer=pTail;
			for(;counter<target;counter++)
			{
				returnPointer=returnPointer->pLast;
			}
			return returnPointer;
		}
	}

}

template<class T>
Client<T> *Chain<T>::getAddressFromData(const T &toFind) const
{
	if(isEmpty())
		return nullptr;
	else
	{
		Client<T> *FindPointer=pHead;
		do
		{
			if(FindPointer->Data==toFind)
				return FindPointer;
			else
				FindPointer=FindPointer->pNext;
		}
		while(FindPointer!=nullptr);
		return nullptr;
	}
}

template<class T>
ErrorCode_List Chain<T>::Extract(int position,T &output)
{
	if(isEmpty())
		return underflow;
	else if(position<0||position>=count)
		return ErrorCode_List::range_error;
	else
	{
		Client<T> *ExtractPointer=getAddressFromPosition(position);
		output=ExtractPointer->Data;
		if(ExtractPointer!=pHead&&ExtractPointer!=pTail)
		{
			ExtractPointer->pLast->pNext=ExtractPointer->pNext;
			ExtractPointer->pNext->pLast=ExtractPointer->pLast;
		}
		else if(ExtractPointer==pHead)
		{
			ExtractPointer->pNext->pLast=ExtractPointer->pLast;
			pHead=ExtractPointer->pNext;
		}
		else if(ExtractPointer==pTail)
		{
			ExtractPointer->pLast->pNext=ExtractPointer->pNext;
			pTail=ExtractPointer->pLast;
		}
		delete ExtractPointer;
		ExtractPointer=nullptr;
		count--;
		return success;
	}

}
template<class T>
ErrorCode_List Chain<T>::AddFirst(const T &toWrite)
{
	if(isFull())
		return overflow;
	if(count>0)
	{
		Client<T> *NewClient=new Client<T>;
		NewClient->Data=toWrite;
		bool Circle=false;
		if(isCircle())
			Circle=true;
		pHead->pLast=NewClient;
		NewClient->pNext=pHead;
		pHead=pHead->pLast;
		if(Circle)
		{
			pHead->pLast=pTail;
			pTail->pNext=pHead;
		}
		else
		{
			pHead->pLast=nullptr;
		}
		count++;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::AddFinal(const T &toWrite)
{
	if(isFull())
		return overflow;
	if(count>0)
	{
		Client<T> *NewClient=new Client<T>;
		NewClient->Data=toWrite;
		bool Circle=false;
		if(isCircle())
			Circle=true;
		pTail->pNext=NewClient;
		NewClient->pLast=pTail;
		pTail=pTail->pNext;
		if(Circle)
		{
			pTail->pNext=pHead;
			pHead->pLast=pTail;
		}
		else
			pTail->pNext=nullptr;
	}
	else
	{
		pHead=new Client<T>;
		pHead->Data=toWrite;
		pHead->pLast=nullptr;
		pHead->pNext=nullptr;
		pTail=pHead;
	}
	count++;
	return success;
}

template<class T>
ErrorCode_List Chain<T>::Delete(int position)
{
	if(isEmpty())
		return underflow;
	else if(position<0||position>=count)
		return ErrorCode_List::range_error;
	else
	{
		Client<T> *DeletePointer=getAddressFromPosition(position);
		if(DeletePointer!=pHead&&DeletePointer!=pTail)
		{
			DeletePointer->pLast->pNext=DeletePointer->pNext;
			DeletePointer->pNext->pLast=DeletePointer->pLast;
		}
		else if(DeletePointer==pHead)
		{
			DeletePointer->pNext->pLast=DeletePointer->pLast;
			pHead=DeletePointer->pNext;
		}
		else if(DeletePointer==pTail)
		{
			DeletePointer->pLast->pNext=DeletePointer->pNext;
			pTail=DeletePointer->pLast;
		}
		delete DeletePointer;
		DeletePointer=nullptr;
		count--;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::DeleteAddress(Client<T> *DeletePointer)
{
	if(DeletePointer==nullptr||isEmpty())
		return ErrorCode_List::range_error;
	else if(DeletePointer!=pHead&&DeletePointer!=pTail)
	{
		DeletePointer->pLast->pNext=DeletePointer->pNext;
		DeletePointer->pNext->pLast=DeletePointer->pLast;
	}
	else if(DeletePointer==pHead)
	{
		if(pHead->pNext==nullptr)
		{
			delete DeletePointer;
			count--;
			return success;
		}
		DeletePointer->pNext->pLast=DeletePointer->pLast;
		pHead=DeletePointer->pNext;
	}
	else if(DeletePointer==pTail)
	{
		DeletePointer->pLast->pNext=DeletePointer->pNext;
		pTail=DeletePointer->pLast;
	}
	delete DeletePointer;
	DeletePointer=nullptr;
	count--;
	return success;
}

template<class T>
ErrorCode_List Chain<T>::Insert(int position,const T &toWrite)
{
	if(isFull())
		return overflow;
	else if(position<0||position>=count)
		return ErrorCode_List::range_error;
	else if(count==0)
	{
		pHead=new Client<T>;
		pHead->Data=toWrite;
		pHead->pLast=nullptr;
		pHead->pNext=nullptr;
		pTail=pHead;
	}
	else
	{
		Client<T> *InsertPointer=getAddressFromPosition(position);
		if(InsertPointer!=pTail)
		{
			Client<T> *Next=InsertPointer->pNext;
			Client<T> *NewClient=new Client<T>;
			NewClient->Data=toWrite;
			InsertPointer->pNext=NewClient;
			NewClient->pNext=Next;
			Next->pLast=NewClient;
			NewClient->pLast=InsertPointer;
		}
		else
		{
			Client<T> *NewClient=new Client<T>;
			NewClient->Data=toWrite;
			bool Circle=false;
			if(isCircle())
				Circle=true;
			pTail->pNext=NewClient;
			NewClient->pLast=pTail;
			pTail=pTail->pNext;
			if(Circle)
			{
				pTail->pNext=pHead;
				pHead->pLast=pTail;
			}
			else
			{
				pTail->pNext=nullptr;
			}
		}
		count++;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::Modify(int position,const T &toWrite)
{
	if(isEmpty())
		return underflow;
	else if(position<0||position>=count)
		return ErrorCode_List::range_error;
	else
	{
		Client<T> *ModifyPointer=getAddressFromPosition(position);
		ModifyPointer->Data=toWrite;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::Retrieve(int position,T &output) const
{
	if(isEmpty())
		return underflow;
	else if(position<0||position>=count)
		return ErrorCode_List::range_error;
	else
	{
		Client<T> *RetrievePointer=getAddressFromPosition(position);
		output=RetrievePointer->Data;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::SwapData(int position1,int position2)
{
	if(isEmpty())
		return underflow;
	else if(position1<0||position1>=count||position2<0||position2>=count)
		return ErrorCode_List::range_error;
	else
	{
		Client<T> *SwapPointer1=getAddressFromPosition(position1);
		Client<T> *SwapPointer2=getAddressFromPosition(position2);
		T tmp;
		tmp=SwapPointer1->Data;
		SwapPointer1->Data=SwapPointer2->Data;
		SwapPointer2->Data=tmp;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::Sort(int (*cmp)(const T &,const T &))
{
	return sort(pHead,pTail,cmp);
}

template<class T>
ErrorCode_List Chain<T>::sort(Client<T> *pFirst,Client<T> *pFinal,int (*cmp)(const T &,const T &))
{
	Client<T> *TracePointer=pFirst;
	Client<T> *MiddlePointer=pFirst;
	int halfCount=0;
	for(;TracePointer!=pFinal&&TracePointer!=nullptr;)
	{
		TracePointer=TracePointer->pNext;
		MiddlePointer=MiddlePointer->pNext;
		TracePointer=TracePointer->pNext;
		halfCount++;
	}
	if(halfCount<InsertionSortLimit_Chain/2)
		for(Client<T> *TraversePointer=pFirst->pNext;TraversePointer!=nullptr;TraversePointer=TraversePointer->pNext)
			if(cmp(TraversePointer->pLast->Data,TraversePointer->Data)>0)
			{
				T storage=TraversePointer->Data;
				Client<T> *WaitPointer=TraversePointer->pLast;
				for(;WaitPointer!=nullptr&&cmp(WaitPointer->Data,storage)>0;WaitPointer=WaitPointer->pLast)
					WaitPointer->pNext->Data=WaitPointer->Data;
				if(WaitPointer==nullptr)
					pFirst->Data=storage;
				else
					WaitPointer->pNext->Data=storage;
			}
			else
			{
				Client<T> *MidLastPointer=MiddlePointer->pLast;
				MidLastPointer->pNext=nullptr;
				MiddlePointer->pLast=nullptr;
				sort(pFirst,MidLastPointer,cmp);
				sort(MiddlePointer,pFinal,cmp);
				MidLastPointer->pNext=MiddlePointer;
				MiddlePointer->pLast=MidLastPointer;
			}
	return success;
}

template<class T>
ErrorCode_List Chain<T>::ExtractAddress(Client<T> *ExtractPointer,T &output)
{
	if(ExtractPointer==nullptr||isEmpty())
		return underflow;
	else
	{
		output=ExtractPointer->Data;
		if(ExtractPointer!=pHead&&ExtractPointer!=pTail)
		{
			ExtractPointer->pLast->pNext=ExtractPointer->pNext;
			ExtractPointer->pNext->pLast=ExtractPointer->pLast;
		}
		else if(ExtractPointer==pHead)
		{
			ExtractPointer->pNext->pLast=ExtractPointer->pLast;
			pHead=ExtractPointer->pNext;
		}
		else if(ExtractPointer==pTail)
		{
			ExtractPointer->pLast->pNext=ExtractPointer->pNext;
			pTail=ExtractPointer->pLast;
		}
		delete ExtractPointer;
		ExtractPointer=nullptr;
		count--;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::ExtractFinal(T &output)
{
	if(isEmpty())
		return underflow;
	else
	{
		Client<T> *ExtractPointer=pTail;
		pTail=pTail->pLast;
		output=ExtractPointer->Data;
		if(ExtractPointer->pLast!=nullptr)
			ExtractPointer->pLast->pNext=ExtractPointer->pNext;
		delete ExtractPointer;
		ExtractPointer=nullptr;
		count--;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::FormCircle()
{
	if(isEmpty())
		return underflow;
	else if(isCircle())
		return success;
	else
	{
		pTail->pNext=pHead;
		pHead->pLast=pTail;
		isCircle();
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::FormLine()
{
	if(isEmpty())
		return underflow;
	else
	{
		if(count==1)
		{
			pHead->pLast=nullptr;
			pHead->pNext=nullptr;
			pTail=pHead;
		}
		else
		{
			pHead->pLast=nullptr;
			pTail->pNext=nullptr;
		}
		isLine();
		return success;
	}
}

template<class T>
void Chain<T>::Traverse(void ( *visit)(T &))
{
	if(isEmpty())
		return;
	Client<T> *TraversePointer=pHead;
	if(isCircle())
	{
		do
		{
			visit(TraversePointer->Data);
			TraversePointer=TraversePointer->pNext;
		}
		while(TraversePointer!=pHead);
	}
	else
	{
		do
		{
			visit(TraversePointer->Data);
			TraversePointer=TraversePointer->pNext;
		}
		while(TraversePointer!=nullptr);
	}
}
template<class T>
Client<T> *Chain<T>::getHead() const
{
	return pHead;
}

template<class T>
Client<T> *Chain<T>::getTail() const
{
	return pTail;
}
#endif //BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSECHAIN_H
