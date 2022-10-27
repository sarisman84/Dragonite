#pragma once
#include <unordered_map>
#include <type_traits>
#include <typeinfo>
namespace Dragonite
{
	template<typename Value>
	class EnumMap
	{
	public:
		EnumMap() = default;
		~EnumMap() = default;


		template<typename EnumVal>
		Value& operator[](const EnumVal aVal) {
			if constexpr (!std::is_enum_v<EnumVal>) return;

			int key = typeid(EnumVal).hash_code() + static_cast<int>(aVal);
			return myValue[key].second;

		}


		std::unordered_map<int, Value>::iterator begin()
		{
			return myValue.begin();
		}

		std::unordered_map<int, Value>::iterator end()
		{
			return myValue.end();
		}


		std::unordered_map<int, Value> myValue;
	};
}


