//
// Created by 星落_月残 on 2017/5/9.
//

#ifndef BASICDATASTRUCTUREANDALGORITHMS_BASICAVLTREE_H
#define BASICDATASTRUCTUREANDALGORITHMS_BASICAVLTREE_H

#include "BasicBinarySearchTree.h"


template<class T>
class AVLTree:public BinarySearchTree<T>
{
public:
	ErrorCode_List InsertClient_AVL(const T &toWrite);
	ErrorCode_List DeleteClient_AVL(const T &toDelete);
	ErrorCode_List InsertRecursive_AVL(BinaryClient<T> *&Entrance,BinaryClient<T> *Senior,const T &toWrite,bool &taller);
	ErrorCode_List DeleteRecursive_AVL(BinaryClient<T> *&Entrance,BinaryClient<T> *Senior,const T &toDelete,bool &shorter);
	//ErrorCode_List TransformFromChain(const Chain<T> &original);
	void SinistralTransformation(BinaryClient<T> *&Entrance);
	void DextralTransformation(BinaryClient<T> *&Entrance);
	void SinistralDextralTransformation(BinaryClient<T> *&Entrance);
	void DextralSinistralTransformation(BinaryClient<T> *&Entrance);
	void CounterBalanceLeftInsert(BinaryClient<T> *&Entrance);
	void CounterBalanceRightInsert(BinaryClient<T> *&Entrance);
	bool CounterBalanceLeftDelete(BinaryClient<T> *&Entrance);
	bool CounterBalanceRightDelete(BinaryClient<T> *&Entrance);

};

template<class T>
ErrorCode_List AVLTree<T>::InsertClient_AVL(const T &toWrite)
{
	bool taller;
	return InsertRecursive_AVL(BinaryTree<T>::Root,nullptr,toWrite,taller);
}

template<class T>
ErrorCode_List AVLTree<T>::DeleteClient_AVL(const T &toDelete)
{
	bool shorter=true;
	return DeleteRecursive_AVL(BinaryTree<T>::Root,nullptr,toDelete,shorter);
}

template<class T>
ErrorCode_List AVLTree<T>::InsertRecursive_AVL(BinaryClient<T> *&Entrance,BinaryClient<T> *Senior,const T &toWrite,bool &taller)
{
	ErrorCode_List result=success;
	if(Entrance==nullptr)
	{
		Entrance=new BinaryClient<T>;
		Entrance->Data=toWrite;
		Entrance->pSenior=Senior;
		BinaryTree<T>::count++;
		taller=true;
	}
	else if(toWrite==Entrance->Data)
	{
		result=duplicate;
		taller=false;
	}
	else if(toWrite<Entrance->Data)
	{
		result=InsertRecursive_AVL(Entrance->pLeftJunior,Entrance,toWrite,taller);
		if(taller)
			switch(Entrance->getBalance())
			{
				case leftHigher:
					CounterBalanceLeftInsert(Entrance);
					taller=false;
					break;
				case equalHeight:
					Entrance->setBalance(leftHigher);
					break;
				case rightHigher:
					Entrance->setBalance(equalHeight);
					taller=false;
					break;
			}
	}
	else
	{
		result=InsertRecursive_AVL(Entrance->pRightJunior,Entrance,toWrite,taller);
		if(taller)
			switch(Entrance->getBalance())
			{
				case leftHigher:
					Entrance->setBalance(equalHeight);
					taller=false;
					break;
				case equalHeight:
					Entrance->setBalance(rightHigher);
					break;
				case rightHigher:
					CounterBalanceRightInsert(Entrance);
					taller=false;
					break;
			}
	}
	return result;
}

