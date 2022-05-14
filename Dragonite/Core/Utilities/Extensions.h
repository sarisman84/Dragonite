#pragma once
#include <functional>
namespace CPPExtend
{

	template<class Value>
	struct Property
	{
	public:
		Property() = default;
		Property(const Value aValue,
			std::function<void(Value&)> aSetter = nullptr,
			std::function<Value& ()> aGetter = nullptr)
		{
			myValue = aValue;
			myGetter = aGetter;
			mySetter = aSetter;
		}

		void operator=(Value aValue)
		{
			if (mySetter)
				mySetter(aValue);

		}

		void operator=(Property<Value> aPropetry)
		{
			if (mySetter)
				mySetter(aPropetry.myGetter ? aPropetry.myGetter() : aPropetry.myValue);

		}

		friend void operator<<(Value& aValue, Property<Value>& aProperty);

		inline Value& GetValue() { return myValue; }

		std::function<void(Value&)> mySetter;
		std::function<Value& ()> myGetter;



	private:
		Value myValue;

	};

	template<class Value>
	void operator<<(Value& aValue, Property<Value>& aProperty)
	{
		aValue = aProperty.myGetter();
	}

}