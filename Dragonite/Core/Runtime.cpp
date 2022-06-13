#include "Runtime.h"
#include  "System.h"
#include "Utilities/UtilityFunctions.h"
#include <iostream>
#include "Scene.h"


Dragonite::Runtime::Runtime(System* aCoreSystem)
{
	mySystem = aCoreSystem;

	mySystem->AddManager<Runtime>();


	myScene = std::make_shared<Scene>();
	myScene->myRuntime = this;
}

GameObjectPtr& Dragonite::Runtime::CreateGameObject()
{
	GameObjectPtr obj = std::make_shared<GameObject>();
	obj->mySystem = mySystem;


	size_t pos = myEntities.size();
	myEntities.push_back(obj);

	return myEntities[pos];
}





void Dragonite::Runtime::Awake()
{
	std::cout << "Executing Runtime!" << std::endl;

	myScene->Awake();
}

void Dragonite::Runtime::Update(float aTimeDelta)
{

	for (size_t i = 0; i < myEntities.size(); i++)
	{
		myEntities[i]->UpdateComponents(aTimeDelta);
	}


	myScene->Update(aTimeDelta);
}






