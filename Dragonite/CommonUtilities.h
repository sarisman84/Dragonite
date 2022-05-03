#pragma once
#include <iostream>
#include <random>
#include <algorithm>
#include <array>
namespace CommonUtilities
{
	namespace Math
	{
#pragma region UtilityFunctions
		//Returns the maximum value.
		template <typename T>
		T Max(const T& aMinVal, const T& aMaxVal)
		{
			return aMaxVal > aMinVal ? aMaxVal : aMinVal;
		}

		//Returns the minimum value.
		template <typename T>
		T Min(const T& aMinVal, const T& aMaxVal)
		{
			return aMaxVal > aMinVal ? aMinVal : aMaxVal;
		}

		//Returns an absolute value.
		template <typename T>
		T Abs(const T& aVal)
		{
			if (aVal >= 0)
				return aVal;


			return aVal * -1;
		}

		//Clamps a value between a minimum value and a maximum value.
		template <typename T>
		T Clamp(const T& aMinValue, const T& aMaxValue, const T& aValue)
		{
			assert(aMinValue <= aMaxValue);
			return aValue < aMinValue ? aMinValue : aValue > aMaxValue ? aMaxValue
				: aValue;
		}

		//Lerps between two values by a third percentage percentage value (between 0 and 1).
		template <typename T>
		T Lerp(const T& aLhsVal, const T& aRhsVal, const float& aPercentageVal)
		{
			//Making sure that the inputed percentage value is clamped between 0 and 1.
			float finalValue = Clamp<float>(0, 1, aPercentageVal);

			//Thanks to this post for the contribution: https://stackoverflow.com/questions/4353525/floating-point-linear-interpolation
			return (aLhsVal * (1.0f - finalValue)) + (aRhsVal * finalValue);
		}

		template <typename T>
		void Swap(T& aLhsVal, T& aRhsVal)
		{
			T temp = aLhsVal;
			aLhsVal = aRhsVal;
			aRhsVal = temp;
		}

		template <typename T>
		T RandomRange(T aLhs, T aRhs)
		{
			std::random_device dev;
			std::uniform_real_distribution<float> dist((float)aLhs, (float)aRhs);
			return (T)dist(dev);
		}
#pragma endregion
#pragma region Vector3
		template <class T>
		class Vector3
		{

		public:

			T x;
			T y;
			T z;

			//constructors
			Vector3() :
				x(T(0)),
				y(T(0)),
				z(T(0))
			{
			}
			Vector3(const T& aX, const T& aY, const T& aZ) :
				x(T(aX)),
				y(T(aY)),
				z(T(aZ))
			{
			}
			Vector3(const Vector3<T>& aVector3) = default;

			//destructor
			~Vector3() = default;

			//assignment operator 
			Vector3<T>& operator=(const Vector3<T>& aVector3) = default;

			//Vector3 Length
			T LengthSqr() const
			{
				T result = (x * x + y * y + z * z);
				return result;

			}

			T Length() const
			{
				T result = sqrt(x * x + y * y + z * z);
				return result;
			}

			//normalize
			void Normalize()
			{
				if (x == 0 && y == 0 && z == 0)
					return;

				T length = Length();
				x /= length;
				y /= length;
				z /= length;
			}

			Vector3<T> GetNormalized() const
			{
				if (x == 0 && y == 0 && z == 0)
					return Vector3<T>();
				Vector3<T> tempVector(x, y, z);
				T length = Length();
				return (tempVector / length);
			}

			//multiplacations
			T Dot(const Vector3<T>& aRHSVector3) const
			{
				T result = x * aRHSVector3.x + y * aRHSVector3.y + z * aRHSVector3.z;

				return result;
			}

			Vector3<T>
				Cross(const Vector3<T>& aRHSVector3) const
			{
				return Vector3(
					y * aRHSVector3.z - aRHSVector3.y * z,
					z * aRHSVector3.x - aRHSVector3.z * x,
					x * aRHSVector3.y - aRHSVector3.x * y
				);
			}

			//set
			void Set(const T& aX, const T& aY, const T& aZ)
			{
				x = aX;
				y = aY;
				z = aZ;
			}


