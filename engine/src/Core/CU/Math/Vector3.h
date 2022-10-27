#pragma once
#include "MathFunctions.h"
#include <array>
namespace Dragonite
{
	template <class T> class Vector3
	{
	public:
		static const Vector3<T> Zero;
		static const Vector3<T> One;
		static const Vector3<T> Up;
		static const Vector3<T> Forward;
		static const Vector3<T> Right;

		T x;
		T y;
		T z;

		Vector3<T>() : x(0), y(0), z(0) {}
		Vector3<T>(const T& aX, const T& aY, const T& aZ) : x(aX), y(aY), z(aZ) {}
		Vector3<T>(const Vector3<T>& aVector) = default;
		Vector3<T>& operator=(const Vector3<T>& aVector3) = default;


		Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }

		static Vector3<T> Lerp(const Vector3<T>& aStart, const Vector3<T>& aEnd, const float aT)
		{
			return (aStart + aT * (aEnd - aStart));
		}

		static Vector3<T> Clamp(Vector3<T>& aValue, const T& aMinScalar, const T& aMaxScalar)
		{
			Math::Clamp(aValue.x, aMinScalar, aMaxScalar);
			Math::Clamp(aValue.y, aMinScalar, aMaxScalar);
			Math::Clamp(aValue.z, aMinScalar, aMaxScalar);

			return aValue;
		}

		static Vector3<T> Clamp(Vector3<T>& aValue, const Vector3<T>& aMinValue, const T& aMaxScalar)
		{
			Math::Clamp(aValue.x, aMinValue.x, aMaxScalar);
			Math::Clamp(aValue.y, aMinValue.y, aMaxScalar);
			Math::Clamp(aValue.z, aMinValue.z, aMaxScalar);

			return aValue;
		}

		static Vector3<T> Clamp(Vector3<T>& aValue, const T& aMinScalar, const Vector3<T>& aMaxValue)
		{
			Math::Clamp(aValue.x, aMinScalar, aMaxValue.x);
			Math::Clamp(aValue.y, aMinScalar, aMaxValue.y);
			Math::Clamp(aValue.z, aMinScalar, aMaxValue.z);

			return aValue;
		}

		static Vector3<T> Clamp(Vector3<T>& aValue, const Vector3<T>& aMinValue, const Vector3<T>& aMaxValue)
		{
			Math::Clamp(aValue.x, aMinValue.x, aMaxValue.x);
			Math::Clamp(aValue.y, aMinValue.y, aMaxValue.y);
			Math::Clamp(aValue.z, aMinValue.z, aMaxValue.z);

			return aValue;
		}


		bool operator==(const Vector3<T>& aVector)
		{
			return (x == aVector.x) && (y == aVector.y) && (z == aVector.z);
		}
		bool operator!=(const Vector3<T>& aVector) { return !(*this == aVector); }
		~Vector3<T>() = default;
		T LengthSqr() const { return x * x + y * y + z * z; }
		T Length() const { return sqrt(LengthSqr()); }
		Vector3<T> GetNormalized() const
		{
			if (x == 0 && y == 0 && z == 0)
				return Vector3<T>();
			return Vector3<T>(x / Length(), y / Length(), z / Length());
		}
		void Normalize()
		{
			if (x == 0 && y == 0 && z == 0)
				return;
			T length = Length();
			x /= length;
			y /= length;
			z /= length;
		}
		T Dot(const Vector3<T>& aVector) const { return x * aVector.x + y * aVector.y + z * aVector.z; }
		Vector3<T> Cross(const Vector3<T>& aVector) const
		{
			return Vector3<T>(y * aVector.z - z * aVector.y, z * aVector.x - x * aVector.z, x * aVector.y - y * aVector.x);
		}
		T& operator[](size_t aValue)
		{
			if (aValue == 0)
				return x;
			if (aValue == 1)
				return y;
			if (aValue == 2)
				return z;
		}
		const T& operator[](size_t aValue) const
		{
			if (aValue == 0)
				return x;
			if (aValue == 1)
				return y;
			if (aValue == 2)
				return z;
		}
	};



	template <class T> Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return Vector3<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z);
	}
	template <class T> Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1) { return aVector0 + (-aVector1); }
	template <class T> Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
	{
		return Vector3<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar);
	}
	template <class T> Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector) { return aVector * aScalar; }
	template <class T> Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar) { return aVector * (T(1) / aScalar); }
	template <class T> void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
	}
	template <class T> void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1) { aVector0 += (-aVector1); }
	template <class T> void operator*=(Vector3<T>& aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
	}
	template <class T> void operator/=(Vector3<T>& aVector, const T& aScalar) { aVector *= (T(1) / aScalar); }

	typedef Vector3<float> Vector3f;

	template<typename T>
	const Vector3<T> Vector3<T>::Zero(0, 0, 0);
	template<typename T>
	const Vector3<T> Vector3<T>::One(1, 1, 1);
	template<typename T>
	const Vector3<T> Vector3<T>::Forward(0, 0, 1);
	template<typename T>
	const Vector3<T> Vector3<T>::Right(1, 0, 0);
	template<typename T>
	const Vector3<T> Vector3<T>::Up(0, 1, 0);

	typedef Vector3<float> Vector3f;
	typedef Vector3<int> Vector3i;
	typedef Vector3<double> Vector3d;
	typedef Vector3<unsigned int> Vector3ui;


}

