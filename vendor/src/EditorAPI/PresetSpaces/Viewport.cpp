#include "Viewport.h"

#include <d3d11.h>
#include "DirectX/DXUtilities.h"

void InitRenderTarget(ID3D11Device* aDevice, const float aWidth, const float aHeight, ID3D11RenderTargetView* aTargetView, ID3D11ShaderResourceView* aShaderView)
{
	DirectX::RenderTargetDesc desc = {};
	desc.myArraySize = 1;
	desc.myCPUAccessFlags = 0;
	desc.myMiscFlags = 0;
	desc.myMipLevels = 1;

	desc.myFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.myUseage = (UINT)D3D11_USAGE_DEFAULT;
	desc.myWidth = aWidth;
	desc.myHeight = aHeight;

	DirectX::CreateRenderTarget(aDevice, desc, &aTargetView, &aShaderView);
}
void SwitchRenderView(ID3D11DeviceContext* aContext, ID3D11RenderTargetView* aTargetView)
{
	float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	aContext->OMSetRenderTargets(1, &aTargetView, nullptr);
	aContext->ClearRenderTargetView(aTargetView, color);
}