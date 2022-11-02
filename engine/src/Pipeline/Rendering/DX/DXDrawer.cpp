#include "DXDrawer.h"
#include <d3d11.h>
#include <cassert>
Dragonite::DirectXDrawer::DirectXDrawer()
{
	mySwapChain = nullptr;
	myDevice = nullptr;
	myContext = nullptr;

}

Dragonite::DirectXDrawer::~DirectXDrawer()
{
	if (mySwapChain)
		mySwapChain->Release();
	if (myDevice)
		myDevice->Release();
	if (myContext)
		myContext->Release();

	mySwapChain = nullptr;
	myDevice = nullptr;
	myContext = nullptr;
}

void Dragonite::DirectXDrawer::Present(bool aVSyncState)
{
	mySwapChain->Present(aVSyncState, 0);
}

void Dragonite::DirectXDrawer::Init(HWND anInstance)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = anInstance;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = false;

	UINT creationFlags = 0;
#if defined(REPORT_DX_WARNINGS)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif



	HRESULT r = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mySwapChain,
		&myDevice,
		nullptr,
		&myContext
	);

	assert(SUCCEEDED(r) && "Failed to create swap chain");
}

void Dragonite::DirectXDrawer::SetRenderTarget(IContent* aTargetBuffer, IContent* aDepthBuffer)
{
}

void Dragonite::DirectXDrawer::ClearRenderTarget(IContent* aTargetBuffer, IContent* aDepthBuffer)
{
}
