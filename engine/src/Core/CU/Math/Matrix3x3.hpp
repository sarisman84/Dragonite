#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4x4.hpp"
#include <cstring>
#include <cassert>
#include <cmath>
#include <array>

namespace Dragonite
{
	template <typename T>
	class Matrix3x3
	{
	public:
		
		Matrix3x3<T>(std::array<T, 9> aList) :
			myArray{ 
				{aList[0],aList[1], aList[2]}, 
				{aList[3],aList[4], aList[5]}, 
				{aList[6],aList[7], aList[8]}} 
		{}
		Matrix3x3<T>() : myArray{ {1,0,0}, {0,1,0}, {0,0,1} } {}
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix)
		{
			std::memcpy(myArray, aMatrix.myArray, sizeof(myArray));
		}
		Matrix3x3<T>& operator=(const Matrix3x3<T>& aMatrix)
		{
			std::memcpy(myArray, aMatrix.myArray, sizeof(myArray));
			return *this;
		}

		Matrix3x3<T>(const Matrix4x4<T>& aMatrix)
		{
			for (size_t row = 0; row < 3; ++row)
			{
				for (size_t col = 0; col < 3; ++col)
					myArray[row][col] = aMatrix(row + 1, col + 1);
			}
		}



		T& operator()(const int aRow, const int aColumn)
		{
			ValidateIndices(aRow, aColumn);
			return myArray[aRow - 1][aColumn - 1];
		}
		const T& operator()(const int aRow, const int aColumn) const
		{
			ValidateIndices(aRow, aColumn);
			return myArray[aRow - 1][aColumn - 1];
		}

		bool operator!=(const Matrix3x3<T>& aMatrix) const
		{
			return std::memcmp(myArray, aMatrix.myArray, sizeof(myArray)); // should be faster than a for loop
		}
		bool operator==(const Matrix3x3<T>& aMatrix) const
		{
			return !(*this != aMatrix);
		}

		Matrix3x3<T>& operator+=(const Matrix3x3<T>& aMatrix)
		{
			for (size_t row = 0; row < 3; ++row)
				myRows[row] += aMatrix.myRows[row];
			return *this;
		}
		friend Matrix3x3<T> operator+(const Matrix3x3<T>& aLhs, const Matrix3x3<T>& aRhs)
		{
			Matrix3x3<T> m(aLhs);
			return m += aRhs;
		}

		Matrix3x3<T>& operator-=(const Matrix3x3<T>& aMatrix)
		{
			for (size_t row = 0; row < 3; ++row)
				myRows[row] -= aMatrix.myRows[row];
			return *this;
		}
		friend Matrix3x3<T> operator-(const Matrix3x3<T>& aLhs, const Matrix3x3<T>& aRhs)
		{
			Matrix3x3<T> m(aLhs);
			return m -= aRhs;
		}

		friend Matrix3x3<T> operator*(const Matrix3x3<T>& aLhs, const Matrix3x3<T>& aRhs)
		{
			Matrix3x3<T> m;
			for (size_t row = 0; row < 3; ++row)
				m.myRows[row] = aLhs.myRows[row] * aRhs;
			return m;
		}




		friend Vector3<T> operator*<>(const Vector3<T>& aVector, const Matrix3x3<T>& aMatrix);
		friend Vector2<T> operator*<>(const Vector2<T>& aVector, const Matrix3x3<T>& aMatrix);
		
		Matrix3x3<T>& operator*=(const Matrix3x3<T>& aMatrix)
		{
			*this = *this * aMatrix;
			return *this;
		}

		static Matrix3x3<T> CreateRotationAroundX(T aAngleInRadians)
		{
			Matrix3x3<T> m;
			m(2, 2) = std::cos(aAngleInRadians);
			m(2, 3) = std::sin(aAngleInRadians);
			m(3, 2) = -std::sin(aAngleInRadians);
			m(3, 3) = std::cos(aAngleInRadians);
			return m;
		}
		static Matrix3x3<T> CreateRotationAroundY(T aAngleInRadians)
		{
			Matrix3x3<T> m;
			m(1, 1) = std::cos(aAngleInRadians);
			m(1, 3) = -std::sin(aAngleInRadians);
			m(3, 1) = std::sin(aAngleInRadians);
			m(3, 3) = std::cos(aAngleInRadians);
			return m;
		}
		static Matrix3x3<T> CreateRotationAroundZ(T aAngleInRadians)
		{
			Matrix3x3<T> m;
			m(1, 1) = std::cos(aAngleInRadians);
			m(1, 2) = std::sin(aAngleInRadians);
			m(2, 1) = -std::sin(aAngleInRadians);
			m(2, 2) = std::cos(aAngleInRadians);
			return m;
		}


		static Matrix3x3<T> CreateScaleMatrix(const Vector2<T>& aScale)
		{
			Matrix3x3<T> m;
			m(1, 1) = aScale.x;
			m(2, 2) = aScale.y;

			return m;
		}
		template<typename A>
		static Matrix3x3<T> CreateScaleMatrix(const Vector2<A>& aScale)
		{
			Matrix3x3<T> m;
			m(1, 1) = static_cast<T>(aScale.x);
			m(2, 2) = static_cast<T>(aScale.y);

			return m;
		}

		static Matrix3x3<T> CreateTranslationMatrix(const Vector2<T>& aPosition)
		{
			Matrix3x3<T> m;
			m(3, 1) = aPosition.x;
			m(3, 2) = aPosition.y;
			return m;
		}

		template<typename A>
		static Matrix3x3<T> CreateTranslationMatrix(const Vector2<A>& aPosition)
		{
			Matrix3x3<T> m;
			m(3, 1) = static_cast<T>(aPosition.x);
			m(3, 2) = static_cast<T>(aPosition.y);
			return m;
		}

		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose)
		{
			Matrix3x3 mt;
			for (size_t row = 0; row < 3; ++row)
			{
				for (size_t col = 0; col < 3; ++col)
					mt.myArray[row][col] = aMatrixToTranspose.myArray[col][row];
			}
			return mt;
		}

	private:
		union
		{
			T myArray[3][3];
			Vector3<T> myRows[3];
		};

		void ValidateIndices(const int aRow, const int aColumn) const
		{
			assert(1 <= aRow && aRow <= 3 && 1 <= aColumn && aColumn <= 3 && "Matrix indices out of bounds");
		}
	};




	template <typename T>
	Dragonite::Vector3<T> operator*(const Dragonite::Vector3<T>& aVector, const Matrix3x3<T>& aMatrix)
	{
		return aVector.x * aMatrix.myRows[0] + aVector.y * aMatrix.myRows[1] + aVector.z * aMatrix.myRows[2];
	}


	template <typename T>
	Dragonite::Vector2<T> operator*(const Dragonite::Vector2<T>& aVector, const Matrix3x3<T>& aMatrix)
	{
		Dragonite::Vector2f row1 = Dragonite::Vector2f(aMatrix.myRows[0].x, aMatrix.myRows[0].y);
		Dragonite::Vector2f row2 = Dragonite::Vector2f(aMatrix.myRows[1].x, aMatrix.myRows[1].y);
		return aVector.x * row1 + aVector.y * row2;
	}

	typedef Matrix3x3<float> Matrix3x3f;
	typedef Matrix3x3<int> Matrix3x3i;









}