template<class T>
ErrorCode_List AVLTree<T>::DeleteRecursive_AVL(BinaryClient<T> *&Entrance,BinaryClient<T> *Senior,const T &toDelete,bool &shorter)
{
	ErrorCode_List result=success;
	if(Entrance==nullptr)
	{
		shorter=false;
		return non_existence;
	}
	else if(toDelete==Entrance->Data)
	{
		BinaryClient<T> *DeletePointer=Entrance;
		BinaryClient<T> *tmpEntrance=Entrance;
		if(tmpEntrance->pRightJunior==nullptr&&tmpEntrance->pLeftJunior!=nullptr)
		{
			Entrance=Entrance->pLeftJunior;
			if(Senior!=nullptr)
			{
				if(Senior->pLeftJunior==tmpEntrance)
					Senior->pLeftJunior=tmpEntrance->pLeftJunior;
				else if(Senior->pRightJunior==tmpEntrance)
					Senior->pRightJunior=tmpEntrance->pLeftJunior;
			}


			Entrance->pSenior=Senior;
			delete DeletePointer;
			shorter=true;
			return success;
		}
		else if(Entrance->pLeftJunior==nullptr&&Entrance->pRightJunior!=nullptr)
		{
			Entrance=Entrance->pRightJunior;
			if(Senior!=nullptr)
			{
				if(Senior->pLeftJunior==Entrance)
					Senior->pLeftJunior=Entrance->pRightJunior;
				else if(Senior->pRightJunior==Entrance)
					Senior->pRightJunior=Entrance->pRightJunior;
			}

			Entrance->pSenior=Senior;
			delete DeletePointer;
			shorter=true;
			return success;
		}
		else if(Entrance->pLeftJunior==nullptr&&Entrance->pRightJunior==nullptr)
		{
			if(Senior!=nullptr)
			{
				if(Senior->pLeftJunior==Entrance)
					Senior->pLeftJunior=nullptr;
				else if(Senior->pRightJunior==Entrance)
					Senior->pRightJunior=nullptr;
			}

			delete DeletePointer;
			shorter=true;
			return success;
		}
		else
		{
			DeletePointer=Entrance->pLeftJunior;
			for(;DeletePointer->pRightJunior!=nullptr;DeletePointer=DeletePointer->pRightJunior);
			Entrance->Data=DeletePointer->Data;
			DeleteRecursive_AVL(Entrance,Entrance->pSenior,DeletePointer->Data,shorter);
		}
	}
	else if(toDelete<Entrance->Data)
	{
		result=DeleteRecursive_AVL(Entrance->pLeftJunior,Entrance,toDelete,shorter);
		if(shorter)
		{
			switch(Entrance->getBalance())
			{
				case leftHigher:
					Entrance->setBalance(equalHeight);
					break;
				case equalHeight:
					Entrance->setBalance(rightHigher);
					shorter=false;
					break;
				case rightHigher:
					shorter=CounterBalanceRightDelete(Entrance);
					break;
			}
		}
	}
	else if(toDelete>Entrance->getBalance())
	{
		result=DeleteRecursive_AVL(Entrance->pRightJunior,Entrance,toDelete,shorter);
		if(shorter)
		{
			switch(Entrance->getBalance())
			{
				case leftHigher:
					shorter=CounterBalanceLeftDelete(Entrance);
					break;
				case equalHeight:
					Entrance->setBalance(leftHigher);
					shorter=false;
					break;
				case rightHigher:
					Entrance->setBalance(equalHeight);
					break;
			}
		}
	}
	return result;
}

template<class T>
void AVLTree<T>::SinistralTransformation(BinaryClient<T> *&Entrance)
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
void AVLTree<T>::DextralTransformation(BinaryClient<T> *&Entrance)
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

template<class T>
void AVLTree<T>::SinistralDextralTransformation(BinaryClient<T> *&Entrance)
{
	SinistralTransformation(Entrance->pLeftJunior);
	DextralTransformation(Entrance);
}

template<class T>
void AVLTree<T>::DextralSinistralTransformation(BinaryClient<T> *&Entrance)
{
	DextralTransformation(Entrance->pRightJunior);
	SinistralTransformation(Entrance);
}

template<class T>
void AVLTree<T>::CounterBalanceLeftInsert(BinaryClient<T> *&Entrance)
{
	BinaryClient<T> *Left=Entrance->pLeftJunior;
	switch(Left->getBalance())
	{
		case leftHigher:
			Entrance->setBalance(equalHeight);
			Left->setBalance(equalHeight);
			DextralTransformation(Entrance);
			break;
		case equalHeight:
			std::cerr<<"Error-Inner Error detected in CounterBalanceLeftInsert:Impossible case"<<std::endl;
			break;
		case rightHigher:
			BinaryClient<T> *SubRight=Left->pRightJunior;
			switch(SubRight->getBalance())
			{
				case leftHigher:
					Entrance->setBalance(rightHigher);
					Left->setBalance(equalHeight);
					break;
				case equalHeight:
					Entrance->setBalance(equalHeight);
					Left->setBalance(equalHeight);
					break;
				case rightHigher:
					Entrance->setBalance(equalHeight);
					Left->setBalance(leftHigher);
			}
			SubRight->setBalance(equalHeight);
			SinistralDextralTransformation(Entrance);
			break;
	}
}

