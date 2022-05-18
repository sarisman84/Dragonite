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


	auto& objB = myRuntime->CreateGameObject();
	objB->myName = "A Pyramid";
	objB->AddComponent<ModelRenderer>()->LoadModel(UNITPYRAMID);
	objB->myTransform.Position = { 1.5f, 2.f, 10 };

	auto cam = myCamera->AddComponent<Camera>();
	myCamera->myTransform.Position() = { 0.f, 2.f, 0.f };

	myRuntime->GetSystem()->GetGraphicsEngine()->SetRenderCamera(cam.get());


	/*int size = 50;
	for (float x = -(size / 2); x <= (size / 2); x++)
	{
		for (float y = -(size / 2); y <= (size / 2); y++)
		{
			auto& obj = myRuntime->CreateGameObject();
			obj->myName = "GameObject";
			obj->AddComponent<ModelRenderer>()->LoadModel(UNITCUBE);
			obj->myTransform.Position = { x,0,y };
		}
	}
	std::cout << "[Log]<Scene>: Created " << factorial(size) << " cubes!" << std::endl;*/



	for (size_t i = 0; i < 1000; i++)
	{
		Material aniMat = Material{ Math::Vector4f(1,1,1,1), "Textures/default.png", "Shaders/ColorShader_VS.cso", "Shaders/AnimatedSinWave_PS.cso" };
		Material defaultMat = ModelRenderer::defaultMaterial;
		auto& tempObj = myRuntime->CreateGameObject();
		tempObj->myName = "GameObject";
		tempObj->AddComponent<ModelRenderer>()->LoadModel(Utilities::RandomRange(0.f, 1.f) > 0.5f ? UNITPYRAMID : UNITCUBE, Utilities::RandomRange(0.f, 1.f) > 0.5f ? aniMat : defaultMat);
		tempObj->myTransform.Position = { Utilities::RandomRange(-100.f, 100.f),Utilities::RandomRange(-100.f, 100.f),Utilities::RandomRange(-100.f, 100.f) };
		auto size = Utilities::RandomRange(0.1f, 10.f);
		tempObj->myTransform.Size = { size,size,size };
		tempObj->myTransform.Rotation = { Utilities::RandomRange(0.f, 360.f),Utilities::RandomRange(0.f, 360.f),Utilities::RandomRange(0.f, 360.f), Utilities::RandomRange(0.f, 360.f) };

		if (Utilities::RandomRange(0.f, 1.f) > 0.5f)
		{
			myObjectsToUpdate.push_back(tempObj);
		}
	}
}

void Scene::Update(float aTimeDelta)
{
	std::cout << '\r' << "[Log]<Runtime>: FPS - " << myRuntime->GetSystem()->GetFPS() << std::flush;
	myCounter += aTimeDelta;
	//myCube->myTransform.SetPosition(myCube->myTransform.GetPosition() + Math::Vector3f(cos(myCounter) * 2.f, sin(myCounter) * 2.f, 0) * aTimeDelta);


	for (auto& obj : myObjectsToUpdate)
	{
		obj->myTransform.Rotation = obj->myTransform.Rotation() + (Math::Vector4f{ -10.f, -10.f, 10.f, 10.f } *aTimeDelta);
		obj->GetComponent<ModelRenderer>()->Color() -= Math::Vector4f{ 0.2f, 0.2f, 0.2f, 0.f } *aTimeDelta;
	}
	//std::cout << aTimeDelta << "/" << myRuntime->GetSystem()->GetTotalTime() << std::endl;
}
