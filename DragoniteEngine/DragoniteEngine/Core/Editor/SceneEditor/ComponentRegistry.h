#pragma once
#include "Core/RuntimeAPI/Components/Camera.h"
#include "Core/RuntimeAPI/Components/ModelRenderer.h"
#include "Core/RuntimeAPI/Components/PlayerController.h"
#include "Core/RuntimeAPI/Components/SpriteRenderer.h"
#include "Core/RuntimeAPI/Components/TestComponent.h"

#include <vector>
#include <memory>


namespace Dragonite
{
	std::vector<std::shared_ptr<Component>> InitializeRegistry() 
	{
		std::vector<std::shared_ptr<Component>> result;

		result.push_back(std::make_shared<SpriteRenderer>());
		result.push_back(std::make_shared<ModelRenderer>());
		result.push_back(std::make_shared<Camera>());
		result.push_back(std::make_shared<PlayerController>());
		result.push_back(std::make_shared<TestComponent>());

		return result;
	}
}
