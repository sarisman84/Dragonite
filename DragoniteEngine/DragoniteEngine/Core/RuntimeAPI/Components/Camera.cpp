#include "Camera.h"
#include "Core/RuntimeAPI/NEW/Object.h"
#include "Core/RuntimeAPI/NEW/Scene.h"

#include "Core/External/imgui/imgui.h"


#include "Core/External/nlohmann/json.hpp"
#include "Core/Graphics/GraphicsAPI.h"

Dragonite::Camera::Camera() : Component(), myPerspectiveProfile(new PerspectiveProfile(90.0f, 0.1f, 1000.0f))
{
	myNearPlane = 0.1f;
	myFarPlane = 1000.0f;
	myFOV = 90.0f;

	myPerspectiveProfile->myFarPlane = myFarPlane;
	myPerspectiveProfile->myNearPlane = myNearPlane;
	myPerspectiveProfile->myFOV = myFOV;

	static unsigned int cameraID = 0;
	myCameraID = cameraID++;
}

void Dragonite::Camera::Awake()
{
	myInterface.GetTransform() = myObject->myTransform;
	myInterface.Profile() = myPerspectiveProfile;
}

void Dragonite::Camera::Update(const float aDt)
{
	myPerspectiveProfile->myFarPlane = myFarPlane;
	myPerspectiveProfile->myNearPlane = myNearPlane;
	myPerspectiveProfile->myFOV = myFOV;


}







void Dragonite::Camera::Start()
{
	auto pol = myObject->GetScene()->myPollingStation;
	auto gi = pol.Get<GraphicalInterface>();
	gi->SetActiveCameraAs(myInterface);
}

void Dragonite::Camera::LateUpdate(const float aDt)
{
}

void* Dragonite::Camera::Serialize()
{
	using namespace nlohmann;

	static json data;

	data["farPlane"] = myFarPlane;
	data["nearPlane"] = myNearPlane;
	data["fov"] = myFOV;

	return (void*)&data;
}

void Dragonite::Camera::Deserialize(void* someData)
{
	using namespace nlohmann;
	json data = *(json*)someData;

	myFarPlane = data["farPlane"];
	myNearPlane = data["nearPlane"];
	myFOV = data["fov"];
}

void Dragonite::Camera::OnInspectorGUI()
{
	ImGui::DragFloat("Near Plane", &myNearPlane, 0.1f);
	ImGui::DragFloat("Far Plane", &myFarPlane, 0.1f);
	ImGui::DragFloat("Field Of View", &myFOV, 0.1f);
}

