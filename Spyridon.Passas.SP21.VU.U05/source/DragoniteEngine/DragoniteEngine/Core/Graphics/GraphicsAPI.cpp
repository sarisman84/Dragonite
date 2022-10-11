#include "GraphicsAPI.h"
#include "../CU/CommonData.h"
#include "Core/Runtime.h"
#include "Core/PollingStation.h"
#include "Models/ModelFactory.h"
#include "Textures/TextureFactory.h"
#include "BaseRenderer.h"
#include "RenderTargets/RenderTarget.h"
#include "RenderTargets/RenderFactory.h"
#include "Camera.h"
#include <d3d11.h>

#include "DirectX11/DXUtilities.h"

#define REPORT_DX_WARNINGS

#pragma warning (disable: 4267)



/*



*/


//Dragonite::GraphicsPipeline::GraphicsPipeline()
//{
//	myRenderFactory = nullptr;
//	myModelFactory = nullptr;
//	myTextureFactory = nullptr;
//}
//
//
//Dragonite::GraphicsPipeline::~GraphicsPipeline()
//{
//	if (myModelFactory)
//		delete myModelFactory;
//	myModelFactory = nullptr;
//
//	if (myTextureFactory)
//		delete myTextureFactory;
//	myTextureFactory = nullptr;
//
//	if (myRenderFactory)
//		delete myRenderFactory;
//	myRenderFactory = nullptr;
//}
//
//bool Dragonite::GraphicsPipeline::Initialize(Runtime* anApplication, HWND aWindowHandle)
//{
//	myClearColor = Color(0.25f, 0.25f, 0.85f, 1.0f);
//	myRenderFactory = new RenderFactory();
//
//	assert(SUCCEEDED(InitializeSwapChain(aWindowHandle)));
//	assert(SUCCEEDED(InitializeBackBuffer()));
//	assert(SUCCEEDED(InitializeSamplers()));
//	assert(SUCCEEDED(InitializeBlendStates()));
//
//
//
//
//
//	DataBufferDesc bufferDesc(
//		sizeof(FrameBufferData),
//		D3D11_USAGE_DYNAMIC,
//		D3D11_BIND_CONSTANT_BUFFER,
//		D3D11_CPU_ACCESS_WRITE);
//
//	if (FAILED(CreateBuffer(myDevice, myFrameBuffer, bufferDesc)))
//		return false;
//
//
//	bufferDesc.mySize = sizeof(ObjectBufferData);
//
//	if (FAILED(CreateBuffer(myDevice, myObjectBuffer, bufferDesc)))
//		return false;
//
//	myApplicationPtr = anApplication;
//	myApplicationPtr->GetPollingStation().AddHandler(this);
//
//	myModelFactory = new ModelFactory();
//	if (!myModelFactory->Initialize(this))
//	{
//		return false;
//	}
//
//	myRenderFactory = new RenderFactory();
//	if (!myRenderFactory->Init(this))
//	{
//		return false;
//	}
//	myTextureFactory = new TextureFactory(this);
//
//	myApplicationPtr->GetPollingStation().AddHandler(myRenderFactory);
//	myApplicationPtr->GetPollingStation().AddHandler(myTextureFactory);
//	myApplicationPtr->GetPollingStation().AddHandler(myModelFactory);
//	myApplicationPtr->GetPollingStation().AddHandler(myRenderFactory);
//
//	return true;
//}
//
//void Dragonite::GraphicsPipeline::SetBlendState(const BlendStateType aType)
//{
//	myContext->OMSetBlendState(myBlendStates[aType].Get(), nullptr, 0xffffffff);
//}
//
//void Dragonite::GraphicsPipeline::SwitchRenderTarget(RenderView aView, DepthStencil aDepthBuffer)
//{
//	myContext->OMSetRenderTargets(1, aView.GetAddressOf(), aDepthBuffer ? aDepthBuffer.Get() : nullptr);
//	myContext->ClearRenderTargetView(aView.Get(), &myClearColor);
//	if (aDepthBuffer)
//		myContext->ClearDepthStencilView(aDepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//}
//
//void Dragonite::GraphicsPipeline::DrawInstructionsToBB()
//{
//	DrawInstructions(myBackBuffer, myDepthBuffer);
//}
//
//void Dragonite::GraphicsPipeline::DrawInstructions(RenderView aView, DepthStencil aDepthBuffer)
//{
//	myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	SwitchRenderTarget(aView, aDepthBuffer);
//
//
//	UpdateFrameBuffer();
//	auto cpy = myElementsToDraw;
//	while (!cpy.empty())
//	{
//		auto element = cpy.back();
//		cpy.pop_back();
//
//
//		
//		if (element->myTexture)
//			element->myTexture->Bind(myContext);
//
//		UpdateObjectBufferAt(element);
//
//		{
//			myContext->IASetInputLayout(element->myInputLayout.Get());
//
//			unsigned int stride = sizeof(Vertex);
//			unsigned int offset = 0;
//
//			myContext->IASetVertexBuffers(0, 1, element->myModel->myVertexBuffer.GetAddressOf(), &stride, &offset);
//			myContext->IASetIndexBuffer(element->myModel->myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//
//
//
//			myContext->VSSetShader(element->myVertexShader.Get(), nullptr, 0);
//			myContext->PSSetShader(element->myPixelShader.Get(), nullptr, 0);
//			myContext->PSSetSamplers(0, 1, &myTextureSamplers[TextureSampleType::Default]);
//		}
//
//
//
//		myContext->DrawIndexed(element->myModel->myIndexCount, 0, 0);
//
//	}
//}
//
//void Dragonite::GraphicsPipeline::Render()
//{
//
//
//	bool setRenderTargetToViewport = false;
//	if (myActiveRenderTarget)
//		setRenderTargetToViewport = myActiveRenderTarget->RenderThisTarget(myDepthBuffer);
//	if (!setRenderTargetToViewport)
//	{
//		DrawInstructionsToBB();
//	}
//
//
//	myElementsToDraw.clear();
//	myApplicationPtr->OnRender()(this);
//	mySwapChain->Present(1, 0);
//}
//
//Dragonite::Vector2f Dragonite::GraphicsPipeline::GetViewPort()
//{
//	UINT amm = 1;
//	D3D11_VIEWPORT ports;
//	myContext->RSGetViewports(&amm, &ports);
//
//	if (amm <= 0) return Vector2f();
//
//	return Vector2f(ports.Width, ports.Height);
//}
//
//
//
//HRESULT Dragonite::GraphicsPipeline::InitializeBlendStates()
//{
//	HRESULT result = S_OK;
//	D3D11_BLEND_DESC blendStateDescription = {};
//
//	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
//	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
//	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
//	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
//	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
//	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
//	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
//	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
//	result = myDevice->CreateBlendState(&blendStateDescription, myBlendStates[BlendStateType::None].GetAddressOf());
//	if (FAILED(result))
//	{
//		return result;
//	}
//
//	blendStateDescription = {};
//	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
//	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
//	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
//	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
//	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
//	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
//	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
//	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
//	result = myDevice->CreateBlendState(&blendStateDescription, myBlendStates[BlendStateType::Alpha].GetAddressOf());
//	if (FAILED(result))
//	{
//		return result;
//	}
//
//	blendStateDescription = {};
//	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
//	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
//	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
//	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
//	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
//	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
//	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
//	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
//	result = myDevice->CreateBlendState(&blendStateDescription, myBlendStates[BlendStateType::Additive].GetAddressOf());
//	if (FAILED(result))
//	{
//		return result;
//	}
//
//	return result;
//}
//
//HRESULT Dragonite::GraphicsPipeline::InitializeSwapChain(HWND anInstance)
//{
//	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
//	swapChainDesc.BufferCount = 1;
//	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	swapChainDesc.OutputWindow = anInstance;
//	swapChainDesc.SampleDesc.Count = 1;
//	swapChainDesc.Windowed = true;
//
//	UINT creationFlags = 0;
//#if defined(REPORT_DX_WARNINGS)
//	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
//
//
//
//	return D3D11CreateDeviceAndSwapChain(
//		nullptr,
//		D3D_DRIVER_TYPE_HARDWARE,
//		nullptr,
//		creationFlags,
//		nullptr,
//		0,
//		D3D11_SDK_VERSION,
//		&swapChainDesc,
//		&mySwapChain,
//		&myDevice,
//		nullptr,
//		&myContext
//	);
//}
//
//HRESULT Dragonite::GraphicsPipeline::InitializeBackBuffer()
//{
//	ID3D11Texture2D* backBufferTexture;
//	HRESULT result = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture);
//	if (FAILED(result))
//	{
//		return result;
//	}
//	result = myDevice->CreateRenderTargetView(backBufferTexture, nullptr, &myBackBuffer);
//	if (FAILED(result))
//	{
//		return result;
//	}
//
//	DXTexture2D depthBufferText;
//	D3D11_TEXTURE2D_DESC textureDesc;
//
//
//	backBufferTexture->GetDesc(&textureDesc);
//	backBufferTexture->Release();
//
//	TextureBufferDesc depthBufferTextDesc(Vector2ui(textureDesc.Width, textureDesc.Height), DXGI_FORMAT_D32_FLOAT, D3D11_BIND_DEPTH_STENCIL);
//
//	if (FAILED(CreateTexture(myDevice, depthBufferText, depthBufferTextDesc)))
//	{
//		return E_INVALIDARG;
//	}
//
//	if (FAILED(myDevice->CreateDepthStencilView(depthBufferText.Get(), nullptr, &myDepthBuffer)))
//	{
//		return E_INVALIDARG;
//	}
//
//
//
//	myContext->OMSetRenderTargets(1, myBackBuffer.GetAddressOf(), myDepthBuffer.Get());
//	D3D11_VIEWPORT viewport = { 0 };
//	viewport.TopLeftX = 0.0f;
//	viewport.TopLeftY = 0.0f;
//	viewport.Width = static_cast<float>(textureDesc.Width);
//	viewport.Height = static_cast<float>(textureDesc.Height);
//	viewport.MinDepth = 0.0f;
//	viewport.MaxDepth = 1.0f;
//	myContext->RSSetViewports(1, &viewport);
//	return S_OK;
//}
//
//
//HRESULT CreateSampler(Dragonite::GraphicsPipeline* aPipeline, Dragonite::TextureSampleType aType, D3D11_SAMPLER_DESC aDesc)
//{
//	return aPipeline->GetDevice()->CreateSamplerState(&aDesc, &(aPipeline->GetTextureSamplers()[aType]));
//}
//
//HRESULT Dragonite::GraphicsPipeline::InitializeSamplers()
//{
//	D3D11_SAMPLER_DESC samplerDesc;
//	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.MipLODBias = 0.0f;
//	samplerDesc.MaxAnisotropy = 1;
//	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
//	samplerDesc.BorderColor[0] = 0;
//	samplerDesc.BorderColor[1] = 0;
//	samplerDesc.BorderColor[2] = 0;
//	samplerDesc.BorderColor[3] = 0;
//	samplerDesc.MinLOD = 0;
//	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
//
//	HRESULT result = CreateSampler(this, TextureSampleType::Default, samplerDesc);
//
//	return result;
//}
//
//
//
//
//HRESULT Dragonite::GraphicsPipeline::CreateBuffer(Device aDevice, DataBuffer& aBuffer, const DataBufferDesc& aDesc)
//{
//	D3D11_BUFFER_DESC desc = { 0 };
//	desc.ByteWidth = aDesc.mySize;
//	desc.Usage = aDesc.myUsage;
//	desc.BindFlags = aDesc.myBindFlag;
//	desc.CPUAccessFlags = aDesc.myCPUFlags;
//
//	D3D11_SUBRESOURCE_DATA data = { 0 };
//	if (aDesc.myData)
//	{
//		data.pSysMem = aDesc.myData;
//
//	}
//	return aDevice->CreateBuffer(&desc, aDesc.myData ? &data : nullptr, &aBuffer);
//}
//
//HRESULT Dragonite::GraphicsPipeline::CreateTexture(Device aDevice, DXTexture2D& aTexture, const TextureBufferDesc& aDesc)
//{
//	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
//	textureDesc.Width = aDesc.myTextureRes.x;
//	textureDesc.Height = aDesc.myTextureRes.y;
//	textureDesc.ArraySize = aDesc.myArraySize;
//	textureDesc.Format = (DXGI_FORMAT)aDesc.myFormat;
//	textureDesc.SampleDesc.Count = 1;
//	textureDesc.BindFlags = aDesc.myBindFlags;
//
//	return aDevice->CreateTexture2D(&textureDesc, nullptr, &aTexture);
//}
//
//HRESULT Dragonite::GraphicsPipeline::MapBuffer(DeviceContext aContext, DataBuffer& aBuffer, void* someData, size_t aDataSize)
//{
//	if (!someData || aDataSize <= 0) return E_INVALIDARG;
//
//	D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
//	aContext->Map(aBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
//	memcpy(mappedBuffer.pData, someData, aDataSize);
//	aContext->Unmap(aBuffer.Get(), 0);
//
//	return S_OK;
//}
//
//void Dragonite::GraphicsPipeline::UpdateFrameBuffer()
//{
//	// TODO: insert return statement here
//
//	if (myActiveCamera)
//	{---
//		FrameBufferData frameBufferData = {};
//		frameBufferData.myWorldToClipMatrix = myActiveCamera->WorldToClipSpace();
//		if (myFrameBuffer)-
//			if (FAILED(MapBuffer(myContext, myFrameBuffer, &frameBufferData, sizeof(FrameBufferData))))
//			{
//				//TODO: Handle this error
//			}
//			else
//				myContext->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
//	}
//}
//
//void Dragonite::GraphicsPipeline::UpdateObjectBufferAt(std::shared_ptr<ModelInstance> anInstance)
//{
//	ObjectBufferData objectBufferData = {};
//	objectBufferData.myModelToWorldMatrix = anInstance->myTransform.GetMatrix();
//
//	if (myObjectBuffer)
//		if (FAILED(MapBuffer(myContext, myObjectBuffer, &objectBufferData, sizeof(ObjectBufferData))))
//		{
//			//TODO: Handle this error
//		}
//		else
//			myContext->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
//}
//
//void Dragonite::GraphicsPipeline::UpdateBufferAt(
//	void* someData,
//	const size_t aSize,
//	const int aSlot,
//	DataBuffer& aBuffer,
//	bool aBindVSFlag,
//	bool aBindPSFlag
//)
//{
//
//	if (someData)
//		if (FAILED(MapBuffer(myContext, aBuffer, someData, aSize)))
//		{
//			return;
//		}
//	if (aBindVSFlag)
//		myContext->VSSetConstantBuffers(aSlot, 1, aBuffer.GetAddressOf());
//	if (aBindPSFlag)
//		myContext->PSSetConstantBuffers(aSlot, 1, aBuffer.GetAddressOf());
//}
//
//Dragonite::RenderInterface::RenderInterface(GraphicsPipeline& aPipeline) : myPipeline(aPipeline)
//{
//
//}
//
//void Dragonite::RenderInterface::DrawElement(std::shared_ptr<ModelInstance> anInstance)
//{
//	myPipeline.myElementsToDraw.push_back(anInstance);
//}

