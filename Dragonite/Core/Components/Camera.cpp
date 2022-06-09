#include "Camera.h"
#include "Utilities/Input.h"
#include <iostream>

Math::Matrix4x4f Camera::GetClipSpaceMatrix()
{
	return  Math::Matrix4x4f::GetFastInverse(myTransform->GetMatrix()) * myProjectionMatrix;
}

void Camera::OnAwake()
{
	myFOV = 90.f;
	myNearPlane = 0.1f;
	myFarPlane = 10000.f;
	myMovementSpeed = 15.f;
	UpdateProjectionMatrix();

}

void Camera::OnUpdate(float aDeltaTime)
{
	aDeltaTime;
	using namespace CommonUtilities;

	float xInput = Keyboard::GetButton(Keyboard::Key::A) ? -1.f : Keyboard::GetButton(Keyboard::Key::D) ? 1.f : 0.f;
	float zInput = Keyboard::GetButton(Keyboard::Key::W) ? 1.f : Keyboard::GetButton(Keyboard::Key::S) ? -1.f : 0.f;
	float yInput = Keyboard::GetButton(Keyboard::Key::Space) ? 1.f : Keyboard::GetButton(Keyboard::Key::Control) ? -1.f : 0.f;

	auto mouseDelta = Mouse::GetMouseDelta();
	if (Mouse::GetButton(Mouse::Key::LeftMouseButton))
		myRotation = Vector4f{ -mouseDelta.y , -mouseDelta.x, 0, 0 };

	auto right = myTransform->Right();
	auto up = myTransform->Up();
	auto forward = myTransform->Forward();
	//myInput = myInput.Lerp({ xInput, zInput }, aDeltaTime);

	float movementSpeed = Keyboard::GetButton(Keyboard::Key::LeftShift) ? myMovementSpeed * 2.f : myMovementSpeed;

	auto result = right * xInput * movementSpeed + up * yInput * movementSpeed + forward * zInput * movementSpeed;
	result;
	auto oldPos = myTransform->Position();
	myTransform->Position = myTransform->Position() + (result * aDeltaTime);
	myTransform->Rotation = myTransform->Rotation() + myRotation * aDeltaTime;




}

void Camera::UpdateProjectionMatrix()
{
	const float wh = mySystem->GetWindowsInfo().GetAspectRatio();
	myProjectionMatrix(1, 1) = 1.0f / std::tan(myFOV / 2.0f);
	myProjectionMatrix(2, 2) = wh * (1.0f / std::tan(myFOV / 2.0f));
	myProjectionMatrix(3, 3) = myFarPlane / (myFarPlane - myNearPlane);
	myProjectionMatrix(3, 4) = 1;
	myProjectionMatrix(4, 3) = -(myNearPlane * myFarPlane) / (myFarPlane - myNearPlane);
	myProjectionMatrix(4, 4) = 0;
}
