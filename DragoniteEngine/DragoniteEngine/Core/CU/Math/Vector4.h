#pragma once
#include "MathFunctions.h"
#include <array>
namespace Dragonite
{
	template<typename T>
	struct Vector4
	{
		union
		{
			T x, y, z, w;
			std::array<T, 4> data;
		};

		Vector4() = default;

		Vector4(T anX, T anY, T anZ, T anW)
		{
			data[0] = anX;
			data[1] = anY;
			data[2] = anZ;
			data[3] = anW;
		}

		Vector4(T* aNewVector)
		{
			data = aNewVector;
		}


		Vector4(const Vector4<T>& aVal)
		{

			data = aVal.data;
		}


		void operator=(const Vector4<T>& aVal)
		{

			data = aVal.data;
		}

		Vector4<T> operator+(const Vector4<T>& someData)
		{
			Vector4<T> r;
			for (size_t i = 0; i < 4; i++)
			{
				r.data[i] = data[i] + someData.data[i];
			}
			return r;
		}


		Vector4<T> operator-(const Vector4<T>& someData)
		{
			Vector4<T> r;
			for (size_t i = 0; i < 4; i++)
			{
				r.data[i] = data[i] - someData.data[i];
			}
			return r;
		}


		void operator*=(const Vector4<T>& someData)
		{
			Vector4<T> r;
			for (size_t i = 0; i < 4; i++)
			{
				data[i] *= someData.data[i];
			}
			return r;
		}


		void operator*=(const T& aScalar)
		{
			Vector4<T> r;
			for (size_t i = 0; i < 4; i++)
			{
				data[i] *= aScalar;
			}
			return r;
		}


		void operator+=(const Vector4<T>& someData)
		{
			Vector3<T> r;
			for (size_t i = 0; i < 4; i++)
			{
				data[i] += someData.data[i];
			}
			return r;
		}


		void operator-=(const Vector4<T>& someData)
		{
			Vector4<T> r;
			for (size_t i = 0; i < 4; i++)
			{
				data[i] -= someData.data[i];
			}
			return r;
		}




		float Length() {
			return sqrtf(static_cast<float>(x) * static_cast<float>(x) + static_cast<float>(y) * static_cast<float>(y) + static_cast<float>(z) * static_cast<float>(z) + static_cast<float>(w) * static_cast<float>(w));
		}



		void Normalize()
		{
			const T magnitude = (x * x) + (y * y) + (z * z) + (w * w);

			const T inversedMagnitude = T(1) / sqrt(magnitude);
			x = x * inversedMagnitude;
			y = y * inversedMagnitude;
			z = z * inversedMagnitude;
			w = w * inversedMagnitude;
		}


		static float Dot(const Vector3<T> aLhs, const Vector3<T> aRhs)
		{
			return (aLhs.x * aRhs.x) + (aLhs.y * aRhs.y) + (aLhs.z * aRhs.z);
		}






		static void Clamp(Vector4<T>& aVal, const Vector4<T> aMin, const Vector4<T> aMax)
		{
			Math::Clamp(aVal.x, aMin.x, aMax.x);
			Math::Clamp(aVal.y, aMin.y, aMax.y);
			Math::Clamp(aVal.z, aMin.z, aMax.z);
			Math::Clamp(aVal.w, aMin.w, aMax.w);
		}


		static Vector3<T> Lerp(const Vector4<T>& aVal, const Vector4<T> aMin, const Vector4<T> aMax)
		{
			Vector3<T> r;
			r.x = Math::Lerp(aVal.x, aMin.x, aMax.x);
			r.y = Math::Lerp(aVal.y, aMin.y, aMax.y);
			r.z = Math::Lerp(aVal.z, aMin.z, aMax.z);
			r.z = Math::Lerp(aVal.w, aMin.w, aMax.w);

			return r;
		}

	};

	template <typename T>
	Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector)
	{
		return aVector * aScalar;
	}

	template <typename T>
	Vector4<T> operator*(const Vector4<T>& aSelf,const Vector4<T>& someData)
	{
		Vector4<T> r;
		for (size_t i = 0; i < 4; i++)
		{
			r.data[i] = aSelf.data[i] * someData.data[i];
		}
		return r;
	}

	template <typename T>
	Vector4<T> operator*(const Vector4<T>& aSelf, const T& aScalar)
	{
		Vector4<T> r;
		for (size_t i = 0; i < 4; i++)
		{
			r.data[i] = aSelf.data[i] * aScalar;
		}
		return r;
	}





	typedef Vector4<float> Vector4f;
	typedef Vector4<int> Vector4i;
	typedef Vector4<double> Vector4d;
	typedef Vector4<unsigned int> Vector4ui;
}

