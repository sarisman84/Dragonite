#include "Scene.h"
#include "Runtime.h"
#include "Components/Camera.h"
#include <iostream>


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


	myCube = myRuntime->CreateGameObject();
	myCube->myName = "A Cube";
	myCubeRenderer = myCube->AddComponent<ModelRenderer>();
	myCubeRenderer->LoadModel(UNITCUBE);
	myCube->myTransform.SetPosition({ 0,2, 10 });


	auto& secondCube = myRuntime->CreateGameObject();
	secondCube->myName = "A Pyramid";
	secondCube->AddComponent<ModelRenderer>()->LoadModel(UNITPYRAMID);
	secondCube->myTransform.SetPosition({ 1.5f, 2.f, 10 });

	auto cam = myCamera->AddComponent<Camera>();

	myRuntime->GetSystem()->GetGraphicsEngine()->SetRenderCamera(cam.get());


	int size = 50;
	for (float x = -(size / 2); x <= (size / 2); x++)
	{
		for (float y = -(size / 2); y <= (size / 2); y++)
		{
			auto& obj = myRuntime->CreateGameObject();
			obj->myName = "GameObject";
			obj->AddComponent<ModelRenderer>()->LoadModel(UNITCUBE);
			obj->myTransform.SetPosition({ x,0,y });
		}
	}
	std::cout << "[Log]<Scene>: Created " << factorial(size) << " cubes!" << std::endl;
}

void Scene::Update(float aTimeDelta)
{
	myCounter += aTimeDelta;
	//myCube->myTransform.SetPosition(myCube->myTransform.GetPosition() + Math::Vector3f(cos(myCounter) * 2.f, sin(myCounter) * 2.f, 0) * aTimeDelta);
	myCube->myTransform.SetRotation(Math::Vector4f{ -10.f, -10.f, 10.f, 10.f } *aTimeDelta);
	myCubeRenderer->Color() -= Math::Vector4f{0.2f, 0.2f, 0.2f, 0.f} * aTimeDelta;
	//std::cout << aTimeDelta << "/" << myRuntime->GetSystem()->GetTotalTime() << std::endl;
}
