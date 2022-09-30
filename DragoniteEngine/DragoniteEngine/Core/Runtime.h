#pragma once
#include "Graphics/Camera.h"

namespace Dragonite
{
	class RuntimeHandler;
	class ModelInstance;
	class Application;
	class RenderInterface;
	class InputManager;

	class Runtime
	{
		friend Application;
	public:
		void Awake();
		void Update(const float aDt);
	private:
		RuntimeHandler* myPollingStation;
		Application* myApplication;
		std::shared_ptr<ModelInstance> myCube;
		Camera myCamera;
		RenderInterface* myRenderInterface;
		InputManager* myInputManager;
	};
}


