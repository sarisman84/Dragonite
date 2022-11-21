#pragma once
#include <cstdint>
#include "Core/CU/Math/Vector.h"

struct ID3D11Buffer;

namespace Dragonite
{
	struct Model
	{
		ID3D11Buffer* myVertices;
		ID3D11Buffer* myIndices;
		uint32_t myIndiciesCount;
	};



	struct Vertex
	{
		Vector3f myPosition;
		Vector2f myUV;
	};
}
