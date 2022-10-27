#include "RenderTarget.h"
#include "Core/Graphics/DirectX11/DXIncludes.h"
#include "Core/Graphics/DirectX11/DXInterface.h"
#include <d3d11.h>
#include <cassert>

#include "Core/Graphics/GraphicsAPI.h"


void Dragonite::RenderTarget::InternalInit(const RenderTargetDesc& aDesc)
{
	if (aDesc.myFormat == DXGI_FORMAT_UNKNOWN) return;

	HRESULT result;

	D3D11_TEXTURE2D_DESC desc = { 0 };

	desc.Width = aDesc.myResolution.x;
	desc.Height = aDesc.myResolution.y;

	desc.MipLevels = aDesc.myMipLevels;
	desc.ArraySize = aDesc.myArraySize;

	desc.Format = aDesc.myFormat;
	desc.SampleDesc = aDesc.mySample;
	desc.Usage = (D3D11_USAGE)aDesc.myUseage;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = aDesc.myCPUAccessFlags;
	desc.MiscFlags = aDesc.myMiscFlags;

	DXTexture2D texture;

	result = DXInterface::Device->CreateTexture2D(&desc, nullptr, &texture);
	assert(SUCCEEDED(result));
	result = DXInterface::Device->CreateShaderResourceView(texture.Get(), nullptr, &myResourceView);
	assert(SUCCEEDED(result));
	result = DXInterface::Device->CreateRenderTargetView(texture.Get(), nullptr, &myRenderView);
	assert(SUCCEEDED(result));
}

Dragonite::RenderTarget::RenderTarget() : myPipeline(nullptr) {};

Dragonite::RenderTarget::RenderTarget(GraphicalInterface* aPipeline, const RenderTargetDesc& aDesc)
{
	myPipeline = aPipeline;
	assert(myPipeline != nullptr);

	InternalInit(aDesc);


};



