#include "GraphicsEngine.h"
#include "DirectX/DXUtilities.h"
#include <d3d11.h>
#include <cassert>
#include "Pipeline/Rendering/DX/DXDrawer.h"

Dragonite::GraphicsEngine::GraphicsEngine() : myShaderFactory(this)
{
	myDrawer = nullptr;
	myBackBuffer = new RTContent();
	myDepthBuffer = new DSContent();
}

Dragonite::GraphicsEngine::~GraphicsEngine()
{
	if (myDrawer)
		delete myDrawer;
	myDrawer = nullptr;

	if (myBackBuffer)
		delete myBackBuffer;
	myBackBuffer = nullptr;

	if (myDepthBuffer)
		delete myDepthBuffer;
	myDepthBuffer = nullptr;
}

void Dragonite::GraphicsEngine::Draw(void* aBackBuffer, void* aDepthBuffer)
{
	if (aBackBuffer)
	{
		myDrawer->SetRenderTarget(aBackBuffer, aDepthBuffer);
		myDrawer->ClearRenderTarget(aBackBuffer, aDepthBuffer);
	}
	else
	{
		myDrawer->SetRenderTarget(myBackBuffer->GetContent(), myDepthBuffer->GetContent());
		myDrawer->ClearRenderTarget(myBackBuffer->GetContent(), myBackBuffer->GetContent());
	}



	for (auto& call : myInstructions)
	{
		DXDrawer* dxDrawer = (DXDrawer*)myDrawer;
		auto context = dxDrawer->Context();


		context->IASetInputLayout(myShaderFactory.GetILOfID(InputLayoutID(call.first)));

		context->VSSetShader(myShaderFactory.GetVSOfID(VertexShaderID(call.first)), nullptr, 0);
		context->PSSetShader(myShaderFactory.GetPSOfID(PixelShaderID(call.first)), nullptr, 0);


		for (auto& renderCall : call.second)
		{
			renderCall->Render();
		}
	}

}

void Dragonite::GraphicsEngine::Present()
{
	myDrawer->Present(false);
}

ID3D11RenderTargetView* Dragonite::GraphicsEngine::BackBuffer()
{
	return (ID3D11RenderTargetView*)myBackBuffer->GetContent();
}

Dragonite::GraphicsEngine* Dragonite::GraphicsEngine::InitializeEngine(HWND anInstance)
{
	GraphicsEngine* engine = new GraphicsEngine();
	engine->Init(anInstance);
	return engine;
}

void Dragonite::GraphicsEngine::Init(HWND anInstance)
{
	DXDrawer* dxDrawer = new DXDrawer();

	dxDrawer->Init(anInstance);



	ID3D11Texture2D* backBufferTexture;

	assert(SUCCEEDED(dxDrawer->SwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture)) && "Failed to fetch backBuffer Information");
	assert(SUCCEEDED(dxDrawer->Device()->CreateRenderTargetView(backBufferTexture, nullptr, (ID3D11RenderTargetView**)myBackBuffer->EditContent())) && "Failed to create render target");


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
	assert(SUCCEEDED(dxDrawer->Device()->CreateDepthStencilView(depthBufferText, nullptr, (ID3D11DepthStencilView**)myDepthBuffer->EditContent())) && "Failed to create depth buffer!");


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
