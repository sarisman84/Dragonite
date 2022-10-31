#include "TestComponent.h"
#include "Core/RuntimeAPI/NEW/Object.h"

#include "imgui/imgui.h"
#include <cmath>


#include "nlohmann/single_include/nlohmann/json.hpp"

void Dragonite::TestComponent::Awake()
{
	//myScaleSpeed = 2.0f;
	//myMinScale = 0.0f;
	myCurrentTime = 0.0f;
}

void Dragonite::TestComponent::Update(const float aDt)
{
	auto& transform = myObject->myTransform;
	myCurrentTime += aDt;

	transform.myScale = Vector3f(1, 1, 1) * std::sin(myCurrentTime) * myScaleSpeed;
	transform.myScale = Vector3f::Clamp(transform.myScale, myMinScale, transform.myScale);
}

void Dragonite::TestComponent::Start()
{
}

void Dragonite::TestComponent::LateUpdate(const float aDt)
{
}

void* Dragonite::TestComponent::Serialize()
{
	using namespace nlohmann;
	static json data;

	data["scaleSpeed"] = myScaleSpeed;
	data["minScale"] = myMinScale;

	return (void*)&data;
}

void Dragonite::TestComponent::Deserialize(void* someData)
{
	using namespace nlohmann;
	json data = *(json*)someData;

	myScaleSpeed = data["scaleSpeed"];
	myMinScale = data["minScale"];

}

void Dragonite::TestComponent::OnInspectorGUI()
{
	ImGui::InputFloat("Scale Speed", &myScaleSpeed);
	ImGui::InputFloat("Min Uniform Size", &myMinScale);
}

std::shared_ptr<Dragonite::Component> Dragonite::TestComponent::Clone()
{
	return std::make_shared<Dragonite::TestComponent>(*this);
}

//void Dragonite::TestComponent::OnInspectorGUI()
//{
//	ImGui::InputFloat("Scale Speed", &myScaleSpeed);
//	ImGui::InputFloat("Min Uniform Size", &myMinScale);
//}



