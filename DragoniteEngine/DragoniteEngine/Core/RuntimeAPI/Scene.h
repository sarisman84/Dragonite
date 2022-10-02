#pragma once
#include "Object.h"
#include <vector>


namespace Dragonite
{
	class PollingStation;
	class ModelInstance;
	class Runtime;
	class RenderInterface;
	class InputManager;

	class Scene
	{
		friend Runtime;
	public:
		void Awake();
		void Update(const float aDt);
		inline std::vector<Object>& SceneObjects() noexcept { return myObjects; }
	private:
		PollingStation* myPollingStation;
		Runtime* myApplication;
		RenderInterface* myRenderInterface;
		InputManager* myInputManager;


		std::vector<Object> myObjects;
	};
}


