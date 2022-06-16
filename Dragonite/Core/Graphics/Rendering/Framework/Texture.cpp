

#include "Texture.h"
#include <d3d11.h>
#include <iostream>
#include "GraphicsEngine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Utilities/STB/ImageImport.h"
#include "Utilities/External/DDSTextureLoader.h"

#include <string>

Dragonite::Texture::Texture() = default;
Dragonite::Texture::~Texture() = default;


Dragonite::Texture::Texture(Dragonite::GraphicsEngine * aGraphicsEngine, const char* aTexturePath, Type aTextureType)
{
	if (!aGraphicsEngine) return;

	ImageInfo info;
	if (FAILED(ImportTexture(aGraphicsEngine, aTexturePath, info, aTextureType == Type::Cubemap)))
	{
		return;
	}
	ID3D11Texture2D* texturePtr = nullptr;
	D3D11_TEXTURE2D_DESC desc;

	desc.Width = info.myWidth;
	desc.Height = info.myHeight;
	desc.MipLevels = 0;
	desc.ArraySize = 1;

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	switch (aTextureType)
	{
	default:
	case Dragonite::Texture::Type::Albedo:
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		break;
	case Dragonite::Texture::Type::Normal:
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	}

	auto context = aGraphicsEngine->GetDeviceContext();
	auto device = aGraphicsEngine->GetDevice();


	if (FAILED(device->CreateTexture2D(&desc, nullptr, &texturePtr)))
		return;

	if (FAILED(device->CreateShaderResourceView(texturePtr, nullptr, &myTextureResource))) return;
	context->UpdateSubresource(texturePtr, 0, nullptr, (void*)info.myImageData, info.myWidth * 4, info.myWidth * info.myHeight * 4);

	context->GenerateMips(myTextureResource.Get());
	texturePtr->Release();
}

Dragonite::Texture::Texture(const ComPtr<ID3D11ShaderResourceView>&aTextureResource)
{
	myTextureResource = aTextureResource;
}

Dragonite::Texture::Texture(const Texture & aTexture)
{
	myTextureResource = aTexture.myTextureResource;
}



void Dragonite::Texture::operator=(const Texture & aTexture)
{
	myTextureResource = aTexture.myTextureResource;
}

void Dragonite::Texture::BindTexture(ComPtr<ID3D11DeviceContext>&aContext, const size_t & aSlot)
{
	aContext->PSSetShaderResources(aSlot, 1, myTextureResource.GetAddressOf());
}

HRESULT Dragonite::Texture::ImportTexture(Dragonite::GraphicsEngine * aGraphicsEngine, const char* aTexturePath, ImageInfo & anOutput, bool anImportDDSFile)
{

	if (anImportDDSFile)
	{
		std::string path = std::string(aTexturePath);
		HRESULT result = DirectX::CreateDDSTextureFromFile(aGraphicsEngine->GetDevice(), std::wstring(path.begin(), path.end()).c_str(), 0, &myTextureResource);

		if (result != NOERROR)
			std::cout << "[ERROR]<Texture/ImportTexture>: Couldnt import DDS file (" << aTexturePath << ")" << std::endl;
		return result;
	}

	int width, height, channels;
	unsigned char* img = stbi_load(aTexturePath, &width, &height, &channels, 0);

	if (!img)
	{
		std::cout << "[ERROR]<Texture/ImportTexture>: STB Reported '" << stbi_failure_reason() << "'" << std::endl;
		return E_INVALIDARG;
	}

	switch (channels)
	{
	case 3:
	{
		std::vector<unsigned char> imageData(width * height * 4);

		for (size_t i = 0; i < width * height; i++)
		{
			imageData[4 * i + 0] = img[3 * i + 0];
			imageData[4 * i + 1] = img[3 * i + 1];
			imageData[4 * i + 2] = img[3 * i + 2];
			imageData[4 * i + 3] = 255;
		}
		img = imageData.data();
		break;
	}
	case 4:
		break;
	default:
		return E_INVALIDARG;
	}

	anOutput.myImageData = img;
	anOutput.myHeight = height;
	anOutput.myWidth = width;
	anOutput.myChannels;

	return NOERROR;
}
