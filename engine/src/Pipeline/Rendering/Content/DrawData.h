#pragma once
#include "entt/single_include/entt/entt.hpp"
#include "Core/CU/Math/Matrix4x4.hpp"

#include "MaterialDef.h"
#include "ModelDef.h"

#include <cstdint>
#include <memory>

namespace Dragonite
{


	struct DrawInstruct
	{
		Material myMaterial;
		Model myModel;
		//Entity ref
		Matrix4x4f myModelMatrix;

	};
}