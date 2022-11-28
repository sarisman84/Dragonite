#pragma once
#include "entt/single_include/entt/entt.hpp"
#include "Core/CU/Math/Matrix4x4.hpp"

#include <cstdint>


namespace Dragonite
{
	struct DrawInstruct
	{
		//Entity ref
		Matrix4x4f myModelMatrix;

	};
}