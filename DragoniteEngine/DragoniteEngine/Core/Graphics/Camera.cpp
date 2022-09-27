#include "Camera.h"

Dragonite::Matrix4x4f Dragonite::PerspectiveProfile::CalculateViewMatrix()
{
	Matrix4x4f result;
	result(1, 1) = 1 / (tanf(myFOV / 2.0f));
	result(2, 2) = (1920.0f / 1080.0f) * (1 / (tanf(myFOV / 2.0f))); //TODO: Get access to resolution information
	result(3, 3) = myFarPlane / (myFarPlane - myNearPlane);
	result(3, 4) = -(myNearPlane * myFarPlane) / (myFarPlane - myNearPlane);
	return result;
}

Dragonite::Camera::Camera() = default;

Dragonite::Camera::~Camera()
{
	if (myProfile)
		delete myProfile;

	myProfile = nullptr;
}
