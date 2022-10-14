#pragma once
#include <type_traits>
#include <tuple>
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


			template<size_t I = 0, typename Func, typename... Tp>
			inline typename std::enable_if<I == sizeof...(Tp), void>::type
				for_each(std::tuple<Tp...>, Func)
			{


			}

			template<size_t I = 0, typename Func, typename... Tp>
			inline typename std::enable_if < I < sizeof...(Tp), void>::type
				for_each(std::tuple<Tp...> aTuple, Func aCallback) {
				aCallback(std::get<I>(aTuple));
				for_each<I + 1, Func, Tp...>(aTuple, aCallback);
			}


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
			return ReflectedType(aName, std::tuple<DerivedTypes...>(), std::make_tuple(std::forward<Members>(someMembers)...));
		}


		template<typename TClass, typename TTuple, typename DTTuple>
		ReflectedType<TTuple, DTTuple> Internal::ReflectionInfo<TClass, TTuple, DTTuple>::reflectedType = RegisterElement<TClass>();


		template<typename TClass>
		const auto& GetMembers()
		{
			auto r = Reflect::RegisterElement<TClass>();
			return Internal::ReflectionInfo<TClass, decltype(r)::Members, decltype(r)::DerivedTypes>::reflectedType;
		}



		template<typename TClass, typename Func>
		void IterateMembers(TClass* anInstance, Func aCallback, const bool aDeepIterationFlag = false, size_t aTypeHash = size_t(0) - 1)
		{
			Internal::for_each(Reflect::GetMembers<TClass>().members, [&anInstance, &aCallback, &aDeepIterationFlag](auto member)
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