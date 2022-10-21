#include "PlayerController.h"

#include "Core/External/imgui/imgui.h"
#include "Core/External/nlohmann/json.hpp"

#include "Core/RuntimeAPI/NEW/Object.h"
#include "Core/RuntimeAPI/NEW/Scene.h"

#include "Core/RuntimeAPI/Components/SpriteRenderer.h"

#include "Core/Utilities/Input.h"
#include "Core/CU/Math/Vector.h"


void Dragonite::PlayerController::Awake()
{
	//myMovementSpeed = myMovementSpeed <= 0 ? 5.0f : myMovementSpeed;
}

void Dragonite::PlayerController::Start()
{
	SpriteRenderer* s;
	mySprite = (s = myObject->GetComponent<SpriteRenderer>().get()) ? s : myObject->AddComponent<SpriteRenderer>().get();
	myInputManager = myObject->GetScene()->myPollingStation.Get<InputManager>();
	myMousePtr = &myInputManager->GetMouse();
}

void Dragonite::PlayerController::Update(const float aDt)
{
	auto rez = Vector2f(1920, 1080);
	constexpr float stoppingDistance = 0.15f;

	if (myMousePtr)
		if (myMousePtr->GetButtonDown(MouseKey::Left))
		{
			myTargetPosition = myMousePtr->position;
		}

	Vector2f dir = (myTargetPosition - myObject->myTransform.myPosition);
	float length = dir.Length();
	Vector2f delta = dir.GetNormalized();

	if (stoppingDistance >= 0.1f)
		myObject->myTransform.myPosition += ToVector3(delta * myMovementSpeed * aDt);

}

void Dragonite::PlayerController::LateUpdate(const float aDt)
{
}

void* Dragonite::PlayerController::Serialize()
{
	using namespace nlohmann;

	static json data;

	data["speed"] = myMovementSpeed;

	return (void*)&data;
}

void Dragonite::PlayerController::Deserialize(void* someData)
{
	using namespace nlohmann;

	json data = *(json*)someData;


	myMovementSpeed = data["speed"];
}

void Dragonite::PlayerController::OnInspectorGUI()
{
	ImGui::DragFloat("Speed", &myMovementSpeed, 0.1f);
}

std::shared_ptr<Dragonite::Component> Dragonite::PlayerController::Clone()
{
	return std::make_shared<PlayerController>(*this);
}
