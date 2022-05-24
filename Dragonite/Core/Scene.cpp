#include "Scene.h"
#include "Runtime.h"
#include "Components/Camera.h"
#include <iostream>
#include "Utilities/UtilityFunctions.h"

float factorial(float aVal)
{
	if (aVal > 1)
		return aVal * factorial(aVal - 1);
	return 1;
}

void Scene::Awake()
{
	myCamera = myRuntime->CreateGameObject();
	myCamera->myName = "Camera";


	auto& obj = myRuntime->CreateGameObject();
	obj->myName = "A Cube";
	obj->AddComponent<ModelRenderer>()->LoadModel(UNITCUBE);
	obj->myTransform.Position = { 0,2, 10 };
	myObjectsToUpdate.push_back(obj);

	myObjectsToUpdate.push_back(obj);

	auto& objB = myRuntime->CreateGameObject();
	objB->myName = "A Pyramid";
	objB->AddComponent<ModelRenderer>()->LoadModel(UNITPYRAMID);
	objB->myTransform.Position = { 1.5f, 2.f, 10 };

	auto cam = myCamera->AddComponent<Camera>();
	myCamera->myTransform.Position() = { 0.f, 2.f, 0.f };

	myRuntime->GetSystem()->GetGraphicsEngine()->SetRenderCamera(cam.get());



}

void Scene::Update(float aTimeDelta)
{
	std::cout << '\r' << "[Log]<Runtime>: FPS - " << myRuntime->GetSystem()->GetFPS() << std::flush;
	myCounter += aTimeDelta;

	for (size_t i = 0; i < myObjectsToUpdate.size(); i++)
	{
		myObjectsToUpdate[i]->myTransform.Rotation = myObjectsToUpdate[i]->myTransform.Rotation() + Vector4f{10.f, 10.f, 10.f, 10.f} *aTimeDelta;
	}
}
