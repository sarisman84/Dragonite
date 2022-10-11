#pragma once
#include "Core/Graphics/CameraInterface.h"
#include "Object.h"
#include <vector>
#include <string>


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
		void OnSceneInit();
		void Update(const float aDt);
		inline std::vector<Object>& SceneObjects() noexcept { return myObjects; }

		inline std::string& Name() { return myName; }

		inline CameraInterface*& GetCamera() { return myViewportInterface; }

		inline const bool IsCameraPrimary(const unsigned int anID) { return myPrimaryCameraID == anID; }
		inline void SetPrimaryCameraID(const unsigned int anID) { myPrimaryCameraID = anID; }
	private:
		PollingStation* myPollingStation;
		Runtime* myApplication;
		GraphicalInterface* myRenderInterface;
		InputManager* myInputManager;
		CameraInterface* myViewportInterface;
		std::vector<Object> myObjects;
		std::string myName;

		unsigned int myPrimaryCameraID;
	};
}


