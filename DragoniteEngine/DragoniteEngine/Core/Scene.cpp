#include "Runtime.h"
#include "Scene.h"
#include <iostream>

#include "Graphics/GraphicsAPI.h"
#include "Graphics/Models/ModelFactory.h"

#include "Utilities/Function.h"
#include "Utilities/Input.h"
#include "Core/PollingStation.h"


void Dragonite::Scene::Awake()
{
	myRenderInterface = myPollingStation->Get<RenderInterface>();
	auto MF = myPollingStation->Get<ModelFactory>();
	myInputManager = myPollingStation->Get<InputManager>();

	myCamera.GetProfile() = new PerspectiveProfile(90.0f, 0.1f, 5000.0f);
	myCamera.GetTransform().myPosition = { 0.0f, 0.0f, 0.0f };
	myCamera.GetTransform().myScale = { 1,1,1 };
	myCamera.GetTransform().myRotation = { 0,0,0 };

	myRenderInterface->SetActiveCameraAs(myCamera);

	myCube = MF->GetModel(PrimitiveType::Cube, Material::defaultMaterial);

	myCube->myTransform.myPosition = { 0.0f, 0.0f, 2.0f };
	myCube->myTransform.myScale = { 1.0f, 1.0f, 1.0f };
	myCube->myTransform.myRotation = { 0,0,0 };



}

void Dragonite::Scene::Update(const float aDt)
{
	auto& mouse = myInputManager->GetMouse();
	if (myInputManager->GetMouse().GetButton(MouseKey::Right) && mouse.delta != Vector2f(0, 0))
	{
		myCamera.GetTransform().myRotation += Vector3f(mouse.delta.y, mouse.delta.x, 0.0f) * 2.0f * aDt;
		mouse.ResetPos();
		
	}


	if (mouse.GetButtonDown(MouseKey::Right))
	{
		mouse.ViewMouse(false);
	}
	if (mouse.GetButtonUp(MouseKey::Right))
	{
		mouse.ViewMouse(true);
	}


	myCube->myTransform.myRotation += {0.0f, 45.0f * aDt, 0.0f};
	myRenderInterface->DrawElement(myCube);



}
