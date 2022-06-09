#pragma once
#include <iostream>
namespace Dragonite
{
#pragma warning (disable: 4267)
	template<typename Value>
	class SparseSet
	{
		using Key = int;
		struct Data
		{
			Value myValue;
			Key myKey;
		};

	public:

		SparseSet(const size_t& mySparseSetSize, const size_t& myDenseSetSize)
		{
			mySparseSet = new Key[mySparseSetSize + 1];
			myDenseSet = new Data[myDenseSetSize];
			myCapacity = myDenseSetSize;
			myMaxValue = mySparseSetSize;
			myCurrentCount = 0;
		}
		~SparseSet()
		{
			delete[] mySparseSet;
			delete[] myDenseSet;
		}


		Value& operator[](const Key& aKey)
		{
			return myDenseSet[mySparseSet[aKey]].myValue;
		}

		Key Find(const SparseSet<Value>::Key& aKey)
		{
			if (aKey > myMaxValue) return -1;

			if (mySparseSet[aKey] < myCurrentCount && myDenseSet[mySparseSet[aKey]].myKey == aKey) return mySparseSet[aKey];

			return -1;
		}
		void Insert(const Value& aValue, const SparseSet<Value>::Key& aKey = -1)
		{
			SparseSet<Value>::Key key = aKey != -1 ? aKey : myNextCounter;


			if (key > myMaxValue) return;
			if (myCurrentCount >= myCapacity) return;
			if (Find(key) != -1) return;

			myDenseSet[myCurrentCount] = Data{ aValue, key };
			mySparseSet[key] = myCurrentCount;
			myCurrentCount++;


			myNextCounter = aKey != -1 ? aKey + 1 : myNextCounter + 1;
		}
		void Remove(const SparseSet<Value>::Key& aKey)
		{
			if (Find(aKey) == -1) return;

			Data temp = myDenseSet[myCurrentCount - 1];
			myDenseSet[mySparseSet[aKey]] = temp;
			mySparseSet[temp.myKey] = mySparseSet[aKey];
			myCurrentCount--;
			myNextCounter--;
		}
		void Clear()
		{
			myCurrentCount = 0;
			myNextCounter = 0;
		}


		size_t Size()
		{
			return myCurrentCount;
		}


		Data* GetDenseSet()
		{
			return myDenseSet;
		}

		Key* GetSparseSet()
		{
			return mySparseSet;
		}






	private:
		Key* mySparseSet;
		Data* myDenseSet;
		size_t myCurrentCount;
		size_t myCapacity;
		size_t myMaxValue;
		size_t myNextCounter;

	};



	template<typename Value>
	std::ostream& operator<<(std::ostream& anOutputStream, SparseSet<Value>& aSet)
	{
		for (size_t i = 0; i < aSet.Size(); i++)
		{
			anOutputStream << aSet.GetDenseSet()[i].myValue << "(" << aSet.Find(aSet.GetDenseSet()[i].myKey) << ")" << (i == aSet.Size() - 1 ? "" : ",");
		}
		return anOutputStream;
	}




}





//Dragonite::SparseSet::SparseSet(const ID& mySparseSetSize, const ID& myDenseSetSize)
//{
//	mySparseSet = new ID[mySparseSetSize + 1];
//	myDenseSet = new ID[myDenseSetSize];
//	myCapacity = myDenseSetSize;
//	myMaxValue = mySparseSetSize;
//	myCurrentCount = 0;
//}
//
//Dragonite::SparseSet::~SparseSet()
//{
//	delete[] mySparseSet;
//	delete[] myDenseSet;
//}
//
//
//
//Dragonite::SparseSet::ID Dragonite::SparseSet::Find(const ID& aValue)
//{
//	if (aValue > myMaxValue) return -1;
//
//	if (mySparseSet[aValue] < myCurrentCount && myDenseSet[mySparseSet[aValue]] == aValue) return mySparseSet[aValue];
//
//	return -1;
//}
//
//void Dragonite::SparseSet::Insert(const ID& aValue)
//{
//
//	if (aValue > myMaxValue) return;
//	if (myCurrentCount >= myCapacity) return;
//	if (Find(aValue) != -1) return;
//
//	myDenseSet[myCurrentCount] = aValue;
//	mySparseSet[aValue] = (ID)myCurrentCount;
//	myCurrentCount++;
//}
//
//void Dragonite::SparseSet::Remove(const ID& aValue)
//{
//	if (Find(aValue) == -1) return;
//
//	Dragonite::SparseSet::ID temp = myDenseSet[myCurrentCount - 1];
//	myDenseSet[mySparseSet[aValue]] = temp;
//	mySparseSet[temp] = mySparseSet[aValue];
//	myCurrentCount--;
//}
//
//void Dragonite::SparseSet::Clear()
//{
//	myCurrentCount = 0;
//}
//
//std::ostream& Dragonite::operator<<(std::ostream& anOutputStream, SparseSet& aSet)
//{
//	for (size_t i = 0; i < aSet.myCurrentCount; i++)
//	{
//		anOutputStream << aSet.myDenseSet[i] << "(" << aSet.Find(aSet.myDenseSet[i]) << ")" << (i == aSet.myCurrentCount - 1 ? "" : ",");
//	}
//	return anOutputStream;
//}

