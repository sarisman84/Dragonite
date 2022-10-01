#pragma once
#include "Graphics/Camera.h"
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
	private:
		PollingStation* myPollingStation;
		Runtime* myApplication;
		Camera myCamera;
		RenderInterface* myRenderInterface;
		InputManager* myInputManager;


		std::vector<std::shared_ptr<ModelInstance>> myModelsToRender;
	};
}


