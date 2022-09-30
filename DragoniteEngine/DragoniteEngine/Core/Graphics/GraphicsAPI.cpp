#include "GraphicsAPI.h"
#include "../CU/CommonData.h"
#include "../Application.h"
#include "Models/ModelFactory.h"
#include "Textures/TextureFactory.h"
#include "Camera.h"
#include <d3d11.h>
#define REPORT_DX_WARNINGS

#pragma warning (disable: 4267)

Dragonite::GraphicsPipeline::GraphicsPipeline()
{
	myModelFactory = nullptr;
	myTextureFactory = nullptr;
}


Dragonite::GraphicsPipeline::~GraphicsPipeline()
{
	if (myModelFactory)
		delete myModelFactory;
	myModelFactory = nullptr;

	if (myTextureFactory)
		delete myTextureFactory;
	myTextureFactory = nullptr;
}

bool Dragonite::GraphicsPipeline::Initialize(Runtime* anApplication, HWND aWindowHandle)
{
	myClearColor = Color(0.25f, 0.25f, 0.85f, 1.0f);

	if (FAILED(InitializeSwapChain(aWindowHandle)))
		return false;

	if (FAILED(InitializeBackBuffer()))
		return false;

	if (FAILED(InitializeSamplers()))
		return false;





	DataBufferDesc bufferDesc(
		sizeof(FrameBufferData),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE);

	if (FAILED(CreateBuffer(myDevice, myFrameBuffer, bufferDesc)))
		return false;


	bufferDesc.mySize = sizeof(ObjectBufferData);

	if (FAILED(CreateBuffer(myDevice, myObjectBuffer, bufferDesc)))
		return false;

	myApplicationPtr = anApplication;
	myApplicationPtr->GetPollingStation().AddHandler(this);

	myModelFactory = new ModelFactory();
	if (!myModelFactory->Initialize(this))
	{
		return false;
	}
	myTextureFactory = new TextureFactory(this);


	myApplicationPtr->GetPollingStation().AddHandler(myTextureFactory);
	myApplicationPtr->GetPollingStation().AddHandler(myModelFactory);

	return true;
}