Dragonite::ForwardRenderer::ForwardRenderer() : Renderer()
{
}

void Dragonite::ForwardRenderer::OnRender(
	std::vector<RenderInstructions> someInstructions,
	CameraInstructions aCameraInstruction,
	ShaderInstructions someShaderInstructions,
	std::function<void(RenderInstructions)> anOnElementDrawCallback)
{

	DXInterface::Context->IASetInputLayout(someShaderInstructions.myInputLayout.Get());



	FrameBufferData fData;
	fData.myWorldToClipMatrix = aCameraInstruction.myViewMatrix * aCameraInstruction.myProjectionMatrix;
	DXInterface::ModifyBuffer(myFrameBuffer, Data(&fData));

	DXInterface::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());


	while (!someInstructions.empty())
	{
		auto instruction = someInstructions.back();
		someInstructions.pop_back();

		if (someShaderInstructions.myIndex != -1 && instruction.myShaderInstructionsID != someShaderInstructions.myIndex) continue;


		if (instruction.myTexture)
			instruction.myTexture->Bind(DXInterface::Context);

		ObjectBufferData oData;
		oData.myModelToWorldMatrix = instruction.myModelMatrix;
		DXInterface::ModifyBuffer(myObjectBuffer, Data(&oData));

		DXInterface::Context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
		DXInterface::Context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

		if (anOnElementDrawCallback)
			anOnElementDrawCallback(instruction);

		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;

		DXInterface::Context->IASetVertexBuffers(0, 1, instruction.myVertexBuffer.GetAddressOf(), &stride, &offset);
		DXInterface::Context->IASetIndexBuffer(instruction.myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		DXInterface::Context->VSSetShader(someShaderInstructions.myVertexShader.Get(), nullptr, 0);
		DXInterface::Context->PSSetShader(someShaderInstructions.myPixelShader.Get(), nullptr, 0);
		DXInterface::Context->PSSetSamplers(0, 1, &DXInterface::GetSampler());



		DXInterface::Context->DrawIndexed(instruction.myIndexCount, 0, 0);
	}
}

