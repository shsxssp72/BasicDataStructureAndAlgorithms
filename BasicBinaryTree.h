//
// Created by 星落_月残 on 2017/4/13.
//
#ifndef ERRORCODE_LIST
#define ERRORCODE_LIST
enum ErrorCode_List
{
	underflow,overflow,success,range_error,non_existence,overwrite,duplicate,new_entry
};
#endif


#ifndef BASICDATASTRUCTUREANDALGORITHMS_BASICBINARYTREE_H
#define BASICDATASTRUCTUREANDALGORITHMS_BASICBINARYTREE_H

#include <iostream>
#include <typeinfo>


enum AVLBalanceStatus
{
	equalHeight,leftHigher,rightHigher
};

//TODO 修改Binary的比较方式，对client增加数据Index，类型long long，用于比较大小，让用户对于每一个T自建转换函数，将T转换为long long，以获得更广泛应用

template<class T>
struct BinaryClient
{
public:
	T Data;
	long long Index;
	AVLBalanceStatus Balance;
	BinaryClient<T> *pSenior;
	BinaryClient<T> *pLeftJunior;
	BinaryClient<T> *pRightJunior;
	BinaryClient();
	BinaryClient(const T &toWrite);
	BinaryClient(const T &toWrite,long long inIndex);
	BinaryClient(const T &toWrite,BinaryClient<T> *parent,BinaryClient<T> *left,BinaryClient<T> *right);
	void setBalance(AVLBalanceStatus current);
	AVLBalanceStatus getBalance() const;
};

template<class T>
BinaryClient<T>::BinaryClient()
{
	Balance=equalHeight;
	Index=0;
	pSenior=nullptr;
	pLeftJunior=nullptr;
	pRightJunior=nullptr;
}

template<class T>
BinaryClient<T>::BinaryClient(const T &toWrite)
{
	Data=toWrite;
	Balance=equalHeight;
	pSenior=nullptr;
	pLeftJunior=nullptr;
	pRightJunior=nullptr;

	if(typeid(T)==typeid(int)||typeid(T)==typeid(long)||typeid(T)==typeid(long long))
		Index=toWrite;
	else if(typeid(T)==typeid(float)||typeid(T)==typeid(double)||typeid(T)==typeid(char)||typeid(T)==typeid(bool))
		Index=static_cast<long long>(toWrite);
	else
		Index=0;
}

template<class T>
BinaryClient::BinaryClient(const T &toWrite,long long inIndex)
{
	Data=toWrite;
	Index=inIndex;
	Balance=equalHeight;
	pSenior=nullptr;
	pLeftJunior=nullptr;
	pRightJunior=nullptr;
}

template<class T>
BinaryClient<T>::BinaryClient(const T &toWrite,BinaryClient<T> *parent,BinaryClient<T> *left,BinaryClient<T> *right)
{
	Data=toWrite;
	pSenior=parent;
	pLeftJunior=left;
	pRightJunior=right;
	Balance=equalHeight;
}

template<class T>
void BinaryClient<T>::setBalance(AVLBalanceStatus current)
{
	Balance=current;
}

template<class T>
AVLBalanceStatus BinaryClient<T>::getBalance() const
{
	return Balance;
}


