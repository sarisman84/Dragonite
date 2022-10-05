#include "Core/Runtime.h"
#include "Scene.h"
#include <iostream>

#include "Core/Graphics/GraphicsAPI.h"
#include "Core/Graphics/Models/ModelFactory.h"

#include "Core/Utilities/Function.h"
#include "Core/Utilities/Input.h"
#include "Core/PollingStation.h"

#include "Core/RuntimeAPI/Components/ModelRenderer.h"

void Dragonite::Scene::Awake()
{
	auto mf = myPollingStation->Get<ModelFactory>();
	myRenderInterface = myPollingStation->Get<RenderInterface>();
	myCamera.Profile() = new PerspectiveProfile(90.0f, 0.1f, 1000.0f);
	myCamera.GetTransform().myPosition = { 0,0,-7.0f };
	myCamera.GetTransform().myScale = { 1,1,1 };
	myRenderInterface->SetActiveCameraAs(myCamera);

	int cubeCount = 10;
	float pos = -cubeCount / 2;
	for (; cubeCount >= 0; cubeCount--)
	{

		
		Object newObject = Object("New GameObject");
		newObject.Init(myPollingStation);
		newObject.GetTransform().myPosition = { pos,0, 1 };

		auto modelRenderer = newObject.AddComponent<ModelRenderer>();

		modelRenderer->Model() = mf->GetModel(PrimitiveType::Cube, Material::defaultMaterial);

		pos++;
		myObjects.push_back(newObject);
	}
}

void Dragonite::Scene::Update(const float aDt)
{
	for (auto& object : myObjects)
	{
		object.Update(aDt);
	}
}
