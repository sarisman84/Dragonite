#pragma once
#include "Vector4.h"
#include "Vector3.h"

#include <array>
namespace Math
{


	template<typename T>
	class Matrix4x4
	{
	public:

		Matrix4x4<T>()
		{
			for (int row = 0; row < myMatrixBuffer.size(); row++)
			{
				for (int column = 0; column < myMatrixBuffer[row].size(); column++)
				{
					myMatrixBuffer[row][column] = row == column ? 1 : T();
				}
			}
		}
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix)
		{
			myMatrixBuffer = aMatrix.myMatrixBuffer;
		}

		void SetRow(const int aRow, const Vector4<T> someNumbers)
		{
			myMatrixBuffer[aRow - 1][0] = someNumbers.x;
			myMatrixBuffer[aRow - 1][1] = someNumbers.y;
			myMatrixBuffer[aRow - 1][2] = someNumbers.z;
			myMatrixBuffer[aRow - 1][3] = someNumbers.w;
		}


		void SetPosition(Vector3<T> aPosition)
		{
			SetRow(4, { aPosition.x, aPosition.y, aPosition.z, 1 });
		}


		Vector3<T> GetPosition()
		{
			Vector3<T> position;
			position.x = (*this)(4, 1);
			position.y = (*this)(4, 2);
			position.z = (*this)(4, 3);
			return position;
		}


		Vector3<T> GetUpDirection()
		{
			Vector3<T> direction;
			direction.x = (*this)(2, 1);
			direction.y = (*this)(2, 2);
			direction.z = (*this)(2, 3);
			return direction;
		}


		Vector3<T> GetForwardDirection()
		{
			Vector3<T> direction;
			direction.x = (*this)(2, 1);
			direction.y = (*this)(2, 2);
			direction.z = (*this)(2, 3);
			return direction;
		}


		Vector3<T> GetRightDirection()
		{
			Vector3<T> direction;
			direction.x = (*this)(2, 1);
			direction.y = (*this)(2, 2);
			direction.z = (*this)(2, 3);
			return direction;
		}


		Vector4<T> GetRow(const int aRow)
		{
			Vector4<T> result;
			result.x = myMatrixBuffer[aRow - 1][0];
			result.y = myMatrixBuffer[aRow - 1][1];
			result.z = myMatrixBuffer[aRow - 1][2];
			result.w = myMatrixBuffer[aRow - 1][3];
			return result;
		}

		T& operator()(const int aRow, const int aColumn)
		{
			int rowKey = aRow - 1;
			int columnKey = aColumn - 1;

			if (rowKey < 0)
			{

				rowKey = 0;

			}

			if (columnKey < 0)
			{
				columnKey = 0;
			}

			return myMatrixBuffer[rowKey][columnKey];
		}
		const T& operator()(const int aRow, const int aColumn) const
		{
			int rowKey = aRow - 1;
			int columnKey = aColumn - 1;

			if (rowKey < 0)
			{
				rowKey = 0;
			}

			if (columnKey < 0)
			{
				columnKey = 0;
			}

			return myMatrixBuffer[rowKey][columnKey];
		}

