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

#ifndef BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSESEQUENCE_H
#define BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSESEQUENCE_H

#include <cstdlib>
#include <iostream>
#include "Constants.h"


template<class T>
class Sequence
{
public:
	Sequence();
	bool isCurrentFull() const;
	bool isFull() const;
	bool isEmpty() const;
	int getSize() const;//当前空间大小
	int getUsage() const;//已用大小
	int getSubscriptFromData(const T &toFind);//顺序查找,假设无重复
	void Clear();
	void SyncSize();
	void Traverse(void ( *visit)(T &));
	ErrorCode_List AddFirst(const T &toWrite);//慎用,效率极低
	ErrorCode_List AddFinal(const T &toWrite);
	ErrorCode_List Delete(int position);
	ErrorCode_List DeleteFinal();
	ErrorCode_List Extract(int position,T &output);//类似ServeAndRetrieve
	ErrorCode_List Insert(int position,const T &toWrite);//postion位置之后插入
	ErrorCode_List Modify(int position,const T &toWrite);
	ErrorCode_List Retrieve(int position,T &output) const;
	ErrorCode_List Swap(int position1,int position2);
	ErrorCode_List TransferToDesignatedSize(const int SizeToCreate);
	ErrorCode_List Write(int position,const T &toWrite);
	T *getPositionPointer(int position) const;
	int CopyOfSize;
	//ErrorCode_List Sort();
	//复制构造函数，操作符重载
private:
	int size;
	int front;
	int rear;
	int count;
	T *Primacy;
};

template<class T>
Sequence<T>::Sequence()
{
	size=InitialSequenceSize;
	rear=0;
	front=0;
	count=0;
	Primacy=new T[InitialSequenceSize];
	CopyOfSize=size;
}

template<class T>
bool Sequence<T>::isEmpty() const
{
	return count==0;
}

template<class T>
int Sequence<T>::getSize() const
{
	return size;
}

template<class T>
int Sequence<T>::getUsage() const
{
	return count;
}

template<class T>
bool Sequence<T>::isCurrentFull() const
{
	return rear>=size;
}

template<class T>
bool Sequence<T>::isFull() const
{
	if(isCurrentFull())
	{
		if(size>=MaxSequenceLength||size+NewDynamicArrayIncrement>=MaxSequenceLength)
			return true;
		else
		{
			T *NewArray=new T[size+NewDynamicArrayIncrement];
			if(NewArray==NULL)
				return true;
			else
			{
				delete NewArray;
				return false;
			}
		}
	}
	return false;
}

template<class T>
int Sequence<T>::getSubscriptFromData(const T &toFind)
{
	if(isEmpty())
		return -1;
	else
	{
		for(int i=front;i<rear;i++)
		{
			if(Primacy[i]==toFind)
			{
				return i;
			}
		}
		return -1;
	}
}

template<class T>
void Sequence<T>::Clear()
{
	size=InitialSequenceSize;
	rear=0;
	count=0;
	front=0;
	delete[] Primacy;
	*Primacy=new T[InitialSequenceSize];
	CopyOfSize=0;
}

template<class T>
void Sequence<T>::SyncSize()
{
	CopyOfSize=size;
}

template<class T>
void Sequence<T>::Traverse(void (*visit)(T &))
{
	if(isEmpty())
		return;
	else
	{
		for(int i=front;i<rear;i++)
		{
			visit(Primacy[i]);
		}
	}
}

