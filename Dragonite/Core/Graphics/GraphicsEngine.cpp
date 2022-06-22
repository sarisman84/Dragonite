#include "GraphicsEngine.h"
#include <d3d11.h>
#include <fstream>
#include "System.h"
#include "Utilities/Math/Vector2.h"
#include "Components/Camera.h"
#include "Rendering/MeshInfo.h"
#include "Rendering/Framework/Texture.h"
#include "Rendering/Framework/RenderTarget.h"
#define REPORT_DX_WARNINGS

#pragma warning (disable: 26812)

Dragonite::GraphicsEngine::GraphicsEngine() : myRenderInstructions(1000, 100)
{

}


Dragonite::GraphicsEngine::~GraphicsEngine() = default;


bool Dragonite::GraphicsEngine::Initialize(Resolution /*aResolution*/, HWND aWindowsHandle, System * aSystem)
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

	if (FAILED(InitializeConstantBuffer(sizeof(StaticBufferData), myStaticBuffer))) return false;



	if (FAILED(InitializeSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, mySamplerState))) return false;

	myViewport = Resolution{ (int)textureDesc.Width, (int)textureDesc.Height };

	if (FAILED(InitializeDepthBuffer(myViewport, myDepthBuffer))) return false;

	if (FAILED(InitializeRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID, myFrontCuller))) return false;

	StaticBufferData data;
	data.myResolution = { static_cast<float>(myViewport.width), static_cast<float>(myViewport.height) };
	UpdateConstantBuffer(myStaticBuffer, &data, sizeof(StaticBufferData), 0, &ID3D11DeviceContext::PSSetConstantBuffers);
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(myViewport.width);
	viewport.Height = static_cast<float>(myViewport.height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	myContext->RSSetViewports(1, &viewport);






	myLightData.myAmbientLight = { 0.5f,0.5f,0.5f,1 };



	myCubemapTexture = std::make_shared<Texture>(this, "Textures/cube_1024_preblurred_angle3_Skansen3.dds", Texture::Type::Cubemap);

	myCubemapTexture->BindTexture(myContext, 9);





	return true;
}


void Dragonite::GraphicsEngine::DrawElements()
{
	auto position = myRenderCamera->GetTransform()->GetPosition();
	auto size = myRenderCamera->GetTransform()->GetSize();
	myContext->PSSetSamplers(0, 1, mySamplerState.GetAddressOf());

	Math::Matrix4x4f reflectedM;
	Math::Matrix4x4f ogM = myRenderCamera->GetTransform()->GetMatrix();

	reflectedM(2, 2) = -1;
	reflectedM(4, 2) = (-2.f * myRenderTarget->GetTransform()->GetPosition().y);

	reflectedM = ogM * reflectedM;

	myRenderCamera->GetTransform()->GetMatrix() = reflectedM;

	RefreshView(myRenderTarget, myFrontCuller);
	UpdateFrameBuffer();
	RenderInstances();


	myRenderCamera->GetTransform()->GetMatrix() = ogM;


	RefreshView(nullptr);
	UpdateFrameBuffer();
	if (myRenderTarget)
		myRenderTarget->Render(this);
	RenderInstances();







	mySwapChain->Present(1, 0);
	myRenderInstructions.Clear();
}

void Dragonite::GraphicsEngine::UpdateConstantBuffer(ComPtr<ID3D11Buffer>&aConstantBuffer, void* someData, const size_t someDataSize, const UINT aSlot,
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

void Dragonite::GraphicsEngine::UpdateFrameBuffer()
{
	FrameBufferData fData;
	fData.myClipSpaceMatrix = myRenderCamera->GetClipSpaceMatrix();
	fData.myTimeDelta = mySystem->GetTimeDelta();
	fData.myTotalTime = mySystem->GetTotalTime();
	fData.myCameraPosition = Math::Vector4f(myRenderCamera->GetTransform()->GetPosition(), 1);

	fData.myAmbientLight = myLightData.myAmbientLight;
	fData.myLightColor = myLightData.myLightColor;
	fData.myLightDirection = myLightData.myLightDirection;

	UpdateConstantBuffer(myFrameBuffer, &fData, sizeof(FrameBufferData), 1, &ID3D11DeviceContext::VSSetConstantBuffers, &ID3D11DeviceContext::PSSetConstantBuffers);
}

void Dragonite::GraphicsEngine::UpdateObjectBuffer(ModelInstance * anInstance)
{
	ObjectBufferData oData;
	oData.myObjectMatrix = anInstance->myTransform.GetMatrix();
	oData.myColor = anInstance->myMaterial.myColor;
	//oData.mySize = { myRenderTarget->GetTransform()->Position() ,0 };
	UpdateConstantBuffer(myObjectBuffer, &oData, sizeof(ObjectBufferData), 2, &ID3D11DeviceContext::VSSetConstantBuffers);
}

void Dragonite::GraphicsEngine::RefreshView(RenderTarget * aRenderTarget, ComPtr<ID3D11RasterizerState> aCullingMode)
{

	float color[4] = { 0.2f,0.2f,0.9f,1.0f }; // RGBA

	ID3D11RenderTargetView* nullRTV = nullptr;
	myContext->OMSetRenderTargets(1, &nullRTV, nullptr);


	if (aRenderTarget)
	{
		myContext->OMSetRenderTargets(1, aRenderTarget->RenderTexture().GetAddressOf(), myDepthBuffer.Get());
		myContext->ClearRenderTargetView(aRenderTarget->RenderTexture().Get(), color);
	}
	else
	{
		myContext->OMSetRenderTargets(1, myBackBuffer.GetAddressOf(), myDepthBuffer.Get());
		myContext->ClearRenderTargetView(myBackBuffer.Get(), color);
	}

	myContext->ClearDepthStencilView(myDepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	myContext->RSSetState(aCullingMode ? aCullingMode.Get() : nullptr);

}

void Dragonite::GraphicsEngine::RenderInstances()
{
	for (size_t i = 0; i < myRenderInstructions.Size(); i++)
	{
		auto instruction = myRenderInstructions[i];


		UpdateObjectBuffer(instruction);



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
				texture->BindTexture(myContext, t);
			}

			myContext->DrawIndexed(static_cast<UINT>(mesh.myIndiciesAmm), 0, 0);
		}

	}


}

HRESULT Dragonite::GraphicsEngine::InitializeRasterizerState(const D3D11_CULL_MODE aCullMode, const D3D11_FILL_MODE aFillMode, ComPtr<ID3D11RasterizerState>&aRasterizerState)
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = aCullMode;
	desc.FillMode = aFillMode;
	//desc.FrontCounterClockwise = false;
	return myDevice->CreateRasterizerState(&desc, &aRasterizerState);
}

HRESULT Dragonite::GraphicsEngine::InitializeConstantBuffer(const size_t someDataSize, ComPtr<ID3D11Buffer>&aBuffer)
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

HRESULT Dragonite::GraphicsEngine::InitializeSampler(D3D11_FILTER aFilter, D3D11_TEXTURE_ADDRESS_MODE aTileType, ComPtr<ID3D11SamplerState>&aSampler)
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

HRESULT Dragonite::GraphicsEngine::InitializeDepthBuffer(const Resolution & aResolution, ComPtr<ID3D11DepthStencilView>&aDepthBuffer)
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







