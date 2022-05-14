#pragma once
#include "Utilities/Math/Vector.h"
#include "Utilities/Math/Matrix.h"

#include "Utilities/Extensions.h"

using namespace Math;
using namespace CPPExtend;
struct Transform
{

	Vector3f myPosition;
	Vector3f mySize;
	Vector4f myRotation;

	Matrix4x4f myTransformMatrix;


private:
	void UpdateMatrix();

};
