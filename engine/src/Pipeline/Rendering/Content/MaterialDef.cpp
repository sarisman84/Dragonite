#include "MaterialDef.h"
#include <d3d11.h>

#include <vector>

#define STB_IMAGE_IMPLEMENTATION
//#include "../../External/STB/stb_image.h"
#include "STB/stb_image.h"


Dragonite::Texture::Texture()
{
	myResourceView = nullptr;
}

Dragonite::Texture::~Texture()
{
	if (myResourceView)
		myResourceView->Release();
	myResourceView = nullptr;
}

void Dragonite::Texture::Init(ID3D11Device* aDevice, void* aTextureBuffer)
{
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = myTextureSize.x;
	desc.Height = myTextureSize.y;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.CPUAccessFlags = 0;
	desc.ArraySize = 1;

	desc.Format = mySRGBFlag ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;


	ID3D11Texture2D* texture = {};
	D3D11_SUBRESOURCE_DATA subRescDesc = {};
	subRescDesc.pSysMem = aTextureBuffer;
	subRescDesc.SysMemPitch = myTextureSize.x * 4;
	subRescDesc.SysMemSlicePitch = myTextureSize.y * myTextureSize.y * 4;


	if (FAILED(aDevice->CreateTexture2D(&desc, &subRescDesc, &texture)))
		return;

	if (FAILED(aDevice->CreateShaderResourceView(texture, NULL, &myResourceView)))
	{
		texture->Release();
		return;
	}
	texture->Release();
}

void Dragonite::Texture::Bind(ID3D11DeviceContext* aContext)
{
	if (!myResourceView || aContext) return;


	aContext->PSSetShaderResources(mySlot, 1, &myResourceView);
}

Dragonite::_internal::PixelData Dragonite::_internal::LoadTextureBuffer(const char* aPath)
{
	PixelData data;

	int channels;

	unsigned char* img = stbi_load(aPath, &data.myResolution.x, &data.myResolution.y, &channels, 0);

	if (img == nullptr) return data;



	auto e = stbi_failure_reason();

	if (channels == 3)
	{
		std::vector<unsigned char> imageData(data.myResolution.x * data.myResolution.y * 4);
		for (int i = 0; i < data.myResolution.x * data.myResolution.y; i++)
		{
			imageData[4 * i + 0] = img[3 * i + 0];
			imageData[4 * i + 1] = img[3 * i + 1];
			imageData[4 * i + 2] = img[3 * i + 2];
			imageData[4 * i + 3] = 255;
		}

		data.myBuffer = imageData.data();
	}
	else if (channels == 4)
	{
		data.myBuffer = img;
	}

	return data;
}