			Vector3<T> Clamp(Vector3<T> aMinValue, Vector3<T> aMaxValue, Vector3<T> aCurrentValue)
			{
				int x = aMinValue.x < aCurrentValue.x ? aMinValue.x : aMaxValue.x > aCurrentValue.x ? aMaxValue.x : aCurrentValue.x;
				int y = aMinValue.y < aCurrentValue.y ? aMinValue.y : aMaxValue.y > aCurrentValue.y ? aMaxValue.y : aCurrentValue.y;
				int z = aMinValue.z < aCurrentValue.z ? aMinValue.z : aMaxValue.z > aCurrentValue.z ? aMaxValue.z : aCurrentValue.z;

				return Vector3<T>(x, y, z);
			}

		};

		template <class T>
		Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
		{
			Vector3<T> result(
				aVector0.x + aVector1.x,
				aVector0.y + aVector1.y,
				aVector0.z + aVector1.z
			);

			return result;
		}

		template <class T>
		Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
		{
			Vector3<T>result(
				aVector0.x - aVector1.x,
				aVector0.y - aVector1.y,
				aVector0.z - aVector1.z
			);

			return result;
		}

		template <class T>
		Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
		{
			Vector3<T>result(
				aVector.x * aScalar,
				aVector.y * aScalar,
				aVector.z * aScalar
			);

			return result;
		}

		template <class T>
		Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector)
		{
			Vector3<T> result = aVector * aScalar;
			return result;
		}

		template <class T>
		Vector3<T> operator/(const Vector3<T>& aVector, T& aScalar)
		{
			Vector3<T>result(
				aVector.x / aScalar,
				aVector.y / aScalar,
				aVector.z / aScalar
			);

			return result;
		}

		template <class T>
		void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
		{
			aVector0 = aVector0 + aVector1;
		}

		//Equivalent to setting aVector0 to (aVector0 - aVector1)
		template <class T>
		void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
		{
			aVector0 = aVector0 - aVector1;
		}

		//Equivalent to setting aVector to (aVector * aScalar)
		template <class T>
		void operator*=(Vector3<T>& aVector, const T& aScalar)
		{
			aVector = aVector * aScalar;
		}

		//Equivalent to setting aVector to (aVector / aScalar)
		template <class T>
		void operator/=(Vector3<T>& aVector, const T& aScalar)
		{
			aVector = aVector / aScalar;
		}


		template <class T>
		std::ostream operator<<(std::ostream& aOutputStream, const Vector3<T> aVector)
		{
			aOutputStream << "(x:" << aVector.x << ", y:" << aVector.y << ", z:" << aVector.z << ")";
			return aOutputStream;
		}
#pragma endregion
#pragma region Vector2
		template <class T>
		class Vector2
		{

		public:

			T x;
			T y;

			//constructors
			Vector2() :
				x(T(0)),
				y(T(0))
			{
			}
			Vector2(const T& aX, const T& aY) :
				x(T(aX)),
				y(T(aY))
			{
			}
			Vector2(const Vector2<T>& aVector2) = default;

			//destructor
			~Vector2() = default;

			//assignment operator 
			Vector2<T>& operator=(const Vector2<T>& aVector2) = default;

			//Vector2 Magnitude SQUARED
			T LengthSqr() const
			{
				T result = (x * x + y * y);
				return result;

			}


			//Vectors Magnitude
			float Length() const
			{
				float result = sqrt((float)(x * x + y * y));
				return result;
			}


			//normalize the current vector
			void Normalize()
			{
				if (x == 0 && y == 0)
					return;

				T length = Length();
				x /= length;
				y /= length;
			}

			//get a normalized version of the vector
			Vector2<T> GetNormalized() const
			{
				if (x == 0 && y == 0)
					return Vector2<T>();

				Vector2<T> tempVector(x, y);
				T length = Length();
				return (tempVector / length);
			}

			//multiplications
			T Dot(const Vector2<T>& aRHSVector2) const
			{
				T result = x * aRHSVector2.x + y * aRHSVector2.y;

				return result;
			}

			//set
			void Set(const T& aX, const T& aY)
			{
				x = aX;
				y = aY;
			}

