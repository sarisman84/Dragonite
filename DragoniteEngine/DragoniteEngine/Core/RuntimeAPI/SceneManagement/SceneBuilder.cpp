#include "SceneBuilder.h"
#include "Core/External/nlohmann/json.hpp"

#include "Core/RuntimeAPI/Object.h"
#include "Core/RuntimeAPI/Scene.h"

#include <fstream>
#include <string>



Dragonite::SceneBuilder Dragonite::SceneBuilder::myInstance;

Dragonite::SceneBuilder::SceneBuilder()
{
	InitializeImportSettings(myImportSettings);
}

const bool Dragonite::SceneBuilder::LoadScene(const char* aPath, Dragonite::Scene& anOutput)
{
	auto file = myInstance.LoadAsset(aPath);


	if (file.is_null()) false;

	anOutput.Name() = file[sceneName];
	auto objects = file[sceneObjects];

	anOutput = Scene();
	auto& pool = anOutput.SceneObjects();

	for (auto& foundObject : objects)
	{
		Object object = Object(foundObject[objectName].get<std::string>().c_str());
		myInstance.AssignTransform(object, foundObject[objectTransform]);
		myInstance.AssignComponents(anOutput, object, foundObject[objectComponents]);
		pool.push_back(object);
	}


	return true;
}

const bool Dragonite::SceneBuilder::SaveScene(const char* aPath, Scene& anInput)
{
	auto newFile = myInstance.ForceLoadAsset(aPath);


	newFile[sceneName] = anInput.Name();

	newFile[sceneObjects] = nlohmann::json::array();
	for (auto& object : anInput.SceneObjects())
	{
		nlohmann::json newObject = nlohmann::json::object();
		newObject[objectTransform] = nlohmann::json::object();
		newObject[objectComponents] = nlohmann::json::array();

		newObject[objectName] = object.Name();
		myInstance.ReadTransform(object, newObject[objectTransform]);
		myInstance.ReadComponents(anInput, object, newObject[objectComponents]);

		newFile[sceneObjects].push_back(newObject);
	}

	std::ofstream ofs(aPath);

	ofs << newFile;

	return true;
}

void Dragonite::SceneBuilder::AssignTransform(Dragonite::Object& anObject, nlohmann::json anJsonIns)
{
	auto& transform = anObject.GetTransform();
	transform.myPosition.x = anJsonIns["position"]["x"];
	transform.myPosition.y = anJsonIns["position"]["y"];
	transform.myPosition.z = anJsonIns["position"]["z"];


	transform.myRotation.x = anJsonIns["rotation"]["x"];
	transform.myRotation.y = anJsonIns["rotation"]["y"];
	transform.myRotation.z = anJsonIns["rotation"]["z"];


	transform.myScale.x = anJsonIns["scale"]["x"];
	transform.myScale.y = anJsonIns["scale"]["y"];
	transform.myScale.z = anJsonIns["scale"]["z"];
}

void Dragonite::SceneBuilder::AssignComponents(Dragonite::Scene& aScene, Dragonite::Object& anObject, nlohmann::json anJsonIns)
{
	for (auto& component : anJsonIns)
	{
		ImportSetting callback = myImportSettings[component[componentType]];

		BuildData data{ aScene };
		callback(component[componentData], data);
	}
}

void Dragonite::SceneBuilder::ReadTransform(Dragonite::Object& anObject, nlohmann::json& anJsonIns)
{
	auto& transform = anObject.GetTransform();
	anJsonIns["position"]["x"] = transform.myPosition.x;
	anJsonIns["position"]["y"] = transform.myPosition.y;
	anJsonIns["position"]["z"] = transform.myPosition.z;

	anJsonIns["rotation"]["x"] = transform.myRotation.x;
	anJsonIns["rotation"]["y"] = transform.myRotation.y;
	anJsonIns["rotation"]["z"] = transform.myRotation.z;

	anJsonIns["scale"]["x"] = transform.myScale.x;
	anJsonIns["scale"]["y"] = transform.myScale.y;
	anJsonIns["scale"]["z"] = transform.myScale.z;
}

void Dragonite::SceneBuilder::ReadComponents(Dragonite::Scene& aScene, Dragonite::Object& anObject, nlohmann::json& anJsonIns)
{
	for (auto& component : anObject.Components())
	{
		nlohmann::json newComp = nlohmann::json::object();

		auto name = component->GetName(); //Cpy the component's name

		std::transform(name.begin(), name.end(), name.begin(),
			[](unsigned char c) { return std::tolower(c); }); //force all characters in name to be lower case

		name.erase(std::remove(name.begin(), name.end(), ' '), name.end()); //remove any spaces in name

		newComp[componentType] = name.c_str();
		newComp[componentData] = nlohmann::json::object();
		ExportSetting callback = myExportSettings[component->GetName()];
		if (callback)
		{
			BuildData data{ aScene };
			callback(newComp[componentData], data);
		}
		
		anJsonIns.push_back(newComp);
	}
}

nlohmann::json Dragonite::SceneBuilder::LoadAsset(const char* aPath)
{
	std::ifstream ifs(aPath);

	nlohmann::json jsonIns;
	if (!ifs || ifs.fail())
	{
		return jsonIns;
	}

	jsonIns.parse(ifs);
	return jsonIns;
}

nlohmann::json Dragonite::SceneBuilder::ForceLoadAsset(const char* aPath)
{
	auto result = LoadAsset(aPath);

	if (result.is_null())
	{
		std::ofstream ofs(aPath);
		ofs << "{}";
		ofs.close();
		return LoadAsset(aPath);
	}

	return result;
}
