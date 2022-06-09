#pragma once
#include <wrl/client.h>
#include <ostream>
using Microsoft::WRL::ComPtr;

struct ID3D11ShaderResourceView;
struct ID3D11DeviceContext;

namespace Engine
{
	namespace Graphics
	{
		class GraphicsEngine;
	}
}


namespace Dragonite
{
	class Texture
	{
	public:
		enum class Type
		{
			Albedo, Normal
		};
		Texture() = default;
		Texture(Engine::Graphics::GraphicsEngine* aGraphicsEngine, const char* aTexturePath, Type aTextureType);

		Texture(const Texture& aResourceView);
		void operator=(const Texture& aResourceView);

		/*ComPtr<ID3D11ShaderResourceView>& GetResource();*/
		void BindTexture(ComPtr<ID3D11DeviceContext>& aContext, const size_t& aSlot);


	private:
		struct ImageInfo
		{
			size_t myWidth, myHeight, myChannels;
			unsigned char* myImageData;
		};


		HRESULT ImportTexture(const char* aTexturePath, ImageInfo& anOutput);
		ComPtr<ID3D11ShaderResourceView> myTextureResource;
	};
}