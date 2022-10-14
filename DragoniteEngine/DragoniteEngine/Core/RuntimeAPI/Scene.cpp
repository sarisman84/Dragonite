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

void Dragonite::Scene::OnSceneInit()
{
	myRenderInterface = myRenderInterface ? myRenderInterface : myPollingStation->Get<GraphicalInterface>();
	myRenderInterface->SetActiveCameraAs(*myViewportInterface);


	for (auto& object : myObjects)
	{
		object.Awake();
	}
}

void Dragonite::Scene::Update(const float aDt)
{

	for (auto& object : myObjects)
	{
		object.Update(aDt);
	}


	for (auto& object : myObjects)
	{
		object.ConstantUpdate();
	}

}

void Dragonite::Scene::Play()
{
	myCurrentState = true;
	OnSceneInit();
}

void Dragonite::Scene::Stop()
{
	if (!myCurrentState) return;
	myCurrentState = false;
	for (auto& object : myObjects)
	{
		object.OnDisable();
	}
}
