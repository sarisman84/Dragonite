#include "Application.h"
#include "Runtime.h"
#include <iostream>

#include "Graphics/GraphicsAPI.h"
#include "Graphics/Models/ModelFactory.h"

void Dragonite::Runtime::Awake()
{
	myRenderInterface = myPollingStation->Get<RenderInterface>();
	auto MF = myPollingStation->Get<ModelFactory>();

	myCamera.GetProfile() = new PerspectiveProfile(90.0f, 0.01f, 10000.0f);
	myCamera.GetTransform().myPosition = { 0.0f, 0.0f, 0.0f };

	myRenderInterface->SetActiveCameraAs(myCamera);

	myCube = MF->GetModel(PrimitiveType::Cube, Material::defaultMaterial);

	myCube->myTransform.myPosition = { 0.0f, 0.0f, 10.0f };
	myCube->myTransform.myScale = { 1.0f, 1.0f, 1.0f };


}

void Dragonite::Runtime::Update(const float aDt)
{
	myRenderInterface->DrawElement(myCube);
}