Dragonite::GraphicalInterface::~GraphicalInterface()
{
	if (myPrimaryRenderer)
		delete myPrimaryRenderer;
	myPrimaryRenderer = nullptr;
}

const bool Dragonite::GraphicalInterface::Init(HWND anInstance, PollingStation* aPollingStation)
{
	if (!DXInterface::Init(anInstance)) return false;


	DXInterface::ClearColor = Color(0.25f, 0.25f, 0.85f, 1.0f);
	DXInterface::VSyncState = 1;

	myPrimaryRenderer = new ForwardRenderer();
	myPollingStation = aPollingStation;


	myPollingStation->AddHandler(new TextureFactory(this));
	myPollingStation->AddHandler(new ModelFactory())->Initialize(this);

	return true;
}

void Dragonite::GraphicalInterface::AddRenderInstructions(const RenderInstructions& anInstruction)
{
	myRenderInstructions.push_back(anInstruction);
}

unsigned int Dragonite::GraphicalInterface::AddShaderInstructions(const Material& aMaterial, VertexShader aVS, PixelShader aPS, InputLayout anInputLayout)
{
	ShaderInstructions instructions;
	instructions.myMaterial = aMaterial;
	instructions.myVertexShader = aVS;
	instructions.myPixelShader = aPS;
	instructions.myInputLayout = anInputLayout;


	auto foundInstructions = std::find(myShaderInstructions.begin(), myShaderInstructions.end(), instructions);
	if (foundInstructions != myShaderInstructions.end()) {

		return foundInstructions - myShaderInstructions.begin();
	}
	instructions.myIndex = myShaderInstructions.size();
	myShaderInstructions.push_back(instructions);

	return instructions.myIndex;
}

