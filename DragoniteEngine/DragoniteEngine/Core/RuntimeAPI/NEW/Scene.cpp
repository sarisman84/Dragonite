#include "Scene.h"
#include "Core/External/nlohmann/json.hpp"

#include <filesystem>
#include <fstream>


#include "Core/RuntimeAPI/Components/Camera.h"
#include "Core/RuntimeAPI/Components/ModelRenderer.h"
#include "Core/RuntimeAPI/Components/TestComponent.h"
#include "Core/RuntimeAPI/Components/SpriteRenderer.h"

#include "Core/Graphics/GraphicsAPI.h"
#include "Core/Graphics/CameraInterface.h"

#include "Core/Runtime.h"


std::string Dragonite::Scene::myLastSavedPath;


Dragonite::Scene::Scene(PollingStation& aPollingStation, std::string aPath) : myPollingStation(aPollingStation)
{
	if (!aPath.empty())
	{
		Load(aPath);
	}
	else
	{
		myNextUUID = 0;
		myName = "New Scene";
	}




}

Dragonite::Scene::Scene(const Scene& aCpy) : myPollingStation(aCpy.myPollingStation)
{
	myName = aCpy.myName;
	for (auto& cpy : aCpy.myObjects)
	{
		myObjects[cpy.first] = std::make_shared<Object>(*cpy.second);
	}

}

void Dragonite::Scene::operator=(const Scene& aCpy)
{
	myName = aCpy.myName;
	myPollingStation = aCpy.myPollingStation;
	for (auto& cpy : aCpy.myObjects)
	{
		myObjects[cpy.first] = std::make_shared<Object>(*cpy.second);
	}
}

void Dragonite::Scene::Serialize(std::string aPath)
{
	Save(aPath);
	myLastSavedPath = aPath;
}

void Dragonite::Scene::Start()
{
	for (size_t i = 0; i < myObjects.size(); i++)
	{
		myObjects[i]->Start();
	}

}




Dragonite::Object* Dragonite::Scene::CreateObject(std::string aName)
{

	auto obj = std::make_shared<Object>(aName);
	obj->myCurrentScene = this;
	obj->myUUID = myNextUUID++;
	myObjects[obj->myUUID] = obj;
	return obj.get();
}

void Dragonite::Scene::Destroy(Object* anObject)
{
	if (!anObject) return;
	myObjects[anObject->myUUID] = nullptr;
}

Dragonite::CameraInterface& Dragonite::Scene::GetMainCamera()
{
	return myPollingStation.Get<GraphicalInterface>()->GetActiveCamera();
}

void Dragonite::Scene::SetProjectSettings(void* someSettings)
{
	using namespace nlohmann;
	std::ofstream ofs("projectSettings.json");
	ofs << *reinterpret_cast<json*>(someSettings);
	ofs.close();
}

void* Dragonite::Scene::GetProjectSettings()
{
	std::ifstream fs("projectSettings.json");
	if (fs.peek() == std::fstream::traits_type::eof())
	{
		fs.close();
		std::ofstream ofs("projectSettings.json");
		ofs << "{}";
		ofs.close();
		return GetProjectSettings();
	}

	static nlohmann::json json;
	json = nlohmann::json::parse(fs);
	return (void*)&json;

}

void Dragonite::Scene::Load(std::string aPath)
{
	using namespace nlohmann;

	std::ifstream ifs(aPath);

	if (!ifs || ifs.fail()) return;


	json jsonIns = json::parse(ifs);

	myName = jsonIns["sceneName"];
	myNextUUID = jsonIns["GUUID"];

	myNextUUID = 0;

	for (size_t i = 0; i < jsonIns["objects"].size(); i++)
	{
		json objIns = jsonIns["objects"][i];
		auto newObj = CreateObject(objIns["name"]);
		newObj->myUUID = objIns["uuid"];

		json transIns = objIns["transform"];

		newObj->myTransform.myPosition = Vector3f(transIns["position"]["x"], transIns["position"]["y"], transIns["position"]["z"]);
		newObj->myTransform.myRotation = Vector3f(transIns["rotation"]["x"], transIns["rotation"]["y"], transIns["rotation"]["z"]);
		newObj->myTransform.myScale = Vector3f(transIns["scale"]["x"], transIns["scale"]["y"], transIns["scale"]["z"]);


		for (size_t x = 0; x < objIns["components"].size(); x++)
		{
			Component* comp = nullptr;
			json compIns = objIns["components"][x];
			if (compIns["type"] == typeid(Camera).name())
			{
				comp = dynamic_cast<Component*>((newObj->AddComponent<Camera>()).get());

			}
			if (compIns["type"] == typeid(ModelRenderer).name())
			{
				comp = (newObj->AddComponent<ModelRenderer>()).get();
			}
			if (compIns["type"] == typeid(SpriteRenderer).name())
			{
				comp = (newObj->AddComponent<SpriteRenderer>()).get();
			}
			if (compIns["type"] == typeid(TestComponent).name())
			{
				comp = (newObj->AddComponent<TestComponent>()).get();
			}

			if (comp)
				comp->Deserialize((void*)&compIns["data"]);
		}
	}


}

void Dragonite::Scene::Save(std::string aPath)
{
	using namespace nlohmann;
	json jsonIns;


	jsonIns["sceneName"] = myName;
	jsonIns["GUUID"] = myNextUUID;

	jsonIns["objects"] = json::array();
	for (size_t i = 0; i < myObjects.size(); i++)
	{
		json objIns;

		objIns["name"] = myObjects[i]->myName;
		objIns["uuid"] = myObjects[i]->myUUID;


		auto transform = myObjects[i]->myTransform;
		json transIns;

		transIns["position"] = { {"x", transform.myPosition.x}, {"y", transform.myPosition.y} , {"z", transform.myPosition.z} };
		transIns["rotation"] = { {"x", transform.myRotation.x}, {"y", transform.myRotation.y} , {"z", transform.myRotation.z} };
		transIns["scale"] = { {"x", transform.myScale.x}, {"y", transform.myScale.y} , {"z", transform.myScale.z} };

		objIns["transform"] = transIns;

		objIns["components"] = json::array();
		for (size_t x = 0; x < myObjects[i]->myComponents.size(); x++)
		{
			json comp;

			comp["type"] = typeid(*myObjects[i]->myComponents[x]).name();
			comp["data"] = *reinterpret_cast<json*>(myObjects[i]->myComponents[x]->Serialize());

			objIns["components"].push_back(comp);
		}



		jsonIns["objects"].push_back(objIns);
	}


	std::ofstream ofs(aPath);
	ofs << jsonIns;
}

void Dragonite::Scene::Update(const float aDt)
{
	for (size_t i = 0; i < myObjects.size(); i++)
	{
		myObjects[i]->Update(aDt);
	}
}

void Dragonite::Scene::LateUpdate(const float aDt)
{
	for (size_t i = 0; i < myObjects.size(); i++)
	{
		myObjects[i]->LateUpdate(aDt);
	}
}