//
// Created by 星落_月残 on 2017/5/17.
//

#ifndef BASICDATASTRUCTUREANDALGORITHMS_BASICSPLAYTREE_H
#define BASICDATASTRUCTUREANDALGORITHMS_BASICSPLAYTREE_H

#include "BasicBinarySearchTree.h"


template<class T>
class SplayTree:public BinarySearchTree<T>
{
public:
	ErrorCode_List SplayAndInsert(const T &toFind);
private:
	void MountToSmallTree(BinaryClient<T> *&Current,BinaryClient<T> *&Hardpoint);
	void MountToLargeTree(BinaryClient<T> *&Current,BinaryClient<T> *&Hardpoint);
	void SinistralTransformation(BinaryClient<T> *&Entrance);
	void DextralTransformation(BinaryClient<T> *&Entrance);
};


template<class T>
ErrorCode_List SplayTree<T>::SplayAndInsert(const T &toFind)
{
	BinaryClient<T> *SmallTreeRoot=new BinaryClient<T>;
	BinaryClient<T> *LargeTreeRoot=new BinaryClient<T>;
	BinaryClient<T> *TraversePointer=BinaryTree<T>::Root;
	BinaryClient<T> *ExplorerPointer=nullptr;//作为TraversePointer的孩子节点进行判断
	BinaryClient<T> *SmallHardpoint=SmallTreeRoot;
	BinaryClient<T> *LargeHardpoint=LargeTreeRoot;

	for(;TraversePointer!=nullptr&&TraversePointer->Data!=toFind;)
	{
		if(toFind<TraversePointer->Data)
		{
			ExplorerPointer=TraversePointer->pLeftJunior;
			if(ExplorerPointer==nullptr||ExplorerPointer->Data==toFind)//zig
				MountToLargeTree(TraversePointer,LargeHardpoint);
			else if(toFind<ExplorerPointer->Data)//zig-zig
			{
				DextralTransformation(TraversePointer);
				MountToLargeTree(TraversePointer,LargeHardpoint);
			}
			else//zig-zag
			{
				MountToLargeTree(TraversePointer,LargeHardpoint);
				MountToSmallTree(TraversePointer,SmallHardpoint);
			}
		}
		else if(toFind>TraversePointer->Data)
		{
			ExplorerPointer=TraversePointer->pRightJunior;
			if(ExplorerPointer==nullptr||ExplorerPointer->Data==toFind)//zag
				MountToSmallTree(TraversePointer,SmallHardpoint);
			else if(toFind<ExplorerPointer->Data)//zag-zig
			{
				SinistralTransformation(TraversePointer);
				MountToSmallTree(TraversePointer,SmallHardpoint);
			}
			else//zag-zag
			{
				MountToSmallTree(TraversePointer,SmallHardpoint);
				MountToLargeTree(TraversePointer,LargeHardpoint);
			}
		}
	}

	ErrorCode_List result;
	if(TraversePointer==nullptr)
	{
		TraversePointer=new BinaryClient<T>(toFind);
		result=new_entry;
		SmallHardpoint->pRightJunior=nullptr;
		LargeHardpoint->pLeftJunior=nullptr;
	}
	else
	{
		result=success;

		SmallHardpoint->pRightJunior=TraversePointer->pLeftJunior;
		if(TraversePointer->pLeftJunior!=nullptr)
		TraversePointer->pLeftJunior->pSenior=SmallHardpoint;
		SmallHardpoint=SmallHardpoint->pRightJunior;

		LargeHardpoint->pLeftJunior=TraversePointer->pRightJunior;
		if(TraversePointer->pRightJunior!=nullptr)
		TraversePointer->pRightJunior->pSenior=LargeHardpoint;
		LargeHardpoint=LargeHardpoint->pLeftJunior;
	}
	BinaryTree<T>::Root=TraversePointer;
	BinaryTree<T>::Root->pLeftJunior=SmallTreeRoot->pRightJunior;
	if(SmallTreeRoot->pRightJunior!=nullptr)
		SmallTreeRoot->pRightJunior->pSenior=BinaryTree<T>::Root;

	BinaryTree<T>::Root->pRightJunior=LargeTreeRoot->pLeftJunior;
	if(LargeTreeRoot->pLeftJunior!=nullptr)
		LargeTreeRoot->pLeftJunior->pSenior=BinaryTree<T>::Root;

	delete LargeTreeRoot;
	delete SmallTreeRoot;
	return result;
}