template<class T>
class BinaryTree
{
public:
	BinaryTree();
	BinaryTree(const BinaryTree &original);
	~BinaryTree();
	void operator =(const BinaryTree &original);
	//bool isCompleteBinaryTree(BinaryClient<T> *Entrance);
	bool isEqualInstance(const BinaryTree &toCompare);
	bool isEqualSubTree(const BinaryClient<T> *Entrance1,const BinaryClient<T> *Entrance2);
	bool isEmpty();
	bool isFull();
	int getTotalHeightCount();
	int getHeightRecursive(BinaryClient<T> *Entrance);
	int getNumberOfLeavesRecursive(BinaryClient<T> *Entrance);
	int getNumberofNonLeavesRecursive(BinaryClient<T> *Entrance);
	//	BinaryClient<T> *getAddressFromData(BinaryClient<T> *Entrance,const T &toFind);
	//	BinaryClient<T> *getMaxClientAndDataBase(BinaryClient<T> *Entrance,T &output);//仅当T类型可以比较
	//	BinaryClient<T> *getMinClientAndDataBase(BinaryClient<T> *Entrance,T &output);//仅当T类型可以比较
	BinaryClient<T> *getSenior(const BinaryClient<T> *Base);
	BinaryClient<T> *getLeftJunior(const BinaryClient<T> *Base);
	BinaryClient<T> *getRightJunior(const BinaryClient<T> *Base);
	BinaryClient<T> *getRoot();
	void PreorderTraverse(BinaryClient<T> *Entrance,void (*visit)(BinaryClient<T> *));
	void InorderTraverse(BinaryClient<T> *Entrance,void (*visit)(BinaryClient<T> *));
	void PostTraverse(BinaryClient<T> *Entrance,void (*visit)(BinaryClient<T> *));
	void LevelTraverse(BinaryClient<T> *Entrance,void (*visit)(BinaryClient<T> *));
	void LevelTraverseRecursive(BinaryClient<T> *Entrance,void (*visit)(BinaryClient<T> *),int level);
	void InvertBinaryTree(BinaryClient<T> *Entrance);
	//ErrorCode_List CreateCompleteBinaryTree();
	ErrorCode_List DeleteSubTree(BinaryClient<T> *Entrance);
	ErrorCode_List DeleteSingleClient(BinaryClient<T> *Entrance);//类似二分查找树
	ErrorCode_List ExtractToNewPosition(BinaryClient<T> *&originalPosition,BinaryClient<T> *&newPosition,bool left);//移动到newPosition之下
	ErrorCode_List InsertJunior(BinaryClient<T> *&Entrance,T toWrite,bool left);
	ErrorCode_List InsertJunior(BinaryClient<T> *&Entrance,T toWrite,long long inIndex,bool left);
	//ErrorCode_List InsertSingleClient(T toWrite);
	ErrorCode_List SwapData(BinaryClient<T> *position1,BinaryClient<T> *position2);
	//ErrorCode_List TransformFromChain(const Chain<T> &original);//按层次插入
	//void TreeSort(bool method);//该方法也许应该在Chain?


	//返回高度(基于数目/递归计数),返回数目,复制,遍历(前中后序,层次),判断相等,删除(后序),查找,取某节点的直接相关节点,取最大最小值,翻转二叉树,求叶子/非叶子节点个数,*二叉树与双向链表转换
	/* 参考:
	 * http://blog.csdn.net/sysu_arui/article/details/7865876
	 * http://blog.csdn.net/ljianhui/article/details/22338405
	 * http://blog.csdn.net/ljianhui/article/details/9257729
	 * http://blog.csdn.net/npy_lp/article/details/7426431
	 * http://blog.csdn.net/touch_2011/article/details/6831924
	 */
protected:
	void copy(BinaryClient<T> *From,BinaryClient<T> *To);
	int count;
	BinaryClient<T> *Root;
};

template<class T>
BinaryTree<T>::BinaryTree()
{
	count=0;
	Root=nullptr;
}

template<class T>
BinaryTree<T>::BinaryTree(const BinaryTree &original)
{
	BinaryTree<T>::operator =(original);
}

template<class T>
BinaryTree<T>::~BinaryTree()
{
	DeleteSubTree(Root);
}

template<class T>
void BinaryTree<T>::operator =(const BinaryTree &original)
{
	if(&original==this)
		return;
	else if(original.Root==nullptr)
	{
		Root=nullptr;
		count=0;
	}
	else
	{
		count=original.count;
		copy(original.Root,Root);
	}
}

template<class T>
void BinaryTree<T>::copy(BinaryClient<T> *From,BinaryClient<T> *To)
{
	if(From==nullptr)
	{
		To=nullptr;
		return;
	}
	else
	{
		To->Data=From->Data;
		copy(From->pLeftJunior,To->pLeftJunior);
		copy(From->pRightJunior,To->pRightJunior);
		return;
	}
}

template<class T>
bool BinaryTree<T>::isEqualInstance(const BinaryTree &toCompare)
{
	isEqualSubTree(Root,toCompare.Root);
}

