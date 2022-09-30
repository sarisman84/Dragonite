#pragma once
#include <vector>
#include <memory>
#include <type_traits>

#include <functional>

namespace Dragonite
{

	template<typename>
	class Function {};

	template< typename... Args>
	class Function<void(Args...)>
	{
	public:
		Function() = default;

		template<typename Func>
		void operator+=(Func aNewCallback)
		{
			myCallbacks.push_back(aNewCallback);
		}

		template<typename Func>
		void operator=(Func aNewCallback)
		{
			myCallbacks.clear();
			myCallbacks.push_back(aNewCallback);
		}

		void operator()(Args... someArgs)
		{
			for (auto& c : myCallbacks)
			{
				c(someArgs...);
			}

		}

		void clear()
		{
			myCallbacks.clear();
		}

	private:
		std::vector<std::function<void(Args...)>> myCallbacks;
	};

	template<typename Ret, typename... Args>
	class Function<Ret(Args...)>
	{
	public:
		Function() = default;

		template<typename Func>
		void operator+=(Func aNewCallback)
		{
			myCallbacks.push_back(aNewCallback);
		}

		template<typename Func>
		void operator=(Func aNewCallback)
		{
			myCallbacks.clear();
			myCallbacks.push_back(aNewCallback);
		}

		Ret operator()(Args... someArgs) {
			Ret r;

			for (auto& c : myCallbacks)
			{
				if (myCallbacks.size() == 1)
					r = c(someArgs...);
				else
					r += c(someArgs...);
			}

			return r;

		}

		void clear()
		{
			myCallbacks.clear();
		}

	private:
		std::vector<std::function<Ret(Args...)>> myCallbacks;
	};


}




