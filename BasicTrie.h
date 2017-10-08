//
// Created by 星落_月残 on 2017/5/24.
//

#include <cstring>

#ifndef ERRORCODE_LIST
#define ERRORCODE_LIST
enum ErrorCode_List
{
	underflow,overflow,success,range_error,non_existence,overwrite,duplicate,new_entry
};
#endif

#ifndef BASICDATASTRUCTUREANDALGORITHMS_BASICTRIE_H
#define BASICDATASTRUCTUREANDALGORITHMS_BASICTRIE_H

constexpr int DegreeOfTrie=26;

//值转换函数，确定输入值对应的指针存储位置，如在字母表中a对应0;
//对于不同输入值类型应该定义不同转换函数
void getIndex_Trie_lowercase(const char *toConvert,int *output)
{
	long long length=strlen(toConvert);
	for(long long i=0;i<length;i++)
		output[i]=toConvert[i]-'a';
	output[length]=-1;
}

template<class T>
struct TrieClient
{
	TrieClient<T> *JuniorList[DegreeOfTrie];
	TrieClient<T> *Senior;
	T data;
	TrieClient();
	TrieClient(const T toWrite);
	TrieClient(TrieClient<T> *seniorPointer);
	TrieClient(const T toWrite,TrieClient<T> *seniorPointer);
};

template<class T>
TrieClient<T>::TrieClient()
{
	for(int i=0;i<DegreeOfTrie;i++)
		JuniorList[i]=nullptr;
	Senior=nullptr;
}

template<class T>
TrieClient<T>::TrieClient(const T toWrite)
{
	for(int i=0;i<DegreeOfTrie;i++)
		JuniorList[i]=nullptr;
	Senior=nullptr;
	data=toWrite;
}

template<class T>
TrieClient<T>::TrieClient(TrieClient<T> *seniorPointer)
{
	for(int i=0;i<DegreeOfTrie;i++)
		JuniorList[i]=nullptr;
	Senior=seniorPointer;
}

template<class T>
TrieClient<T>::TrieClient(const T toWrite,TrieClient<T> *seniorPointer)
{
	for(int i=0;i<DegreeOfTrie;i++)
		JuniorList[i]=nullptr;
	Senior=seniorPointer;
	data=toWrite;
}

template<class T>
class Trie
{
public:
	Trie();
	~Trie();
	ErrorCode_List InsertClient(const int *toWriteIndex,const T toWrite);
	ErrorCode_List DeleteClient(const int *toDeleteIndex);
	ErrorCode_List RetrieveData(const int *toFindIndex,T &output);
	void Clear(TrieClient<T> *Entrance);

private:
	TrieClient<T> *Root;
};

template<class T>
Trie<T>::Trie()
{
	Root=new TrieClient<T>;
}

template<class T>
void Trie<T>::Clear(TrieClient<T> *Entrance)
{
	if(Entrance!=nullptr)
	{
		for(int i=0;i<DegreeOfTrie;i++)
		{
			if(Entrance->JuniorList[i]!=nullptr)
			{
				Clear(Entrance->JuniorList[i]);
			}
		}
		delete Entrance;
	}

}

template<class T>
Trie<T>::~Trie()
{
	Clear(Root);
}

template<class T>
ErrorCode_List Trie<T>::InsertClient(const int *toWriteIndex,const T toWrite)
{
	TrieClient<T> *InsertPointer=Root;
	for(int i=0;;i++)
	{
		int index=toWriteIndex[i];
		if(index<-1||index>=DegreeOfTrie)//非法输入
			return range_error;
		else if(index==-1)//查找结束
		{
			InsertPointer->data=toWrite;
			return success;
		}
		else
		{
			if(InsertPointer->JuniorList[index]==nullptr)//指针不存在
				InsertPointer->JuniorList[index]=new TrieClient<T>(InsertPointer);
			InsertPointer=InsertPointer->JuniorList[index];
		}
	}
}

template<class T>
ErrorCode_List Trie<T>::DeleteClient(const int *toDeleteIndex)
{
	TrieClient<T> *DeletePointer=Root;
	for(int i=0;;i++)
	{
		int index=toDeleteIndex[i];
		if(index<-1||index>=DegreeOfTrie)//非法输入
			return range_error;
		else if(index==-1)//查找结束
		{
			delete DeletePointer->JuniorList[index];
			return success;
		}
		else if(DeletePointer->JuniorList[index]==nullptr)//指针不存在
			return non_existence;
		else//向下一层
			DeletePointer=DeletePointer->JuniorList[index];
	}
}

template<class T>
ErrorCode_List Trie<T>::RetrieveData(const int *toFindIndex,T &output)
{
	TrieClient<T> *RetrievePointer=Root;
	for(int i=0;;i++)
	{
		int index=toFindIndex[i];
		if(index<-1||index>=DegreeOfTrie)//非法输入
			return range_error;
		else if(index==-1)//查找结束
		{
			output=RetrievePointer->data;
			return success;
		}
		else if(RetrievePointer->JuniorList[index]==nullptr)//指针不存在
			return non_existence;
		else//向下一层
			RetrievePointer=RetrievePointer->JuniorList[index];
	}
}

//Insert的递归版本
/*
template<class T>
ErrorCode_List Trie<T>::InsertClient(TrieClient<T> *&Entrance,TrieClient<T> *Senior,const int *toWriteIndex,const T toWrite)
{
	if(Entrance==nullptr)
		Entrance=new TrieClient<T>(Senior);

	int index=toWriteIndex[0];
	if(index!=-1)//位置确定
		if(index<DegreeOfTrie&&index>=0)
			InsertClient(Entrance->JuniorList[index],Entrance,(toWrite+1),toWrite);
		else//非法输入
			return range_error;
	else
	{
		Entrance->data=toWrite;
		return success;//可能覆盖原有数据
	}
}
*/

#endif //BASICDATASTRUCTUREANDALGORITHMS_BASICTRIE_H