			Vector2<T> Lerp(Vector2<T> aRhis, float aTValue)
			{
				x = CommonUtilities::Math::Lerp(x, aRhis.x, aTValue);
				y = CommonUtilities::Math::Lerp(y, aRhis.y, aTValue);
				return *this;
			}


			Vector2<T> Clamp(Vector2<T> aMinValue, Vector2<T> aMaxValue)
			{
				x = aMinValue.x > x ? aMinValue.x : aMaxValue.x < x ? aMaxValue.x : x;
				y = aMinValue.y > y ? aMinValue.y : aMaxValue.y < y ? aMaxValue.y : y;

				return Vector2<T>(x, y);
			}

			Vector2<T> RandomRange(Vector2<T> aMinValue, Vector2<T> aMaxValue)
			{
				std::random_device rd;
				std::uniform_real_distribution<T> d(aMinValue.x, aMaxValue.x);
				x = d(rd);

				d = std::uniform_real_distribution<T>(aMinValue.y, aMaxValue.y);
				y = d(rd);
				return Vector2<T>(x, y);
			}

			static float Distance(Vector2<T> aLhs, Vector2<T> aRhs)
			{
				return Abs((aLhs - aRhs).Length());
			}


			const bool operator==(Vector2<T> aRhs)
			{
				return x == aRhs.x && y == aRhs.y;
			}

			const bool operator!=(Vector2<T> aRhs)
			{
				return x != aRhs.x && y != aRhs.y;
			}

			template<typename OtherT>
			Vector2<OtherT> Cast()
			{
				return Vector2<OtherT>(static_cast<OtherT>(x), static_cast<OtherT>(y));
			}

		};





		template <class T>
		Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
		{
			Vector2<T> result(
				aVector0.x + aVector1.x,
				aVector0.y + aVector1.y
			);

			return result;
		}

		template <class T>
		Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
		{
			Vector2<T>result(
				aVector0.x - aVector1.x,
				aVector0.y - aVector1.y
			);

			return result;
		}

		template <class T>
		Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar)
		{
			Vector2<T>result(
				aVector.x * aScalar,
				aVector.y * aScalar
			);

			return result;
		}

		template <class T>
		Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
		{
			Vector2<T> result = aVector * aScalar;
			return result;
		}


		template <class T>
		Vector2<T> operator/(const Vector2<T>& aVector, T& aScalar)
		{
			Vector2<T>result(
				aVector.x / aScalar,
				aVector.y / aScalar
			);

			return result;
		}

		template <class T>
		void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
		{
			aVector0 = aVector0 + aVector1;
		}

		//Equivalent to setting aVector0 to (aVector0 - aVector1)
		template <class T>
		void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
		{
			aVector0 = aVector0 - aVector1;
		}

		//Equivalent to setting aVector to (aVector * aScalar)
		template <class T>
		void operator*=(Vector2<T>& aVector, const T& aScalar)
		{
			aVector = aVector * aScalar;
		}

		//Equivalent to setting aVector to (aVector / aScalar)
		template <class T>
		void operator/=(Vector2<T>& aVector, const T& aScalar)
		{
			aVector = aVector / aScalar;
		}


		template <class T>
		std::ostream& operator<<(std::ostream& aOutputStream, const Vector2<T>& aVector)
		{
			aOutputStream << "(x:" << aVector.x << ", y:" << aVector.y << ")";
			return aOutputStream;
		}
