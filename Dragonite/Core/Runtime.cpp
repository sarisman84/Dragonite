#include "Runtime.h"
#include  "System.h"
#include "Utilities/UtilityFunctions.h"
#include <iostream>
#include "Scene.h"


Engine::Runtime::Runtime(System* aCoreSystem)
{
	mySystem = aCoreSystem;

	mySystem->AddManager<Runtime>();


	myScene = std::make_shared<Scene>();
	myScene->myRuntime = this;
}

GameObject& Engine::Runtime::CreateGameObject()
{
	GameObject obj = GameObject();
	obj.mySystem = mySystem;
	myEntities.push_back(obj);
	return myEntities.back();
}





void Engine::Runtime::Awake()
{
	std::cout << "Executing Runtime!" << std::endl;
	
	myScene->Awake();
}

void Engine::Runtime::Update(float aTimeDelta)
{
	
	for (size_t i = 0; i < myEntities.size(); i++)
	{
		myEntities[i].UpdateComponents(aTimeDelta);
	}


	myScene->Update(aTimeDelta);
}