template<class T>
void SplayTree<T>::MountToLargeTree(BinaryClient<T> *&Current,BinaryClient<T> *&Hardpoint)
{
	BinaryClient<T> *tmpCurrent=Current;
	Current=Current->pLeftJunior;
	if(Current!=nullptr)
	Current->pSenior=nullptr;
	tmpCurrent->pLeftJunior=nullptr;
	tmpCurrent->pSenior=Hardpoint;
	Hardpoint->pLeftJunior=tmpCurrent;
	Hardpoint=Hardpoint->pLeftJunior;
}

template<class T>
void SplayTree<T>::MountToSmallTree(BinaryClient<T> *&Current,BinaryClient<T> *&Hardpoint)
{
	BinaryClient<T> *tmpCurrent=Current;
	Current=Current->pRightJunior;
	if(Current!=nullptr)
	Current->pSenior=nullptr;
	tmpCurrent->pRightJunior=nullptr;
	tmpCurrent->pSenior=Hardpoint;
	Hardpoint->pRightJunior=tmpCurrent;
	Hardpoint=Hardpoint->pRightJunior;
}

template<class T>
void SplayTree<T>::SinistralTransformation(BinaryClient<T> *&Entrance)
{
	if(Entrance==nullptr||Entrance->pRightJunior==nullptr)
		std::cerr<<"Error-Inner error detected in SinistralTransformation:NullPointer"<<std::endl;
	else
	{
		BinaryClient<T> *Senior=Entrance->pSenior;
		BinaryClient<T> *Right=Entrance->pRightJunior;
		BinaryClient<T> *tmpEntrance=Entrance;

		if(Senior!=nullptr)
			if(Senior->pLeftJunior==tmpEntrance)
				Senior->pLeftJunior=Right;
			else if(Senior->pRightJunior==tmpEntrance)
				Senior->pRightJunior=Right;
		Right->pSenior=Senior;

		tmpEntrance->pSenior=Right;
		if(Right->pLeftJunior!=nullptr)
			Right->pLeftJunior->pSenior=tmpEntrance;
		tmpEntrance->pRightJunior=Right->pLeftJunior;
		Right->pLeftJunior=tmpEntrance;

		Entrance=Right;
	}
}

template<class T>
void SplayTree<T>::DextralTransformation(BinaryClient<T> *&Entrance)
{
	if(Entrance==nullptr||Entrance->pLeftJunior==nullptr)
		std::cerr<<"Error-Inner error detected in DextralTransformation:NullPointer"<<std::endl;
	else
	{
		BinaryClient<T> *Senior=Entrance->pSenior;
		BinaryClient<T> *Left=Entrance->pLeftJunior;
		BinaryClient<T> *tmpEntrance=Entrance;

		if(Senior!=nullptr)
			if(Senior->pLeftJunior==tmpEntrance)
				Senior->pLeftJunior=Left;
			else if(Senior->pRightJunior==tmpEntrance)
				Senior->pRightJunior=Left;
		Left->pSenior=Senior;

		tmpEntrance->pSenior=Left;
		if(Left->pRightJunior!=nullptr)
			Left->pRightJunior->pSenior=tmpEntrance;
		tmpEntrance->pLeftJunior=Left->pRightJunior;
		Left->pRightJunior=tmpEntrance;

		Entrance=Left;
	}
}
/*
template<class T>
SplayTree<T>:
 */
#endif //BASICDATASTRUCTUREANDALGORITHMS_BASICSPLAYTREE_H
