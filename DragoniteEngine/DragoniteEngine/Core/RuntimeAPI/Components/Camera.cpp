#include "Camera.h"
#include "Core/RuntimeAPI/Object.h"
#include "Core/RuntimeAPI/Scene.h"

#include "Core/External/imgui/imgui.h"

Dragonite::Camera::Camera() : Component(), myPerspectiveProfile(new PerspectiveProfile(90.0f, 0.1f, 1000.0f))
{
	static unsigned int cameraID = 0;
	myCameraID = cameraID++;
}
void Dragonite::Camera::Awake()
{
	myCurrentScene = myCurrentScene ? myCurrentScene : myPollingStation->Get<Scene>();
	myPreviousCameraInterface = myCurrentScene->GetCamera();
	SetCameraAsPrimary();
}

void Dragonite::Camera::Update(const float aDt)
{
	myPerspectiveProfile->myFarPlane = myFarPlane;
	myPerspectiveProfile->myNearPlane = myNearPlane;
	myPerspectiveProfile->myFOV = myFOV;


}

void Dragonite::Camera::OnInspectorGUI()
{
	ImGui::SetNextItemWidth(50.0f);
	ImGui::InputFloat("Near Plane", &myNearPlane, 0.0f, 0.0f, "%.1f");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(50.0f);
	ImGui::InputFloat("Far Plane", &myFarPlane);
	ImGui::InputFloat("Field Of View", &myFOV);
}

std::string Dragonite::Camera::GetName()
{
	return  "Camera";
}

void Dragonite::Camera::SetCameraAsPrimary()
{
	myCurrentScene->GetCamera() = &myInterface;
}

void Dragonite::Camera::OnCreate()
{
	myInterface.GetTransform() = myObject->GetTransform();
	myInterface.Profile() = myPerspectiveProfile;
}

void Dragonite::Camera::ConstantUpdate()
{
}

void Dragonite::Camera::OnDisable()
{
	myCurrentScene->GetCamera() = myPreviousCameraInterface;
}
