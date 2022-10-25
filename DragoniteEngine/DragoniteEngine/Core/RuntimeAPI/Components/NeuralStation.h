#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
namespace Dragonite
{
	class PlayerController;
	class ComputerTerminal;

	class NeuralStation
	{
	public:
		static NeuralStation& Instance() 
		{
			static NeuralStation station;
			return station;
		}

#pragma region Poll against a Computer State
		inline void PollPlayer(PlayerController* const aPlayerPtr) 
		{
			myPlayerPtr = aPlayerPtr;
		}
		
		void PollPlayerBehaviourAgainstTerminal(ComputerTerminal* const aTerminal);
		void ResetInformation();

		const bool IsPlayerHacking(ComputerTerminal*& anOutput);
#pragma endregion

		

#pragma region Event System
		void OnComputerHack(ComputerTerminal* const aTerminal);

		void RegisterOnComputerHackEvent(unsigned int anID, std::function<void(ComputerTerminal*)> anEvent);
		

#pragma endregion


	private:
		bool myIsPlayerHackingATerminalFlag;
		PlayerController* myPlayerPtr = nullptr;
		ComputerTerminal* myFocusedTerminal = nullptr;
		std::unordered_map<unsigned int, std::function<void(ComputerTerminal*)>> myEvents;
		std::unordered_map<ComputerTerminal*, bool> myStates;


	};
}