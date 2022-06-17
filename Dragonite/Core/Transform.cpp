#include "Transform.h"
#include <iostream>









Transform::Transform()
{
	Position = Property<Vector3f>([this]()->Vector3f { return myPosition; }, [this](const Vector3f aValue)
		{
			myPosition = aValue;
			myPositionMatrix = Matrix4x4f::CreateTransformMatrix(aValue);


		});

	Rotation = Property<Vector4f>([this]()->Vector4f { return myRotation; }, [this](const Vector4f aValue)
		{

			float pi = 3.141592653589793238f;
			Matrix4x4f xRot = Matrix4x4f::CreateRotationAroundX((aValue.x * pi) / 180.f);
			Matrix4x4f yRot = Matrix4x4f::CreateRotationAroundY((aValue.y * pi) / 180.f);
			Matrix4x4f zRot = Matrix4x4f::CreateRotationAroundZ((aValue.z * pi) / 180.f);


			myRotation = aValue;
			myRotationMatrix = xRot * yRot * zRot;

		});

	Size = Property<Vector3f>([this]()->Vector3f { return mySize; }, [this](const Vector3f aValue)
		{
			mySize = aValue;
			myScaleMatrix = Matrix4x4f::CreateSizeMatrix(aValue);

		});


	Up = Property<Vector3f>([this]()->Vector3f
		{
			auto transformMatrix = GetMatrix();
			Vector3f result = { transformMatrix(2,1), transformMatrix(2,2), transformMatrix(2,3) };
			return result;
		});


	Right = Property<Vector3f>([this]()->Vector3f
		{
			auto transformMatrix = GetMatrix();
			Vector3f result = { transformMatrix(1,1), transformMatrix(1,2), transformMatrix(1,3) };
			return result;
		});


	Forward = Property<Vector3f>([this]()->Vector3f
		{
			auto transformMatrix = GetMatrix();
			Vector3f result = { transformMatrix(3,1), transformMatrix(3,2), transformMatrix(3,3) };
			return result;
		});
}

Matrix4x4f Transform::GetMatrix()
{

	//return myObjectMatrix = m;
	return  myScaleMatrix * myRotationMatrix * myPositionMatrix;
}

void Transform::SetRotation(Math::Vector3f aRotation, const bool aRotateGlobally)
{
	Vector4f pos;
	if (aRotateGlobally)
	{
		pos = myLocalToWorldMatrix.GetRow(4);
		myLocalToWorldMatrix.SetRow(4, { 0,0,0,1 });
	}

	float pi = 3.141592653589793238f;

	Matrix4x4f rotationMatrix;
	rotationMatrix = Matrix4x4f::CreateRotationAroundX((aRotation.x * pi) / 180.f) * rotationMatrix;
	rotationMatrix = Matrix4x4f::CreateRotationAroundY((aRotation.y * pi) / 180.f) * rotationMatrix;
	rotationMatrix = Matrix4x4f::CreateRotationAroundZ((aRotation.z * pi) / 180.f) * rotationMatrix;

	myLocalToWorldMatrix = rotationMatrix * myLocalToWorldMatrix;

	if (aRotateGlobally)
	{
		myLocalToWorldMatrix.SetRow(4, pos);
	}
}


