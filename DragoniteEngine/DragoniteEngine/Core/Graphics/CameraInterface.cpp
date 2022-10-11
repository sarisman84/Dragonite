#include "CameraInterface.h"
#include "Core/CU/Math/MathFunctions.h"


Dragonite::PerspectiveProfile::PerspectiveProfile(const float aFOV, const float aNearPlane, const float aFarPlane) : CameraProfile(), myNearPlane(aNearPlane), myFarPlane(aFarPlane), myFOV(aFOV)
{
}

Dragonite::Matrix4x4f Dragonite::PerspectiveProfile::CalculateProjectionMatrix()
{
	float fov = myFOV * Math::DegToRad;
	Matrix4x4f result;
	result(1, 1) = 1 / (tanf(fov / 2.0f));
	result(2, 2) = (1920.0f / 1080.0f) * (1 / (tanf(fov / 2.0f))); //TODO: Get access to resolution information
	result(3, 3) = myFarPlane / (myFarPlane - myNearPlane);
	result(3, 4) = 1.0f;
	result(4, 3) = -(myNearPlane * myFarPlane) / (myFarPlane - myNearPlane);
	result(4, 4) = 0;

	/*
			projectionMatrix(1, 1) = 1.f / (float)(tan(aHorizontalFovInDeg / 2.f));
			projectionMatrix(2, 2) = ((float)aRes.x / (float)aRes.y) * (float)(1.f / (tan(aHorizontalFovInDeg / 2.f)));
			projectionMatrix(3, 3) = aFarPlane / (aFarPlane - aNearPlane);
			projectionMatrix(3, 4) = 1.f;
			projectionMatrix(4, 3) = (-aNearPlane * aFarPlane) / (aFarPlane - aNearPlane);
			projectionMatrix(4, 4) = 0;

			return projectionMatrix;
	*/

	return result;
}

Dragonite::CameraInterface::CameraInterface() = default;

Dragonite::CameraInterface::~CameraInterface()
{
	if (myProfile)
		delete myProfile;

	myProfile = nullptr;
}

Dragonite::OrthographicProfile::OrthographicProfile(Vector2f& aViewPort, const float aNearPlane, const float aFarPlane) : 
	myCurrentViewPort(aViewPort), myNearPlane(aNearPlane), myFarPlane(aFarPlane) {}

Dragonite::Matrix4x4f Dragonite::OrthographicProfile::CalculateProjectionMatrix()
{
	Matrix4x4f result;
	const float n = myNearPlane;
	const float f = myFarPlane;

	const float l = 0;
	const float r = myCurrentViewPort.x;
	const float b = 0;
	const float t = myCurrentViewPort.y;

	result(1, 1) = 2 / (r - l);
	result(2, 2) = 2 / (b - t);
	result(3, 3) = 1 / (n - f);

	result(4, 1) = -(r + l) / (r - l);
	result(4, 2) = -(b + t) / (b - t);
	result(4, 3) = -(n + f) / (n - f);

	return result;
}

