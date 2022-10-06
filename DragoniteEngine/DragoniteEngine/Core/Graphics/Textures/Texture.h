#pragma once
#include "Core/Graphics/DXIncludes.h"
#include "../../CU/Math/Vector2.h"

#include <d3d11.h>
#include <memory>

namespace Dragonite
{
	class GraphicsPipeline;
	struct TextureLoaderDesc
	{
		enum class LoaderType
		{
			PNG, DDS
		};
		LoaderType myLoaderType;
		bool myGenerateMipMap;
		DXGI_FORMAT myTextureFormat;
		unsigned char* myRgbaPixels;
		Vector2i myResolution;
		int myTargetSlot;
	};


	

	class Texture
	{
		friend class TextureFactory;
		friend class ModelFactory;
	public:
		Texture();
		~Texture();

		inline const Vector2i GetResolution() const noexcept { return myResolution; }


		inline ShaderResourceV GetData() { return myResourceView; }

		bool Init(GraphicsPipeline* aPipeline,const TextureLoaderDesc& aDesc);
		void Bind(DeviceContext& someContext);


	private:
		ShaderResourceV myResourceView;
		Vector2i myResolution;
		int mySlot;
		const char* myName;
	};

	typedef std::shared_ptr<Texture> TextureRef;
}


