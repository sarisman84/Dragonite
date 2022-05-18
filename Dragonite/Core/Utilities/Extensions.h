#pragma once
#include <functional>
namespace CPPExtend
{


	template<class T>
	class Property
	{
	public:
		Property() : myGetter([this]()->T { return T(); }), mySetter([this](const T) {}) {};
		Property(std::function<T(void)> aGetter) : myGetter(aGetter), mySetter([](const T) {}) {}
		Property(std::function<T(void)> aGetter, std::function<void(const T)> aSetter) : myGetter(aGetter), mySetter(aSetter) {}
		Property<T>& operator=(const T aValue)
		{
			mySetter(aValue);
			return *this;
		}

		T operator()()
		{
			return myGetter();
		}

		T const& operator ()() const
		{
			return myGetter();
		}
	private:
		std::function<void(const T)> mySetter;
		std::function<T()> myGetter;
	};




}