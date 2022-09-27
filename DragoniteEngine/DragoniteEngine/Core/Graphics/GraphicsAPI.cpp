#include "GraphicsAPI.h"
#include "../CU/CommonData.h"
#include <d3d11.h>
#define REPORT_DX_WARNINGS

Dragonite::GraphicsAPI::GraphicsPipeline::GraphicsPipeline() = default;


Dragonite::GraphicsAPI::GraphicsPipeline::~GraphicsPipeline() = default;

bool Dragonite::GraphicsAPI::GraphicsPipeline::Initialize(HWND aWindowHandle)
{
	myClearColor = Color(0.25f, 0.25f, 0.85f, 1.0f);

	if (FAILED(InitializeSwapChain(aWindowHandle)))
		return false;

	if (FAILED(InitializeBackBuffer()))
		return false;

	return true;
}

void Dragonite::GraphicsAPI::GraphicsPipeline::Render()
{
	myContext->ClearRenderTargetView(myBackBuffer.Get(), &myClearColor);

	mySwapChain->Present(1, 0);
}

HRESULT Dragonite::GraphicsAPI::GraphicsPipeline::InitializeSwapChain(HWND anInstance)
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



	return D3D11CreateDeviceAndSwapChain(
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
}

HRESULT Dragonite::GraphicsAPI::GraphicsPipeline::InitializeBackBuffer()
{
	ID3D11Texture2D* backBufferTexture;
	HRESULT result = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture);
	if (FAILED(result))
	{
		return result;
	}
	result = myDevice->CreateRenderTargetView(backBufferTexture, nullptr, &myBackBuffer);
	if (FAILED(result))
	{
		return result;
	}
	D3D11_TEXTURE2D_DESC textureDesc;
	backBufferTexture->GetDesc(&textureDesc);
	backBufferTexture->Release();
	myContext->OMSetRenderTargets(1, myBackBuffer.GetAddressOf(), nullptr);
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(textureDesc.Width);
	viewport.Height = static_cast<float>(textureDesc.Height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	myContext->RSSetViewports(1, &viewport);
	return S_OK;
}
