#include "GraphicsEngine.h"
#include "DirectX/DXUtilities.h"
#include <d3d11.h>
#include <cassert>

#include "Pipeline/Rendering/DX/DXDrawer.h"
#include "Core/CU/Transform.h"
#include "Content/ContentFactories.h"


Dragonite::GraphicsEngine::GraphicsEngine()
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


	if (myMaterialFactory)
		delete myMaterialFactory;
	myMaterialFactory = nullptr;

	if (myModelFactory)
		delete myModelFactory;
	myModelFactory = nullptr;
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


	auto drawer = GetDrawer<DXDrawer>();
	auto context = drawer->Context();

	for (size_t i = 0; i < myInstructions.size(); i++)
	{
		auto instruction = myInstructions[i];

		auto material = myMaterialFactory->GetMaterial(instruction.myInputLayout);

		context->IASetInputLayout(material.myInputLayout);
		context->VSSetShader(material.myVertexShader, NULL, 0);
		context->PSSetShader(material.myPixelShader, NULL, 0);

		const unsigned int strides = 0;
		const unsigned int offsets = 0;

		switch (instruction.myDrawType)
		{
		case 0:
			Model model = myModelFactory->GetModel(instruction.myModelID);

			context->IASetIndexBuffer(model.myIndices, DXGI_FORMAT_R32_UINT, 0);
			context->IASetVertexBuffers(0, 1, &model.myVertices, &strides, &offsets);
			context->DrawIndexed(model.myIndiciesCount, 0, 0);
			break;
		default:
			break;
		}



	}



	myInstructions.clear();
}

void Dragonite::GraphicsEngine::Present()
{
	myDrawer->Present(false);
}

void Dragonite::GraphicsEngine::Submit(const DrawInstruct& someDataToDraw)
{
	myInstructions.push_back(someDataToDraw);
}

ID3D11RenderTargetView* Dragonite::GraphicsEngine::BackBuffer()
{
	return (ID3D11RenderTargetView*)myBackBuffer->GetContent();
}

Dragonite::GraphicsEngine* Dragonite::GraphicsEngine::InitializeEngine(HWND anInstance)
{
	GraphicsEngine* engine = new GraphicsEngine();
	engine->myModelFactory = new ModelFactory(engine);
	engine->myMaterialFactory = new MaterialFactory(engine);

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

