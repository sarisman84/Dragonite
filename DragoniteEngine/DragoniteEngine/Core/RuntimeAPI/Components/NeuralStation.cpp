#include "NeuralStation.h"
#include "ComputerTerminal.h"
#include "PlayerController.h"
#include "Core/RuntimeAPI/NEW/Object.h"
#include "Core/RuntimeAPI/NEW/Scene.h"
#include "Core/CU/Bounds.h"


void Dragonite::NeuralStation::PollPlayerBehaviourAgainstTerminal(ComputerTerminal* const aTerminal)
{
	if (!aTerminal || myIsPlayerHackingATerminalFlag) return;

	bool r = aTerminal->myObject->myBounds.IsIntersectingAgainst(myPlayerPtr->myObject->myBounds);
	myFocusedTerminal = aTerminal;
	myIsPlayerHackingATerminalFlag = r;
}

void Dragonite::NeuralStation::ResetInformation()
{
	myIsPlayerHackingATerminalFlag = false;
	myFocusedTerminal = nullptr;
}

const bool Dragonite::NeuralStation::IsPlayerHacking(ComputerTerminal*& anOutput)
{
	anOutput = myFocusedTerminal;
	return myIsPlayerHackingATerminalFlag;
}

void Dragonite::NeuralStation::OnComputerHack(ComputerTerminal* const aTerminal)
{
	for (auto& event : myEvents)
	{
		if (event.second)
			event.second(aTerminal);
	}


}

void Dragonite::NeuralStation::RegisterOnComputerHackEvent(unsigned int anID, std::function<void(ComputerTerminal*)> anEvent)
{
	myEvents[anID] = anEvent;
}


