#include "Runtime.h"
#include  "System.h"
#include "Utilities/UtilityFunctions.h"
#include <iostream>
#include "ComponentManager.h"
#include "Rendering/CommonComponents.h"
#include "Rendering/MeshRendering.h"
#include "Rendering/Camera.h"

Engine::Runtime::Runtime(System* aCoreSystem)
{
	mySystem = aCoreSystem;
}





void Engine::Runtime::Awake()
{

	/*Object triangle = mySystem->GetGraphicsEngine()->Create2DElement(Primitive2D::Triangle);
	Object quad = mySystem->GetGraphicsEngine()->Create2DElement(Primitive2D::Quad);
	someShape = mySystem->GetGraphicsEngine()->Create2DElement(Primitive2D::Circle);
	someOtherShape = mySystem->GetGraphicsEngine()->Create2DElement(Primitive2D::Circle);


	someShape->SetSize({ 0.25f, 0.25f, 0.25f });
	someOtherShape->SetSize({ 0.25f, 0.25f, 0.25f });
	triangle->SetSize({ 2.f, 1.f, 1.0f });
	quad->SetSize({ 0.5f, 0.5f, 0.5f });
	quad->SetPixelShader("Shaders/SinWave_PS.cso");
	someShape->SetPosition({ 0.5f, 0.f, 0.f });
	someOtherShape->SetPixelShader("Shaders/SinWave_PS.cso");
	someShape->SetPixelShader("Shaders/SinWave_PS.cso");*/


	EntityID entity = mySystem->Get<EntityManager>()->CreateEntity();
	mySystem->Get<EntityManager>()->AddComponent<Transform>(entity);
	mySystem->Get<EntityManager>()->AddComponent<ModelRenderer>(entity, mySystem->Get<ModelFactory>()->GetModel("UnitCube"));

	mySystem->Get<EntityManager>()->RegisterUpdateCall<ModelRenderer>(UpdateModel);
}




void Engine::Runtime::Update()
{
	//someCount++;
	//Math::Vector3f pos = { 0.5f * sin(someCount * 0.01f), 0.5f * cos(someCount * 0.01f), 0.f };
	//pos.x /= mySystem->GetWindowsInfo().GetAspectRatio();
	////pos.x = Utilities::Clamp(-0.3f, 0.3f, pos.x);
	//someShape->SetPosition(pos);

	//pos = { 0.5f * -sin(someCount * 0.01f), 0.5f * -cos(someCount * 0.01f), 0.f };
	//pos.x /= mySystem->GetWindowsInfo().GetAspectRatio();
	////pos.x = Utilities::Clamp(-0.3f, 0.3f, pos.x);


	//someOtherShape->SetPosition(pos);



	mySystem->Get<EntityManager>()->UpdateComponent<ModelRenderer>();








}

