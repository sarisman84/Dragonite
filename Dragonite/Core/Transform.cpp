#include "Transform.h"


void Transform::SetPosition(Vector3f aPosition)
{
	myTransformMatrix.SetRow(4, { aPosition.x, aPosition.y, aPosition.z, 1.f });
}

Vector3f Transform::GetPosition()
{
	auto result = myTransformMatrix.GetRow(4);
	return { result.x, result.y, result.z };
}

void Transform::SetRotation(Vector4f aRotation)
{
	float pi = 3.141592653589793238f;

	auto rotMatrix = Matrix4x4f::CreateRotationAroundX(aRotation.x * pi / 180.f);
	rotMatrix = Matrix4x4f::CreateRotationAroundY(aRotation.y * pi / 180.f) * rotMatrix;
	rotMatrix = Matrix4x4f::CreateRotationAroundZ(aRotation.z * pi / 180.f) * rotMatrix;

	myTransformMatrix = rotMatrix * myTransformMatrix;
	myRotation = aRotation;
}

Vector4f Transform::GetRotation()
{
	return myRotation;
}

void Transform::SetSize(Vector3f aSize)
{
	myTransformMatrix(1, 1) = aSize.x;
	myTransformMatrix(2, 2) = aSize.y;
	myTransformMatrix(3, 3) = aSize.z;
}

Vector3f Transform::GetSize()
{
	return { myTransformMatrix(1, 1) ,  myTransformMatrix(2, 2), myTransformMatrix(3, 3) };
}

Vector3f Transform::GetUp()
{
	return {myTransformMatrix(2,1), myTransformMatrix(2,2), myTransformMatrix(2,3) };
}

Vector3f Transform::GetRight()
{
	return { myTransformMatrix(1,1), myTransformMatrix(1,2), myTransformMatrix(1,3) };
}

Vector3f Transform::GetForward()
{
	return { myTransformMatrix(3,1), myTransformMatrix(3,2), myTransformMatrix(3,3) };;
}
