#include "Camera.h"
#include "Utilities/Input.h"
#include <iostream>

Math::Matrix4x4f Camera::GetClipSpaceMatrix()
{
	return  Math::Matrix4x4f::GetFastInverse(myTransform->myTransformMatrix) * myProjectionMatrix;
}

void Camera::OnAwake()
{
	myFOV = 90.f;
	myNearPlane = 0.1f;
	myFarPlane = 10000.f;
	myMovementSpeed = 5.f;
	UpdateProjectionMatrix();

}

void Camera::OnUpdate(float aDeltaTime)
{
	using namespace CommonUtilities;

	float xInput = Keyboard::GetButton(Keyboard::Key::A) ? 1.f : Keyboard::GetButton(Keyboard::Key::D) ? -1.f : 0.f;
	float zInput = Keyboard::GetButton(Keyboard::Key::W) ? 1.f : Keyboard::GetButton(Keyboard::Key::S) ? -1.f : 0.f;
	float yInput = Keyboard::GetButton(Keyboard::Key::Space) ? 1.f : Keyboard::GetButton(Keyboard::Key::Control) ? -1.f : 0.f;

	//myInput = myInput.Lerp({ xInput, zInput }, aDeltaTime);

	myTransform->myTransformMatrix.SetPosition(myTransform->myTransformMatrix.GetPosition() + Math::Vector3f(xInput * myMovementSpeed, yInput, zInput * myMovementSpeed) * aDeltaTime);

	std::cout << '\r' << "[Log]<Camera>: Pos ( x: " << myTransform->myTransformMatrix.GetPosition().x << ", y:" << myTransform->myTransformMatrix.GetPosition().y << ", z:" << myTransform->myTransformMatrix.GetPosition().z << ")" << std::flush;
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
	myProjectionMatrix(3, 4) = 1.f;
	myProjectionMatrix(4, 3) = nearPlane;
}