template<class T>
bool BinaryTree<T>::isEqualSubTree(const BinaryClient<T> *Entrance1,const BinaryClient<T> *Entrance2)
{
	if(Entrance1==nullptr&&Entrance2==nullptr)
		return true;
	else if((Entrance1==nullptr&&Entrance2!=nullptr)||(Entrance1!=nullptr&&Entrance2==nullptr))
		return false;
	else
	{
		if(Entrance1->Data!=Entrance2->Data)
			return false;
		else
		{
			if(!isEqualSubTree(Entrance1->pLeftJunior,Entrance2->pLeftJunior))
				return false;
			if(!isEqualSubTree(Entrance1->pRightJunior,Entrance2->pRightJunior))
				return false;
		}
	}
	return true;
}

template<class T>
bool BinaryTree<T>::isEmpty()
{
	return Root==nullptr;
}

template<class T>
bool BinaryTree<T>::isFull()
{
	BinaryClient<T> *tmpPointer=new BinaryClient<T>;
	if(tmpPointer==nullptr)
	{
		return true;
	}
	else
	{
		delete tmpPointer;
	}
	return false;
}

template<class T>
int BinaryTree<T>::getTotalHeightCount()
{
	/*if(!isCompleteBinaryTree(Root))
		return -1;
	else
	{*/
	int threshold=1;
	int copyOfCount=count;
	int tmpHeight=0;
	for(;copyOfCount>0;threshold*=2)
	{
		copyOfCount-=threshold;
		tmpHeight++;
	}
	return tmpHeight;
	//	}
}

template<class T>
int BinaryTree<T>::getHeightRecursive(BinaryClient<T> *Entrance)
{
	if(Entrance==nullptr)
		return 0;
	int leftHeight=getHeightRecursive(Entrance->pLeftJunior);
	int rightHeight=getHeightRecursive(Entrance->pRightJunior);
	if(leftHeight>rightHeight)
		return leftHeight+1;
	else
		return rightHeight+1;
}

template<class T>
int BinaryTree<T>::getNumberOfLeavesRecursive(BinaryClient<T> *Entrance)
{
	if(Entrance==nullptr)
		return 0;
	else if(Entrance->pLeftJunior==nullptr&&Entrance->pRightJunior==nullptr)
	{
		return 1;
	}
	else
	{
		int tmpCount=0;
		tmpCount+=getNumberOfLeavesRecursive(Entrance->pLeftJunior);
		tmpCount+=getNumberOfLeavesRecursive(Entrance->pRightJunior);
		return tmpCount;
	}
}

template<class T>
int BinaryTree<T>::getNumberofNonLeavesRecursive(BinaryClient<T> *Entrance)
{
	if(Entrance==nullptr||(Entrance->pLeftJunior==nullptr&&Entrance->pRightJunior==nullptr))
		return 0;
	else
	{
		int tmpCount=1;
		tmpCount+=getNumberofNonLeavesRecursive(Entrance->pLeftJunior);
		tmpCount+=getNumberofNonLeavesRecursive(Entrance->pRightJunior);
		return tmpCount;
	}

}

template<class T>
BinaryClient<T> *BinaryTree<T>::getRoot()
{
	return Root;
}

template<class T>
void BinaryTree<T>::PreorderTraverse(BinaryClient<T> *Entrance,void (*visit)(BinaryClient<T> *))
{
	if(Entrance==nullptr)
		return;
	else
	{
		visit(Entrance);
		PreorderTraverse(Entrance->pLeftJunior,visit);
		PreorderTraverse(Entrance->pRightJunior,visit);
	}
}

template<class T>
void BinaryTree<T>::InorderTraverse(BinaryClient<T> *Entrance,void (*visit)(BinaryClient<T> *))
{
	if(Entrance==nullptr)
		return;
	else
	{
		InorderTraverse(Entrance->pLeftJunior,visit);
		visit(Entrance);
		InorderTraverse(Entrance->pRightJunior,visit);
	}
}

template<class T>
void BinaryTree<T>::PostTraverse(BinaryClient<T> *Entrance,void (*visit)(BinaryClient<T> *))
{
	if(Entrance==nullptr)
		return;
	else
	{
		PostTraverse(Entrance->pLeftJunior,visit);
		PostTraverse(Entrance->pRightJunior,visit);
		visit(Entrance);
	}
}

