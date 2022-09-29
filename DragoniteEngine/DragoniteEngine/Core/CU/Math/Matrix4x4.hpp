#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include <cstring>
#include <cassert>
#include <cmath>

namespace Dragonite
{

	template<typename T>
	class Matrix3x3;

	template <typename T>
	class Matrix4x4
	{
	public:
		Matrix4x4<T>() : myArray{ {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1} } {}
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix)
		{
			std::memcpy(myArray, aMatrix.myArray, sizeof(myArray));
		}

		inline T& operator[](const unsigned int& anIndex)
		{
			/*	assert((aIndex < 16) && "Index out of bounds.");*/
			return myData[anIndex];
		}


		inline const T& operator[](const unsigned int& anIndex) const
		{
			/*	assert((aIndex < 16) && "Index out of bounds.");*/
			return myData[anIndex];
		}

		Vector3<T> GetPosition() const
		{
			return { (*this)(4,1), (*this)(4,2), (*this)(4,3) };
		}
		Vector4<T> GetPositionVec4() const
		{
			return { (*this)(4,1), (*this)(4,2), (*this)(4,3), (*this)(4,4) };
		}

		Vector3<T> GetForward() const
		{
			Matrix4x4<T> rotation = *this;
			rotation(4, 1) = 0;
			rotation(4, 2) = 0;
			rotation(4, 3) = 0;
			auto result = Vector4<T>{ 0, 0, 1.f, 1.f } *rotation;
			return { result.x, result.y, result.z };
		}

		Vector3<T> GetUp() const
		{
			Matrix4x4<T> rotation = *this;
			rotation(4, 1) = 0;
			rotation(4, 2) = 0;
			rotation(4, 3) = 0;
			auto result = Vector4<T>{ 0, 1.f, 0, 1.f } *rotation;
			return { result.x, result.y, result.z };
		}

		Vector3<T> GetRight() const
		{
			Matrix4x4<T> rotation = *this;
			rotation(4, 1) = 0;
			rotation(4, 2) = 0;
			rotation(4, 3) = 0;
			auto result = Vector4<T>{ 1.f, 0, 0, 1.f } *rotation;
			return { result.x, result.y, result.z };
		}
		void SetPositionRelative(const Vector3<T>& aVector3)
		{
			myRows[3].x = aVector3.x;
			myRows[3].y = aVector3.y;
			myRows[3].z = aVector3.z;
		}
		void SetPositionRelative(const T& x, const T& y, const T& z)
		{
			myRows[3].x = x;
			myRows[3].y = y;
			myRows[3].z = z;
		}

		static Matrix4x4<T> CreateTranslationMatrix(const Vector3<T>& aVector3)
		{
			Matrix4x4<T> result;

			result.myRows[3].x = aVector3.x;
			result.myRows[3].y = aVector3.y;
			result.myRows[3].z = aVector3.z;

			return result;
		}
		void SetScale(const Vector3<T>& aVector3)
		{
			myRows[0].x = aVector3.x;
			myRows[1].y = aVector3.y;
			myRows[2].z = aVector3.z;
		}
		void SetRotation(const Vector3<T>& aVector3)
		{
			ResetRotation();
			*this *= CreateRollPitchYawMatrix(aVector3);
		}
		void SetRotation(const T& aValue)
		{
			Vector3f rotationVec = { aValue, aValue, 0.f };
			ResetRotation();
			*this *= CreateRollPitchYawMatrix(rotationVec);
		}
		inline void ResetRotation()
		{
			myArray[1][1] = 1;
			myArray[1][2] = 0;
			myArray[2][1] = 0;
			myArray[2][2] = 1;
			myArray[0][0] = 1;
			myArray[0][2] = 0;
			myArray[2][0] = 0;
			myArray[0][1] = 0;
			myArray[1][0] = 0;
		}



		template<typename T>
		static Matrix4x4<T> CreateScaleMatrix(Vector3<T> aScaleVector)
		{
			Matrix4x4<T> result;
			result.SetScale(aScaleVector);
			return result;
		}


