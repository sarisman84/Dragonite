#include "ComputerTerminal.h"

#include "Core/RuntimeAPI/NEW/Scene.h"
#include "Core/RuntimeAPI/NEW/Object.h"
#include "Core/RuntimeAPI/Components/SpriteRenderer.h"

#include "PlayerController.h"
#include "NeuralStation.h"

#include "Core/External/imgui/imgui.h"
#include "Core/External/nlohmann/json.hpp"

void Dragonite::ComputerTerminal::Awake()
{
}

void Dragonite::ComputerTerminal::Start()
{
	SpriteRenderer* s;
	mySprite = (s = myObject->GetComponent<SpriteRenderer>().get()) ? s : myObject->AddComponent<SpriteRenderer>().get();
}

void Dragonite::ComputerTerminal::Update(const float aDt)
{
	NeuralStation::Instance().PollPlayerBehaviourAgainstTerminal(this);
	auto& objs = myObject->GetScene()->SceneObjects();
	for (size_t i = 0; i < objs.size(); i++)
	{
		auto& obj = objs[i];
		if (obj->GetComponent<PlayerController>())
		{
			if (obj->myBounds.IsIntersectingAgainst(myObject->myBounds))
			{
				NeuralStation::Instance().OnComputerHack(this); //Invoke event
			}
		}
	}
}

void Dragonite::ComputerTerminal::LateUpdate(const float aDt)
{
	/*NeuralStation::Instance().ResetInformation();*/
}

void* Dragonite::ComputerTerminal::Serialize()
{
	

    return nullptr;
}

void Dragonite::ComputerTerminal::Deserialize(void* someData)
{
}

void Dragonite::ComputerTerminal::OnInspectorGUI()
{
}

std::shared_ptr<Dragonite::Component> Dragonite::ComputerTerminal::Clone()
{
    return std::make_shared<ComputerTerminal>(*this);
}
