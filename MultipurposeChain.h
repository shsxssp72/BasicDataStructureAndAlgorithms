//
// Created by 星落_月残 on 2017/4/13.
//

#ifndef ERRORCODE_LIST
#define ERRORCODE_LIST
enum ErrorCode_List
{
	underflow,overflow,success,range_error,non_existence
};
#endif

#ifndef BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSECHAIN_H
#define BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSECHAIN_H

#include <cstdlib>
#include "Constants.h"


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
	pLast=NULL;
	pNext=NULL;
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
	ErrorCode_List FormCircle();
	ErrorCode_List FormLine();
	ErrorCode_List Insert(int position,const T &toWrite);
	ErrorCode_List Modify(int position,const T &toWrite);//postion位置之后插入
	ErrorCode_List Retrieve(int position,T &output) const;
	ErrorCode_List Swap(int position1,int position2);
	//ErrorCode_List Sort();
	//ErrorCode_List TransformToBinaryTree();
private:
	int count;
	bool circle;
	bool line;
	Client<T> *pHead;
	Client<T> *pTail;
	Client<T> *ArbitraryPointer;//用于指向常用值,应用于Find
};

template<class T>
Chain<T>::Chain()
{
	count=0;
	pHead=NULL;
	pTail=NULL;
	ArbitraryPointer=NULL;
}

template<class T>
Chain<T>::Chain(const Chain &original)
{
	count=original.count;
	circle=false;
	line=true;
	if(original.pHead==NULL)
	{
		pHead=pTail=ArbitraryPointer=NULL;
	}
	else
	{
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
		tmpPointerNew->pNext=NULL;
	}
}

template<class T>
Chain<T>::~Chain()
{
	Clear();
	if(pHead!=NULL)
		delete pHead;
	if(pTail!=NULL)
		delete pTail;
}

template<class T>
void Chain<T>::operator =(const Chain &original)
{
	if(this==&original)
		return;
	else if(original.pHead==NULL)
	{
		pHead=pTail=ArbitraryPointer=NULL;
	}
	else
	{
		Clear();
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
		tmpPointerNew->pNext=NULL;
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
	if(tmpPointer==NULL)
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
	return count<=0;
}

template<class T>
bool Chain<T>::isCircle()
{
	circle=(pTail->pNext==pHead&&pHead->pLast==pTail);
	return circle;
}

template<class T>
bool Chain<T>::isLine()
{
	line=(pTail->pNext==NULL&&pHead->pLast==NULL);
	return line;
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
	}
	else
	{
		Client<T> *tmpPointer=pTail;
		pTail=pTail->pLast;
		if(isCircle())
			pTail->pNext=pHead;
		else
			pTail->pNext=NULL;
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
		return NULL;
	else if(position<0||position>=count)
		return NULL;
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
		return NULL;
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
		while(FindPointer!=NULL);
		return NULL;
	}
}

template<class T>
ErrorCode_List Chain<T>::Extract(int position,T &output)
{
	if(isEmpty())
		return underflow;
	else if(position<0||position>=count)
		return range_error;
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
		ExtractPointer=NULL;
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
		pHead->pLast=NewClient;
		NewClient->pNext=pHead;
		pHead=pHead->pLast;
		if(isCircle())
		{
			pHead->pLast=pTail;
			pTail->pNext=pHead;
		}
		else
		{
			pHead->pLast=NULL;
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
		pTail->pNext=NewClient;
		NewClient->pLast=pTail;
		pTail=pTail->pNext;
		if(isCircle())
		{
			pTail->pNext=pHead;
			pHead->pLast=pTail;
		}
		else
			pTail->pNext=NULL;
	}
	else
	{
		pHead=new Client<T>;
		pHead->Data=toWrite;
		pHead->pLast=NULL;
		pHead->pNext=NULL;
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
		return range_error;
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
		DeletePointer=NULL;
		count--;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::DeleteAddress(Client<T> *DeletePointer)
{
	if(DeletePointer==NULL)
		return range_error;
	else if(DeletePointer!=pHead&&DeletePointer!=pTail)
	{
		DeletePointer->pLast->pNext=DeletePointer->pNext;
		DeletePointer->pNext->pLast=DeletePointer->pLast;
	}
	else if(DeletePointer==pHead)
	{
		if(pHead->pNext==NULL)
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
	DeletePointer=NULL;
	count--;
	return success;
}

template<class T>
ErrorCode_List Chain<T>::Insert(int position,const T &toWrite)
{
	if(isFull())
		return overflow;
	else if(position<0||position>=count)
		return range_error;
	else if(count==0)
	{
		pHead=new Client<T>;
		pHead->Data=toWrite;
		pHead->pLast=NULL;
		pHead->pNext=NULL;
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
			pTail->pNext=NewClient;
			NewClient->pLast=pTail;
			pTail=pTail->pNext;
			if(isCircle())
			{
				pTail->pNext=pHead;
				pHead->pLast=pTail;
			}
			else
			{
				pTail->pNext=NULL;
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
		return range_error;
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
		return range_error;
	else
	{
		Client<T> *RetrievePointer=getAddressFromPosition(position);
		output=RetrievePointer->Data;
		return success;
	}
}

template<class T>
ErrorCode_List Chain<T>::Swap(int position1,int position2)
{
	if(isEmpty())
		return underflow;
	else if(position1<0||position1>=count||position2<0||position2>=count)
		return range_error;
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
			pHead->pLast=NULL;
			pHead->pNext=NULL;
			pTail=pHead;
		}
		else
		{
			pHead->pLast=NULL;
			pTail->pNext=NULL;
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
		while(TraversePointer!=NULL);
	}
}

#endif //BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSECHAIN_H
