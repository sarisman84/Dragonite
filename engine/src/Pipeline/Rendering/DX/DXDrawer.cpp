#include "DXDrawer.h"
#include <d3d11.h>
#include <cassert>

#pragma comment(lib, "d3d11.lib")

Dragonite::DXDrawer::DXDrawer()
{
	mySwapChain = nullptr;
	myDevice = nullptr;
	myContext = nullptr;

}

Dragonite::DXDrawer::~DXDrawer()
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

void Dragonite::DXDrawer::Present(bool aVSyncState)
{
	mySwapChain->Present(aVSyncState, 0);
}

void Dragonite::DXDrawer::Init(HWND anInstance)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = anInstance;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = true;

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


void Dragonite::DXDrawer::SetRenderTarget(void* aTargetBuffer, void* aDepthBuffer)
{
	ID3D11RenderTargetView* tView = (ID3D11RenderTargetView*)aTargetBuffer;
	ID3D11DepthStencilView* dView = (ID3D11DepthStencilView*)aDepthBuffer;

	myContext->OMSetRenderTargets(1, &tView, dView);
	ClearRenderTarget(aTargetBuffer, aDepthBuffer);
}

void Dragonite::DXDrawer::ClearRenderTarget(void* aTargetBuffer, void* aDepthBuffer)
{
	ID3D11RenderTargetView* tView = (ID3D11RenderTargetView*)aTargetBuffer;
	ID3D11DepthStencilView* dView = (ID3D11DepthStencilView*)aDepthBuffer;

	float color[4] = { 0,0,0,1 };
	myContext->ClearRenderTargetView(tView, color);
	if (dView)
		myContext->ClearDepthStencilView(dView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
}


Dragonite::RTView::RTView()
{
	myRenderTargetView = nullptr;
}

Dragonite::RTView::~RTView()
{
	if (myRenderTargetView)
		myRenderTargetView->Release();
	myRenderTargetView = nullptr;
}

//void** Dragonite::RTView::EditContent()
//{
//	return (void**)&myRenderTargetView;
//}
//
//void* Dragonite::RTView::GetContent() const
//{
//	return (void*)myRenderTargetView;
//}



Dragonite::DSView::DSView()
{
	myDepthStencilView = nullptr;
}

Dragonite::DSView::~DSView()
{
	if (myDepthStencilView)
		myDepthStencilView->Release();
	myDepthStencilView = nullptr;
}

//void** Dragonite::DSView::EditContent()
//{
//	return (void**)myDepthStencilView;
//}
//
//void* Dragonite::DSView::GetContent() const
//{
//	return (void*)myDepthStencilView;
//}
