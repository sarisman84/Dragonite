#pragma once
#include "Core/CU/Math/Vector3.h"

#include <array>
namespace Dragonite
{
	struct Vertex
	{
		Vector3f myPosition;
	};

	struct Triangle
	{
		std::array<uint32_t, 3> myTriangle;
	};
}