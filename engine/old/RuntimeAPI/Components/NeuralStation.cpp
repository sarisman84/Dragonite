#include "NeuralStation.h"
#include "ComputerTerminal.h"
#include "PlayerController.h"
#include "Core/RuntimeAPI/NEW/Object.h"
#include "Core/RuntimeAPI/NEW/Scene.h"
#include "Core/CU/Bounds.h"


void Dragonite::NeuralStation::PollPlayerBehaviourAgainstTerminal(ComputerTerminal* const aTerminal)
{
	if (!aTerminal) return;

	bool r = aTerminal->myObject->myBounds.IsIntersectingAgainst(myPlayerPtr->myObject->myBounds);
	myStates[aTerminal] = r;
}

void Dragonite::NeuralStation::ResetInformation()
{
	myIsPlayerHackingATerminalFlag = false;
}

const bool Dragonite::NeuralStation::IsPlayerHacking(ComputerTerminal*& anOutput)
{

	for (auto& state : myStates)
	{
		if (state.second)
		{
			anOutput = state.first;

			return state.second;
		}
	}

	return false;
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


