#include "PlayerController.h"

#include "imgui/imgui.h"
#include "nlohmann/single_include/nlohmann/json.hpp"

#include "Core/RuntimeAPI/NEW/Object.h"
#include "Core/RuntimeAPI/NEW/Scene.h"

#include "Core/RuntimeAPI/Components/SpriteRenderer.h"

#include "Core/Utilities/Input.h"
#include "Core/CU/Math/Vector.h"


#include "NeuralStation.h"


void Dragonite::PlayerController::Awake()
{
	//myMovementSpeed = myMovementSpeed <= 0 ? 5.0f : myMovementSpeed;
}

void Dragonite::PlayerController::Start()
{
	NeuralStation::Instance().PollPlayer(this);


	SpriteRenderer* s;
	mySprite = (s = myObject->GetComponent<SpriteRenderer>().get()) ? s : myObject->AddComponent<SpriteRenderer>().get();
	myInputManager = myObject->GetScene()->myPollingStation.Get<InputManager>();
	myMousePtr = &myInputManager->GetMouse();
}

void Dragonite::PlayerController::Update(const float aDt)
{
	constexpr float stoppingDistance = 0.15f;

	if (myMousePtr)
		if (myMousePtr->GetButtonDown(MouseKey::Left))
		{
			myTargetPosition = myMousePtr->position;
		}

	Vector2f dir = (myTargetPosition - myObject->myTransform.myPosition);
	float length = dir.Length();
	Vector2f delta = dir.GetNormalized();

	if (length > stoppingDistance)
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
