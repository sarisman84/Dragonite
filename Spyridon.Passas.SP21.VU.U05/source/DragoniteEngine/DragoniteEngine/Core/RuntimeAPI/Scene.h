#pragma once
#include "Core/Graphics/Camera.h"
#include "Object.h"
#include <vector>


namespace Dragonite
{
	class PollingStation;
	class ModelInstance;
	class Runtime;
	class GraphicalInterface;
	class InputManager;

	class Scene
	{
		friend Runtime;
	public:
		void Awake();
		void Update(const float aDt);
		inline std::vector<Object>& SceneObjects() noexcept { return myObjects; }
		inline Camera& GetCamera() { return myCamera; }
	private:
		PollingStation* myPollingStation;
		Runtime* myApplication;
		GraphicalInterface* myRenderInterface;
		InputManager* myInputManager;

		Camera myCamera;
		std::vector<Object> myObjects;
	};
}


