#pragma once
#include "Rendering/RenderObject.h"
#include <vector>
#include "GameObject.h"
#include <memory>


using GameObjectPtr = std::shared_ptr<GameObject>;

class Scene;
namespace Engine
{
	class System;

	class Runtime
	{
		friend System;

	public:
		Runtime() = default;
		std::vector<GameObjectPtr>& GetAllGameObjects() { return myEntities; }
		inline System* GetSystem() { return mySystem; }
		GameObjectPtr& CreateGameObject();
	private:
		Runtime(System* aCoreSystem);
		std::vector<GameObjectPtr> myEntities;
		
		System* mySystem;
		void Awake();
		void Update(float aTimeDelta);
		std::shared_ptr<Scene> myScene;
	};
}