		static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians)
		{
			Matrix4x4<T> result;

			result(2, 2) = static_cast<T>(cos(aAngleInRadians));
			result(2, 3) = static_cast<T>(sin(aAngleInRadians));

			result(3, 2) = static_cast<T>(-sin(aAngleInRadians));
			result(3, 3) = static_cast<T>(cos(aAngleInRadians));


			return result;
		}
		static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians)
		{
			Matrix4x4<T> result;

			result(1, 1) = static_cast<T>(cos(aAngleInRadians));
			result(1, 3) = static_cast<T>(-sin(aAngleInRadians));

			result(3, 3) = static_cast<T>(cos(aAngleInRadians));
			result(3, 1) = static_cast<T>(sin(aAngleInRadians));


			return result;
		}
		static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians)
		{
			Matrix4x4<T> result;

			result(1, 1) = static_cast<T>(cos(aAngleInRadians));
			result(1, 2) = static_cast<T>(sin(aAngleInRadians));

			result(2, 1) = static_cast<T>(-sin(aAngleInRadians));
			result(2, 2) = static_cast<T>(cos(aAngleInRadians));


			return result;
		}


		static Matrix4x4<T> CreateSizeMatrix(Vector3<T> aSize)
		{
			Matrix4x4<T> result;
			result(1, 1) = aSize.x;
			result(2, 2) = aSize.y;
			result(3, 3) = aSize.z;
			return result;
		}

		static Matrix4x4<T> CreateTransformMatrix(Vector3<T> aPosition)
		{
			Matrix4x4<T> result;
			result(4, 1) = aPosition.x;
			result(4, 2) = aPosition.y;
			result(4, 3) = aPosition.z;
			result(4, 4) = 1;
			return result;
		}

		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose)
		{
			Matrix4x4<T> result;

			/*
			1 2 3 4     1 5 9 4
			5 6 7 8 ==> 2 6 1 5
			9 1 2 3     3 7 2 6
			4 5 6 7     4 8 3 7
			*/

			for (int row = 1; row <= 4; row++)
			{
				for (int column = 1; column <= 4; column++)
				{
					result(column, row) = aMatrixToTranspose(row, column); // row 2 column 1 => row 1 column 2
				}
			}

			return result;
		}

		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform)
		{

			Matrix4x4<T> copy = aTransform;
			Matrix4x4<T> rotationInverse = Matrix4x4<T>::Transpose(aTransform);



			Matrix4x4<T> translationInverse;
			translationInverse(4, 1) = -aTransform(4, 1);
			translationInverse(4, 2) = -aTransform(4, 2);
			translationInverse(4, 3) = -aTransform(4, 3);

			if ((copy * (translationInverse * rotationInverse)) != Matrix4x4<T>()) return translationInverse * rotationInverse;

			if ((copy * translationInverse) != Matrix4x4<T>()) return rotationInverse;

			if ((copy * rotationInverse) != Matrix4x4<T>()) return translationInverse;


			return Matrix4x4<T>();
		}

		Matrix4x4<T> operator+(const Matrix4x4<T> aRhs)
		{

			Matrix4x4<T> result;

			for (int row = 0; row < myMatrixBuffer.size(); row++)
			{
				for (int column = 0; column < myMatrixBuffer[row].size(); column++)
				{
					result(row + 1, column + 1) = myMatrixBuffer[row][column] + aRhs.myMatrixBuffer[row][column];
				}
			}

			return result;
		}
		void operator+=(const Matrix4x4<T> aRhs)
		{
			for (int row = 0; row < myMatrixBuffer.size(); row++)
			{
				for (int column = 0; column < myMatrixBuffer[row].size(); column++)
				{
					myMatrixBuffer[row][column] += aRhs.myMatrixBuffer[row][column];
				}
			}
		}

		Matrix4x4<T> operator-(const Matrix4x4<T> aRhs)
		{

			Matrix4x4<T> result;

			for (int row = 0; row < myMatrixBuffer.size(); row++)
			{
				for (int column = 0; column < myMatrixBuffer[row].size(); column++)
				{
					result(row + 1, column + 1) = myMatrixBuffer[row][column] - aRhs.myMatrixBuffer[row][column];
				}
			}

			return result;
		}
		void operator-=(const Matrix4x4<T> aRhs)
		{
			for (int row = 0; row < myMatrixBuffer.size(); row++)
			{
				for (int column = 0; column < myMatrixBuffer[row].size(); column++)
				{
					myMatrixBuffer[row][column] -= aRhs.myMatrixBuffer[row][column];
				}
			}
		}

		Matrix4x4<T> operator*(const Matrix4x4<T> aMatrix)
		{
			Matrix4x4<T> temp;
			for (int targetRow = 1; targetRow <= 4; targetRow++)
			{
				for (int targetColumn = 1; targetColumn <= 4; targetColumn++)
				{
					temp(targetRow, targetColumn) = T();
					for (int currentColumn = 1; currentColumn <= 4; currentColumn++)
					{

						temp(targetRow, targetColumn) += myMatrixBuffer[targetRow - 1][currentColumn - 1] * aMatrix(currentColumn, targetColumn);

					}
				}
			}

			return temp;
		}
		void operator*=(const Matrix4x4<T> aMatrix)
		{

			Matrix4x4<T> temp;
			for (int targetRow = 1; targetRow <= 4; targetRow++)
			{
				for (int targetColumn = 1; targetColumn <= 4; targetColumn++)
				{
					temp(targetRow, targetColumn) = T();
					for (int currentColumn = 1; currentColumn <= 4; currentColumn++)
					{

						temp(targetRow, targetColumn) += myMatrixBuffer[targetRow - 1][currentColumn - 1] * aMatrix(currentColumn, targetColumn);

					}
				}
			}

			for (int row = 0; row < 4; row++)
			{
				for (int column = 0; column < 4; column++)
				{
					myMatrixBuffer[row][column] = temp(row + 1, column + 1);
				}
			}
		}

		const bool operator==(const Matrix4x4<T> aMatrix) const
		{
			for (int row = 0; row < myMatrixBuffer.size(); row++)
			{
				for (int column = 0; column < myMatrixBuffer[row].size(); column++)
				{
					if (myMatrixBuffer[row][column] != aMatrix.myMatrixBuffer[row][column]) return false;
				}
			}

			return true;
		}

		const bool operator!=(const Matrix4x4<T> aMatrix) const
		{
			return !(*this == aMatrix);
		}

		void operator=(const Matrix4x4<T> aRhs)
		{
			myMatrixBuffer = aRhs.myMatrixBuffer;
		}

	private:

		std::array<std::array<T, 4>, 4> myMatrixBuffer;

	};


	template<typename T>
	Vector4<T> operator *(const Vector4<T> aLhs, const Matrix4x4<T> aRhs)
	{
		std::array<T, 4> vectorElements = { aLhs.x, aLhs.y, aLhs.z, aLhs.w };
		Vector4<T> result;

		for (int row = 1; row <= 4; row++)
		{
			result.x += (vectorElements[row - 1] * aRhs(row, 1));
		}


		for (int row = 1; row <= 4; row++)
		{
			result.y += (vectorElements[row - 1] * aRhs(row, 2));
		}


		for (int row = 1; row <= 4; row++)
		{
			result.z += (vectorElements[row - 1] * aRhs(row, 3));
		}


		for (int row = 1; row <= 4; row++)
		{
			result.w += (vectorElements[row - 1] * aRhs(row, 4));
		}


		return result;
	}


	using Matrix4x4f = Matrix4x4<float>;
}