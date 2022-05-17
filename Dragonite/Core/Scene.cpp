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
	myCube->AddComponent<ModelRenderer>()->LoadModel(UNITCUBE, "", "Shaders/AnimatedSinWave_PS.cso");
	myCube->myTransform.SetPosition({ 0,0, 10 });


	auto& secondCube = myRuntime->CreateGameObject();
	secondCube->myName = "Another Cube";
	secondCube->AddComponent<ModelRenderer>()->LoadModel(UNITPIRAMID);
	secondCube->myTransform.SetPosition({ 1.5f, 0.f, 10 });

	auto cam = myCamera->AddComponent<Camera>();

	myRuntime->GetSystem()->GetGraphicsEngine()->SetRenderCamera(cam.get());
}

void Scene::Update(float aTimeDelta)
{
	myCounter += aTimeDelta;
	myCube->myTransform.SetPosition(myCube->myTransform.GetPosition() + Math::Vector3f(cos(myCounter) * 2.f, sin(myCounter) * 2.f, 0) * aTimeDelta);
	myCube->myTransform.SetRotation(Math::Vector4f{ -10.f, -10.f, 10.f, 10.f } *aTimeDelta);
	//std::cout << aTimeDelta << "/" << myRuntime->GetSystem()->GetTotalTime() << std::endl;
}
