#pragma once
#include "Core/Graphics/DirectX11/DXIncludes.h"
#include "Core/CU/Math/Vector2.h"

#include <d3d11.h>
#include <memory>
#include <string>

namespace Dragonite
{
	class GraphicalInterface;
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

		bool Init(GraphicalInterface* aPipeline, const TextureLoaderDesc& aDesc);
		void Bind(DeviceContext& someContext);


		inline std::wstring Name() const noexcept { return myName; }

	private:
		ShaderResourceV myResourceView;
		Vector2i myResolution;
		int mySlot;
		std::wstring myName;
	};

	typedef std::shared_ptr<Texture> TextureRef;
}