template<class T>
void BinaryTree<T>::LevelTraverse(BinaryClient<T> *Entrance,void (*visit)(BinaryClient<T> *))
{
	int level=getHeightRecursive(Entrance);
	if(Entrance==nullptr||level<0)
		return;
	else
		for(int i=0;i<level;i++)
		{
			LevelTraverseRecursive(Entrance,visit,i);
		}

}
template<class T>
void BinaryTree<T>::LevelTraverseRecursive(BinaryClient<T> *Entrance,void (*visit)(BinaryClient<T> *),int level)
{
	if(Entrance==nullptr||level<0)
		return;
	else if(level==0)
		visit(Entrance);
	else
	{
		LevelTraverseRecursive(Entrance->pLeftJunior,visit,level-1);
		LevelTraverseRecursive(Entrance->pRightJunior,visit,level-1);
	}
}

template<class T>
void BinaryTree<T>::InvertBinaryTree(BinaryClient<T> *Entrance)
{
	if(Entrance!=nullptr)
	{
		BinaryClient<T> *tmpPointer;
		if(Entrance->pLeftJunior!=nullptr||Entrance->pRightJunior!=nullptr)
		{
			tmpPointer=Entrance->pLeftJunior;
			Entrance->pLeftJunior=Entrance->pRightJunior;
			Entrance->pRightJunior=tmpPointer;
			InvertBinaryTree(Entrance->pLeftJunior);
			InvertBinaryTree(Entrance->pRightJunior);
		}
	}
}

template<class T>
ErrorCode_List BinaryTree<T>::DeleteSubTree(BinaryClient<T> *Entrance)
{
	if(Entrance==nullptr)
		return success;
	else
	{
		int sum=0;
		sum+=getNumberOfLeavesRecursive(Entrance);
		sum+=getNumberofNonLeavesRecursive(Entrance);
		count-=sum;
		DeleteSubTree(Entrance->pLeftJunior);
		DeleteSubTree(Entrance->pRightJunior);
		delete Entrance;
		Entrance=nullptr;
		return success;
	}
}

template<class T>
ErrorCode_List BinaryTree<T>::DeleteSingleClient(BinaryClient<T> *Entrance)
{
	if(Entrance==BinaryTree<T>::Root)
	{
		BinaryClient<T> *Precursor=Root->pLeftJunior;
		for(;Precursor->pRightJunior!=nullptr;Precursor=Precursor->pRightJunior);
		BinaryTree<T>::Root->Data=Precursor->Data;
		BinaryTree<T>::Root->Index=Precursor->Index;
		DeleteSingleClient(Precursor);
	}
	else if(Entrance->pLeftJunior==nullptr&&Entrance->pRightJunior==nullptr)
	{
		if(Entrance->pSenior->pLeftJunior==Entrance)
			Entrance->pSenior->pLeftJunior=nullptr;
		else if(Entrance->pSenior->pRightJunior==Entrance)
			Entrance->pSenior->pRightJunior=nullptr;
		delete Entrance;
	}
	else if(Entrance->pLeftJunior!=nullptr&&Entrance->pRightJunior==nullptr)
	{
		if(Entrance->pSenior->pLeftJunior==Entrance)
			Entrance->pSenior->pLeftJunior=Entrance->pLeftJunior;
		else if(Entrance->pSenior->pRightJunior==Entrance)
			Entrance->pSenior->pRightJunior=Entrance->pLeftJunior;
		Entrance->pLeftJunior->pSenior=Entrance->pSenior;
		delete Entrance;
	}
	else if(Entrance->pRightJunior!=nullptr&&Entrance->pLeftJunior==nullptr)
	{
		if(Entrance->pSenior->pLeftJunior==Entrance)
			Entrance->pSenior->pLeftJunior=Entrance->pRightJunior;
		else if(Entrance->pSenior->pRightJunior==Entrance)
			Entrance->pSenior->pRightJunior=Entrance->pRightJunior;
		Entrance->pRightJunior->pSenior=Entrance->pSenior;
		delete Entrance;
	}
	else if(Entrance->pLeftJunior!=nullptr&&Entrance->pRightJunior!=nullptr)
	{
		BinaryClient<T> *Precursor=Entrance->pLeftJunior;
		for(;Precursor->pRightJunior!=nullptr;Precursor=Precursor->pRightJunior);
		Entrance->Data=Precursor->Data;
		Entrance->Index=Precursor->Index;
		DeleteSingleClient(Precursor);
	}
}

