#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include "Object.h"

#include "Core/PollingStation.h"

namespace Dragonite
{

	class CameraInterface;
	class Scene
	{
	public:
		Scene(PollingStation& aPollingStation, std::string aPath = "");
		Scene(const Scene& aCpy);
		void operator=(const Scene& aCpy);

		void Serialize(std::string aPath);
		void Start();
		void Update(const float aDt);
		void LateUpdate(const float aDt);

		Object* CreateObject(std::string aName = "New GameObject");
		void Destroy(Object* anObject);

		CameraInterface& GetMainCamera();

	public:
		static inline std::string LastSavedPath() { return myLastSavedPath; }
		static void SetProjectSettings(void* someSettings);
		static void* GetProjectSettings();
	public:
		inline std::unordered_map<unsigned int, std::shared_ptr<Object>>& SceneObjects() { return myObjects; }

	public:
		PollingStation& myPollingStation;
		std::string myName;

	private:
		void Load(std::string aPath);
		void Save(std::string aPath);




	private:
		static std::string myLastSavedPath;

		unsigned int myNextUUID;
		std::unordered_map<unsigned int, std::shared_ptr<Object>> myObjects;


	};
}