#include "Texture.h"
#include <d3d11.h>

#include "../GraphicsAPI.h"
#include "Core/Graphics/DirectX11/DXInterface.h"

Dragonite::Texture::Texture()
{
}

Dragonite::Texture::~Texture()
{
}


const bool GenerateMipMap(Dragonite::GraphicalInterface* aPipeline, Dragonite::ShaderResourceV& aResourceView, D3D11_TEXTURE2D_DESC& aDesc, Dragonite::DXTexture2D aTexture2D, unsigned char* rgbaPixels, int width, int height)
{
	HRESULT result;

	aDesc.MipLevels = 0;
	aDesc.Usage = D3D11_USAGE_DEFAULT;
	aDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	aDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	result = Dragonite::DXInterface::Device->CreateTexture2D(&aDesc, nullptr, &aTexture2D);
	if (FAILED(result))
		return false;

	result = Dragonite::DXInterface::Device->CreateShaderResourceView(aTexture2D.Get(), NULL, &aResourceView);
	if (FAILED(result))
		return false;

	Dragonite::DXInterface::Context->UpdateSubresource(aTexture2D.Get(), 0, nullptr, (void*)rgbaPixels, width * 4, width * height * 4);
	Dragonite::DXInterface::Context->GenerateMips(aResourceView.Get());
	return true;
}

bool Dragonite::Texture::Init(GraphicalInterface* aPipeline, const TextureLoaderDesc& aDesc)
{
	Device device = DXInterface::Device;
	DXTexture2D texture;

	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = aDesc.myResolution.x;
	desc.Height = aDesc.myResolution.y;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.CPUAccessFlags = 0;
	desc.ArraySize = 1;

	desc.Format = aDesc.myTextureFormat;


	mySlot = aDesc.myTargetSlot;

	if (aDesc.myGenerateMipMap)
	{
		return GenerateMipMap(aPipeline, myResourceView, desc, texture, aDesc.myRgbaPixels, aDesc.myResolution.x, aDesc.myResolution.y);
	}
	else
	{
		desc.MipLevels = 1;
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MiscFlags = 0;


		D3D11_SUBRESOURCE_DATA subResrcDesc = {};
		subResrcDesc.pSysMem = (void*)aDesc.myRgbaPixels;
		subResrcDesc.SysMemPitch = aDesc.myResolution.x * 4;
		subResrcDesc.SysMemSlicePitch = aDesc.myResolution.x * aDesc.myResolution.y * 4;

		if (FAILED(device->CreateTexture2D(&desc, &subResrcDesc, &texture)))
		{
			return false;
		}

		if (FAILED(device->CreateShaderResourceView(texture.Get(), NULL, &myResourceView)))
		{
			return false;
		}
	}



	return true;
}

void Dragonite::Texture::Bind(DeviceContext& someContext)
{
	someContext->PSSetShaderResources(mySlot, 1, myResourceView.GetAddressOf());
}
