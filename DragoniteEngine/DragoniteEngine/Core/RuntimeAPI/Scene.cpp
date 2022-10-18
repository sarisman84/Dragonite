#include "Core/Runtime.h"
#include "Scene.h"
#include <iostream>

#include "Core/Graphics/GraphicsAPI.h"
#include "Core/Graphics/Models/ModelFactory.h"

#include "Core/Utilities/Function.h"
#include "Core/Utilities/Input.h"
#include "Core/PollingStation.h"

#include "Core/RuntimeAPI/Components/ModelRenderer.h"
#include "Core/RuntimeAPI/Components/Camera.h"

const bool Dragonite::Scene::New(Scene& aNewScene, Runtime* anApp, GraphicalInterface* anGI, PollingStation* aPS)
{
	if (!anApp || !anGI || !aPS) return false;


	aNewScene.myApplication = anApp;
	aNewScene.myRenderInterface = anGI;
	aNewScene.myPollingStation = aPS;


	Object camObject = Object("Camera", &aNewScene);
	camObject.AddComponent<Camera>();
	aNewScene.myObjects.push_back(camObject);


	return true;
}

Dragonite::Scene::Scene()
{
	myNextUUID = 1;





}

Dragonite::Scene::Scene(const Scene& aCpy)
{
	CopyScene(&aCpy);

}

void Dragonite::Scene::operator=(const Scene& aCpy)
{
	CopyScene(&aCpy);
}

void Dragonite::Scene::CopyScene(const Scene* aScene)
{
	myPollingStation = aScene->myPollingStation;
	myApplication = aScene->myApplication;
	myCurrentState = aScene->myCurrentState;
	myInputManager = aScene->myInputManager;
	myName = aScene->myName;
	myNextUUID = aScene->myNextUUID;
	myRenderInterface = aScene->myRenderInterface;
	myViewportInterface = aScene->myViewportInterface;
	myObjects = aScene->myObjects;

	for (size_t i = 0; i < myObjects.size(); i++)
	{
		myObjects[i].myCurrentScene = this;
		for (size_t x = 0; x < myObjects[i].Components().size(); x++)
		{
			myObjects[i].Components()[x]->OnCreate();
			myObjects[i].Components()[x]->myObject = &myObjects[i];
			myObjects[i].Components()[x]->myPollingStation = myPollingStation;
		}
	}


}

void Dragonite::Scene::OnSceneInit()
{
	for (auto& object : myObjects)
	{
		object.Awake();
	}

	myRenderInterface = myRenderInterface ? myRenderInterface : myPollingStation->Get<GraphicalInterface>();
	myRenderInterface->SetActiveCameraAs(*myViewportInterface);
}

void Dragonite::Scene::Update(const float aDt)
{
	for (size_t i = 0; i < myObjects.size(); ++i)
	{
		auto& object = myObjects[i];
		object.Update(aDt);
	}
}

void Dragonite::Scene::LateUpdate()
{
	for (size_t i = 0; i < myObjects.size(); ++i)
	{
		auto& object = myObjects[i];
		object.ConstantUpdate();
	}

}

Dragonite::Scene* Dragonite::Scene::Play()
{
	Scene* cpy = new Scene(*this);

	myPollingStation->AddHandler(cpy);

	cpy->OnSceneInit();

	myApplication->OnUpdate() += [cpy](const float aDt)
	{
		cpy->Update(aDt);
	};

	return cpy;

}

void Dragonite::Scene::Stop(Scene* aScene)
{
	myApplication->OnUpdate().operator--();
}
