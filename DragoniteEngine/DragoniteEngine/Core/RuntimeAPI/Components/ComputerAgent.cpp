#include "ComputerAgent.h"

#include "NeuralStation.h"

#include "PlayerController.h"
#include "ComputerTerminal.h"
#include <typeinfo>

#include "Core/RuntimeAPI/NEW/Object.h"

#include "Core/External/imgui/imgui.h"
#include "Core/External/nlohmann/json.hpp"

#include "Core/RuntimeAPI/Components/SpriteRenderer.h"

void Dragonite::ComputerAgent::Awake()
{

}

void Dragonite::ComputerAgent::Start()
{
	//Begin in a neutral state

	myHasReachedDestination = true;

	//Make sure that we have a sprite renderer
	SpriteRenderer* s;
	mySprite = (s = myObject->GetComponent<SpriteRenderer>().get()) ? s : myObject->AddComponent<SpriteRenderer>().get();



	if (myPollInformationFlag) return;

	//If we are not polling for information, register an event instead
	NeuralStation::Instance().RegisterOnComputerHackEvent(myObject->UUID(), [this](ComputerTerminal* aCompPtr)
		{
			myTargetPosition = ToVector2(aCompPtr->myObject->myTransform.myPosition);
			myHasReachedDestination = false;
		});

}

void Dragonite::ComputerAgent::Update(const float aDt)
{
	if (myPollInformationFlag)
	{
		ComputerTerminal* terminal = nullptr;
		if (NeuralStation::Instance().IsPlayerHacking(terminal) && terminal)
		{
			myTargetPosition = ToVector2(terminal->myObject->myTransform.myPosition);
			myHasReachedDestination = false;
		}
		else
		{
			myHasReachedDestination = true;
		}

	}


	if (!myHasReachedDestination)
	{
		Vector2f dir = (myTargetPosition - ToVector2(myObject->myTransform.myPosition));

		myObject->myTransform.myPosition += ToVector3(dir.GetNormalized() * myMovementSpeed * aDt);


		if (dir.Length() <= 0.1f)
		{
			myHasReachedDestination = true;

		}
	}
}

void Dragonite::ComputerAgent::LateUpdate(const float aDt)
{

}

void* Dragonite::ComputerAgent::Serialize()
{
	using namespace nlohmann;

	static json data;


	data["movementSpeed"] = myMovementSpeed;
	data["pollInformation"] = myPollInformationFlag;

	return (void*)&data;
}

void Dragonite::ComputerAgent::Deserialize(void* someData)
{
	using namespace nlohmann;

	json data = *(json*)someData;

	myMovementSpeed = data["movementSpeed"];
	myPollInformationFlag = data["pollInformation"];
}

void Dragonite::ComputerAgent::OnInspectorGUI()
{
	ImGui::Checkbox("Enable Polling", &myPollInformationFlag);
	ImGui::Text("Communication Method: %s", (myPollInformationFlag ? "Polling" : "Event Detection"));
	ImGui::DragFloat("Movement Speed", &myMovementSpeed, 0.1f);
}

std::shared_ptr<Dragonite::Component> Dragonite::ComputerAgent::Clone()
{
	return std::make_shared<ComputerAgent>(*this);
}
