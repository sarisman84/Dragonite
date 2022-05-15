#include "Scene.h"
#include "Runtime.h"
#include "Components/Camera.h"
#include "Components/ModelRenderer.h"
#include <iostream>

void Scene::Awake()
{
	myCamera = myRuntime->CreateGameObject();
	myCamera->myName = "Camera";


	myCube = myRuntime->CreateGameObject();
	myCube->myName = "A Cube";
	myCube->AddComponent<ModelRenderer>();
	myCube->myTransform.myTransformMatrix.SetPosition({ 0,0, 10 });

	auto cam = myCamera->AddComponent<Camera>();

	myRuntime->GetSystem()->GetGraphicsEngine()->SetRenderCamera(cam.get());
}

void Scene::Update(float aTimeDelta)
{
	myCounter += aTimeDelta;
	std::cout << '\r' << myCounter << std::flush;
}
