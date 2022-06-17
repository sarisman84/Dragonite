#include "RenderTarget.h"
#include "GraphicsEngine.h"
#include "Rendering/ModelFactory.h"
#include "Rendering/Framework/Texture.h"

#include <d3d11.h>

RenderTarget::RenderTarget() = default;

RenderTarget::~RenderTarget() = default;

void RenderTarget::OnAwake()
{
	myInstance = nullptr;
	InitializeTexture();


	Material mat;
	mat.myVertexShader = "Shaders/ColorShader_VS.cso";
	mat.myPixelShader = "Shaders/PlanarReflection_PS.cso";
	myInstance = mySystem->Get<ModelFactory>()->CreateInstanceOf(UNITPLANE, mat);

	if (myInstance)
	{
		for (auto& mesh : myInstance->myModel->myMesh)
		{
			mesh.myTextureBuffer.push_back(std::make_shared<Texture>(myTextureResource));
		}
		myTransform->Size = { 1000.f,1.f ,1000.f };
	}



	auto graphicsEngine = mySystem->GetGraphicsEngine();

	graphicsEngine->SetRenderTarget(this);
}

void RenderTarget::OnUpdate(float /*aDeltaTime*/)
{
	if (myInstance)
		myInstance->myTransform = *myTransform;
}

void RenderTarget::Render(Dragonite::GraphicsEngine * aGraphicsEngine)
{
	if (!aGraphicsEngine && myInstance)return;

	auto graphicsEngine = aGraphicsEngine;
	auto context = graphicsEngine->GetDeviceContext();


	graphicsEngine->UpdateFrameBuffer();
	graphicsEngine->UpdateObjectBuffer(myInstance.get());


	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(myInstance->myModel->myInputLayout.Get());

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;



	for (auto& mesh : myInstance->myModel->myMesh)
	{
		context->IASetVertexBuffers(0, 1, mesh.myVertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(mesh.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		context->VSSetShader(myInstance->myModel->myVertexShader.Get(), nullptr, 0);
		context->PSSetShader(myInstance->myModel->myPixelShader.Get(), nullptr, 0);

		for (size_t t = 0; t < mesh.myTextureBuffer.size(); t++)
		{
			auto& texture = mesh.myTextureBuffer[t];
			/*myContext->PSSetShaderResources(texture.mySlot, 1, texture.myTextureResource.GetAddressOf());*/
			texture->BindTexture(context, t);
		}

		context->DrawIndexed(static_cast<UINT>(mesh.myIndiciesAmm), 0, 0);
	}


}

void RenderTarget::InitializeTexture()
{
	auto graphicsEngine = mySystem->GetGraphicsEngine();
	auto device = graphicsEngine->GetDevice();



	HRESULT result;


	D3D11_TEXTURE2D_DESC desc;
	desc.Width = graphicsEngine->Viewport().width;
	desc.Height = graphicsEngine->Viewport().height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;


	ID3D11Texture2D* texture;


	result = device->CreateTexture2D(&desc, nullptr, &texture);
	if (FAILED(result)) return;

	device->CreateShaderResourceView(texture, nullptr, &myTextureResource);
	if (FAILED(result)) return;

	device->CreateRenderTargetView(texture, nullptr, &myRenderTarget);
	if (FAILED(result)) return;

	texture->Release();
}
