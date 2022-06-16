#pragma once
#include <wrl/client.h>
#include <ostream>
using Microsoft::WRL::ComPtr;

struct ID3D11ShaderResourceView;
struct ID3D11DeviceContext;

namespace Dragonite
{

	class GraphicsEngine;

}


namespace Dragonite
{
	class Texture
	{
	public:
		enum class Type
		{
			Albedo, Normal, Metallic, Cubemap
		};
		Texture();
		Texture(Dragonite::GraphicsEngine* aGraphicsEngine, const char* aTexturePath, Type aTextureType);
		Texture(const ComPtr<ID3D11ShaderResourceView>& aTextureResource);
		Texture(const Texture& aResourceView);
		~Texture();
		void operator=(const Texture& aResourceView);

		/*ComPtr<ID3D11ShaderResourceView>& GetResource();*/
		void BindTexture(ComPtr<ID3D11DeviceContext>& aContext, const size_t& aSlot);


	private:
		struct ImageInfo
		{
			size_t myWidth, myHeight, myChannels;
			unsigned char* myImageData;
		};


		HRESULT ImportTexture(Dragonite::GraphicsEngine* aGraphicsEngine, const char* aTexturePath, ImageInfo& anOutput, bool anImportDDSFile = false);
		ComPtr<ID3D11ShaderResourceView> myTextureResource;
	};
}