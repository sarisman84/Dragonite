#pragma once
#include <type_traits>
#include <tuple>
#include <xtr1common>
namespace Dragonite
{
	namespace Reflect
	{
		template<typename TupleType, typename DerivedTupleType>
		struct ReflectedType
		{
			using Members = TupleType;
			using DerivedTypes = DerivedTupleType;

			ReflectedType(const char* aName, DerivedTypes&& someDerivedTypes, Members&& someMembers) : members(someMembers), derivedTypes(someDerivedTypes)
			{
				myName = aName;
			}


			const char* myName;
			Members members;
			DerivedTypes derivedTypes;
		};







		namespace Internal
		{

			template<typename T, typename TupleType, typename DerivedTupleType>
			struct ReflectionInfo
			{
				using Type = T;
				static ReflectedType<TupleType, DerivedTupleType> reflectedType;

				static const char* name()
				{
					return reflectedType.myName;
				}

			};


			template<typename TClass, typename T>
			class Element
			{
			public:
				using OwnerType = TClass;
				using MemberType = T;
				using MemberPtr = T TClass::*;

				Element(const char* aName, MemberPtr aMPtr)
				{
					myName = aName;
					myMemberPtr = aMPtr;
				}

				const char* Name() noexcept
				{
					return myName;
				}


				T& ValueOf(TClass* anInstance)
				{
					return anInstance->*myMemberPtr;
				}


			private:
				const char* myName;
				MemberPtr myMemberPtr;
			};


			template<typename Func>
			auto invoke_with_args(Func) {};


			template<typename Func, typename Arg0, typename... Args>
			auto invoke_with_args(Func&& anF, Arg0&& anArg0, Args&&... someArgs)
			{
				if constexpr (std::is_same_v<void, std::invoke_result_t<Func&&, Arg0&&, Args&&...>>)
				{
					anF(std::forward<Arg0>(anArg0));
					Reflect::Internal::invoke_with_args(anF, std::forward<Args>(someArgs)...);
				}
				else
				{
					return anF(std::forward<Arg0>(anArg0));
				}
			}


			template<typename TTuple, typename Func, size_t... Indices>
			auto for_each_helper(TTuple&& aT, Func&& aFunc, std::index_sequence<Indices...>)
			{
				constexpr bool isVoid = std::is_same_v<void, decltype(Reflect::Internal::invoke_with_args(std::move(aFunc), std::get<Indices>(std::forward<TTuple>(aT))...))>;
				if (isVoid)
				{
					Reflect::Internal::invoke_with_args(std::move(aFunc), std::get<Indices>(std::forward<TTuple>(aT))...);
				}
				else
				{
					return Reflect::Internal::invoke_with_args(std::move(aFunc), std::get<Indices>(std::forward<TTuple>(aT))...);
				}


			}


			template<typename TTuple, typename Func>
			auto for_each(TTuple&& aT, Func&& aFunc)
			{
				using size = std::tuple_size<typename std::remove_reference<TTuple>::type>;

				constexpr bool isVoid = std::is_same<void, decltype(Reflect::Internal::for_each_helper
				(
					std::forward<TTuple>(aT),
					std::move(aFunc),
					std::make_index_sequence<size::value>()
				))>::value;



				if (isVoid)
				{
					Reflect::Internal::for_each_helper
					(
						std::forward<TTuple>(aT),
						std::move(aFunc),
						std::make_index_sequence<size::value>()
					);

				}
				else
				{
					return  Reflect::Internal::for_each_helper
					(
						std::forward<TTuple>(aT),
						std::move(aFunc),
						std::make_index_sequence<size::value>()
					);
				}



			}






			template<typename T>
			struct TypeInfo
			{
				typedef T type;
				const size_t id = typeid(type).hash_code();
			};



			template<typename>
			struct is_template : std::false_type
			{
			};

			template<template<typename...> class Tmpl, typename ...Args>
			struct is_template<Tmpl<Args...>> : std::true_type
			{
			};


			template<typename Test, template<typename...> class Ref>
			struct is_specialization : std::false_type
			{
			};

			template<template<typename...> class Ref, typename... Args>
			struct is_specialization<Ref<Args...>, Ref> : std::true_type
			{
			};


			template<typename, typename>
			struct tuple_holds;

			template<typename ...Ts, typename T>
			struct tuple_holds<std::tuple<Ts...>, T>
				: std::disjunction<std::is_same<Ts, T>...>
			{
			};





		}

		template<typename TElement>
		inline auto RegisterElement()
		{
			return ReflectedType("unknown", std::make_tuple(), std::make_tuple());
		}






		template<typename TClass, typename T>
		constexpr inline auto Member(const char* aName, T TClass::* aPtr)
		{
			return Internal::Element{ aName,aPtr };
		}

		template<typename... DerivedTypes, typename... Members>
		constexpr inline auto Class(const char* aName, Members&&... someMembers)
		{
			return ReflectedType(aName, std::make_tuple(Internal::TypeInfo<DerivedTypes>()...), std::make_tuple(std::forward<Members>(someMembers)...));
		}


		template<typename TClass, typename TTuple, typename DTTuple>
		ReflectedType<TTuple, DTTuple> Internal::ReflectionInfo<TClass, TTuple, DTTuple>::reflectedType = RegisterElement<TClass>();


		template<typename TClass>
		const auto& GetReflectedType()
		{
			auto r = Reflect::RegisterElement<TClass>();
			return Internal::ReflectionInfo<TClass, decltype(r)::Members, decltype(r)::DerivedTypes>::reflectedType;
		}




		//template<typename TClass>
		//inline auto GetReflectedType(TClass* anInstance)
		//{
		//	if (typeid(TClass).hash_code() == typeid(*anInstance).hash_code()) return Reflect::GetReflectedType<TClass>();

		//	auto lambda = [anInstance](auto element)
		//	{
		//		if constexpr (Internal::is_specialization<decltype(element), Internal::TypeInfo>::value)
		//		{
		//			if (typeid(*anInstance).hash_code() == element.id)
		//			{
		//				return Reflect::GetReflectedType<decltype(element)::type>();
		//			}
		//		}


		//	};

		//	using type = decltype(Reflect::Internal::for_each(Reflect::GetReflectedType<TClass>().derivedTypes, lambda));
		//	using derivedTypes = decltype(Reflect::GetReflectedType<TClass>().derivedTypes);
		//	using lambdaT = decltype(lambda);

		//	using for_each_type = std::invoke_result_t<typename type, typename derivedTypes, typename lambdaT>;

		//	using isVoid = std::is_same<for_each_type, void>;


		//	if constexpr (isVoid::value) return Reflect::GetReflectedType<TClass>();

		//	return Reflect::Internal::for_each(Reflect::GetReflectedType<TClass>().derivedTypes, lambda);

		//}



		template<typename TClass, typename Func>
		void IterateMembers(TClass* anInstance, Func aCallback, const bool aDeepIterationFlag = false)
		{
			Internal::for_each(Reflect::GetReflectedType<TClass>().members, [&anInstance, &aCallback, &aDeepIterationFlag](auto member)
				{
					if constexpr (!Internal::is_template<decltype(member)>::value) return;

					using MemberType = decltype(member)::MemberType;
					using TupleType = decltype(RegisterElement<MemberType>());

					aCallback(member);

					if (!std::is_same<TupleType, std::tuple<>>::value && aDeepIterationFlag)
					{
						Reflect::IterateMembers(&member.ValueOf(anInstance), aCallback, aDeepIterationFlag);
					}
				});
		}

	}



}