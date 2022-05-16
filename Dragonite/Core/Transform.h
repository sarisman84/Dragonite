#pragma once
#include "Utilities/Math/Vector.h"
#include "Utilities/Math/Matrix.h"

#include "Utilities/Extensions.h"

using namespace Math;
using namespace CPPExtend;
struct Transform
{
	Matrix4x4f myTransformMatrix;

	void SetPosition(Vector3f aPosition);
	Vector3f GetPosition();

	void SetRotation(Vector4f aRotation);
	Vector4f GetRotation();

	void SetSize(Vector3f aSize);
	Vector3f GetSize();


	Vector3f GetUp();
	Vector3f GetRight();
	Vector3f GetForward();

private:
	Vector4f myRotation;

};
