#pragma once
#include "IRecipe.h"

#include <cstdint>

struct D3D11_INPUT_ELEMENT_DESC;

namespace Dragonite
{
	struct ShaderRecipe
	{
		const char* myVSPath;
		const char* myPSPath;
		D3D11_INPUT_ELEMENT_DESC* myInputDescs;
		uint32_t myInputDescsSize;
	};

}