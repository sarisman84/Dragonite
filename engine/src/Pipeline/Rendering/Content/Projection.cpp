#include "Projection.h"
#include "Core/CU/Math/MathFunctions.h"
Dragonite::Perspective::Perspective(const float anFOV, const Vector2ui& aResolution, const float aNearPlane, const float aFarPlane)
{
	float fov = anFOV * Math::DegToRad;
	Matrix4x4f result;
	result(1, 1) = 1 / (tanf(fov / 2.0f));
	result(2, 2) = ((float)aResolution.x / (float)aResolution.y) * (1 / (tanf(fov / 2.0f))); //TODO: Get access to resolution information
	result(3, 3) = aFarPlane / (aFarPlane - aNearPlane);
	result(3, 4) = 1.0f;
	result(4, 3) = -(aNearPlane * aFarPlane) / (aFarPlane - aNearPlane);
	result(4, 4) = 0;

	myProjectionMatrix = result;
}

Dragonite::Matrix4x4f Dragonite::Projection::CalculateProjection(const Matrix4x4f& aTransform)
{
	return  Matrix4x4f::GetFastInverse(aTransform) * myProjectionMatrix;
}
