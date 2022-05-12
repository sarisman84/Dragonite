#pragma once
#include "Rendering/RenderObject.h"
#include "ComponentManager.h"
#include <vector>
namespace Engine
{
	class System;

	class Runtime
	{
		friend System;
	private:
		Runtime(System* aCoreSystem);
		std::vector<EntityID> myEntities;

	protected:
		System* mySystem;
		virtual void Awake();
		virtual void Update();

	};
}


