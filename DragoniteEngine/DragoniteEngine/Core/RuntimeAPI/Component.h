#pragma once
#include "Core/PollingStation.h"
namespace Dragonite 
{
	class Component 
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		virtual void Awake() = 0;
		virtual void Update(const float aDt) = 0;
	protected:
		bool myActiveState;
		PollingStation* myPollingStation;
	};
}