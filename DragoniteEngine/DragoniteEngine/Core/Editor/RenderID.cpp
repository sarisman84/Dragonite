#include "RenderID.h"
#include "Core/Graphics/GraphicsAPI.h"
#include "Core/RuntimeAPI/Scene.h"
#include "Core/Graphics/Models/Model.h"
#include "Core/Graphics/Models/ModelFactory.h"
#include "Core/Graphics/DirectX11/DXInterface.h"
#include "Core/Graphics/DirectX11/DXUtilities.h"

#include "Core/Runtime.h"
#include "Core/PollingStation.h"

#include "Core/Graphics/Camera.h"
#include "Core/Utilities/Input.h"

#include <d3d11.h>
Dragonite::RenderID::RenderID() = default;
Dragonite::RenderID::RenderID(GraphicalInterface* aPipeline) : RenderTarget(aPipeline, RenderTargetDesc())
{

	RefreshRenderView(DXInterface::GetViewportResolution());
	std::string vsData;

	//Create VS and PS Instances
	assert(SUCCEEDED(DXInterface::CreateVSInstance("Fullscreen", myRenderIDVertexShader, vsData)));
	assert(SUCCEEDED(DXInterface::CreatePSInstance("RenderID", myWriteRenderIDPixelShader)));

	auto desc = BufferDesc(
		sizeof(RenderIDBuffer),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE);
	assert(SUCCEEDED(DXInterface::CreateBuffer(myRenderIDBuffer,
		&desc))
	);



}




void Dragonite::RenderID::RefreshRenderView(Vector2f aViewportResolution)
{
	RenderTargetDesc fullscreenPass = {};
	fullscreenPass.myResolution = aViewportResolution;
	fullscreenPass.mySample.Count = 1;
	fullscreenPass.mySample.Quality = 0;
	fullscreenPass.myFormat = DXGI_FORMAT_R32_UINT;
	fullscreenPass.myArraySize = 1;
	fullscreenPass.myMipLevels = 1;
	fullscreenPass.myUseage = (UINT)D3D11_USAGE_DEFAULT;
	fullscreenPass.myCPUAccessFlags = 0;
	fullscreenPass.myMiscFlags = 0;

	InternalInit(fullscreenPass);
}

const bool Dragonite::RenderID::TryGetElement(Mouse* someScreenCoordPos, int& anOutputElement)
{
	auto viewport = DXInterface::GetViewportResolution();
	if (someScreenCoordPos->position.x < 0 || someScreenCoordPos->position.x > viewport.x - 1 ||
		someScreenCoordPos->position.y < 0 || someScreenCoordPos->position.y > viewport.y - 1)
	{
		anOutputElement = 0;
		return false;
	}


	auto& device = DXInterface::Device;
	auto& context = DXInterface::Context;



	DXTexture2DRaw* pixel = nullptr;
	D3D11_BOX srcBox{};

	DXResourceRaw* src;
	myRenderView.Get()->GetResource(&src);
	D3D11_MAPPED_SUBRESOURCE msr = {};

	D3D11_TEXTURE2D_DESC textDesc = { 0 };
	textDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textDesc.Usage = D3D11_USAGE_STAGING;
	textDesc.SampleDesc.Count = 1;
	textDesc.SampleDesc.Quality = 0;
	textDesc.ArraySize = 1;
	textDesc.MipLevels = 1;
	textDesc.Format = DXGI_FORMAT_R32_UINT;

	textDesc.Width = 1;
	textDesc.Height = 1;



	srcBox.left = someScreenCoordPos->position.x;
	srcBox.right = someScreenCoordPos->position.x + 1;
	srcBox.bottom = someScreenCoordPos->position.y + 1;
	srcBox.top = someScreenCoordPos->position.y;

	srcBox.front = 0;
	srcBox.back = 1;

	if (FAILED(device->CreateTexture2D(&textDesc, nullptr, &pixel)))
	{
		anOutputElement = 0;
		return false;
	}


	context->CopySubresourceRegion(pixel, 0, 0, 0, 0, src, 0, &srcBox);



	context->Map(pixel, 0, D3D11_MAP_READ, 0, &msr);

	anOutputElement = *reinterpret_cast<uint32_t*>(msr.pData);
	context->Unmap(pixel, 0);

	return anOutputElement != 0;
}



const bool Dragonite::RenderID::TryGetElement(const Vector2f& someScreenCoordPos, const Vector2f& aViewport, int& anOutputElement)
{
	if (someScreenCoordPos.x < 0 || someScreenCoordPos.x > aViewport.x - 1 ||
		someScreenCoordPos.y < 0 || someScreenCoordPos.y > aViewport.y - 1)
	{
		anOutputElement = 0;
		return false;
	}


	auto& device = DXInterface::Device;
	auto& context = DXInterface::Context;



	DXTexture2DRaw* pixel = nullptr;
	D3D11_BOX srcBox{};

	DXResourceRaw* src;
	myRenderView.Get()->GetResource(&src);
	D3D11_MAPPED_SUBRESOURCE msr = {};

	D3D11_TEXTURE2D_DESC textDesc = { 0 };
	textDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textDesc.Usage = D3D11_USAGE_STAGING;
	textDesc.SampleDesc.Count = 1;
	textDesc.SampleDesc.Quality = 0;
	textDesc.ArraySize = 1;
	textDesc.MipLevels = 1;
	textDesc.Format = DXGI_FORMAT_R32_UINT;

	textDesc.Width = 1;
	textDesc.Height = 1;

	float aspectRatio = aViewport.y / aViewport.x;

	srcBox.left = someScreenCoordPos.x;
	srcBox.right = (someScreenCoordPos.x + 1);
	srcBox.bottom = (someScreenCoordPos.y + 1);
	srcBox.top = someScreenCoordPos.y;

	srcBox.front = 0;
	srcBox.back = 1;

	if (FAILED(device->CreateTexture2D(&textDesc, nullptr, &pixel)))
	{
		anOutputElement = 0;
		return false;
	}


	context->CopySubresourceRegion(pixel, 0, 0, 0, 0, src, 0, &srcBox);



	context->Map(pixel, 0, D3D11_MAP_READ, 0, &msr);

	anOutputElement = *reinterpret_cast<uint32_t*>(msr.pData);
	context->Unmap(pixel, 0);

	return anOutputElement != 0;
}

void Dragonite::RenderID::Render()
{

	auto og = DXInterface::ClearColor;
	DXInterface::ClearColor = Color(0, 0, 0, 0);
	DXInterface::SwitchRenderTarget(myRenderView, DXInterface::GetDepthBuffer());
	myPipeline->DrawInstructions(
		myPipeline->GetShaderInstructions()[0].myVertexShader,
		myWriteRenderIDPixelShader,
		myPipeline->GetShaderInstructions()[0].myInputLayout,
		[this](RenderInstructions anInstruction) mutable {

			RenderIDBuffer data;
			data.myID = anInstruction.myID;
			DXInterface::ModifyBuffer(myRenderIDBuffer, Data(&data));
			DXInterface::Context->PSSetConstantBuffers(2, 1, myRenderIDBuffer.GetAddressOf());

		});
	DXInterface::ClearColor = og;
}
