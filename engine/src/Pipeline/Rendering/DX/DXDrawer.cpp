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

void Dragonite::InitializeBuffer(ID3D11Device * aDevice, ID3D11Buffer ** aBuffer, const size_t aTypeSize)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = aTypeSize;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	HRESULT hr = aDevice->CreateBuffer(&desc, nullptr, aBuffer);



}

void Dragonite::EditBuffer(ID3D11DeviceContext* aContext, UINT aSlot, ID3D11Buffer* aBuffer, const size_t aTypeSize, void* someData)
{
	D3D11_MAPPED_SUBRESOURCE mR;
	ZeroMemory(&mR, sizeof(D3D11_MAPPED_SUBRESOURCE));

	aContext->Map(aBuffer, aSlot, D3D11_MAP_WRITE_DISCARD, 0, &mR);
	memcpy(mR.pData, someData, aTypeSize);
	aContext->Unmap(aBuffer, aSlot);
}

void Dragonite::BindBuffer(ID3D11DeviceContext* aContext, UINT aBindType, ID3D11Buffer** aBuffer, UINT aSlot)
{
	switch (aBindType)
	{
	case 0:
		aContext->VSSetConstantBuffers(aSlot, 1, aBuffer);
		break;
	case 1:
		aContext->PSSetConstantBuffers(aSlot, 1, aBuffer);
		break;
	default:
		break;
	}
	
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
