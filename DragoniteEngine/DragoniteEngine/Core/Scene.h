#pragma once
#include "Graphics/Camera.h"

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
	private:
		PollingStation* myPollingStation;
		Runtime* myApplication;
		std::shared_ptr<ModelInstance> myCube;
		Camera myCamera;
		RenderInterface* myRenderInterface;
		InputManager* myInputManager;
	};
}


