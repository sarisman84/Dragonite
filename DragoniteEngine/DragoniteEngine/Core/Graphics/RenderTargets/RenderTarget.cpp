#include "RenderTarget.h"
#include <d3d11.h>
#include <cassert>


Dragonite::RenderTarget* Dragonite::RenderTarget::CreateTarget(Device aDevice, const RenderTargetDesc& aDesc)
{
	HRESULT result;
	RenderTarget* target = new RenderTarget();

	D3D11_TEXTURE2D_DESC desc = { 0 };

	desc.Width = aDesc.myResolution.x;
	desc.Height = aDesc.myResolution.y;

	desc.MipLevels = aDesc.myMipLevels;
	desc.ArraySize = aDesc.myArraySize;

	desc.Format = aDesc.myFormat;
	desc.SampleDesc = aDesc.mySample;
	desc.Usage = (D3D11_USAGE)aDesc.myUseage;
	desc.BindFlags = aDesc.myBindFlags;
	desc.CPUAccessFlags = aDesc.myCPUAccessFlags;
	desc.MiscFlags = aDesc.myMiscFlags;

	DXTexture2D texture;

	result = aDevice->CreateTexture2D(&desc, nullptr, &texture);
	assert(SUCCEEDED(result));
	result = aDevice->CreateShaderResourceView(texture.Get(), nullptr, &target->myResourceView);
	assert(SUCCEEDED(result));
	result = aDevice->CreateRenderTargetView(texture.Get(), nullptr, &target->myRenderView);



	return target;
}