#pragma endregion
#pragma region Vector4
		template <class T>
		class Vector4
		{

		public:

			T x;
			T y;
			T z;
			T w;

			//constructors
			Vector4() :
				x(T(0)),
				y(T(0)),
				z(T(0)),
				w(T(0))
			{
			}
			Vector4(const T& aX, const T& aY, const T& aZ, const T& aW) :
				x(T(aX)),
				y(T(aY)),
				z(T(aZ)),
				w(T(aW))
			{
			}
			Vector4(const Vector4<T>& aVector4) = default;

			//destructor
			~Vector4() = default;

			//assignment operator 
			Vector4<T>& operator=(const Vector4<T>& aVector4) = default;

			//Vector4 Magnitude SQUARED
			T LengthSqr() const
			{
				T result = (x * x + y * y + z * z + w * w);
				return result;

			}

			//Vectors Magnitude
			T Length() const
			{
				T result = sqrt(x * x + y * y + z * z + w * w);
				return result;
			}

			//normalize the current vector
			void Normalize()
			{
				if (x == 0 && y == 0 && z == 0 && w == 0)
					return;

				T length = Length();
				x /= length;
				y /= length;
				z /= length;
				w /= length;
			}

			//get a normalized version of the vector
			Vector4<T> GetNormalized() const
			{
				if (x == 0 && y == 0 && z == 0 && w == 0)
					return Vector4<T>();

				Vector4<T> tempVector(x, y, z, w);
				T length = Length();
				return (tempVector / length);
			}

			//multiplacations
			T Dot(const Vector4<T>& aRHSVector4) const
			{
				T result = x * aRHSVector4.x + y * aRHSVector4.y + z * aRHSVector4.z + w * aRHSVector4.w;

				return result;
			}

			//set 
			void Set(const T& aX, const T& aY, const T& aZ, const T& aW)
			{
				x = aX;
				y = aY;
				z = aZ;
				w = aW;


			}

			Vector4<T> Clamp(Vector4<T> aMinValue, Vector4<T> aMaxValue, Vector4<T> aCurrentValue)
			{
				int x = aMinValue.x < aCurrentValue.x ? aMinValue.x : aMaxValue.x > aCurrentValue.x ? aMaxValue.x : aCurrentValue.x;
				int y = aMinValue.y < aCurrentValue.y ? aMinValue.y : aMaxValue.y > aCurrentValue.y ? aMaxValue.y : aCurrentValue.y;
				int z = aMinValue.z < aCurrentValue.z ? aMinValue.z : aMaxValue.z > aCurrentValue.z ? aMaxValue.z : aCurrentValue.z;
				int w = aMinValue.w < aCurrentValue.w ? aMinValue.w : aMaxValue.w > aCurrentValue.w ? aMaxValue.w : aCurrentValue.w;

				return Vector4<T>(x, y, z, w);
			}
		};

		template <class T>
		Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
		{
			Vector4<T> result(
				aVector0.x + aVector1.x,
				aVector0.y + aVector1.y,
				aVector0.z + aVector1.z,
				aVector0.w + aVector1.w
			);

			return result;
		}

		template <class T>
		Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
		{
			Vector4<T>result(
				aVector0.x - aVector1.x,
				aVector0.y - aVector1.y,
				aVector0.z - aVector1.z,
				aVector0.w - aVector1.w
			);

			return result;
		}

		template <class T>
		Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar)
		{
			Vector4<T>result(
				aVector.x * aScalar,
				aVector.y * aScalar,
				aVector.z * aScalar,
				aVector.w * aScalar
			);

			return result;
		}

		template <class T>
		Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector)
		{
			Vector4<T> result = aVector * aScalar;
			return result;
		}


		template <class T>
		Vector4<T> operator/(const Vector4<T>& aVector, T& aScalar)
		{
			Vector4<T>result(
				aVector.x / aScalar,
				aVector.y / aScalar,
				aVector.z / aScalar,
				aVector.w / aScalar
			);

			return result;
		}

		template <class T>
		void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
		{
			aVector0 = aVector0 + aVector1;
		}


		template <class T>
		void operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
		{
			aVector0 = aVector0 - aVector1;
		}


		template <class T>
		void operator*=(Vector4<T>& aVector, const T& aScalar)
		{
			aVector = aVector * aScalar;
		}


		template <class T>
		void operator/=(Vector4<T>& aVector, const T& aScalar)
		{
			aVector = aVector / aScalar;
		}


		template <class T>
		std::ostream operator<<(std::ostream& aOutputStream, const Vector4<T> aVector)
		{
			aOutputStream << "(x:" << aVector.x << ", y:" << aVector.y << ", z:" << aVector.z << ", w:" << aVector.w << ")";
			return aOutputStream;
		}
#pragma endregion
#pragma region Matrix4x4
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


			Matrix4x4<T> GetRotation()
			{
				Matrix4x4<T> r;

				return r;
			}

			Matrix4x4<T> GetTransform()
			{

				Matrix4x4<T> r;
				r.SetRow(4, this->GetRow(4));
				return r;
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
#pragma endregion
#pragma region Matrix3x3
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



#pragma endregion



	}
}