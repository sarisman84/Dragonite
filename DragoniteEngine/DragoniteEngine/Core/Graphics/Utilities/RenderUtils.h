#pragma once
#include "Core/Graphics/DXIncludes.h"
#include <string>
namespace Dragonite
{
	struct RenderUtils
	{
		static HRESULT CreateVSInstance(Device aDevice, const char* aPath, VertexShader& aShader, std::string& someExtraData);
		static HRESULT CreatePSInstance(Device aDevice, const char* aPath, PixelShader& aShader);
	};
}


