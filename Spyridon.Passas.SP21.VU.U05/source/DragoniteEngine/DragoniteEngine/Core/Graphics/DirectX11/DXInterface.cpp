#include "DXInterface.h"
#include "DXUtilities.h"
#include <d3d11.h>
#include <fstream>

Dragonite::DXInterface Dragonite::DXInterface::myInstance;
Dragonite::Device Dragonite::DXInterface::Device;
Dragonite::DeviceContext Dragonite::DXInterface::Context;
Dragonite::SwapChain Dragonite::DXInterface::SwapChain;
int Dragonite::DXInterface::VSyncState;
Dragonite::Color Dragonite::DXInterface::ClearColor;
bool Dragonite::DXInterface::Fullscreen;


const bool Dragonite::DXInterface::Init(HWND anInstance)
{
	myInstance = DXInterface();

	myInstance.myWindowInstance = anInstance;
	if (!RefreshRender())
		return false;



	SamplerDesc desc(
		D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_WRAP,
		Color(0, 0, 0, 0));
	if (FAILED(CreateSampler(0, &desc))) {
		return false;
	}


	AlphaBlend aBlend = AlphaBlend();
	if (FAILED(CreateBlendState(BLEND_ALPHA, &aBlend)))
	{
		return false;
	}

	AdditiveBlend adBlend = AdditiveBlend();
	if (FAILED(CreateBlendState(BLEND_ADDITIVE, &adBlend)))
	{
		return false;
	}
	NormalBlend nBlend = NormalBlend();
	if (FAILED(CreateBlendState(BLEND_NONE, &nBlend)))
	{
		return false;
	}

	return true;
}

void Dragonite::DXInterface::Present()
{
	SwapChain->Present(VSyncState, 0);
}

const bool Dragonite::DXInterface::RefreshRender()
{
	if (FAILED(InitializeSwapChain()))
		return false;

	if (FAILED(myInstance.InitializeBackBuffer()))
		return false;

	return true;
}

const HRESULT Dragonite::DXInterface::InitializeSwapChain(HWND anInstance)
{
	HWND h = anInstance ? anInstance : myInstance.myWindowInstance;


	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = h;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = !Fullscreen;

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
		&SwapChain,
		&Device,
		nullptr,
		&Context
	);
}

const HRESULT Dragonite::DXInterface::CreateSampler(const unsigned int aKey, SamplerDesc* aSamplerDesc)
{
	return Device->CreateSamplerState(aSamplerDesc, myInstance.mySamplers[aKey].GetAddressOf());
}



const HRESULT Dragonite::DXInterface::CreateBlendState(const unsigned int aKey, D3D11_BLEND_DESC* aDesc)
{
	HRESULT r;
	r = Device->CreateBlendState(aDesc, myInstance.myBlendStates[aKey].GetAddressOf());
	return r;
}

const HRESULT Dragonite::DXInterface::CreateBuffer(DataBuffer& aBuffer, BufferDesc* someBufferDesc)
{
	auto desc = someBufferDesc;

	if (desc->ContainsData()) {

		auto subData = desc->GetSubResourceData();
		return Device->CreateBuffer(desc, &subData, &aBuffer);
	}

	return Device->CreateBuffer(desc, nullptr, &aBuffer);

}

const HRESULT Dragonite::DXInterface::CreateTexture(DXTexture2D& aTexture, D3D11_TEXTURE2D_DESC* someTextureDesc)
{
	return Device->CreateTexture2D(someTextureDesc, nullptr, &aTexture);
}

