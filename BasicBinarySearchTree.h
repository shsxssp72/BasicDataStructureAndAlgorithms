//
// Created by 星落_月残 on 2017/5/9.
//

#ifndef BASICDATASTRUCTUREANDALGORITHMS_BASICBINARYSEARCHTREE_H
#define BASICDATASTRUCTUREANDALGORITHMS_BASICBINARYSEARCHTREE_H

#include "BasicBinaryTree.h"


template<class T>
class BinarySearchTree: public BinaryTree<T>
{
public:
	ErrorCode_List InsertClient_Binary(T toWrite);
	ErrorCode_List InsertClient_Binary(T toWrite,long long int inIndex);
	BinaryClient<T> *getAddressFromData_Binary(BinaryClient<T> *Entrance,const T toFind);
	BinaryClient<T> *getAddressFromIndex_Binary(BinaryClient<T> *Entrance,long long inIndex);
	ErrorCode_List DeleteClient_Binary(BinaryClient<T> *toDelete);
	//ErrorCode_List TransformFromChain(const Chain<T> &original);
	BinaryClient<T> *getMaxAddress_Binary(BinaryClient<T> *Entrance);
	BinaryClient<T> *getMinAddress_Binary(BinaryClient<T> *Entrance);
protected:
	ErrorCode_List insertRecursive_Binary(BinaryClient<T> *&Entrance,BinaryClient<T> *Senior,const T &toWrite);
	ErrorCode_List insertRecursive_Binary(BinaryClient<T> *&Entrance,BinaryClient<T> *Senior,const T &toWrite,long long int inIndex);
};

template<class T>
ErrorCode_List BinarySearchTree<T>::InsertClient_Binary(T toWrite)
{
	if(BinaryTree<T>::Root->Index>static_cast<long long>(toWrite))
		return insertRecursive_Binary(BinaryTree<T>::Root->pLeftJunior,BinaryTree<T>::Root,toWrite);
	else if(BinaryTree<T>::Root->Index<static_cast<long long>(toWrite))
		return insertRecursive_Binary(BinaryTree<T>::Root->pRightJunior,BinaryTree<T>::Root,toWrite);
	else
		return duplicate;
}

template<class T>
ErrorCode_List BinarySearchTree<T>::insertRecursive_Binary(BinaryClient<T> *&Entrance,BinaryClient<T> *Senior,const T &toWrite)
{
	if(Entrance==nullptr)
	{
		Entrance=new BinaryClient<T>(toWrite);
		Entrance->Index=static_cast<long long>(toWrite);
		Entrance->pSenior=Senior;
		BinaryTree<T>::count++;
		return success;
	}
	else if(Entrance->Index>static_cast<long long>(toWrite))
		return insertRecursive_Binary(Entrance->pLeftJunior,Entrance,toWrite);
	else if(Entrance->Index<static_cast<long long>(toWrite))
		return insertRecursive_Binary(Entrance->pRightJunior,Entrance,toWrite);
	else
		return duplicate;
}


template<class T>
ErrorCode_List BinarySearchTree<T>::InsertClient_Binary(T toWrite,long long inIndex)
{
	if(BinaryTree<T>::Root->Index>inIndex)
		return insertRecursive_Binary(BinaryTree<T>::Root->pLeftJunior,BinaryTree<T>::Root,toWrite,inIndex);
	else if(BinaryTree<T>::Root->Index<inIndex)
		return insertRecursive_Binary(BinaryTree<T>::Root->pRightJunior,BinaryTree<T>::Root,toWrite,inIndex);
	else
		return duplicate;
}

template<class T>
ErrorCode_List BinarySearchTree<T>::insertRecursive_Binary(BinaryClient<T> *&Entrance,BinaryClient<T> *Senior,const T &toWrite,long long inIndex)
{
	if(Entrance==nullptr)
	{
		Entrance=new BinaryClient<T>(toWrite);
		Entrance->Index=inIndex;
		Entrance->pSenior=Senior;
		BinaryTree<T>::count++;
		return success;
	}
	else if(Entrance->Index>inIndex)
		return insertRecursive_Binary(Entrance->pLeftJunior,Entrance,toWrite,inIndex);
	else if(Entrance->Index<inIndex)
		return insertRecursive_Binary(Entrance->pRightJunior,Entrance,toWrite,inIndex);
	else
		return duplicate;
}

template<class T>
BinaryClient<T> *BinarySearchTree<T>::getAddressFromData_Binary(BinaryClient<T> *Entrance,const T toFind)
{
	if(Entrance==nullptr)
		return nullptr;
	else if(Entrance->Index==static_cast<long long>(toFind))
		return Entrance;
	else if(Entrance->Index>static_cast<long long>(toFind))
		return getAddressFromData_Binary(Entrance->pLeftJunior,toFind);
	else if(Entrance->Index<static_cast<long long>(toFind))
		return getAddressFromData_Binary(Entrance->pRightJunior,toFind);
}

