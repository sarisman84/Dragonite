#include "Transform.h"
#include <iostream>









Transform::Transform()
{

}

Matrix4x4f& Transform::GetMatrix()
{
	//return myObjectMatrix = m;
	return myLocalToWorldMatrix;
}

void Transform::SetRotation(Math::Vector3f aRotation, const bool aRotateLocally)
{
	Vector4f pos;
	if (aRotateLocally)
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

	if (aRotateLocally)
	{
		myLocalToWorldMatrix.SetRow(4, pos);
	}

	myRotation = aRotation;
}

void Transform::SetRotationX(const float anXVal, const bool aRotateLocally)
{
	Vector4f pos;
	if (aRotateLocally)
	{
		pos = myLocalToWorldMatrix.GetRow(4);
		myLocalToWorldMatrix.SetRow(4, { 0,0,0,1 });
	}

	float pi = 3.141592653589793238f;

	Matrix4x4f rotationMatrix;
	rotationMatrix = Matrix4x4f::CreateRotationAroundX((anXVal * pi) / 180.f) * rotationMatrix;

	myLocalToWorldMatrix = rotationMatrix * myLocalToWorldMatrix;

	if (aRotateLocally)
	{
		myLocalToWorldMatrix.SetRow(4, pos);
	}

	myRotation.x = anXVal;
}

void Transform::SetRotationY(const float anYVal, const bool aRotateLocally)
{
	Vector4f pos;
	if (aRotateLocally)
	{
		pos = myLocalToWorldMatrix.GetRow(4);
		myLocalToWorldMatrix.SetRow(4, { 0,0,0,1 });
	}

	float pi = 3.141592653589793238f;

	Matrix4x4f rotationMatrix;
	rotationMatrix = Matrix4x4f::CreateRotationAroundY((anYVal * pi) / 180.f) * rotationMatrix;

	myLocalToWorldMatrix = rotationMatrix * myLocalToWorldMatrix;

	if (aRotateLocally)
	{
		myLocalToWorldMatrix.SetRow(4, pos);
	}

	myRotation.y = anYVal;
}

void Transform::SetRotationZ(const float anZVal, const bool aRotateLocally)
{
	Vector4f pos;
	if (aRotateLocally)
	{
		pos = myLocalToWorldMatrix.GetRow(4);
		myLocalToWorldMatrix.SetRow(4, { 0,0,0,1 });
	}

	float pi = 3.141592653589793238f;

	Matrix4x4f rotationMatrix;
	rotationMatrix = Matrix4x4f::CreateRotationAroundZ((anZVal * pi) / 180.f) * rotationMatrix;

	myLocalToWorldMatrix = rotationMatrix * myLocalToWorldMatrix;

	if (aRotateLocally)
	{
		myLocalToWorldMatrix.SetRow(4, pos);
	}

	myRotation.y = anZVal;
}

Math::Vector3f Transform::GetRotation()
{
	return myRotation;
}

void Transform::SetPosition(Math::Vector3f aPosition)
{
	myLocalToWorldMatrix.SetRow(4, { aPosition, 1 });
}

Math::Vector3f Transform::GetPosition()
{
	return { myLocalToWorldMatrix(4,1),myLocalToWorldMatrix(4,2), myLocalToWorldMatrix(4,3) };
}

void Transform::SetSize(Math::Vector3f aSize)
{
	myLocalToWorldMatrix(1, 1) = aSize.x;
	myLocalToWorldMatrix(2, 2) = aSize.y;
	myLocalToWorldMatrix(3, 3) = aSize.z;
}

Math::Vector3f Transform::GetSize()
{
	return { myLocalToWorldMatrix(1,1), myLocalToWorldMatrix(2,2), myLocalToWorldMatrix(3,3) };
}

Math::Vector3f Transform::GetForward()
{
	return { myLocalToWorldMatrix(3,1),myLocalToWorldMatrix(3,2), myLocalToWorldMatrix(3,3) };
}

Math::Vector3f Transform::GetUp()
{
	return { myLocalToWorldMatrix(2,1),myLocalToWorldMatrix(2,2), myLocalToWorldMatrix(2,3) };
}

Math::Vector3f Transform::GetRight()
{
	return { myLocalToWorldMatrix(1,1),myLocalToWorldMatrix(1,2), myLocalToWorldMatrix(1,3) };
}