template<class T>
ErrorCode_List Sequence<T>::AddFirst(const T &toWrite)
{
	if(front>0)
	{
		Primacy[front-1]=toWrite;
		front--;
		count++;
	}
	else if(size>0&&rear<size)
	{
		for(int i=rear-1;i>=front;i--)
		{
			Primacy[i+1]=Primacy[i];
		}
		Primacy[front]=toWrite;
		rear++;
		count++;
	}
	else if(size>0&&rear>=size)
	{
		if(isFull())
			return overflow;
		T *NewArray=new T[size+NewDynamicArrayIncrement];
		size=size+NewDynamicArrayIncrement;
		NewArray[0]=toWrite;
		for(int i=front;i<rear;i++)
		{
			NewArray[i-front+1]=Primacy[i];
		}
		delete[]Primacy;
		Primacy=NewArray;
		front=0;
		rear++;
		count++;
	}
	else if(size==0)
	{
		if(Primacy!=NULL)
		{
			std::cerr<<"Inner Error -Exception occurs when processing."<<std::endl;
			std::cerr<<"Data may be lost."<<std::endl;
			delete[]Primacy;
		}
		T *NewArray=new T[InitialSequenceSize];
		size=InitialSequenceSize;
		front=0;
		count=0;
		rear=0;
		Primacy=NewArray;
		Primacy[0]=toWrite;
		rear++;
		count++;
	}
	SyncSize();
	return success;
}

template<class T>
ErrorCode_List Sequence<T>::AddFinal(const T &toWrite)
{
	if(size>0&&rear<size)
	{
		Primacy[rear]=toWrite;
		rear++;
	}
	else if(size>0&&rear>=size)
	{
		if(front>0)
		{
			for(int i=front;i<rear;i++)
			{
				Primacy[i-1]=Primacy[i];
			}
			Primacy[rear-1]=toWrite;
			front--;
			count++;
		}
		else
		{
			if(isFull())
				return overflow;
			T *NewArray=new T[size+NewDynamicArrayIncrement];
			size=size+NewDynamicArrayIncrement;
			for(int i=front;i<rear;i++)
			{
				NewArray[i-front]=Primacy[i];
			}
			NewArray[rear]=toWrite;
			front=0;
			rear++;
			count++;
			delete[]Primacy;
			Primacy=NewArray;
		}
	}
	else if(size==0)
	{
		if(Primacy!=NULL)
		{
			std::cerr<<"Inner Error -Exception occurs when processing."<<std::endl;
			std::cerr<<"Data may be lost."<<std::endl;
			delete[]Primacy;
		}
		T *NewArray=new T[InitialSequenceSize];
		size=InitialSequenceSize;
		front=0;
		count=0;
		rear=0;
		Primacy=NewArray;
		Primacy[0]=toWrite;
		rear++;
		count++;
	}
	SyncSize();
	return success;
}

template<class T>
ErrorCode_List Sequence<T>::Delete(int position)
{
	int realPosition=position+front;
	if(isEmpty())
		return underflow;
	else if(realPosition<front||realPosition>=rear)
		return range_error;
	else
	{
		for(int i=realPosition;i<rear-1;i++)
		{
			Primacy[i]=Primacy[i+1];
		}
		rear--;
		count--;
		SyncSize();
		return success;
	}
}

template<class T>
ErrorCode_List Sequence<T>::DeleteFinal()
{
	if(isEmpty())
		return underflow;
	else
	{
		rear--;
		count--;
	}
	SyncSize();
	return success;
}

template<class T>
ErrorCode_List Sequence<T>::Extract(int position,T &output)
{
	int realPosition=position+front;
	if(isEmpty())
		return underflow;
	else if(realPosition<front||realPosition>=rear)
		return range_error;
	else
	{
		output=Primacy[position];
		for(int i=realPosition;i<rear-1;i++)
		{
			Primacy[i]=Primacy[i+1];
		}
		rear--;
		count--;
		SyncSize();
		return success;
	}
}

