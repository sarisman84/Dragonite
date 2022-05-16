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

	float xInput = Keyboard::GetButton(Keyboard::Key::A) ? -1.f : Keyboard::GetButton(Keyboard::Key::D) ? 1.f : 0.f;
	float zInput = Keyboard::GetButton(Keyboard::Key::W) ? 1.f : Keyboard::GetButton(Keyboard::Key::S) ? -1.f : 0.f;
	float yInput = Keyboard::GetButton(Keyboard::Key::Space) ? 1.f : Keyboard::GetButton(Keyboard::Key::Control) ? -1.f : 0.f;

	auto mouseDelta = Mouse::GetMouseDelta();
	if (mouseDelta.y != 0 || mouseDelta.x != 0)
		myRotation = Vector4f{ mouseDelta.y , mouseDelta.x, 0, 0 };
	else
		myRotation = { 0,0,0,0 };

	//myInput = myInput.Lerp({ xInput, zInput }, aDeltaTime);
	auto result = myTransform->GetRight() * xInput * myMovementSpeed + myTransform->GetUp() * yInput * myMovementSpeed + myTransform->GetForward() * zInput * myMovementSpeed;
	result *= aDeltaTime;
	myTransform->SetPosition(result + myTransform->GetPosition());
	myTransform->SetRotation(myRotation);

	
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
