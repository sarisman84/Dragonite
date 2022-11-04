#include "GraphicsAPI.h"
#include "../CU/CommonData.h"
#include "Pipeline/Runtime.h"
#include "Core/PollingStation.h"
#include "Models/ModelFactory.h"
#include "Textures/TextureFactory.h"
#include "BaseRenderer.h"
#include "RenderTargets/RenderTarget.h"
#include "RenderTargets/RenderFactory.h"
#include "CameraInterface.h"
#include <d3d11.h>

#include "DirectX11/DXUtilities.h"

#define REPORT_DX_WARNINGS

#pragma warning (disable: 4267)

Dragonite::ForwardRenderer::ForwardRenderer() : Renderer()
{
}

void Dragonite::ForwardRenderer::OnRender(
	std::vector<RenderInstructions> someInstructions,
	CameraInterface* anInterface,
	ShaderInstructions someShaderInstructions,
	std::function<void(RenderInstructions)> anOnElementDrawCallback)
{

	DXInterface::Context->IASetInputLayout(someShaderInstructions.myInputLayout.Get());
	DXInterface::Context->VSSetShader(someShaderInstructions.myVertexShader.Get(), nullptr, 0);
	DXInterface::Context->PSSetShader(someShaderInstructions.myPixelShader.Get(), nullptr, 0);

	for (size_t i = 0; i < anInterface->Profiles().size(); i++)
	{

		if (!anInterface->Profiles()[i]->IsActive()) continue;

		auto cpy = someInstructions;
		FrameBufferData fData;
		fData.myWorldToClipMatrix = anInterface->ViewMatrix() * anInterface->Profiles()[i]->CalculateProjectionMatrix();
		DXInterface::ModifyBuffer(myFrameBuffer, Data(&fData));

		DXInterface::Context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());


		DXInterface::SetDepthStencilState(anInterface->Profiles()[i]->GetDepthStencilState());
		DXInterface::SetRasterizerState(anInterface->Profiles()[i]->GetCullMode());

		while (!cpy.empty())
		{
			auto instruction = cpy.back();
			cpy.pop_back();

			auto id = anInterface->Profiles()[i]->GetID();
			if (instruction.myProfileIndex != id) continue;
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

			DXInterface::Context->PSSetSamplers(0, 1, &DXInterface::GetSampler());



			DXInterface::Context->DrawIndexed(instruction.myIndexCount, 0, 0);
		}
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
	if (foundInstructions != myShaderInstructions.end())
	{

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
		myActiveCamera,
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
		myActiveCamera,
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

void Dragonite::GraphicalInterface::SetActiveCameraAs(CameraInterface& aNewCamera)
{
	myActiveCamera = &aNewCamera;
}

Dragonite::ShaderInstructions::ShaderInstructions() : myMaterial("unknown", "unknown")
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
