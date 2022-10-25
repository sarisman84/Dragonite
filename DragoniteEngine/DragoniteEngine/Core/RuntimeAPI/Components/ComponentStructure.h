#pragma once
#include "Core/Utilities/Reflection.h"


#include "Core/RuntimeAPI/Components/Camera.h"
#include "Core/RuntimeAPI/Components/ComputerAgent.h"
#include "Core/RuntimeAPI/Components/ComputerTerminal.h"
#include "Core/RuntimeAPI/Components/ModelRenderer.h"
#include "Core/RuntimeAPI/Components/PlayerController.h"
#include "Core/RuntimeAPI/Components/SpriteRenderer.h"
#include "Core/RuntimeAPI/Components/TestComponent.h"


namespace Dragonite
{
	template<>
	inline auto Reflect::RegisterElement<Component>() 
	{
		return Class<Camera, ComputerAgent, ComputerTerminal, ModelRenderer, PlayerController, SpriteRenderer, TestComponent>("Component");
	}
}