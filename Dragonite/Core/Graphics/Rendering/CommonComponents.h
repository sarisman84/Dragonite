#pragma once
#include "Utilities/Math/Matrix.h"
#include "Utilities/Math/Vector.h"

struct Transform
{
	Math::Vector3f myPosition;
	Math::Vector4f myRotation;
	Math::Vector3f mySize;
};