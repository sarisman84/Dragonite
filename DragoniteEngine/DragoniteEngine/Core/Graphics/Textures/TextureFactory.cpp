#include "TextureFactory.h"
#include "Core/Graphics/GraphicsAPI.h"
#include "Core/Runtime.h"

#include "../../External/DirectX/DDSTextureLoader11.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../External/STB/stb_image.h"


#include <stdlib.h>
#include <d3d11.h>



Dragonite::TextureFactory::TextureFactory(GraphicsPipeline* aPipeline)
{
	myPipeline = aPipeline;
	myLoadedTextures[L"null"] = nullptr;
}

Dragonite::TextureRef& Dragonite::TextureFactory::LoadTexture(const wchar_t* aPath, const bool aUseSRGB, const bool aGenerateMipMaps)
{

	std::wstring path(aPath);

	if (myLoadedTextures.contains(path))
	{
		return myLoadedTextures[path];
	}

	TextureLoaderDesc desc = {};
	TextureRef texture = std::make_shared<Texture>();

	std::wstring name = aPath;
	texture->myName = std::string(name.begin(), name.end()).c_str();
	const size_t lastDot = path.find_last_of(L".");
	std::wstring extension = path.substr(lastDot + 1);

	desc.myLoaderType = extension == L"dds" ? TextureLoaderDesc::LoaderType::DDS : TextureLoaderDesc::LoaderType::PNG;
	desc.myGenerateMipMap = aGenerateMipMaps;
	desc.myTextureFormat = aUseSRGB ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.myTargetSlot = 0; //TODO: Setup Normal, Material and Albedo slots here.

	if (!LoadTexture_Impl(texture, aPath, desc))
		return myLoadedTextures[L"null"];


	myLoadedTextures[path] = texture;

	return myLoadedTextures[path];
}


const bool LoadDDS(Dragonite::Device aDevice, const wchar_t* aPath, Dragonite::ShaderResourceV& aResourceView, Dragonite::Vector2i& anOutputResolution) {
	using namespace Dragonite;

	DXTexture2D text2D;
	D3D11_TEXTURE2D_DESC textInfo;
	DXResource texture;


	HRESULT result = DirectX::CreateDDSTextureFromFile(aDevice.Get(), aPath, &texture, &aResourceView);
	if (FAILED(result))
		return false;

	texture->QueryInterface(IID_ID3D11Texture2D, (void**)&text2D); //TODO: Optimize this line (apparently slow: https://gyazo.com/516e3143cd95e4277b7ca776f09f6ade)

	text2D->GetDesc(&textInfo);
	anOutputResolution = Dragonite::Vector2i(textInfo.Width, textInfo.Height);


	return true;
}

const bool LoadPNG(Dragonite::GraphicsPipeline* aPipeline, const wchar_t* aPath, Dragonite::TextureRef& anOutput, Dragonite::TextureLoaderDesc& aDesc)
{
	int width, height, channels;

	const auto size = wcslen(aPath);
	char path[1000];
	size_t r;
	wcstombs_s(&r, path, aPath, size);



	unsigned char* img = stbi_load(path, &width, &height, &channels, 0);

	if (img == nullptr) return false;



	auto e = stbi_failure_reason();

	aDesc.myResolution.x = width;
	aDesc.myResolution.y = height;




	if (channels == 3)
	{
		std::vector<unsigned char> imageData(width * height * 4);
		for (int i = 0; i < width * height; i++)
		{
			imageData[4 * i + 0] = img[3 * i + 0];
			imageData[4 * i + 1] = img[3 * i + 1];
			imageData[4 * i + 2] = img[3 * i + 2];
			imageData[4 * i + 3] = 255;
		}

		aDesc.myRgbaPixels = imageData.data();

		return anOutput->Init(aPipeline, aDesc);
	}
	else if (channels == 4)
	{
		aDesc.myRgbaPixels = img;
		return anOutput->Init(aPipeline, aDesc);
	}
	else
		return false;

}

const bool Dragonite::TextureFactory::LoadTexture_Impl(TextureRef& anOutput, const wchar_t* aPath, TextureLoaderDesc& aDesc)
{
	bool result = false;




	switch (aDesc.myLoaderType)
	{
	case Dragonite::TextureLoaderDesc::LoaderType::PNG:
		result = LoadPNG(myPipeline, aPath, anOutput, aDesc);
		break;
	case Dragonite::TextureLoaderDesc::LoaderType::DDS:
		result = LoadDDS(myPipeline->GetDevice(), aPath, anOutput->myResourceView, anOutput->myResolution);
		anOutput->mySlot = aDesc.myTargetSlot;
		break;
	default:
		break;
	}




	return result;
}
