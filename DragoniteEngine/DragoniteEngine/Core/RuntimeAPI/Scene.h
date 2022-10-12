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
		friend class SceneBuilder;
		friend Runtime;
	public:
		Scene();

		void OnSceneInit();
		void Update(const float aDt);
		inline std::vector<Object>& SceneObjects() noexcept { return myObjects; }

		inline std::string& Name() { return myName; }

		inline CameraInterface*& GetCamera() { return myViewportInterface; }

		inline const bool IsCameraPrimary(const unsigned int anID) { return myPrimaryCameraID == anID; }
		inline void SetPrimaryCameraID(const unsigned int anID) { myPrimaryCameraID = anID; }

		inline const unsigned int GetNextID() noexcept
		{
			return myNextUUID++;
		}

		inline PollingStation* GetPollingStation() const noexcept { return myPollingStation; }
		inline Runtime* GetRuntime() const noexcept { return myApplication; }
		inline GraphicalInterface* GetGraphicsInterface() const noexcept { return myRenderInterface; }

	private:
		PollingStation* myPollingStation;
		Runtime* myApplication;
		GraphicalInterface* myRenderInterface;
		InputManager* myInputManager;
		CameraInterface* myViewportInterface;
		std::vector<Object> myObjects;
		std::string myName;

		unsigned int myPrimaryCameraID;
		unsigned int myNextUUID;

		std::vector<unsigned int> ourUsedUUIDs;

	};
}


