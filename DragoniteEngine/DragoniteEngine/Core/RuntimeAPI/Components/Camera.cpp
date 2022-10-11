#include "Camera.h"
#include "Core/RuntimeAPI/Object.h"
#include "Core/RuntimeAPI/Scene.h"

Dragonite::Camera::Camera() : Component(), myPerspectiveProfile(PerspectiveProfile(90.0f, 0.1f, 1000.0f))
{
	static unsigned int cameraID = 0;
	myCameraID = cameraID++;

	myInterface.GetTransform() = myObject->GetTransform();
	myInterface.Profile() = &myPerspectiveProfile;

}
void Dragonite::Camera::Awake()
{

}

void Dragonite::Camera::Update(const float aDt)
{
	myPerspectiveProfile.myFarPlane = myFarPlane;
	myPerspectiveProfile.myNearPlane = myNearPlane;
	myPerspectiveProfile.myFOV = myFOV;


}

void Dragonite::Camera::OnInspectorGUI()
{
}

std::string Dragonite::Camera::GetName()
{
	return  "Camera";
}

void Dragonite::Camera::SetCameraAsPrimary()
{
	myCurrentScene = myCurrentScene ? myCurrentScene : myPollingStation->Get<Scene>();
	myCurrentScene->GetCamera() = &myInterface;
	myCurrentScene->SetPrimaryCameraID(myCameraID);
}
