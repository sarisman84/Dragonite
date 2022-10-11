#pragma once
#include "Core/RuntimeAPI/Scene.h"
#include "ComponentStructure.h"


namespace Dragonite
{
	inline static const char* sceneName = "sceneName";
	inline static const char* sceneObjects = "objects";
	inline static const char* objectComponents = "components";
	inline static const char* componentType = "type";
	inline static const char* componentData = "data";
	inline static const char* objectTransform = "transform";
	inline static const char* objectName = "name";



	class SceneBuilder
	{
	public:
		SceneBuilder();
		static const bool LoadScene(const char* aPath, Scene& anOutput);
		static const bool SaveScene(const char* aPath, Scene& anInput);
	private:
		void AssignTransform(Dragonite::Object& anObject, nlohmann::json anJsonIns);
		void AssignComponents(Dragonite::Scene& aScene, Dragonite::Object& anObject, nlohmann::json anJsonIns);
		
		void ReadTransform(Dragonite::Object& anObject, nlohmann::json& anJsonIns);
		void ReadComponents(Dragonite::Scene& aScene, Dragonite::Object& anObject, nlohmann::json& anJsonIns);
		
		nlohmann::json LoadAsset(const char* aPath);
		nlohmann::json ForceLoadAsset(const char* aPath);

		static SceneBuilder myInstance;
		std::unordered_map<std::string, ImportSetting> myImportSettings;
		std::unordered_map<std::string, ExportSetting> myExportSettings;



	};

}

