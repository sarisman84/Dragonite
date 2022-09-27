#pragma once
#include "MathFunctions.h"
#include <array>
namespace Dragonite
{
	template<typename T>
	struct Vector3
	{
		union
		{
			T x, y, z;
			std::array<T, 3> data;
		};

		Vector3() = default;

		Vector3(T anX, T anY, T anZ)
		{
			data[0] = anX;
			data[1] = anY;
			data[2] = anZ;
		}

		Vector3(T* aNewVector)
		{
			data = aNewVector;
		}


		Vector3(const Vector3<T>& aVal)
		{

			data = aVal.data;
		}


		void operator=(const Vector3<T>& aVal)
		{
			data = aVal.data;
		}

		Vector3<T> operator+(const Vector3<T>& someData)
		{
			Vector3<T> r;
			for (size_t i = 0; i < 3; i++)
			{
				r = data[i] + someData.data[i];
			}
			return r;
		}


		Vector3<T> operator-(const Vector3<T>& someData)
		{
			Vector3<T> r;
			for (size_t i = 0; i < 3; i++)
			{
				r = data[i] - someData.data[i];
			}
			return r;
		}




		void operator+=(const Vector3<T>& someData)
		{
			Vector3<T> r;
			for (size_t i = 0; i < 3; i++)
			{
				data[i] += someData.data[i];
			}
			return r;
		}


		void operator-=(const Vector3<T>& someData)
		{
			Vector3<T> r;
			for (size_t i = 0; i < 3; i++)
			{
				data[i] -= someData.data[i];
			}
			return r;
		}

		void operator*=(const Vector3<T>& someData)
		{
			Vector3<T> r;
			for (size_t i = 0; i < 3; i++)
			{
				data[i] *= someData.data[i];
			}
			return r;
		}

		void operator*=(const T& aScalar)
		{
			Vector3<T> r;
			for (size_t i = 0; i < 3; i++)
			{
				data[i] *= aScalar;
			}
			return r;
		}


		float Length() {
			return sqrtf(static_cast<float>(x) * static_cast<float>(x) + static_cast<float>(y) * static_cast<float>(y) + static_cast<float>(z) * static_cast<float>(z));
		}


		static float Dot(const Vector3<T> aLhs, const Vector3<T> aRhs)
		{
			return (aLhs.x * aRhs.x) + (aLhs.y * aRhs.y) + (aLhs.z * aRhs.z);
		}



		static Vector3<T> Cross(const Vector3<T> aLhs, const Vector3<T> aRhs)
		{
			Vector3<T> r;
			r.x = aLhs.y * aRhs.z - aLhs.z * aRhs.y;
			r.y = aLhs.z * aRhs.x - aLhs.x * aRhs.z;
			r.z = aLhs.x * aRhs.y - aLhs.y * aRhs.x;

			return r;
		}


		static void Clamp(Vector3<T>& aVal, const Vector3<T> aMin, const Vector3<T> aMax)
		{
			Math::Clamp(aVal.x, aMin.x, aMax.x);
			Math::Clamp(aVal.y, aMin.y, aMax.y);
			Math::Clamp(aVal.z, aMin.z, aMax.z);
		}


		static Vector3<T> Lerp(const Vector3<T>& aVal, const Vector3<T> aMin, const Vector3<T> aMax)
		{
			Vector3<T> r;
			r.x = Math::Lerp(aVal.x, aMin.x, aMax.x);
			r.y = Math::Lerp(aVal.y, aMin.y, aMax.y);
			r.z = Math::Lerp(aVal.z, aMin.z, aMax.z);

			return r;
		}

	};


	template <typename T>
	Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector)
	{
		return aVector * aScalar;
	}

	template <typename T>
	Vector3<T> operator*(const Vector3<T>& aSelf, const Vector3<T>& someData)
	{
		Vector3<T> r;
		for (size_t i = 0; i < 3; i++)
		{
			r.data[i] = aSelf.data[i] * someData.data[i];
		}
		return r;
	}

	template <typename T>
	Vector3<T> operator*(const Vector3<T>& aSelf, const T& aScalar)
	{
		Vector3<T> r;
		for (size_t i = 0; i < 3; i++)
		{
			r.data[i] = aSelf.data[i] * aScalar;
		}
		return r;
	}

	typedef Vector3<float> Vector3f;
	typedef Vector3<int> Vector3i;
	typedef Vector3<double> Vector3d;
	typedef Vector3<unsigned int> Vector3ui;
}

