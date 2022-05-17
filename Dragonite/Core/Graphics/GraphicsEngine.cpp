#include "GraphicsEngine.h"
#include <d3d11.h>
#include <fstream>
#include "System.h"
#include "Utilities/Math/Vector2.h"
#include "Components/Camera.h"
#include "Rendering/MeshInfo.h"
#define REPORT_DX_WARNINGS



Engine::Graphics::GraphicsEngine::GraphicsEngine() = default;


Engine::Graphics::GraphicsEngine::~GraphicsEngine() = default;


bool Engine::Graphics::GraphicsEngine::Initialize(Resolution /*aResolution*/, HWND aWindowsHandle, System * aSystem)
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


	D3D11_BUFFER_DESC bufferDesc = { 0 };
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDesc.ByteWidth = sizeof(FrameBufferData),
		result = myDevice->CreateBuffer(&bufferDesc, nullptr, &myFrameBuffer);
	if (FAILED(result)) return false;

	bufferDesc.ByteWidth = sizeof(ObjectBufferData);
	result = myDevice->CreateBuffer(&bufferDesc, nullptr, &myObjectBuffer);
	if (FAILED(result)) return false;


	bufferDesc.ByteWidth = sizeof(MaterialBufferData);
	result = myDevice->CreateBuffer(&bufferDesc, nullptr, &myMaterialBuffer);
	if (FAILED(result)) return false;



	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
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
	// Create the texture sampler state.

	result = myDevice->CreateSamplerState(&samplerDesc, &mySamplerState);

	if (FAILED(result))
	{
		return false;
	}


	ID3D11Texture2D* depthBufferTexture;
	D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };
	depthBufferDesc.Width = static_cast<unsigned int>(textureDesc.Width);
	depthBufferDesc.Height = static_cast<unsigned int>(textureDesc.Height);
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	result = myDevice->CreateTexture2D(&depthBufferDesc, nullptr, &depthBufferTexture);
	if (FAILED(result))
	{
		return false;
	}
	result = myDevice->CreateDepthStencilView(depthBufferTexture, nullptr, &myDepthBuffer);
	if (FAILED(result))
	{
		return false;
	}
	depthBufferTexture->Release();










	//D3D11_TEXTURE2D_DESC textureDesc;
	//backBufferTexture->GetDesc(&textureDesc);
	//backBufferTexture->Release();
	myContext->OMSetRenderTargets(1, myBackBuffer.GetAddressOf(), myDepthBuffer.Get());
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(textureDesc.Width);
	viewport.Height = static_cast<float>(textureDesc.Height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	myContext->RSSetViewports(1, &viewport);








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


	D3D11_MAPPED_SUBRESOURCE resource;
	myContext->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	FrameBufferData data;
	data.myClipSpaceMatrix = myRenderCamera->GetClipSpaceMatrix();
	data.myTimeDelta = mySystem->GetTimeDelta();
	data.myTotalTime = mySystem->GetTotalTime();
	memcpy(resource.pData, &data, sizeof(FrameBufferData));

	myContext->Unmap(myFrameBuffer.Get(), 0);
	myContext->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
	myContext->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());



	while (!myRenderInstructions.empty())
	{
		auto instruction = myRenderInstructions.front();




		D3D11_MAPPED_SUBRESOURCE oResource;
		myContext->Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &oResource);
		ObjectBufferData objData;
		objData.myObjectMatrix = instruction->myTransform.myTransformMatrix;
		memcpy(oResource.pData, &objData, sizeof(ObjectBufferData));

		myContext->Unmap(myObjectBuffer.Get(), 0);

		myContext->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());




		D3D11_MAPPED_SUBRESOURCE mResource;
		myContext->Map(myMaterialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mResource);
		MaterialBufferData mData;
		mData.myColor = instruction->myMaterial.myColor;
		memcpy(mResource.pData, &objData, sizeof(MaterialBufferData));

		myContext->Unmap(myMaterialBuffer.Get(), 0);

		myContext->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());


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
			myContext->PSSetSamplers(0, 1, mySamplerState.GetAddressOf());
			myContext->PSSetShaderResources(instruction->myModel->myTexture.mySlot, 1, instruction->myModel->myTexture.myTextureResource.GetAddressOf());
			myContext->DrawIndexed(static_cast<UINT>(mesh.myIndiciesAmm), 0, 0);
		}



		myRenderInstructions.pop();


	}



	mySwapChain->Present(1, 0);
}





//Shape Engine::Graphics::GraphicsEngine::GetUnitCircle()
//{
//	Shape circle;
//	int segments = 50;
//
//	circle.myVerticesAmm = 3 * segments;
//	circle.myIndicesAmm = 3 * segments;
//	int posIndex = 0;
//	for (size_t i = 0; i < segments; i++)
//	{
//		float p = 3.14159265f * (i * (360.f / segments) / 180.f);
//		float p2 = 3.14159265f * ((i + 1) * (360.f / segments) / 180.f);
//		Math::Vector2f pos(cos(p), sin(p));
//		Math::Vector2f pos3(cos(p2), sin(p2));
//		Math::Vector2f pos2(0.f, 0.f);
//
//		circle.myIndicies[posIndex] = posIndex;
//		circle.myIndicies[posIndex + 1] = posIndex + 1;
//		circle.myIndicies[posIndex + 2] = posIndex + 2;
//
//		circle.myVertices[posIndex] = Vertex2D{ Math::Vector4f{pos.x,  pos.y,  1, 1}, Math::Vector4f{   1, 1, 1,1 } };
//		circle.myVertices[posIndex + 1] = Vertex2D{ Math::Vector4f{pos2.x, pos2.y, 1, 1}, Math::Vector4f{   0, 0, 0,1 } };
//		circle.myVertices[posIndex + 2] = Vertex2D{ Math::Vector4f{pos3.x, pos3.y, 1, 1}, Math::Vector4f{   1, 1, 1,1 } };
//
//		posIndex += 3;
//	}
//
//
//	return circle;
//}


