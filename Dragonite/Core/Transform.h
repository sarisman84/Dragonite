#pragma once
#include "Utilities/Math/Vector.h"
#include "Utilities/Math/Matrix.h"

#include "Utilities/Extensions.h"

using namespace Math;
using namespace CPPExtend;
struct Transform
{
	Transform();
	Property<Vector3f> Position;
	Property<Vector3f> Size;
	Property<Vector4f> Rotation;

	Property<Vector3f> Up;
	Property<Vector3f> Right;
	Property<Vector3f> Forward;

	//Vector3f GetUp();
	//Vector3f GetRight();
	//Vector3f GetForward();

	Matrix4x4f GetMatrix();


	void SetRotation(Math::Vector3f aRotation, const bool aRotateGlobally = false);

private:
	Vector3f myPosition;
	Vector3f mySize;
	Vector4f myRotation;

	Matrix4x4f myPositionMatrix;
	Matrix4x4f myRotationMatrix;
	Matrix4x4f myScaleMatrix;


	Matrix4x4f myLocalToWorldMatrix;

};