void Dragonite::GraphicsPipeline::Render()
{
	myContext->ClearRenderTargetView(myBackBuffer.Get(), &myClearColor);
	myContext->ClearDepthStencilView(myDepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	myApplicationPtr->OnPreRender()();

	auto cpy = myElementsToDraw;
	myElementsToDraw.clear();

	myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (myActiveCamera)
	{
		FrameBufferData frameBufferData = {};
		frameBufferData.myWorldToClipMatrix = myActiveCamera->WorldToClipSpace();
		if (myFrameBuffer)
			if (FAILED(SetBuffer(myContext, myFrameBuffer, &frameBufferData, sizeof(FrameBufferData))))
			{
				//TODO: Handle this error
			}
			else
				myContext->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	}




	while (!cpy.empty())
	{
		auto element = cpy.back();
		cpy.pop_back();



		if (element->myTexture)
			element->myTexture->Bind(myContext);

		{
			ObjectBufferData objectBufferData = {};
			objectBufferData.myModelToWorldMatrix = element->myTransform.GetMatrix();

			if (myObjectBuffer)
				if (FAILED(SetBuffer(myContext, myObjectBuffer, &objectBufferData, sizeof(ObjectBufferData))))
				{
					//TODO: Handle this error
				}
				else
					myContext->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
		}

		{
			myContext->IASetInputLayout(element->myInputLayout.Get());

			unsigned int stride = sizeof(Vertex);
			unsigned int offset = 0;

			myContext->IASetVertexBuffers(0, 1, element->myModel->myVertexBuffer.GetAddressOf(), &stride, &offset);
			myContext->IASetIndexBuffer(element->myModel->myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			myContext->PSSetSamplers(0, 1, &myTextureSamplers[TextureSampleType::Default]);

			myContext->VSSetShader(element->myVertexShader.Get(), nullptr, 0);
			myContext->PSSetShader(element->myPixelShader.Get(), nullptr, 0);
		}



		myContext->DrawIndexed(element->myModel->myIndexCount, 0, 0);

	}


	myApplicationPtr->OnRender()();


	mySwapChain->Present(1, 0);
}

HRESULT Dragonite::GraphicsPipeline::InitializeSwapChain(HWND anInstance)
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

HRESULT Dragonite::GraphicsPipeline::InitializeBackBuffer()
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

	DXTexture2D depthBufferText;
	D3D11_TEXTURE2D_DESC textureDesc;


	backBufferTexture->GetDesc(&textureDesc);
	backBufferTexture->Release();

	TextureBufferDesc depthBufferTextDesc(Vector2ui(textureDesc.Width, textureDesc.Height), DXGI_FORMAT_D32_FLOAT, D3D11_BIND_DEPTH_STENCIL);

	if (FAILED(CreateTexture(myDevice, depthBufferText, depthBufferTextDesc)))
	{
		return E_INVALIDARG;
	}

	if (FAILED(myDevice->CreateDepthStencilView(depthBufferText.Get(), nullptr, &myDepthBuffer)))
	{
		return E_INVALIDARG;
	}



	myContext->OMSetRenderTargets(1, myBackBuffer.GetAddressOf(), myDepthBuffer.Get());
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


HRESULT CreateSampler(Dragonite::GraphicsPipeline* aPipeline, Dragonite::TextureSampleType aType, D3D11_SAMPLER_DESC aDesc)
{
	return aPipeline->GetDevice()->CreateSamplerState(&aDesc, &(aPipeline->GetTextureSamplers()[aType]));
}

HRESULT Dragonite::GraphicsPipeline::InitializeSamplers()
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT result = CreateSampler(this, TextureSampleType::Default, samplerDesc);

	return result;
}


void Dragonite::GraphicsPipeline::DrawToNewRenderTarget(const RenderTarget& aTarget, const RasterizerState& aNewState)
{
	myContext->OMSetRenderTargets(1, aTarget.GetAddressOf(), myDepthBuffer.Get());
	myContext->ClearRenderTargetView(aTarget.Get(), &myClearColor);
	myContext->ClearDepthStencilView(myDepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	myContext->RSSetState(aNewState ? aNewState.Get() : nullptr);
}

void Dragonite::GraphicsPipeline::DrawToBackBuffer()
{
	DrawToNewRenderTarget(myBackBuffer);
}

HRESULT Dragonite::GraphicsPipeline::CreateBuffer(Device aDevice, DataBuffer& aBuffer, const DataBufferDesc& aDesc)
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = aDesc.mySize;
	desc.Usage = aDesc.myUsage;
	desc.BindFlags = aDesc.myBindFlag;
	desc.CPUAccessFlags = aDesc.myCPUFlags;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	if (aDesc.myData)
	{
		data.pSysMem = aDesc.myData;

	}
	return aDevice->CreateBuffer(&desc, aDesc.myData ? &data : nullptr, &aBuffer);
}

HRESULT Dragonite::GraphicsPipeline::CreateTexture(Device aDevice, DXTexture2D& aTexture, const TextureBufferDesc& aDesc)
{
	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	textureDesc.Width = aDesc.myTextureRes.x;
	textureDesc.Height = aDesc.myTextureRes.y;
	textureDesc.ArraySize = aDesc.myArraySize;
	textureDesc.Format = (DXGI_FORMAT)aDesc.myFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.BindFlags = aDesc.myBindFlags;

	return aDevice->CreateTexture2D(&textureDesc, nullptr, &aTexture);
}

HRESULT Dragonite::GraphicsPipeline::SetBuffer(DeviceContext aContext, DataBuffer& aBuffer, void* someData, size_t aDataSize)
{
	if (!someData || aDataSize <= 0) return E_INVALIDARG;

	D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
	aContext->Map(aBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	memcpy(mappedBuffer.pData, someData, aDataSize);
	aContext->Unmap(aBuffer.Get(), 0);

	return S_OK;
}

Dragonite::RenderInterface::RenderInterface(GraphicsPipeline& aPipeline) : myPipeline(aPipeline)
{

}

void Dragonite::RenderInterface::DrawElement(std::shared_ptr<ModelInstance> anInstance)
{
	myPipeline.myElementsToDraw.push_back(anInstance);
}
