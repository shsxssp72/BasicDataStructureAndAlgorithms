//
// Created by 星落_月残 on 2017/4/18.
//
#ifndef ERRORCODE_LIST
#define ERRORCODE_LIST
enum ErrorCode_List
{
	underflow,overflow,success,range_error,non_existence
};
#endif

#ifndef BASICDATASTRUCTUREANDALGORITHMS_HASHCHAIN_H
#define BASICDATASTRUCTUREANDALGORITHMS_HASHCHAIN_H

#include "MultipurposeSequence.h"
#include "MultipurposeChain.h"


template<class T>
class HashChain: public Sequence<Chain<T> >
{
public:
	HashChain(int (*hash)(const T &input,const int size),int hash_size);
	ErrorCode_List Add(const T &toWrite);
	ErrorCode_List Remove(const T &toFind,T &output);
	ErrorCode_List getData(const T &toFind,T &output) const;
private:
	int (*HashFunction)(const T &input,const int size);
	int hashSize;
};


template<class T>
HashChain<T>::HashChain(int (*hash)(const T &input,const int size),int hash_size)
{
	HashFunction=hash;
	hashSize=hash_size;
}

template<class T>
ErrorCode_List HashChain<T>::Add(const T &toWrite)
{
	Chain<T> *PositionPointer=Sequence<Chain<T>>::getPositionPointer(HashFunction(toWrite,hashSize));
	PositionPointer->AddFinal(toWrite);
}

template<class T>
ErrorCode_List HashChain<T>::getData(const T &toFind,T &output) const
{
	Chain<T> *PositionPointer=Sequence<Chain<T>>::getPositionPointer(HashFunction(toFind,hashSize));
	Client<T> *ElementPointer=PositionPointer->getAddressFromData(toFind);
	if(ElementPointer==NULL)
		return non_existence;
	else
	{
		output=ElementPointer->Data;
		return success;
	}
}

template<class T>
ErrorCode_List HashChain<T>::Remove(const T &toFind,T &output)
{
	Chain<T> *PositionPointer=Sequence<Chain<T>>::getPositionPointer(HashFunction(toFind,hashSize));
	Client<T> *ElementPointer=PositionPointer->getAddressFromData(toFind);
	if(ElementPointer==NULL)
		return non_existence;
	else
	{
		output=ElementPointer->Data;
		PositionPointer->DeleteAddress(ElementPointer);
		return success;
	}
}

#endif //BASICDATASTRUCTUREANDALGORITHMS_HASHCHAIN_H
