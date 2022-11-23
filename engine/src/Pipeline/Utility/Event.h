#pragma once
namespace Dragonite
{
	//Taken from this: https://stackoverflow.com/a/72230664/10483209

	template <typename FuncType>
	class function
	{
		FuncType* (fn);

	public:
		function(FuncType R) : fn{ R } {}

		template <typename... _ArgTypes>
		auto operator()(_ArgTypes... vlist)
		{
			return fn(vlist...);
		}
	};


	



}