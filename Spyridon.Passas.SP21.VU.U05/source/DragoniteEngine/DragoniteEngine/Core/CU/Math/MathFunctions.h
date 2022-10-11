#pragma once
#include <cmath>

namespace Dragonite
{
	namespace Math
	{
		static constexpr float PI = 3.14159265359f;
		static constexpr float RadToDeg = 180.0f / PI;
		static constexpr float DegToRad = PI / 180.0f;


		template<typename T>
		void Clamp(T& aVal, const T& aMin, const T& aMax)
		{
			aVal = aVal < aMin ? aMin : aVal > aMax ? aMax : aVal;
		}

		template<typename T>
		T Lerp(const T& aVal, const T& aLhs, const T& aRhs, const float aPercentage)
		{
			return aLhs * (1.0f - aPercentage) + (aRhs * aPercentage);
		}
	}
}
