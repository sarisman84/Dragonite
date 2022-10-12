#include "Core/Runtime.h"
#include "Scene.h"
#include <iostream>

#include "Core/Graphics/GraphicsAPI.h"
#include "Core/Graphics/Models/ModelFactory.h"

#include "Core/Utilities/Function.h"
#include "Core/Utilities/Input.h"
#include "Core/PollingStation.h"

#include "Core/RuntimeAPI/Components/ModelRenderer.h"

Dragonite::Scene::Scene()
{
	myNextUUID = 1;
}

void Dragonite::Scene::OnSceneInit()
{
	for (auto& object : myObjects)
	{
		object.Awake();
	}
}

void Dragonite::Scene::Update(const float aDt)
{
	for (auto& object : myObjects)
	{
		object.Update(aDt);
	}
}