		static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians)
		{
			Matrix4x4<T> m;
			m(2, 2) = std::cos(aAngleInRadians);
			m(2, 3) = std::sin(aAngleInRadians);
			m(3, 2) = -std::sin(aAngleInRadians);
			m(3, 3) = std::cos(aAngleInRadians);
			return m;
		}
		static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians)
		{
			Matrix4x4<T> m;
			m(1, 1) = std::cos(aAngleInRadians);
			m(1, 3) = -std::sin(aAngleInRadians);
			m(3, 1) = std::sin(aAngleInRadians);
			m(3, 3) = std::cos(aAngleInRadians);
			return m;
		}
		static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians)
		{
			Matrix4x4<T> m;
			m(1, 1) = std::cos(aAngleInRadians);
			m(1, 2) = std::sin(aAngleInRadians);
			m(2, 1) = -std::sin(aAngleInRadians);
			m(2, 2) = std::cos(aAngleInRadians);
			return m;
		}

		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose)
		{
			Matrix4x4 mt;
			for (size_t row = 0; row < 4; ++row)
			{
				for (size_t col = 0; col < 4; ++col)
					mt.myArray[row][col] = aMatrixToTranspose.myArray[col][row];
			}
			return mt;
		}
		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform)
		{
			Matrix4x4<T> inv = aTransform;



			auto t1 = aTransform.myRows[0].Dot(aTransform.myRows[3]);
			auto t2 = aTransform.myRows[1].Dot(aTransform.myRows[3]);
			auto t3 = aTransform.myRows[2].Dot(aTransform.myRows[3]);

			inv.myRows[3] = { 0,0,0,1 };

			inv = Transpose(inv);
			inv.myRows[3] = { -t1, -t2, -t3, 1 };
			return inv;
		}

		static Matrix4x4<T>  GetInverse(const Matrix4x4<T>& aMatrixToInverse)
		{
			T inv[16];

			inv[0] =
				aMatrixToInverse[5] * aMatrixToInverse[10] * aMatrixToInverse[15] -
				aMatrixToInverse[5] * aMatrixToInverse[11] * aMatrixToInverse[14] -
				aMatrixToInverse[9] * aMatrixToInverse[6] * aMatrixToInverse[15] +
				aMatrixToInverse[9] * aMatrixToInverse[7] * aMatrixToInverse[14] +
				aMatrixToInverse[13] * aMatrixToInverse[6] * aMatrixToInverse[11] -
				aMatrixToInverse[13] * aMatrixToInverse[7] * aMatrixToInverse[10];

			inv[4] = -aMatrixToInverse[4] * aMatrixToInverse[10] * aMatrixToInverse[15] +
				aMatrixToInverse[4] * aMatrixToInverse[11] * aMatrixToInverse[14] +
				aMatrixToInverse[8] * aMatrixToInverse[6] * aMatrixToInverse[15] -
				aMatrixToInverse[8] * aMatrixToInverse[7] * aMatrixToInverse[14] -
				aMatrixToInverse[12] * aMatrixToInverse[6] * aMatrixToInverse[11] +
				aMatrixToInverse[12] * aMatrixToInverse[7] * aMatrixToInverse[10];

			inv[8] = aMatrixToInverse[4] * aMatrixToInverse[9] * aMatrixToInverse[15] -
				aMatrixToInverse[4] * aMatrixToInverse[11] * aMatrixToInverse[13] -
				aMatrixToInverse[8] * aMatrixToInverse[5] * aMatrixToInverse[15] +
				aMatrixToInverse[8] * aMatrixToInverse[7] * aMatrixToInverse[13] +
				aMatrixToInverse[12] * aMatrixToInverse[5] * aMatrixToInverse[11] -
				aMatrixToInverse[12] * aMatrixToInverse[7] * aMatrixToInverse[9];

			inv[12] = -aMatrixToInverse[4] * aMatrixToInverse[9] * aMatrixToInverse[14] +
				aMatrixToInverse[4] * aMatrixToInverse[10] * aMatrixToInverse[13] +
				aMatrixToInverse[8] * aMatrixToInverse[5] * aMatrixToInverse[14] -
				aMatrixToInverse[8] * aMatrixToInverse[6] * aMatrixToInverse[13] -
				aMatrixToInverse[12] * aMatrixToInverse[5] * aMatrixToInverse[10] +
				aMatrixToInverse[12] * aMatrixToInverse[6] * aMatrixToInverse[9];

			inv[1] = -aMatrixToInverse[1] * aMatrixToInverse[10] * aMatrixToInverse[15] +
				aMatrixToInverse[1] * aMatrixToInverse[11] * aMatrixToInverse[14] +
				aMatrixToInverse[9] * aMatrixToInverse[2] * aMatrixToInverse[15] -
				aMatrixToInverse[9] * aMatrixToInverse[3] * aMatrixToInverse[14] -
				aMatrixToInverse[13] * aMatrixToInverse[2] * aMatrixToInverse[11] +
				aMatrixToInverse[13] * aMatrixToInverse[3] * aMatrixToInverse[10];

			inv[5] = aMatrixToInverse[0] * aMatrixToInverse[10] * aMatrixToInverse[15] -
				aMatrixToInverse[0] * aMatrixToInverse[11] * aMatrixToInverse[14] -
				aMatrixToInverse[8] * aMatrixToInverse[2] * aMatrixToInverse[15] +
				aMatrixToInverse[8] * aMatrixToInverse[3] * aMatrixToInverse[14] +
				aMatrixToInverse[12] * aMatrixToInverse[2] * aMatrixToInverse[11] -
				aMatrixToInverse[12] * aMatrixToInverse[3] * aMatrixToInverse[10];

			inv[9] = -aMatrixToInverse[0] * aMatrixToInverse[9] * aMatrixToInverse[15] +
				aMatrixToInverse[0] * aMatrixToInverse[11] * aMatrixToInverse[13] +
				aMatrixToInverse[8] * aMatrixToInverse[1] * aMatrixToInverse[15] -
				aMatrixToInverse[8] * aMatrixToInverse[3] * aMatrixToInverse[13] -
				aMatrixToInverse[12] * aMatrixToInverse[1] * aMatrixToInverse[11] +
				aMatrixToInverse[12] * aMatrixToInverse[3] * aMatrixToInverse[9];

			inv[13] = aMatrixToInverse[0] * aMatrixToInverse[9] * aMatrixToInverse[14] -
				aMatrixToInverse[0] * aMatrixToInverse[10] * aMatrixToInverse[13] -
				aMatrixToInverse[8] * aMatrixToInverse[1] * aMatrixToInverse[14] +
				aMatrixToInverse[8] * aMatrixToInverse[2] * aMatrixToInverse[13] +
				aMatrixToInverse[12] * aMatrixToInverse[1] * aMatrixToInverse[10] -
				aMatrixToInverse[12] * aMatrixToInverse[2] * aMatrixToInverse[9];

			inv[2] = aMatrixToInverse[1] * aMatrixToInverse[6] * aMatrixToInverse[15] -
				aMatrixToInverse[1] * aMatrixToInverse[7] * aMatrixToInverse[14] -
				aMatrixToInverse[5] * aMatrixToInverse[2] * aMatrixToInverse[15] +
				aMatrixToInverse[5] * aMatrixToInverse[3] * aMatrixToInverse[14] +
				aMatrixToInverse[13] * aMatrixToInverse[2] * aMatrixToInverse[7] -
				aMatrixToInverse[13] * aMatrixToInverse[3] * aMatrixToInverse[6];

			inv[6] = -aMatrixToInverse[0] * aMatrixToInverse[6] * aMatrixToInverse[15] +
				aMatrixToInverse[0] * aMatrixToInverse[7] * aMatrixToInverse[14] +
				aMatrixToInverse[4] * aMatrixToInverse[2] * aMatrixToInverse[15] -
				aMatrixToInverse[4] * aMatrixToInverse[3] * aMatrixToInverse[14] -
				aMatrixToInverse[12] * aMatrixToInverse[2] * aMatrixToInverse[7] +
				aMatrixToInverse[12] * aMatrixToInverse[3] * aMatrixToInverse[6];

			inv[10] = aMatrixToInverse[0] * aMatrixToInverse[5] * aMatrixToInverse[15] -
				aMatrixToInverse[0] * aMatrixToInverse[7] * aMatrixToInverse[13] -
				aMatrixToInverse[4] * aMatrixToInverse[1] * aMatrixToInverse[15] +
				aMatrixToInverse[4] * aMatrixToInverse[3] * aMatrixToInverse[13] +
				aMatrixToInverse[12] * aMatrixToInverse[1] * aMatrixToInverse[7] -
				aMatrixToInverse[12] * aMatrixToInverse[3] * aMatrixToInverse[5];

			inv[14] = -aMatrixToInverse[0] * aMatrixToInverse[5] * aMatrixToInverse[14] +
				aMatrixToInverse[0] * aMatrixToInverse[6] * aMatrixToInverse[13] +
				aMatrixToInverse[4] * aMatrixToInverse[1] * aMatrixToInverse[14] -
				aMatrixToInverse[4] * aMatrixToInverse[2] * aMatrixToInverse[13] -
				aMatrixToInverse[12] * aMatrixToInverse[1] * aMatrixToInverse[6] +
				aMatrixToInverse[12] * aMatrixToInverse[2] * aMatrixToInverse[5];

			inv[3] = -aMatrixToInverse[1] * aMatrixToInverse[6] * aMatrixToInverse[11] +
				aMatrixToInverse[1] * aMatrixToInverse[7] * aMatrixToInverse[10] +
				aMatrixToInverse[5] * aMatrixToInverse[2] * aMatrixToInverse[11] -
				aMatrixToInverse[5] * aMatrixToInverse[3] * aMatrixToInverse[10] -
				aMatrixToInverse[9] * aMatrixToInverse[2] * aMatrixToInverse[7] +
				aMatrixToInverse[9] * aMatrixToInverse[3] * aMatrixToInverse[6];

			inv[7] = aMatrixToInverse[0] * aMatrixToInverse[6] * aMatrixToInverse[11] -
				aMatrixToInverse[0] * aMatrixToInverse[7] * aMatrixToInverse[10] -
				aMatrixToInverse[4] * aMatrixToInverse[2] * aMatrixToInverse[11] +
				aMatrixToInverse[4] * aMatrixToInverse[3] * aMatrixToInverse[10] +
				aMatrixToInverse[8] * aMatrixToInverse[2] * aMatrixToInverse[7] -
				aMatrixToInverse[8] * aMatrixToInverse[3] * aMatrixToInverse[6];

			inv[11] = -aMatrixToInverse[0] * aMatrixToInverse[5] * aMatrixToInverse[11] +
				aMatrixToInverse[0] * aMatrixToInverse[7] * aMatrixToInverse[9] +
				aMatrixToInverse[4] * aMatrixToInverse[1] * aMatrixToInverse[11] -
				aMatrixToInverse[4] * aMatrixToInverse[3] * aMatrixToInverse[9] -
				aMatrixToInverse[8] * aMatrixToInverse[1] * aMatrixToInverse[7] +
				aMatrixToInverse[8] * aMatrixToInverse[3] * aMatrixToInverse[5];

			inv[15] = aMatrixToInverse[0] * aMatrixToInverse[5] * aMatrixToInverse[10] -
				aMatrixToInverse[0] * aMatrixToInverse[6] * aMatrixToInverse[9] -
				aMatrixToInverse[4] * aMatrixToInverse[1] * aMatrixToInverse[10] +
				aMatrixToInverse[4] * aMatrixToInverse[2] * aMatrixToInverse[9] +
				aMatrixToInverse[8] * aMatrixToInverse[1] * aMatrixToInverse[6] -
				aMatrixToInverse[8] * aMatrixToInverse[2] * aMatrixToInverse[5];

			T det = aMatrixToInverse[0] * inv[0] + aMatrixToInverse[1] * inv[4] + aMatrixToInverse[2] * inv[8] + aMatrixToInverse[3] * inv[12];

			//if (det == 0)
			//	return false;

			det = T(1.0) / det;

			Matrix4x4<T> returnMatrix;
			for (int i = 0; i < 16; i++)
			{
				returnMatrix[i] = inv[i] * det;
			}

			return returnMatrix;
		}


		void  DecomposeMatrix(Vector3f& aPosition, Vector3f& aRotation, Vector3f& aScale) const
		{
			static const float ZPI = 3.1415f;
			static const float RAD2DEG = (180.f / ZPI);
			const Matrix4x4<T>& mat = *this;
			Vector4f scaleX = { mat(1,1),mat(1,2) ,mat(1,3) ,mat(1,4) };
			Vector4f scaleY = { mat(2,1),mat(2,2) ,mat(2,3) ,mat(2,4) };
			Vector4f scaleZ = { mat(3,1),mat(3,2) ,mat(3,3) ,mat(3,4) };

			aScale.x = scaleX.Length();
			aScale.y = scaleY.Length();
			aScale.z = scaleZ.Length();

			scaleX.Normalize();
			scaleY.Normalize();
			scaleZ.Normalize();

			aRotation.x = RAD2DEG * atan2f(scaleY.z, scaleZ.z);
			aRotation.y = RAD2DEG * atan2f(-scaleX.z, sqrtf(scaleY.z * scaleY.z + scaleZ.z * scaleZ.z));
			aRotation.z = RAD2DEG * atan2f(scaleX.y, scaleX.x);

			aPosition.x = mat.GetPosition().x;
			aPosition.y = mat.GetPosition().y;
			aPosition.z = mat.GetPosition().z;
		}

		Matrix4x4<T>& operator=(const Matrix4x4<T>& aMatrix)
		{
			std::memcpy(myArray, aMatrix.myArray, sizeof(myArray));
			return *this;
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
		bool operator!=(const Matrix4x4<T>& aMatrix) const
		{
			return std::memcmp(myArray, aMatrix.myArray, sizeof(myArray)); // should be faster than a for loop
		}
		bool operator==(const Matrix4x4<T>& aMatrix) const
		{
			return !(*this != aMatrix);
		}
		Matrix4x4<T>& operator+=(const Matrix4x4<T>& aMatrix)
		{
			for (size_t row = 0; row < 4; ++row)
				myRows[row] += aMatrix.myRows[row];
			return *this;
		}



		void operator=(Matrix3x3<T>& aLhs)
		{
			for (size_t row = 0; row < 3; ++row)
			{
				for (size_t collum = 0; collum < 3; ++collum)
				{
					myArray[row][collum] = aLhs(row + 1, collum + 1);
				}
			}
		}

		friend Matrix4x4<T> operator+(const Matrix4x4<T>& aLhs, const Matrix4x4<T>& aRhs)
		{
			Matrix4x4<T> m(aLhs);
			return m += aRhs;
		}
		Matrix4x4<T>& operator-=(const Matrix4x4<T>& aMatrix)
		{
			for (size_t row = 0; row < 4; ++row)
				myRows[row] -= aMatrix.myRows[row];
			return *this;
		}
		friend Matrix4x4<T> operator-(const Matrix4x4<T>& aLhs, const Matrix4x4<T>& aRhs)
		{
			Matrix4x4<T> m(aLhs);
			return m -= aRhs;
		}
		friend Matrix4x4<T> operator*(const Matrix4x4<T>& aLhs, const Matrix4x4<T>& aRhs)
		{
			Matrix4x4<T> m;
			for (size_t row = 0; row < 4; ++row)
				m.myRows[row] = aLhs.myRows[row] * aRhs;
			return m;
		}
		friend Vector4<T> operator*<>(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix);
		friend Vector4<T> operator*<>(const Vector3<T>& aVector, const Matrix4x4<T>& aMatrix);
		Matrix4x4<T>& operator*=(const Matrix4x4<T>& aMatrix)
		{
			*this = *this * aMatrix;
			return *this;
		}

	private:
		union
		{
			T myArray[4][4];
			Vector4<T> myRows[4];
			T myData[16];
		};

		void ValidateIndices(const int aRow, const int aColumn) const
		{
			assert(1 <= aRow && aRow <= 4 && 1 <= aColumn && aColumn <= 4 && "Matrix indices out of bounds");
		}
	};

	template <typename T>
	Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		return aVector.x * aMatrix.myRows[0] + aVector.y * aMatrix.myRows[1] + aVector.z * aMatrix.myRows[2] + aVector.w * aMatrix.myRows[3];
	}

	template <typename T>
	Vector4<T> operator*(const Vector3<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		return aVector.x * aMatrix.myRows[0] + aVector.y * aMatrix.myRows[1] + aVector.z * aMatrix.myRows[2] + 1.f * aMatrix.myRows[3];
	}

	typedef Matrix4x4<float> Matrix4x4f;

}