template<class T>
void AVLTree<T>::CounterBalanceRightInsert(BinaryClient<T> *&Entrance)
{
	BinaryClient<T> *Right=Entrance->pRightJunior;
	switch(Right->getBalance())
	{
		case rightHigher:
			Entrance->setBalance(equalHeight);
			Right->setBalance(equalHeight);
			SinistralTransformation(Entrance);
			break;
		case equalHeight:
			std::cerr<<"Error-Inner Error detected in CounterBalanceRightInsert:Impossible case"<<std::endl;
			break;
		case leftHigher:
			BinaryClient<T> *SubLeft=Right->pLeftJunior;
			switch(SubLeft->getBalance())
			{
				case leftHigher:
					Entrance->setBalance(equalHeight);
					Right->setBalance(rightHigher);
					break;
				case equalHeight:
					Entrance->setBalance(equalHeight);
					Right->setBalance(equalHeight);
					break;
				case rightHigher:
					Entrance->setBalance(leftHigher);
					Right->setBalance(equalHeight);
			}
			SubLeft->setBalance(equalHeight);
			DextralSinistralTransformation(Entrance);
			break;
	}
}

template<class T>
bool AVLTree<T>::CounterBalanceLeftDelete(BinaryClient<T> *&Entrance)
{
	bool shorter;
	BinaryClient<T> *Left=Entrance->pLeftJunior;
	switch(Left->getBalance())
	{
		case leftHigher:
			Entrance->setBalance(equalHeight);
			Left->setBalance(equalHeight);
			DextralTransformation(Entrance);
			shorter=true;
			break;
		case equalHeight:
			Left->setBalance(rightHigher);
			DextralTransformation(Entrance);
			shorter=false;
			break;
		case rightHigher:
			BinaryClient<T> *SubLeft=Left->pLeftJunior;
			switch(SubLeft->getBalance())
			{
				case leftHigher:
					Entrance->setBalance(equalHeight);
					Left->setBalance(leftHigher);
					break;
				case equalHeight:
					Entrance->setBalance(equalHeight);
					Left->setBalance(equalHeight);
					break;
				case rightHigher:
					Entrance->setBalance(rightHigher);
					Left->setBalance(equalHeight);
					break;
			}
			SubLeft->setBalance(equalHeight);
			SinistralDextralTransformation(Entrance);
			shorter=true;
			break;
	}
	return shorter;
}

template<class T>
bool AVLTree<T>::CounterBalanceRightDelete(BinaryClient<T> *&Entrance)
{
	bool shorter;
	BinaryClient<T> *Right=Entrance->pRightJunior;
	switch(Right->getBalance())
	{
		case rightHigher:
			Entrance->setBalance(equalHeight);
			Right->setBalance(equalHeight);
			SinistralTransformation(Entrance);
			shorter=true;
			break;
		case equalHeight:
			Right->setBalance(leftHigher);
			SinistralTransformation(Entrance);
			shorter=false;
			break;
		case leftHigher:
			BinaryClient<T> *SubLeft=Right->pLeftJunior;
			switch(SubLeft->getBalance())
			{
				case leftHigher:
					Entrance->setBalance(equalHeight);
					Right->setBalance(rightHigher);
					break;
				case equalHeight:
					Entrance->setBalance(equalHeight);
					Right->setBalance(equalHeight);
					break;
				case rightHigher:
					Entrance->setBalance(leftHigher);
					Right->setBalance(equalHeight);
					break;
			}
			SubLeft->setBalance(equalHeight);
			DextralSinistralTransformation(Entrance);
			shorter=true;
			break;
	}
	return shorter;
}

#endif //BASICDATASTRUCTUREANDALGORITHMS_BASICAVLTREE_H
