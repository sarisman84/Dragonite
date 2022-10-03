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
	myRenderInterface = myPollingStation->Get<RenderInterface>();
	myCamera.GetProfile() = new PerspectiveProfile(90.0f, 0.1f, 1000.0f);
	myCamera.GetTransform().myPosition = { 0,0,0 };
	myCamera.GetTransform().myScale = { 1,1,1 };
	myRenderInterface->SetActiveCameraAs(myCamera);
}

void Dragonite::Scene::Update(const float aDt)
{
	for (auto& object : myObjects)
	{
		object.Update(aDt);
	}
}