void Dragonite::GraphicalInterface::DrawInstructions(ShaderInstructions aShaderInstruciton, std::function<void(RenderInstructions)> anOnElementDrawCallback)
{
	if (!myActiveCamera) return;
	DXInterface::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	myPrimaryRenderer->OnRender(
		myRenderInstructions,
		CameraInstructions{
			myActiveCamera->ViewMatrix(),
			myActiveCamera->Profile()->CalculateProjectionMatrix()
		},
		aShaderInstruciton,
		anOnElementDrawCallback
	);





}

void Dragonite::GraphicalInterface::DrawInstructions(VertexShader aVS, PixelShader aPS, InputLayout anIL, std::function<void(RenderInstructions)> anOnElementDrawCallback)
{
	if (!myActiveCamera) return;
	DXInterface::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	ShaderInstructions instruction;
	instruction.myVertexShader = aVS;
	instruction.myPixelShader = aPS;
	instruction.myInputLayout = anIL;
	instruction.myIndex = -1;
	myPrimaryRenderer->OnRender(
		myRenderInstructions,
		CameraInstructions{
			myActiveCamera->ViewMatrix(),
			myActiveCamera->Profile()->CalculateProjectionMatrix()
		},
		instruction,
		anOnElementDrawCallback
	);
}

void Dragonite::GraphicalInterface::DrawInstructions(std::function<void(RenderInstructions)> anOnElementDrawCallback)
{
	for (size_t i = 0; i < myShaderInstructions.size(); i++)
	{
		DrawInstructions(myShaderInstructions[i], anOnElementDrawCallback);
	}
}




