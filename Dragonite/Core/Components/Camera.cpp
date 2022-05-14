#include "Camera.h"


Math::Matrix4x4f Camera::GetClipSpaceMatrix()
{
	mySystem;
	myGameObject;

	return Math::Matrix4x4f::GetFastInverse(myTransform->myTransformMatrix) * myProjectionMatrix;
}

void Camera::OnAwake()
{
	UpdateProjectionMatrix();

}

void Camera::OnUpdate(float /*aDeltaTime*/)
{
}

void Camera::UpdateProjectionMatrix()
{
	float nearPlane = ((-myNearPlane) * myFarPlane) / (myFarPlane - myNearPlane);
	float farPlane = myFarPlane / (myFarPlane - myNearPlane);

	float aspectRatio = mySystem->GetWindowsInfo().GetAspectRatio();

	float xFOV = 1 / tan(myFOV / 2.f);
	float yFOV = aspectRatio * (1 / tan(myFOV / 2.f));


	myProjectionMatrix(1, 1) = xFOV;
	myProjectionMatrix(2, 2) = yFOV;
	myProjectionMatrix(3, 3) = farPlane;
	myProjectionMatrix(3, 4) = nearPlane;
}
