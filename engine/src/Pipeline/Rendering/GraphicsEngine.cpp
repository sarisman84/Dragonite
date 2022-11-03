#include "GraphicsEngine.h"
#include "DirectX/DXUtilities.h"
#include <d3d11.h>
#include <cassert>
#include "Pipeline/Rendering/DX/DXDrawer.h"

Dragonite::GraphicsEngine::GraphicsEngine()
{
	myDrawer = nullptr;
	myBackBuffer = nullptr;
	myDepthBuffer = nullptr;
}

Dragonite::GraphicsEngine::~GraphicsEngine()
{
	if (myDrawer)
		delete myDrawer;
	myDrawer = nullptr;

	if (myBackBuffer)
		myBackBuffer->Release();
	myBackBuffer = nullptr;

	if (myDepthBuffer)
		myDepthBuffer->Release();
	myDepthBuffer = nullptr;
}

void Dragonite::GraphicsEngine::Draw()
{
	for (auto& call : myInstructions)
	{
		DirectXDrawer* dxDrawer = (DirectXDrawer*)myDrawer;
		auto context = dxDrawer->Context();


		context->IASetInputLayout(myInputLayout[InputLayoutID(call.first)]);

		context->VSSetShader(myVertexShaders[VertexShaderID(call.first)], nullptr, 0);
		context->PSSetShader(myPixelShaders[PixelShaderID(call.first)], nullptr, 0);
		

		for (auto& renderCall : call.second)
		{
			renderCall->Render();
		}
	}

	myDrawer->Present(false);
}

Dragonite::GraphicsEngine* Dragonite::GraphicsEngine::InitializeEngine(HWND anInstance)
{
	GraphicsEngine* engine = new GraphicsEngine();
	engine->Init(anInstance);
	return engine;
}

void Dragonite::GraphicsEngine::Init(HWND anInstance)
{
	DirectXDrawer*  dxDrawer = new DirectXDrawer();

	dxDrawer->Init(anInstance);




	ID3D11Texture2D* backBufferTexture;

	assert(SUCCEEDED(dxDrawer->SwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture)) && "Failed to fetch backBuffer Information");
	assert(SUCCEEDED(dxDrawer->Device()->CreateRenderTargetView(backBufferTexture, nullptr, &myBackBuffer)) && "Failed to create render target");


	ID3D11Texture2D* depthBufferText;
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

	assert(SUCCEEDED(dxDrawer->Device()->CreateTexture2D(&depthBufferDesc, nullptr, &depthBufferText)) && "Failed to create depth texture!");
	assert(SUCCEEDED(dxDrawer->Device()->CreateDepthStencilView(depthBufferText, nullptr, &myDepthBuffer)) && "Failed to create depth buffer!");


	D3D11_VIEWPORT newViewport = { 0 };
	newViewport.Width = static_cast<float>(textureDesc.Width);
	newViewport.Height = static_cast<float>(textureDesc.Height);
	dxDrawer->Context()->RSSetViewports(1, &newViewport);


	myDrawer = dxDrawer;
}

const int Dragonite::GraphicsEngine::VertexShaderID(const int anRenderCallID)
{
	return myInstructions[anRenderCallID][0]->myVSID;
}

const int Dragonite::GraphicsEngine::PixelShaderID(const int anRenderCallID)
{
	return myInstructions[anRenderCallID][0]->myPSID;
}

const int Dragonite::GraphicsEngine::InputLayoutID(const int anRenderCallID)
{
	return myInstructions[anRenderCallID][0]->myIID;
}
