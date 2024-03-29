#include "Scene.h"
#include "Runtime.h"
#include "Components/Camera.h"
#include <iostream>
#include "Utilities/UtilityFunctions.h"
#include "Components/ModelRenderer.h"
#include "Rendering/Framework/RenderTarget.h"


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
	obj->myTransform.SetPosition({ 0,2, 10 });
	myObjectsToUpdate.push_back(obj);



	for (int i = 0; i < 10; i++)
	{
		auto& obj2 = myRuntime->CreateGameObject();
		obj2->myName = "A Cube";
		obj2->AddComponent<ModelRenderer>()->LoadModel(UNITCUBE);
		obj2->myTransform.SetPosition({ (float)i,i + 1.0f, i + 10.0f });
		myObjectsToUpdate.push_back(obj2);
	}

	auto& objB = myRuntime->CreateGameObject();
	objB->myName = "A Pyramid";
	objB->AddComponent<ModelRenderer>()->LoadModel(UNITPYRAMID);
	objB->myTransform.SetPosition({ 1.5f, 2.f, 10 });


	auto& objC = myRuntime->CreateGameObject();
	objC->myName = "A Terrain";
	objC->AddComponent<ModelRenderer>()->LoadModel(GEN_TERRAIN, ModelRenderer::terrainMaterial);
	objC->myTransform.SetPosition({ 0,0,0 });



	auto& objD = myRuntime->CreateGameObject();
	objD->myName = "Some water";
	objD->AddComponent<RenderTarget>();
	objD->myTransform.SetPosition({ 0,-0.05f,0 });

	auto cam = myCamera->AddComponent<Camera>();
	myCamera->myTransform.SetPosition({ 0.f, 2.f, 0.f });

	myRuntime->GetSystem()->GetGraphicsEngine()->SetRenderCamera(cam.get());

	//myRuntime->GetSystem()->GetGraphicsEngine()->GlobalLightData().myAmbientLight = { 0.25f,0.25f,0.25f,1 };
	myRuntime->GetSystem()->GetGraphicsEngine()->GlobalLightData().myLightColor = { 0.75f, 0.75f, 0.75f, 1.f };
	myRuntime->GetSystem()->GetGraphicsEngine()->GlobalLightData().myLightDirection = { 0.f, -1.f, 0.f, 0.f };



}

void Scene::Update(float aTimeDelta)
{

	myCounter += aTimeDelta;

	for (size_t i = 0; i < myObjectsToUpdate.size(); i++)
	{
		myObjectsToUpdate[i]->myTransform.SetRotation(Vector3f{ 10.f, 10.f, 10.f } *aTimeDelta);
	}
	myRuntime->GetSystem()->GetGraphicsEngine()->GlobalLightData().myLightDirection = Math::Vector4f::Lerp(myRuntime->GetSystem()->GetGraphicsEngine()->GlobalLightData().myLightDirection, { (cosf(myRuntime->GetSystem()->GetTotalTime() / 2.f) * 4.f),-1.0f, 0.f, 0.f }, aTimeDelta);

	//std::cout << '\r' << "[Log]<Runtime>: Direction - " << myRuntime->GetSystem()->GetGraphicsEngine()->GlobalLightData().myLightDirection << std::flush;


}