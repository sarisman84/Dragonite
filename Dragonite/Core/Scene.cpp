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

Scene::Scene()
{
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


	auto& objC = myRuntime->CreateGameObject();
	objC->myName = "A Terrain";
	objC->AddComponent<ModelRenderer>()->LoadModel(GEN_TERRAIN, ModelRenderer::terrainMaterial);
	objC->myTransform.Position = { 0,0,0 };

	auto cam = myCamera->AddComponent<Camera>();
	myCamera->myTransform.Position() = { 0.f, 2.f, 0.f };

	myRuntime->GetSystem()->GetGraphicsEngine()->SetRenderCamera(cam.get());

	myRuntime->GetSystem()->GetGraphicsEngine()->GlobalLightData().myAmbientLight = { 0.75f,0.0f,0.75f,1 };
	myRuntime->GetSystem()->GetGraphicsEngine()->GlobalLightData().myLightColor = { 0.75f, 0.75f, 0.75f, 1.f };
	myRuntime->GetSystem()->GetGraphicsEngine()->GlobalLightData().myLightDirection = { 0.f, -1.f, 0.f, 0.f };



}

void Scene::Update(float aTimeDelta)
{

	myCounter += aTimeDelta;

	for (size_t i = 0; i < myObjectsToUpdate.size(); i++)
	{
		myObjectsToUpdate[i]->myTransform.Rotation = myObjectsToUpdate[i]->myTransform.Rotation() + Vector4f{ 10.f, 10.f, 10.f, 10.f } *aTimeDelta;
	}
	myRuntime->GetSystem()->GetGraphicsEngine()->GlobalLightData().myLightDirection = Math::Vector4f::Lerp(myRuntime->GetSystem()->GetGraphicsEngine()->GlobalLightData().myLightDirection, { (cosf(myRuntime->GetSystem()->GetTotalTime() / 2.f) * 4.f),-1.0f, 0.f, 0.f }, aTimeDelta);
	
	//std::cout << '\r' << "[Log]<Runtime>: Direction - " << myRuntime->GetSystem()->GetGraphicsEngine()->GlobalLightData().myLightDirection << std::flush;


}