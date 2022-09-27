#pragma once
#include "MathFunctions.h"
namespace Dragonite
{
	template<typename T>
	struct Vector2
	{
		union
		{
			T x, y;
			T* data;
		};

		Vector2() = default;

		Vector2(T anX, T anY) : x(anX), y(anY), data(&anX)
		{

		}

		Vector2(T* aNewVector) : data(aNewVector), x(aNewVector[0]), y(aNewVector[1]) {}


		Vector2(const Vector2<T>& aVal)
		{
			x = aVal.x;
			y = aVal.y;
			data = aVal.data;
		}


		void operator=(const Vector2<T>& aVal)
		{
			x = aVal.x;
			y = aVal.y;
		
			data = aVal.data;
		}

		Vector2<T> operator+(const Vector2<T>& someData)
		{
			Vector2<T> r;
			for (size_t i = 0; i < 2; i++)
			{
				r = data[i] + someData.data[i];
			}
			return r;
		}


		Vector2<T> operator-(const Vector2<T>& someData)
		{
			Vector2<T> r;
			for (size_t i = 0; i < 2; i++)
			{
				r = data[i] - someData.data[i];
			}
			return r;
		}

		Vector2<T> operator*(const Vector2<T>& someData)
		{
			Vector2<T> r;
			for (size_t i = 0; i < 2; i++)
			{
				r = data[i] * someData.data[i];
			}
			return r;
		}


		Vector2<T> operator*(const T& aScalar)
		{
			Vector2<T> r;
			for (size_t i = 0; i < 2; i++)
			{
				r = data[i] * aScalar;
			}
			return r;
		}


		void operator+=(const Vector2<T>& someData)
		{
			Vector2<T> r;
			for (size_t i = 0; i < 2; i++)
			{
				data[i] += someData.data[i];
			}
			return r;
		}


		void operator-=(const Vector2<T>& someData)
		{
			Vector2<T> r;
			for (size_t i = 0; i < 2; i++)
			{
				data[i] -= someData.data[i];
			}
			return r;
		}

		void operator*=(const Vector2<T>& someData)
		{
			Vector2<T> r;
			for (size_t i = 0; i < 2; i++)
			{
				data[i] *= someData.data[i];
			}
			return r;
		}

		void operator*=(const T& aScalar)
		{
			Vector2<T> r;
			for (size_t i = 0; i < 3; i++)
			{
				data[i] *= aScalar;
			}
			return r;
		}


		float Length() {
			return sqrtf(static_cast<float>(x) * static_cast<float>(x) + static_cast<float>(y) * static_cast<float>(y));
		}


		static float Dot(const Vector2<T> aLhs, const Vector2<T> aRhs)
		{
			return (aLhs.x * aRhs.x) + (aLhs.y * aRhs.y);
		}





		static void Clamp(Vector2<T>& aVal, const Vector2<T> aMin, const Vector2<T> aMax)
		{
			Math::Clamp(aVal.x, aMin.x, aMax.x);
			Math::Clamp(aVal.y, aMin.y, aMax.y);
		}


		static Vector2<T> Lerp(const Vector2<T>& aVal, const Vector2<T> aMin, const Vector2<T> aMax)
		{
			Vector2<T> r;
			r.x = Math::Lerp(aVal.x, aMin.x, aMax.x);
			r.y = Math::Lerp(aVal.y, aMin.y, aMax.y);

			return r;
		}

	};

	typedef Vector2<float> Vector2f;
	typedef Vector2<int> Vector2i;
	typedef Vector2<unsigned int> Vector2ui;
}