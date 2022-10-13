#include "TestComponent.h"
#include "Core/RuntimeAPI/Object.h"

#include "Core/External/imgui/imgui.h"
#include <cmath>

void Dragonite::TestComponent::Awake()
{
}

void Dragonite::TestComponent::Update(const float aDt)
{
	auto& transform = myObject->GetTransform();
	myCurrentTime += aDt;

	transform.myScale = Vector3f(1, 1, 1) * std::sin(myCurrentTime) * myScaleSpeed;
	transform.myScale = Vector3f::Clamp(transform.myScale, myMinScale, transform.myScale);
}

void Dragonite::TestComponent::OnInspectorGUI()
{
	ImGui::InputFloat("Scale Speed", &myScaleSpeed);
	ImGui::InputFloat("Min Uniform Size", &myMinScale);
}

void Dragonite::TestComponent::OnDisable()
{
	myCurrentTime = 0;
}

std::string Dragonite::TestComponent::GetName()
{
	return "Fancy Component";
}

void Dragonite::TestComponent::ConstantUpdate()
{
}

void Dragonite::TestComponent::OnCreate()
{
}
