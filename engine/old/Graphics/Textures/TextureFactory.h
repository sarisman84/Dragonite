#pragma once
#include "Core/Graphics/DirectX11/DXIncludes.h"

#include <memory>
#include <unordered_map>
#include <string>

#include "Texture.h"

namespace Dragonite
{


	class GraphicalInterface;
	class TextureFactory
	{
	public:
		TextureFactory(GraphicalInterface* aPipeline);

		TextureRef& LoadTexture(const wchar_t* aPath, const bool aUseSRGB = true, const bool aGenerateMipMaps = true);

	private:


		const bool LoadTexture_Impl(TextureRef& anOutput, const wchar_t* aPath, TextureLoaderDesc& aDesc);


		std::unordered_map<std::wstring, TextureRef> myLoadedTextures;

		GraphicalInterface* myPipeline;
	};
}


