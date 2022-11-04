#include "Viewport.h"

#include <d3d11.h>
#include "DirectX/DXUtilities.h"

void EmberGUILayout::InitRenderTarget(ID3D11Device* aDevice, const float aWidth, const float aHeight, ID3D11RenderTargetView** aTargetView, ID3D11ShaderResourceView** aShaderView)
{
	D3D11_TEXTURE2D_DESC desc = { 0 };

	desc.Width = aWidth;
	desc.Height = aHeight;

	desc.MipLevels = 1;
	desc.ArraySize = 1;

	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ID3D11Texture2D* texture;

	HRESULT result = aDevice->CreateTexture2D(&desc, nullptr, &texture);
	assert(SUCCEEDED(result));
	result = aDevice->CreateShaderResourceView(texture, nullptr, aShaderView);
	assert(SUCCEEDED(result));
	result = aDevice->CreateRenderTargetView(texture, nullptr, aTargetView);
	assert(SUCCEEDED(result));
}

void EmberGUILayout::ReleaseElements(ID3D11RenderTargetView* aTargetView, ID3D11ShaderResourceView* aShaderView)
{
	if (aTargetView)
		aTargetView->Release();
	aTargetView = nullptr;

	if (aShaderView)
		aShaderView->Release();
	aShaderView = nullptr;
}
