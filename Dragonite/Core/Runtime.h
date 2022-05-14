#pragma once
#include "Rendering/RenderObject.h"
#include <vector>
#include "GameObject.h"
#include <memory>

class Scene;
namespace Engine
{
	class System;

	class Runtime
	{
		friend System;

	public:
		Runtime() = default;
		std::vector<GameObject>& GetAllGameObjects() { return myEntities; }
		inline System* GetSystem() { return mySystem; }
		GameObject& CreateGameObject();
	private:
		Runtime(System* aCoreSystem);
		std::vector<GameObject> myEntities;
		
		System* mySystem;
		void Awake();
		void Update(float aTimeDelta);
		std::shared_ptr<Scene> myScene;
	};
}