template<class T>
ErrorCode_List Sequence<T>::Insert(int position,const T &toWrite)
{
	int realPosition=position+front;
	if(isEmpty())
		return underflow;
	else if(realPosition<front||realPosition>=rear)
		return range_error;
	else
	{
		if(size>0&&front>0)
		{
			for(int i=front;i<=realPosition;i++)
			{
				Primacy[i-1]=Primacy[i];
			}
			Primacy[realPosition+1]=toWrite;
			front--;
			count++;
		}
		else if(size>0&&rear<size)
		{
			for(int i=rear-1;i>realPosition;i--)
			{
				Primacy[i+1]=Primacy[i];
			}
			Primacy[realPosition+1]=toWrite;
			rear++;
			count++;
		}
		else if(size>0&&rear>=size)
		{
			if(isFull())
				return overflow;
			T *NewArray=new T[size+NewDynamicArrayIncrement];
			size=size+NewDynamicArrayIncrement;
			for(int i=front;i<=realPosition;i++)
			{
				NewArray[i-front]=Primacy[i];
			}
			NewArray[position+1]=toWrite;
			for(int i=realPosition+1;i<rear;i++)
			{
				NewArray[i-front+1]=Primacy[i];
			}
			delete[]Primacy;
			Primacy=NewArray;
			rear=count+1;
			front=0;
			count++;
		}
		else if(size==0)
		{
			if(Primacy!=NULL)
			{
				std::cerr<<"Inner Error -Exception occurs when processing."<<std::endl;
				std::cerr<<"Data may be lost."<<std::endl;
				delete[]Primacy;
			}
			T *NewArray=new T[InitialSequenceSize];
			size=InitialSequenceSize;
			front=0;
			count=0;
			rear=0;
			Primacy=NewArray;
			Primacy[0]=toWrite;
			rear++;
			count++;
		}
		SyncSize();
		return success;
	}
}

template<class T>
ErrorCode_List Sequence<T>::Modify(int position,const T &toWrite)
{
	int realPosition=position+front;
	if(isEmpty())
		return underflow;
	else if(realPosition<front||realPosition>=rear)
		return range_error;
	else
	{
		Primacy[realPosition]=toWrite;
		return success;
	}
}

template<class T>
ErrorCode_List Sequence<T>::Retrieve(int position,T &output) const
{
	int realPosition=position+front;
	if(isEmpty())
		return underflow;
	else if(realPosition<front||realPosition>=rear)
		return range_error;
	else
	{
		output=Primacy[realPosition];
		return success;
	}
}

template<class T>
ErrorCode_List Sequence<T>::Swap(int position1,int position2)
{
	int realPosition1=position1+front;
	int realPosition2=position2+front;
	if(isEmpty())
		return underflow;
	else if(realPosition1<front||realPosition1>=rear||realPosition2<front||realPosition2>=rear)
		return range_error;
	else
	{
		T tmp=Primacy[realPosition1];
		Primacy[realPosition1]=Primacy[realPosition2];
		Primacy[realPosition2]=tmp;
		return success;
	}
}

template<class T>
ErrorCode_List Sequence<T>::TransferToDesignatedSize(const int SizeToCreate)
{
	if(size>SizeToCreate)
		return underflow;
	else if(isFull())
		return overflow;
	else if(SizeToCreate>MaxSequenceLength)
		return overflow;
	else
	{
		T *NewArray=new T[SizeToCreate];
		size=SizeToCreate;
		for(int i=front;i<rear;i++)
		{
			NewArray[i-front]=Primacy[i];
		}
		delete[] Primacy;
		front=0;
		rear=count;
		Primacy=NewArray;
		return success;
	}
}

template<class T>
ErrorCode_List Sequence<T>::Write(int position,const T &toWrite)
{
	int realPosition=position+front;
	if(isEmpty())
		return underflow;
	else if(realPosition<front||realPosition>=rear)
		return range_error;
	else
	{
		Primacy[position]=toWrite;
	}
}

template<class T>
T *Sequence<T>::getPositionPointer(int position) const
{
	return Primacy+position;
}
/*
template <class T>
Sequence<T>:
*/
#endif //BASICDATASTRUCTUREANDALGORITHMS_MULTIPURPOSESEQUENCE_H