const HRESULT Dragonite::DXInterface::ModifyBuffer(DataBuffer& aBuffer, Data&& someData)
{
	if (!someData.GetData() || someData.GetDataSize() <= 0 || !aBuffer) return E_INVALIDARG;


	D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
	Context->Map(aBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	memcpy(mappedBuffer.pData, someData.GetData(), someData.GetDataSize());
	Context->Unmap(aBuffer.Get(), 0);

	return S_OK;
}

const HRESULT Dragonite::DXInterface::CreateDepthStencil(const DXTexture2D& aTexture, DepthStencil& aDepthStencil)
{
	auto r = Device->CreateDepthStencilView(aTexture.Get(), nullptr, &aDepthStencil);
	return r;
}

const HRESULT Dragonite::DXInterface::CreateVSInstance(const char* aPath, VertexShader& aShader, std::string& someExtraData)
{
	typedef std::istreambuf_iterator<char> ShaderIterator;
	std::string path = "resources/shaders/" + std::string(aPath) + "_VS.cso";
	std::ifstream file(path, std::ios::binary);

	if (!file || file.fail())
		return E_ACCESSDENIED;

	bool hasFailed = file.is_open();
	someExtraData = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
	file.close();
	hasFailed = file.is_open();

	HRESULT r = Device->CreateVertexShader(someExtraData.data(), someExtraData.size(), nullptr, &aShader);
	return r;
}

const HRESULT Dragonite::DXInterface::CreatePSInstance(const char* aPath, PixelShader& aShader)
{
	typedef std::istreambuf_iterator<char> ShaderIterator;
	std::string data;

	std::ifstream file;
	std::string path = "resources/shaders/" + std::string(aPath) + "_PS.cso";
	file.open(path, std::ios::binary);
	data = { ShaderIterator(file), ShaderIterator() };

	return Device->CreatePixelShader(data.data(), data.size(), nullptr, &aShader);
}

const HRESULT Dragonite::DXInterface::SetViewport(const Vector2f& aNewViewport, const Vector2f& aTopLeftCoord, const float aMinDepth, const float aMaxDepth)
{
	D3D11_VIEWPORT newViewport;
	newViewport.TopLeftX = aTopLeftCoord.x;
	newViewport.TopLeftY = aTopLeftCoord.y;
	newViewport.Width = aNewViewport.x;
	newViewport.Height = aNewViewport.y;
	newViewport.MinDepth = aMinDepth;
	newViewport.MaxDepth = aMaxDepth;
	
	Context->RSSetViewports(1, &newViewport);
	return S_OK;
}

const Dragonite::Vector2f Dragonite::DXInterface::GetViewportResolution()
{
	D3D11_VIEWPORT port;
	UINT amm = 1;
	Context->RSGetViewports(&amm, &port);
	return Vector2f(port.Width, port.Height);
}

Dragonite::TextureSampler Dragonite::DXInterface::GetSampler(unsigned int aKey)
{
	return myInstance.mySamplers[aKey];
}

void Dragonite::DXInterface::SetBlendState(unsigned int aNewState)
{
	Context->OMSetBlendState(myInstance.myBlendStates[aNewState].Get(), nullptr, 0xffffffff);
}

void Dragonite::DXInterface::DrawTo(RenderView aTarget, DepthStencil aDepthStencil)
{
	Context->OMSetRenderTargets(1, aTarget.GetAddressOf(), aDepthStencil ? aDepthStencil.Get() : nullptr);
}

void Dragonite::DXInterface::DrawToBackBuffer()
{
	SwitchRenderTarget(myInstance.myBackBuffer, myInstance.myDepthBuffer);
}

void Dragonite::DXInterface::ClearTarget(RenderView aTarget, DepthStencil aDepthStencil)
{
	Context->ClearRenderTargetView(aTarget.Get(), &ClearColor);
	if (aDepthStencil)
		Context->ClearDepthStencilView(aDepthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
}

void Dragonite::DXInterface::SwitchRenderTarget(RenderView aTarget, DepthStencil aDepthStencil)
{
	DrawTo(aTarget, aDepthStencil);
	ClearTarget(aTarget, aDepthStencil);
}

const HRESULT Dragonite::DXInterface::InitializeBackBuffer()
{
	ID3D11Texture2D* backBufferTexture;

	if (FAILED(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture)))
	{
		return  E_INVALIDARG;
	}

	if (FAILED(Device->CreateRenderTargetView(backBufferTexture, nullptr, &myBackBuffer)))
	{
		return E_INVALIDARG;
	}


	DXTexture2D depthBufferText;
	D3D11_TEXTURE2D_DESC textureDesc;


	backBufferTexture->GetDesc(&textureDesc);
	backBufferTexture->Release();


	D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };
	depthBufferDesc.Width = static_cast<unsigned int>(textureDesc.Width);
	depthBufferDesc.Height = static_cast<unsigned int>(textureDesc.Height);
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;


	if (FAILED(CreateTexture(depthBufferText, &depthBufferDesc)))
	{
		return E_INVALIDARG;
	}


	if (FAILED(CreateDepthStencil(depthBufferText, myDepthBuffer))) {
		return E_INVALIDARG;
	}

	SetViewport(Vector2f(textureDesc.Width, textureDesc.Height));

	return S_OK;
}
