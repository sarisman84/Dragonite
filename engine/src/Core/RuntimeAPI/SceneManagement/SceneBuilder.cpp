//#include "SceneBuilder.h"
//#include "Core/External/nlohmann/json.hpp"
//
//#include "Core/RuntimeAPI/Object.h"
//#include "Core/RuntimeAPI/Scene.h"
//
//#include <fstream>
//#include <string>
//#include <cassert>
//
//
//
//Dragonite::SceneBuilder Dragonite::SceneBuilder::myInstance;
//
//Dragonite::SceneBuilder::SceneBuilder()
//{
//	InitializeImportSettings(myImportSettings);
//	InitializeExportSettings(myExportSettings);
//}
//
//const bool Dragonite::SceneBuilder::LoadScene(const char* aPath, Dragonite::Scene& anOutput)
//{
//	nlohmann::json file = nlohmann::json::object();
//	if (!myInstance.LoadAsset(aPath, file)) return false;
//
//	anOutput.Name() = file[sceneName];
//	auto objects = file[sceneObjects];
//
//	anOutput.Stop(nullptr);
//	auto cpy = anOutput;
//	anOutput = Scene();
//	anOutput.myCurrentState = false;
//
//	auto& pool = anOutput.SceneObjects();
//	anOutput.myInputManager = cpy.myInputManager;
//	anOutput.myPollingStation = cpy.myPollingStation;
//	anOutput.myApplication = cpy.myApplication;
//	anOutput.myRenderInterface = cpy.myRenderInterface;
//
//	for (auto& foundObject : objects)
//	{
//		unsigned int id = foundObject[objectID].get<unsigned int>();
//		Object object = Object(foundObject[objectName].get<std::string>().c_str(), id);
//		object.myPollingStation = anOutput.myPollingStation;
//		anOutput.myNextUUID = ++id;
//
//		myInstance.AssignTransform(object, foundObject[objectTransform]);
//		myInstance.AssignComponents(anOutput, object, foundObject[objectComponents]);
//		pool.push_back(object);
//	}
//
//
//	return true;
//}
//
//const bool Dragonite::SceneBuilder::SaveScene(const char* aPath, Scene& anInput)
//{
//	static nlohmann::json projectSettings = GetProjectSettings();
//
//
//	nlohmann::json newFile;
//	newFile[sceneName] = anInput.Name();
//
//	newFile[sceneObjects] = nlohmann::json::array();
//	for (auto& object : anInput.SceneObjects())
//	{
//		nlohmann::json newObject = nlohmann::json::object();
//		newObject[objectTransform] = nlohmann::json::object();
//		newObject[objectComponents] = nlohmann::json::array();
//
//		newObject[objectName] = object.Name();
//		newObject[objectID] = object.UUID();
//		myInstance.ReadTransform(object, newObject[objectTransform]);
//		myInstance.ReadComponents(anInput, object, newObject[objectComponents]);
//
//		newFile[sceneObjects].push_back(newObject);
//	}
//
//	std::ofstream ofs(aPath);
//
//	ofs << newFile;
//
//	projectSettings[mainScene] = aPath;
//
//	SetProjectSettings(projectSettings);
//
//	return true;
//}
//
//nlohmann::json Dragonite::SceneBuilder::GetProjectSettings()
//{
//	std::ifstream fs("projectSettings.json");
//	if (fs.peek() == std::fstream::traits_type::eof())
//	{
//		fs.close();
//		std::ofstream ofs("projectSettings.json");
//		ofs << "{}";
//		ofs.close();
//		return GetProjectSettings();
//	}
//
//	nlohmann::json json = nlohmann::json::parse(fs);
//	return json;
//}
//
//void Dragonite::SceneBuilder::SetProjectSettings(nlohmann::json& someSettings)
//{
//	std::ofstream ofs("projectSettings.json");
//	ofs << someSettings;
//	ofs.close();
//}
//
//void Dragonite::SceneBuilder::AssignTransform(Dragonite::Object& anObject, nlohmann::json anJsonIns)
//{
//	auto& transform = anObject.GetTransform();
//	transform.myPosition.x = anJsonIns["position"]["x"];
//	transform.myPosition.y = anJsonIns["position"]["y"];
//	transform.myPosition.z = anJsonIns["position"]["z"];
//
//
//	transform.myRotation.x = anJsonIns["rotation"]["x"];
//	transform.myRotation.y = anJsonIns["rotation"]["y"];
//	transform.myRotation.z = anJsonIns["rotation"]["z"];
//
//
//	transform.myScale.x = anJsonIns["scale"]["x"];
//	transform.myScale.y = anJsonIns["scale"]["y"];
//	transform.myScale.z = anJsonIns["scale"]["z"];
//}
//
//void Dragonite::SceneBuilder::AssignComponents(Dragonite::Scene& aScene, Dragonite::Object& anObject, nlohmann::json anJsonIns)
//{
//	for (auto& component : anJsonIns)
//	{
//		ImportSetting callback = myImportSettings[component[componentType]];
//
//		if (callback)
//		{
//			BuildData data{ aScene, anObject };
//			callback(component[componentData], data);
//		}
//
//	}
//}
//
//void Dragonite::SceneBuilder::ReadTransform(Dragonite::Object& anObject, nlohmann::json& anJsonIns)
//{
//	auto& transform = anObject.GetTransform();
//	anJsonIns["position"]["x"] = transform.myPosition.x;
//	anJsonIns["position"]["y"] = transform.myPosition.y;
//	anJsonIns["position"]["z"] = transform.myPosition.z;
//
//	anJsonIns["rotation"]["x"] = transform.myRotation.x;
//	anJsonIns["rotation"]["y"] = transform.myRotation.y;
//	anJsonIns["rotation"]["z"] = transform.myRotation.z;
//
//	anJsonIns["scale"]["x"] = transform.myScale.x;
//	anJsonIns["scale"]["y"] = transform.myScale.y;
//	anJsonIns["scale"]["z"] = transform.myScale.z;
//}
//
//void Dragonite::SceneBuilder::ReadComponents(Dragonite::Scene& aScene, Dragonite::Object& anObject, nlohmann::json& anJsonIns)
//{
//	for (auto& component : anObject.Components())
//	{
//		nlohmann::json newComp = nlohmann::json::object();
//
//		auto name = component->GetName(); //Cpy the component's name
//
//		std::transform(name.begin(), name.end(), name.begin(),
//			[](unsigned char c) { return std::tolower(c); }); //force all characters in name to be lower case
//
//		name.erase(std::remove(name.begin(), name.end(), ' '), name.end()); //remove any spaces in name
//
//		newComp[componentType] = name.c_str();
//		newComp[componentData] = nlohmann::json::object();
//		ExportSetting callback = myExportSettings[newComp[componentType]];
//		if (callback)
//		{
//			BuildData data{ aScene, anObject };
//			callback(newComp[componentData], data);
//		}
//
//		anJsonIns.push_back(newComp);
//	}
//}
//
//const bool Dragonite::SceneBuilder::LoadAsset(const char* aPath, nlohmann::json& anOutput)
//{
//	std::ifstream ifs(aPath);
//
//
//	if (!ifs || ifs.fail())
//	{
//		return false;
//	}
//
//	anOutput = nlohmann::json::parse(ifs);
//
//
//	if (anOutput.empty())
//		return false;
//
//	return true;
//}
//
//const bool Dragonite::SceneBuilder::ForceLoadAsset(const char* aPath, nlohmann::json& anOutput)
//{
//	if (!LoadAsset(aPath, anOutput))
//	{
//		std::ofstream ofs(aPath);
//		ofs << "{}";
//		ofs.close();
//		return true;
//	}
//
//	return true;
//}