template<class T>
ErrorCode_List BinaryTree<T>::ExtractToNewPosition(BinaryClient<T> *&originalPosition,BinaryClient<T> *&newPosition,bool left)
{
	if(originalPosition->pSenior!=nullptr)
	{
		if(originalPosition->pSenior->pLeftJunior==originalPosition)
			originalPosition->pSenior->pLeftJunior=nullptr;
		else if(originalPosition->pSenior->pRightJunior==originalPosition)
			originalPosition->pSenior->pRightJunior=nullptr;
	}
	if(left)
	{
		if(newPosition->pLeftJunior!=nullptr)
		{
			std::cerr<<"Error-The left subtree of the new position is not empty."<<std::endl;
			std::cerr<<"Data may be lost. Still process? (y/n)"<<std::endl;
			char commander=0;
			std::cin>>commander;
			if(commander!='y'||commander!='Y')
				return overwrite;
		}
		originalPosition->pSenior=newPosition;
		newPosition->pLeftJunior=originalPosition;
		return success;

	}
	else
	{
		if(newPosition->pRightJunior!=nullptr)
		{
			std::cerr<<"Error-The left subtree of the new position is not empty."<<std::endl;
			std::cerr<<"Data may be lost. Still process? (y/n)"<<std::endl;
			char commander=0;
			std::cin>>commander;
			if(commander!='y'||commander!='Y')
				return overwrite;
		}
		originalPosition->pSenior=newPosition;
		newPosition->pRightJunior=originalPosition;
		return success;
	}

}

template<class T>
ErrorCode_List BinaryTree<T>::InsertJunior(BinaryClient<T> *&Entrance,T toWrite,bool left)
{
	if(Entrance==nullptr)
		return range_error;
	else
	{
		if(left)
		{
			if(Entrance->pLeftJunior!=nullptr)
			{
				std::cerr<<"Error-The left subtree of the new position is not empty."<<std::endl;
				std::cerr<<"Data may be lost. Still process? (y/n)"<<std::endl;
				char commander=0;
				std::cin>>commander;
				if(commander!='y'&&commander!='Y')
					return overwrite;
			}
			BinaryClient<T> *NewClient;
			NewClient=new BinaryClient<T>;
			NewClient->Data=toWrite;
			NewClient->pSenior=Entrance;
			Entrance->pLeftJunior=NewClient;
		}
		else
		{
			if(Entrance->pRightJunior!=nullptr)
			{
				std::cerr<<"Error-The left subtree of the new position is not empty."<<std::endl;
				std::cerr<<"Data may be lost. Still process? (y/n)"<<std::endl;
				char commander=0;
				std::cin>>commander;
				if(commander!='y'&&commander!='Y')
					return overwrite;
			}
			BinaryClient<T> *NewClient;
			NewClient=new BinaryClient<T>;
			NewClient->Data=toWrite;
			NewClient->pSenior=Entrance;
			Entrance->pRightJunior=NewClient;
		}
		count++;
		return success;
	}
}


template<class T>
ErrorCode_List BinaryTree<T>::InsertJunior(BinaryClient<T> *&Entrance,T toWrite,long long inIndex,bool left)
{
	if(Entrance==nullptr)
		return range_error;
	else
	{
		if(left)
		{
			if(Entrance->pLeftJunior!=nullptr)
			{
				std::cerr<<"Error-The left subtree of the new position is not empty."<<std::endl;
				std::cerr<<"Data may be lost. Still process? (y/n)"<<std::endl;
				char commander=0;
				std::cin>>commander;
				if(commander!='y'&&commander!='Y')
					return overwrite;
			}
			BinaryClient<T> *NewClient;
			NewClient=new BinaryClient<T>;
			NewClient->Data=toWrite;
			NewClient->Index=inIndex;
			NewClient->pSenior=Entrance;
			Entrance->pLeftJunior=NewClient;
		}
		else
		{
			if(Entrance->pRightJunior!=nullptr)
			{
				std::cerr<<"Error-The left subtree of the new position is not empty."<<std::endl;
				std::cerr<<"Data may be lost. Still process? (y/n)"<<std::endl;
				char commander=0;
				std::cin>>commander;
				if(commander!='y'&&commander!='Y')
					return overwrite;
			}
			BinaryClient<T> *NewClient;
			NewClient=new BinaryClient<T>;
			NewClient->Data=toWrite;
			NewClient->Index=inIndex;
			NewClient->pSenior=Entrance;
			Entrance->pRightJunior=NewClient;
		}
		count++;
		return success;
	}
}

