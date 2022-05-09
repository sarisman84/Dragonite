#pragma once
#include "Vector3.h"
#include <array>

namespace Math
{
	template<typename T>
	class Matrix3x3
	{
	public:
		Matrix3x3<T>()
		{
			for (int row = 0; row < myMatrixBuffer.size(); row++)
			{
				for (int column = 0; column < myMatrixBuffer[row].size(); column++)
				{
					myMatrixBuffer[row][column] = row == column ? 1 : T();
				}
			}
		}
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix)
		{
			myMatrixBuffer = aMatrix.myMatrixBuffer;
		}
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix)
		{
			for (int row = 0; row < myMatrixBuffer.size(); row++)
			{
				for (int column = 0; column < myMatrixBuffer[row].size(); column++)
				{
					myMatrixBuffer[row][column] = aMatrix(row + 1, column + 1);
				}
			}
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

			if (rowKey < 0 || columnKey < 0) return T();

			return myMatrixBuffer[rowKey][columnKey];
		}




		Matrix3x3<T> operator+(Matrix3x3<T> aMatrix)
		{
			Matrix3x3<T> result;
			for (int row = 0; row < myMatrixBuffer.size(); row++)
			{
				for (int column = 0; column < myMatrixBuffer[row].size(); column++)
				{
					result(row + 1, column + 1) = myMatrixBuffer[row][column] + aMatrix.myMatrixBuffer[row][column];
				}
			}

			return result;
		}

		void operator += (Matrix3x3<T> aMatrix)
		{
			for (int row = 0; row < myMatrixBuffer.size(); row++)
			{
				for (int column = 0; column < myMatrixBuffer[row].size(); column++)
				{
					myMatrixBuffer[row][column] += aMatrix.myMatrixBuffer[row][column];
				}
			}
		}


		Matrix3x3<T> operator-(Matrix3x3<T> aMatrix)
		{
			Matrix3x3<T> result;
			for (int row = 0; row < myMatrixBuffer.size(); row++)
			{
				for (int column = 0; column < myMatrixBuffer[row].size(); column++)
				{
					result(row + 1, column + 1) = myMatrixBuffer[row][column] - aMatrix.myMatrixBuffer[row][column];
				}
			}
			return result;
		}

		void operator -= (Matrix3x3<T> aMatrix)
		{
			for (int row = 0; row < myMatrixBuffer.size(); row++)
			{
				for (int column = 0; column < myMatrixBuffer[row].size(); column++)
				{
					myMatrixBuffer[row][column] -= aMatrix.myMatrixBuffer[row][column];
				}
			}
		}

		Matrix3x3<T> operator *(Matrix3x3<T> aMatrix)
		{
			Matrix3x3<T> temp;
			for (int targetRow = 1; targetRow <= 3; targetRow++)
			{
				for (int targetColumn = 1; targetColumn <= 3; targetColumn++)
				{
					temp(targetRow, targetColumn) = T();
					for (int currentColumn = 1; currentColumn <= 3; currentColumn++)
					{

						temp(targetRow, targetColumn) += myMatrixBuffer[targetRow - 1][currentColumn - 1] * aMatrix(currentColumn, targetColumn);

					}
				}
			}
			return temp;


		}


		void operator *= (Matrix3x3<T> aMatrix)
		{
			/*
			* A        B
			1 0 0  - 3 2 1 -> 3*1 + 0*4 + 0*4 -> 3 0 0
			0 1 0    4 5 6    0*4 + 1*5 + 0*6    0 5 0
			0 0 1    4 2 0    0*4 + 0*2 + 1*0    0 0 0


			*/

			Matrix3x3<T> temp;
			for (int targetRow = 1; targetRow <= 3; targetRow++)
			{
				for (int targetColumn = 1; targetColumn <= 3; targetColumn++)
				{
					temp(targetRow, targetColumn) = T();
					for (int currentColumn = 1; currentColumn <= 3; currentColumn++)
					{

						temp(targetRow, targetColumn) += myMatrixBuffer[targetRow - 1][currentColumn - 1] * aMatrix(currentColumn, targetColumn);

					}
				}
			}

			for (int row = 0; row < 3; row++)
			{
				for (int column = 0; column < 3; column++)
				{
					myMatrixBuffer[row][column] = temp(row + 1, column + 1);
				}
			}

		}



		void operator =(const Matrix3x3<T> aMatrix)
		{
			myMatrixBuffer = aMatrix.myMatrixBuffer;
		}

		const bool operator==(const Matrix3x3<T> aMatrix) const
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








		// Static functions for creating rotation matrices.
		static Matrix3x3<T> CreateRotationAroundX(T aAngleInRadians)
		{
			Matrix3x3<T> result;

			result(2, 2) = static_cast<T>(cos(aAngleInRadians));
			result(2, 3) = static_cast<T>(sin(aAngleInRadians));

			result(3, 2) = static_cast<T>(-sin(aAngleInRadians));
			result(3, 3) = static_cast<T>(cos(aAngleInRadians));


			return result;
		}
		static Matrix3x3<T> CreateRotationAroundY(T aAngleInRadians)
		{
			Matrix3x3<T> result;

			result(1, 1) = static_cast<T>(cos(aAngleInRadians));
			result(1, 3) = static_cast<T>(-sin(aAngleInRadians));

			result(3, 3) = static_cast<T>(cos(aAngleInRadians));
			result(3, 1) = static_cast<T>(sin(aAngleInRadians));


			return result;
		}
		static Matrix3x3<T> CreateRotationAroundZ(T aAngleInRadians)
		{
			Matrix3x3<T> result;

			result(1, 1) = static_cast<T>(cos(aAngleInRadians));
			result(1, 2) = static_cast<T>(sin(aAngleInRadians));

			result(2, 1) = static_cast<T>(-sin(aAngleInRadians));
			result(2, 2) = static_cast<T>(cos(aAngleInRadians));


			return result;
		}
		// Static function for creating a transpose of a matrix.
		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose)
		{
			Matrix3x3<T> result;

			for (int row = 1; row <= 3; row++)
			{
				for (int column = 1; column <= 3; column++)
				{
					result(column, row) = aMatrixToTranspose(row, column); // row 2 column 1 => row 1 column 2
				}
			}

			return result;
		}



	private:
		std::array<std::array<T, 3>, 3> myMatrixBuffer;
	};

	template<typename T>
	Vector3<T> operator *(const Vector3<T> aLhs, const Matrix3x3<T> aRhs)
	{

		std::array<T, 3> vectorElements = { aLhs.x, aLhs.y, aLhs.z };
		Vector3<T> result;

		for (int row = 1; row <= 3; row++)
		{
			result.x += (vectorElements[row - 1] * aRhs(row, 1));
		}


		for (int row = 1; row <= 3; row++)
		{
			result.y += (vectorElements[row - 1] * aRhs(row, 2));
		}


		for (int row = 1; row <= 3; row++)
		{
			result.z += (vectorElements[row - 1] * aRhs(row, 3));
		}


		return result;
	}
}