void Dragonite::GraphicalInterface::Render()
{
	if (!myPrimaryRenderer) return;

	if (myRenderToBackBufferFlag) 
	{
		DXInterface::DrawToBackBuffer();
		DrawInstructions();
	}


	for (auto& call : myExtraRenderCalls)
	{
		call();
	}

	DXInterface::Present();
	myRenderInstructions.clear();
}

void Dragonite::GraphicalInterface::SetActiveCameraAs(Camera& aNewCamera)
{
	myActiveCamera = &aNewCamera;
}

Dragonite::ShaderInstructions::ShaderInstructions() : myMaterial("unknown")
{
}

bool Dragonite::ShaderInstructions::operator==(const ShaderInstructions& aCpy)
{
	return
		strcmp(myMaterial.myVSInfo.myVertexShader, aCpy.myMaterial.myVSInfo.myVertexShader) == 0 &&
		strcmp(myMaterial.myPSInfo.myPixelShader, aCpy.myMaterial.myPSInfo.myPixelShader) == 0;
}

bool Dragonite::ShaderInstructions::operator!=(const ShaderInstructions& aCpy)
{
	return !(*this == aCpy);
}

Dragonite::Renderer::Renderer()
{
	auto desc = BufferDesc();
	desc.ByteWidth = sizeof(ObjectBufferData);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	auto& subResource = desc.GetSubResourceData();
	subResource.pSysMem = nullptr;
	subResource.SysMemPitch = 0;
	subResource.SysMemSlicePitch = 0;

	DXInterface::CreateBuffer(myObjectBuffer, &desc);
	desc.ByteWidth = sizeof(FrameBufferData);
	DXInterface::CreateBuffer(myFrameBuffer, &desc);
}