template<class T>
BinaryClient<T> *BinarySearchTree<T>::getAddressFromIndex_Binary(BinaryClient<T> *Entrance,long long inIndex)
{
	if(Entrance==nullptr)
		return nullptr;
	else if(Entrance->Index==inIndex)
		return Entrance;
	else if(Entrance->Index>inIndex)
		return getAddressFromIndex_Binary(Entrance->pLeftJunior,inIndex);
	else if(Entrance->Index<inIndex)
		return getAddressFromIndex_Binary(Entrance->pRightJunior,inIndex);
}

template<class T>
BinaryClient<T> *BinarySearchTree<T>::getMaxAddress_Binary(BinaryClient<T> *Entrance)
{
	if(Entrance==nullptr)
		return nullptr;
	BinaryClient<T> *MaxPointer=Entrance;
	for(;MaxPointer->pRightJunior!=nullptr;MaxPointer=MaxPointer->pRightJunior);
	return MaxPointer;
}

template<class T>
BinaryClient<T> *BinarySearchTree<T>::getMinAddress_Binary(BinaryClient<T> *Entrance)
{
	if(Entrance==nullptr)
		return nullptr;
	BinaryClient<T> *MinPointer=Entrance;
	for(;MinPointer->pLeftJunior!=nullptr;MinPointer=MinPointer->pLeftJunior);
	return MinPointer;
}

template<class T>
ErrorCode_List BinarySearchTree<T>::DeleteClient_Binary(BinaryClient<T> *toDelete)
{
	/*	if(toDelete==nullptr)
			return underflow;
		else */if(toDelete==BinaryTree<T>::Root)
	{
		BinaryClient<T> *Precursor=getMaxAddress_Binary(BinaryTree<T>::Root->pLeftJunior);
		if(Precursor!=nullptr)
		{
			BinaryTree<T>::Root->Data=Precursor->Data;
			BinaryTree<T>::Root->Index=Precursor->Index;
			DeleteClient_Binary(Precursor);
		}
		else
		{
			BinaryClient<T> *Successor=getMinAddress_Binary(BinaryTree<T>::Root->pRightJunior);
			if(Successor!=nullptr)
			{
				BinaryTree<T>::Root->Data=Successor->Data;
				BinaryTree<T>::Root->Index=Successor->Index;
				DeleteClient_Binary(Successor);
			}
			else//Root没有孩子
			{
				delete BinaryTree<T>::Root;
			}
		}

	}
	else if(toDelete->pLeftJunior==nullptr&&toDelete->pRightJunior==nullptr)
	{
		if(toDelete->pSenior->pLeftJunior==toDelete)
			toDelete->pSenior->pLeftJunior=nullptr;
		else if(toDelete->pSenior->pRightJunior==toDelete)
			toDelete->pSenior->pRightJunior=nullptr;
		delete toDelete;
	}
	else if(toDelete->pLeftJunior!=nullptr&&toDelete->pRightJunior==nullptr)
	{
		if(toDelete->pSenior->pLeftJunior==toDelete)
			toDelete->pSenior->pLeftJunior=toDelete->pLeftJunior;
		else if(toDelete->pSenior->pRightJunior==toDelete)
			toDelete->pSenior->pRightJunior=toDelete->pLeftJunior;
		toDelete->pLeftJunior->pSenior=toDelete->pSenior;
		delete toDelete;
	}
	else if(toDelete->pRightJunior!=nullptr&&toDelete->pLeftJunior==nullptr)
	{
		if(toDelete->pSenior->pLeftJunior==toDelete)
			toDelete->pSenior->pLeftJunior=toDelete->pRightJunior;
		else if(toDelete->pSenior->pRightJunior==toDelete)
			toDelete->pSenior->pRightJunior=toDelete->pRightJunior;
		toDelete->pRightJunior->pSenior=toDelete->pSenior;
		delete toDelete;
	}
	else if(toDelete->pLeftJunior!=nullptr&&toDelete->pRightJunior!=nullptr)
	{
		BinaryClient<T> *Precursor=getMaxAddress_Binary(toDelete->pLeftJunior);
		toDelete->Data=Precursor->Data;
		toDelete->Index=Precursor->Index;
		DeleteClient_Binary(Precursor);
	}
}

#endif //BASICDATASTRUCTUREANDALGORITHMS_BASICBINARYSEARCHTREE_H
