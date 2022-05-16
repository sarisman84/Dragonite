#include "GraphicsEngine.h"
#include <d3d11.h>
#include <fstream>
#include "Rendering/RenderObject.h"
#include "System.h"
#include "Utilities/Math/Vector2.h"
#include "Components/Camera.h"
#include "Rendering/MeshInfo.h"
#define REPORT_DX_WARNINGS



Engine::Graphics::GraphicsEngine::GraphicsEngine() = default;


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

	//D3D11_TEXTURE2D_DESC textureDesc;
	//backBufferTexture->GetDesc(&textureDesc);
	//backBufferTexture->Release();

	myContext->OMSetRenderTargets(1, myBackBuffer.GetAddressOf(), nullptr);
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(aResolution.width);
	viewport.Height = static_cast<float>(aResolution.height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	myContext->RSSetViewports(1, &viewport);








	return true;
}


void Engine::Graphics::GraphicsEngine::DrawElements()
{
	float color[4] = { 0.2f,0.2f,0.2f,1.0f }; // RGBA
	myContext->ClearRenderTargetView(myBackBuffer.Get(), color);


	D3D11_MAPPED_SUBRESOURCE resource;


	myContext->Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	FrameBufferData data;
	data.myClipSpaceMatrix = myRenderCamera->GetClipSpaceMatrix();
	data.myTimeDelta = mySystem->GetTimeDelta();
	data.myTotalTime = mySystem->GetTotalTime();
	memcpy(resource.pData, &data, sizeof(FrameBufferData));

	myContext->Unmap(myFrameBuffer.Get(), 0);
	myContext->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());


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

		myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		myContext->IASetInputLayout(instruction->myInputLayout.Get());

		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;


		for (auto& mesh : instruction->myModel->myMesh)
		{
			myContext->IASetVertexBuffers(0, 1, mesh.myVertexBuffer.GetAddressOf(), &stride, &offset);
			myContext->IASetIndexBuffer(mesh.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			myContext->VSSetShader(instruction->myVertexShader.Get(), nullptr, 0);
			myContext->PSSetShader(instruction->myPixelShader.Get(), nullptr, 0);

			myContext->DrawIndexed(static_cast<UINT>(mesh.myIndicies.size()), 0, 0);
		}



		myRenderInstructions.pop();


	}

	for (size_t i = 0; i < myRenderTargets.size(); i++)
	{
		myRenderTargets[i]->Draw();
	}

	mySwapChain->Present(1, 0);
}

std::shared_ptr<RenderObject> Engine::Graphics::GraphicsEngine::Create2DElement(Primitive2D aPrimitiveShape)
{
	Shape aShape;
	switch (aPrimitiveShape)
	{
	case Primitive2D::Circle:
		aShape = GetUnitCircle();
		break;
	case Primitive2D::Quad:
		aShape = GetUnitQuad();
		break;

	case Primitive2D::Triangle:
		aShape = GetUnitTriangle();
		break;
	}

	auto renderObject = std::make_shared<RenderObject>(aShape, myDevice.Get(), myContext.Get(), mySystem);
	if (!renderObject->Initialize()) return nullptr;
	myRenderTargets.push_back(renderObject);
	return myRenderTargets.back();

}

Shape Engine::Graphics::GraphicsEngine::GetUnitTriangle()
{
	using namespace Engine::Graphics;
	Shape triangle;

	Vertex2D vertices[] =
	{
		Vertex2D{Math::Vector4f(-1.f, -1.f,  0, 1), Math::Vector4f(1,0,0,1)},
		Vertex2D{Math::Vector4f(0,     1.f,  0, 1), Math::Vector4f(0,1,0,1)},
		Vertex2D{Math::Vector4f(1.f,  -1.f,  0, 1), Math::Vector4f(0,0,1,1)}
	};

	unsigned int indices[] =
	{
		0,1,2
	};

	memcpy(triangle.myVertices, vertices, sizeof(vertices));
	memcpy(triangle.myIndicies, indices, sizeof(indices));
	triangle.myIndicesAmm = 3;
	triangle.myVerticesAmm = 3;

	return triangle;
}

Shape Engine::Graphics::GraphicsEngine::GetUnitQuad()
{
	using namespace Engine::Graphics;
	Shape quad;
	Vertex2D vertices[] =
	{
		{Math::Vector4f{1.f,   1.f,  1, 1}, Math::Vector4f{1,0,0,1}},
		{Math::Vector4f{-1.f,  1.f,  1, 1}, Math::Vector4f{0,1,0,1}},
		{Math::Vector4f{-1.f, -1.f,  1, 1}, Math::Vector4f{0,0,1,1}},
		{Math::Vector4f{1.f,  -1.f,  1, 1}, Math::Vector4f{1,0,1,1}}
	};

	unsigned int indices[] =
	{
		3,1,0,2,1,3
	};

	memcpy(quad.myVertices, vertices, sizeof(vertices));
	memcpy(quad.myIndicies, indices, sizeof(indices));
	quad.myIndicesAmm = 6;
	quad.myVerticesAmm = 4;

	return quad;
}

Shape Engine::Graphics::GraphicsEngine::GetUnitCircle()
{
	Shape circle;
	int segments = 50;

	circle.myVerticesAmm = 3 * segments;
	circle.myIndicesAmm = 3 * segments;
	int posIndex = 0;
	for (size_t i = 0; i < segments; i++)
	{
		float p = 3.14159265f * (i * (360.f / segments) / 180.f);
		float p2 = 3.14159265f * ((i + 1) * (360.f / segments) / 180.f);
		Math::Vector2f pos(cos(p), sin(p));
		Math::Vector2f pos3(cos(p2), sin(p2));
		Math::Vector2f pos2(0.f, 0.f);

		circle.myIndicies[posIndex] = posIndex;
		circle.myIndicies[posIndex + 1] = posIndex + 1;
		circle.myIndicies[posIndex + 2] = posIndex + 2;

		circle.myVertices[posIndex] = Vertex2D{ Math::Vector4f{pos.x,  pos.y,  1, 1}, Math::Vector4f{   1, 1, 1,1 } };
		circle.myVertices[posIndex + 1] = Vertex2D{ Math::Vector4f{pos2.x, pos2.y, 1, 1}, Math::Vector4f{   0, 0, 0,1 } };
		circle.myVertices[posIndex + 2] = Vertex2D{ Math::Vector4f{pos3.x, pos3.y, 1, 1}, Math::Vector4f{   1, 1, 1,1 } };

		posIndex += 3;
	}


	return circle;
}


