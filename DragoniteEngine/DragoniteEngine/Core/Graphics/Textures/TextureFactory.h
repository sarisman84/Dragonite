#pragma once
#include "../DXIncludes.h"

#include <memory>
#include <unordered_map>
#include <string>

#include "Texture.h"

namespace Dragonite
{
	

	class GraphicsPipeline;
	class TextureFactory
	{
	public:
		TextureFactory(GraphicsPipeline* aPipeline);

		TextureRef LoadTexture(const wchar_t* aPath, const bool aUseSRGB = true, const bool aGenerateMipMaps = true);

	private:


		const bool LoadTexture_Impl(TextureRef& anOutput, const wchar_t* aPath, const TextureLoaderDesc& aDesc);


		std::unordered_map<std::wstring, TextureRef> myLoadedTextures;

		GraphicsPipeline* myPipeline;
	};
}


