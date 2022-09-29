#include "Application.h"
#include "Runtime.h"
#include <iostream>

#include "Graphics/GraphicsAPI.h"
#include "Graphics/Models/ModelFactory.h"


void Dragonite::Runtime::Awake()
{
	myRenderInterface = myPollingStation->Get<RenderInterface>();
	auto MF = myPollingStation->Get<ModelFactory>();

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

void Dragonite::Runtime::Update(const float aDt)
{
	myCube->myTransform.myRotation += {0.0f, 45.0f * aDt, 0.0f};
	myRenderInterface->DrawElement(myCube);
}
