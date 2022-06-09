#include "GraphicsEngine.h"
#include <d3d11.h>
#include <fstream>
#include "System.h"
#include "Utilities/Math/Vector2.h"
#include "Components/Camera.h"
#include "Rendering/MeshInfo.h"
#define REPORT_DX_WARNINGS

#pragma warning (disable: 26812)

Engine::Graphics::GraphicsEngine::GraphicsEngine() : myRenderInstructions(1000, 100)
{

}


Engine::Graphics::GraphicsEngine::~GraphicsEngine() = default;


bool Engine::Graphics::GraphicsEngine::Initialize(Resolution aResolution, HWND aWindowsHandle, System * aSystem)
{

	mySystem = aSystem;

	HRESULT result;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = aWindowsHandle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = true;

	UINT creationFlags = 0;
#if defined(REPORT_DX_WARNINGS)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	result = D3D11CreateDeviceAndSwapChain(
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


	ID3D11Texture2D* backBufferTexture;
	result = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture);
	if (FAILED(result))
	{
		return false;
	}

	result = myDevice->CreateRenderTargetView(backBufferTexture, nullptr, &myBackBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	backBufferTexture->GetDesc(&textureDesc);
	backBufferTexture->Release();



	if (FAILED(InitializeConstantBuffer(sizeof(FrameBufferData), myFrameBuffer))) return false;

	if (FAILED(InitializeConstantBuffer(sizeof(ObjectBufferData), myObjectBuffer))) return false;

	if (FAILED(InitializeConstantBuffer(sizeof(MaterialBufferData), myMaterialBuffer))) return false;

	if (FAILED(InitializeConstantBuffer(sizeof(GlobalLightBufferData), myGlobalLightBuffer))) return false;

	if (FAILED(InitializeSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, mySamplerState))) return false;

	if (FAILED(InitializeDepthBuffer(Resolution{ (int)textureDesc.Width, (int)textureDesc.Height }, myDepthBuffer))) return false;

	//D3D11_TEXTURE2D_DESC textureDesc;
	//backBufferTexture->GetDesc(&textureDesc);
	//backBufferTexture->Release();
	myContext->OMSetRenderTargets(1, myBackBuffer.GetAddressOf(), myDepthBuffer.Get());
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(aResolution.width);
	viewport.Height = static_cast<float>(aResolution.height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	myContext->RSSetViewports(1, &viewport);






	myLightData.myAmbientLight = { 1,1,1,1 };

	return true;
}


void Engine::Graphics::GraphicsEngine::DrawElements()
{
	float color[4] = { 0.2f,0.2f,0.2f,1.0f }; // RGBA
	myContext->ClearRenderTargetView(myBackBuffer.Get(), color);
	myContext->ClearDepthStencilView(
		myDepthBuffer.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);
	myContext->PSSetSamplers(0, 1, mySamplerState.GetAddressOf());

	FrameBufferData fData;
	fData.myClipSpaceMatrix = myRenderCamera->GetClipSpaceMatrix();
	fData.myTimeDelta = mySystem->GetTimeDelta();
	fData.myTotalTime = mySystem->GetTotalTime();

	UpdateConstantBuffer(myFrameBuffer, &fData, sizeof(FrameBufferData), 0, &ID3D11DeviceContext::VSSetConstantBuffers, &ID3D11DeviceContext::PSSetConstantBuffers);

	UpdateConstantBuffer(myGlobalLightBuffer, &myLightData, sizeof(GlobalLightBufferData), 3, &ID3D11DeviceContext::PSSetConstantBuffers);

	RenderInstances();




	mySwapChain->Present(1, 0);
}