template<class T>
ErrorCode_List BinaryTree<T>::SwapData(BinaryClient<T> *position1,BinaryClient<T> *position2)
{
	T tmp=position1->Data;
	position1->Data=position2->Data;
	position2->Data=tmp;
	long long tmpIndex=position1->Index;
	position1->Index=position2->Index;
	position2->Index=tmpIndex;
}
/*
template<class T>
BinaryClient<T> *BinaryTree<T>::getAddressFromData(BinaryClient<T> *Entrance,const T &toFind)
{
	if(Entrance==nullptr)
		return nullptr;
	else
	{
		if(Entrance->Data==toFind)
			return Entrance;
		else if(Entrance->Data>toFind)
			return getAddressFromData(Entrance->pLeftJunior,toFind);
		else if(Entrance->Data<toFind)
			return getAddressFromData(Entrance->pRightJunior,toFind);
	}
}

template<class T>
BinaryClient<T> *BinaryTree<T>::getMaxClientAndDataBase(BinaryClient<T> *Entrance,T &output)
{
	if(Entrance==nullptr)
		return nullptr;
	else
	{
		BinaryClient<T> *MaxPointer=Entrance;
		BinaryClient<T> *tmpPointer=nullptr;
		tmpPointer=getMaxClientAndDataBase(Entrance->pLeftJunior,output);
		if(tmpPointer!=nullptr)
		{
			if(tmpPointer->Data>Entrance->Data)
			{
				MaxPointer=tmpPointer;
				output=MaxPointer->Data;
			}
		}
		tmpPointer=getMaxClientAndDataBase(Entrance->pRightJunior,output);
		if(tmpPointer!=nullptr)
		{
			if(tmpPointer->Data>Entrance->Data)
			{
				MaxPointer=tmpPointer;
				output=MaxPointer->Data;
			}
		}
		return MaxPointer;
	}
}

template<class T>
BinaryClient<T> *BinaryTree<T>::getMinClientAndDataBase(BinaryClient<T> *Entrance,T &output)
{
	if(Entrance==nullptr)
		return nullptr;
	else
	{
		BinaryClient<T> *MinPointer=Entrance;
		BinaryClient<T> *tmpPointer=nullptr;
		tmpPointer=getMaxClientAndDataBase(Entrance->pLeftJunior,output);
		if(tmpPointer!=nullptr)
		{
			if(tmpPointer->Data>Entrance->Data)
			{
				MinPointer=tmpPointer;
				output=MinPointer->Data;
			}
		}
		tmpPointer=getMaxClientAndDataBase(Entrance->pRightJunior,output);
		if(tmpPointer!=nullptr)
		{
			if(tmpPointer->Data>Entrance->Data)
			{
				MinPointer=tmpPointer;
				output=MinPointer->Data;
			}
		}
		return MinPointer;
	}
}
*/
template<class T>
BinaryClient<T> *BinaryTree<T>::getSenior(const BinaryClient<T> *Base)
{
	if(Base==nullptr||Base->pSenior==nullptr)
		return nullptr;
	else
	{
		return Base->pSenior;
	}
}

template<class T>
BinaryClient<T> *BinaryTree<T>::getLeftJunior(const BinaryClient<T> *Base)
{
	if(Base==nullptr||Base->pLeftJunior==nullptr)
		return nullptr;
	else
	{
		return Base->pLeftJunior;
	}
}

template<class T>
BinaryClient<T> *BinaryTree<T>::getRightJunior(const BinaryClient<T> *Base)
{
	if(Base==nullptr||Base->pRightJunior==nullptr)
		return nullptr;
	else
	{
		return Base->pRightJunior;
	}
}
//template<class T>

#endif //BASICDATASTRUCTUREANDALGORITHMS_BASICBINARYTREE_H
