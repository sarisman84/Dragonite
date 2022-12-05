#include "Camera.h"

Dragonite::Camera::Camera() : myPerspectiveProj(Perspective(90, { 1920, 1080 }))
{
}

Dragonite::Camera::~Camera()
{
}

const Dragonite::Matrix4x4f Dragonite::Camera::GetWorldToClipSpace()
{
	return myPerspectiveProj.CalculateProjection(myTransform);
}