void Engine::Graphics::GraphicsEngine::UpdateConstantBuffer(ComPtr<ID3D11Buffer>&aConstantBuffer, void* someData, const size_t someDataSize, const UINT aSlot,
	void (ID3D11DeviceContext:: * anOnConstantBufferUpdateCallback)(UINT aStartSlot, UINT aNumBuffers, ID3D11Buffer* const* aConstantBuffer),
	void (ID3D11DeviceContext:: * anotherOnConstantBufferUpdateCallback)(UINT aStartSlot, UINT aNumBuffers, ID3D11Buffer* const* aConstantBuffer))
{

	D3D11_MAPPED_SUBRESOURCE resource;
	myContext->Map(aConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, someData, someDataSize);
	myContext->Unmap(aConstantBuffer.Get(), 0);

	if (anOnConstantBufferUpdateCallback != 0)
		(myContext.Get()->*anOnConstantBufferUpdateCallback)(aSlot, 1, aConstantBuffer.GetAddressOf());
	if (anotherOnConstantBufferUpdateCallback != 0)
		(myContext.Get()->*anotherOnConstantBufferUpdateCallback)(aSlot, 1, aConstantBuffer.GetAddressOf());
}

void Engine::Graphics::GraphicsEngine::RenderInstances()
{
	for (size_t i = 0; i < myRenderInstructions.Size(); i++)
	{
		auto instruction = myRenderInstructions[i];

		ObjectBufferData oData;
		oData.myObjectMatrix = instruction->myTransform.GetMatrix();
		UpdateConstantBuffer(myObjectBuffer, &oData, sizeof(ObjectBufferData), 1, &ID3D11DeviceContext::VSSetConstantBuffers);

		MaterialBufferData mData;
		mData.myColor = instruction->myMaterial.myColor;
		UpdateConstantBuffer(myMaterialBuffer, &mData, sizeof(MaterialBufferData), 2, &ID3D11DeviceContext::PSSetConstantBuffers);


		myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		myContext->IASetInputLayout(instruction->myModel->myInputLayout.Get());

		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;



		for (auto& mesh : instruction->myModel->myMesh)
		{
			myContext->IASetVertexBuffers(0, 1, mesh.myVertexBuffer.GetAddressOf(), &stride, &offset);
			myContext->IASetIndexBuffer(mesh.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			myContext->VSSetShader(instruction->myModel->myVertexShader.Get(), nullptr, 0);
			myContext->PSSetShader(instruction->myModel->myPixelShader.Get(), nullptr, 0);

			for (size_t t = 0; t < mesh.myTextureBuffer.size(); t++)
			{
				auto& texture = mesh.myTextureBuffer[t];
				/*myContext->PSSetShaderResources(texture.mySlot, 1, texture.myTextureResource.GetAddressOf());*/
				texture.BindTexture(myContext, t);
			}

			myContext->DrawIndexed(static_cast<UINT>(mesh.myIndiciesAmm), 0, 0);
		}

	}

	myRenderInstructions.Clear();
}

HRESULT Engine::Graphics::GraphicsEngine::InitializeConstantBuffer(const size_t someDataSize, ComPtr<ID3D11Buffer>&aBuffer)
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDesc.ByteWidth = someDataSize;
	result = myDevice->CreateBuffer(&bufferDesc, nullptr, &aBuffer);
	return result;
}

HRESULT Engine::Graphics::GraphicsEngine::InitializeSampler(D3D11_FILTER aFilter, D3D11_TEXTURE_ADDRESS_MODE aTileType, ComPtr<ID3D11SamplerState>&aSampler)
{


	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = aFilter;
	samplerDesc.AddressU = aTileType;
	samplerDesc.AddressV = aTileType;
	samplerDesc.AddressW = aTileType;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;


	return myDevice->CreateSamplerState(&samplerDesc, &aSampler);
}

HRESULT Engine::Graphics::GraphicsEngine::InitializeDepthBuffer(const Resolution & aResolution, ComPtr<ID3D11DepthStencilView>&aDepthBuffer)
{
	HRESULT result;
	ID3D11Texture2D* depthBufferTexture;
	D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };
	depthBufferDesc.Width = static_cast<unsigned int>(aResolution.width);
	depthBufferDesc.Height = static_cast<unsigned int>(aResolution.height);
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	result = myDevice->CreateTexture2D(&depthBufferDesc, nullptr, &depthBufferTexture);
	if (FAILED(result))
	{
		return result;
	}
	result = myDevice->CreateDepthStencilView(depthBufferTexture, nullptr, &aDepthBuffer);
	depthBufferTexture->Release();
	return result;
}







