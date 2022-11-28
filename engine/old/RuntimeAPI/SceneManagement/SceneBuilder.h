//#pragma once
//#include "Core/RuntimeAPI/Scene.h"
//#include "ComponentStructure.h"
//
//
//#define JSONKEY inline static constexpr const char*
//
//namespace Dragonite
//{
//	JSONKEY sceneName = "sceneName";
//	JSONKEY sceneObjects = "objects";
//	JSONKEY objectComponents = "components";
//	JSONKEY componentType = "type";
//	JSONKEY componentData = "data";
//	JSONKEY objectTransform = "transform";
//	JSONKEY objectName = "name";
//	JSONKEY objectID = "id";
//
//
//	JSONKEY mainScene = "main_scene";
//
//
//	class SceneBuilder
//	{
//	public:
//		SceneBuilder();
//		static const bool LoadScene(const char* aPath, Scene& anOutput);
//		static const bool SaveScene(const char* aPath, Scene& anInput);
//		static nlohmann::json GetProjectSettings();
//		static void SetProjectSettings(nlohmann::json& someSettings);
//		
//	private:
//	
//
//
//		void AssignTransform(Dragonite::Object& anObject, nlohmann::json anJsonIns);
//		void AssignComponents(Dragonite::Scene& aScene, Dragonite::Object& anObject, nlohmann::json anJsonIns);
//		
//		void ReadTransform(Dragonite::Object& anObject, nlohmann::json& anJsonIns);
//		void ReadComponents(Dragonite::Scene& aScene, Dragonite::Object& anObject, nlohmann::json& anJsonIns);
//		
//		const bool LoadAsset(const char* aPath, nlohmann::json& anOutput);
//		const bool ForceLoadAsset(const char* aPath, nlohmann::json& anOutput);
//
//		static SceneBuilder myInstance;
//		std::unordered_map<std::string, ImportSetting> myImportSettings;
//		std::unordered_map<std::string, ExportSetting> myExportSettings;
//
//
//
//
//
//	};
//
//}
//
