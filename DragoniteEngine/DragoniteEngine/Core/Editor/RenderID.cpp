#include "RenderID.h"
#include "Core/Graphics/GraphicsAPI.h"
#include "Core/RuntimeAPI/Scene.h"
#include "Core/Graphics/Models/Model.h"
#include "Core/Graphics/Models/ModelFactory.h"
#include "Core/Graphics/Utilities/RenderUtils.h"

#include "Core/Runtime.h"
#include "Core/PollingStation.h"

#include "Core/Graphics/Camera.h"
#include "Core/Utilities/Input.h"

#include <d3d11.h>
Dragonite::RenderID::RenderID() = default;
Dragonite::RenderID::~RenderID() = default;
Dragonite::RenderID::RenderID(GraphicsPipeline* aPipeline) : RenderTarget(aPipeline, RenderTargetDesc())
{
	RenderTargetDesc fullscreenPass = {};
	fullscreenPass.myResolution = aPipeline->GetViewPort();
	fullscreenPass.mySample.Count = 1;
	fullscreenPass.mySample.Quality = 0;
	fullscreenPass.myFormat = DXGI_FORMAT_R32_UINT;
	fullscreenPass.myArraySize = 1;
	fullscreenPass.myMipLevels = 1;
	fullscreenPass.myUseage = (UINT)D3D11_USAGE_DEFAULT;
	fullscreenPass.myCPUAccessFlags = 0;
	fullscreenPass.myMiscFlags = 0;

	InternalInit(fullscreenPass);

	std::string vsData;

	//Create VS and PS Instances
	assert(SUCCEEDED(RenderUtils::CreateVSInstance(myPipeline->GetDevice(), "Fullscreen", myRenderIDVertexShader, vsData)));
	assert(SUCCEEDED(RenderUtils::CreatePSInstance(myPipeline->GetDevice(), "RenderID", myWriteRenderIDPixelShader)));
	assert(SUCCEEDED(RenderUtils::CreatePSInstance(myPipeline->GetDevice(), "FullscreenUnlit", myReadRenderIDPixelShader)));


	DataBufferDesc desc = DataBufferDesc(sizeof(RenderIDBuffer), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE);


	assert(SUCCEEDED(myPipeline->CreateBuffer(myPipeline->GetDevice(), myRenderIDBuffer, desc)));


}

void Dragonite::RenderID::SetupRenderID(Scene* aCurrentScene)
{
	if (!aCurrentScene) return;
	myScene = aCurrentScene;

	auto& pollingStation = myPipeline->GetApplication()->GetPollingStation();
	auto mf = pollingStation.Get<ModelFactory>();


}

const bool Dragonite::RenderID::OnRender()
{
	if (!myScene) return false;

	auto myContext = myPipeline->GetContext();
	auto myTextureSamplers = myPipeline->GetTextureSamplers();
	auto myElementsToDraw = myPipeline->GetInstructions();


	
	myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	myPipeline->UpdateFrameBuffer();
	auto cpy = myElementsToDraw;
	auto size = cpy.size();

	while (!cpy.empty())
	{

		auto element = cpy.back();
		cpy.pop_back();


		myContext->IASetInputLayout(element->myInputLayout.Get());

		RenderIDBuffer buffer;

		buffer.myID = element->myID;


		myPipeline->UpdateObjectBufferAt(element);
		myPipeline->UpdateBufferAt(&buffer, sizeof(RenderIDBuffer), 2, myRenderIDBuffer, false, true);

		{
			unsigned int stride = sizeof(Vertex);
			unsigned int offset = 0;

			myContext->IASetVertexBuffers(0, 1, element->myModel->myVertexBuffer.GetAddressOf(), &stride, &offset);
			myContext->IASetIndexBuffer(element->myModel->myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			myContext->VSSetShader(element->myVertexShader.Get(), nullptr, 0);
			myContext->PSSetShader(myWriteRenderIDPixelShader.Get(), nullptr, 0);
			myContext->PSSetSamplers(0, 1, &myTextureSamplers[TextureSampleType::Default]);
		}



		myContext->DrawIndexed(element->myModel->myIndexCount, 0, 0);

	}
	if (myViewRenderID)
	{
		DrawRenderID();
		return true;
	}


	return false;
}

const bool Dragonite::RenderID::TryGetElement(Mouse* someScreenCoordPos, int& anOutputElement)
{
	auto viewport = myPipeline->GetViewPort();
	if (someScreenCoordPos->position.x < 0 || someScreenCoordPos->position.x > viewport.x - 1 ||
		someScreenCoordPos->position.y < 0 || someScreenCoordPos->position.y > viewport.y - 1)
	{
		anOutputElement = 0;
		return false;
	}
		

	auto& device = myPipeline->GetDevice();
	auto& context = myPipeline->GetContext();

	

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
	//anOutputElement = //TODO: Get actual ID.
	context->Unmap(pixel, 0);

	return anOutputElement != 0;
}

void Dragonite::RenderID::DrawRenderID()
{
	auto myContext = myPipeline->GetContext();
	auto myTextureSamplers = myPipeline->GetTextureSamplers();

	myPipeline->SetBlendState(BlendStateType::Alpha);
	myPipeline->SwitchRenderTarget(myPipeline->GetBackBuffer(), myPipeline->GetDefaultDepthBuffer());


	myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	myContext->PSSetShaderResources(0, 1, myResourceView.GetAddressOf());

	myContext->IASetInputLayout(nullptr);

	{
		myContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
		myContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

		myContext->VSSetShader(myRenderIDVertexShader.Get(), nullptr, 0);
		myContext->PSSetShader(myReadRenderIDPixelShader.Get(), nullptr, 0);
		myContext->PSSetSamplers(0, 1, &myTextureSamplers[TextureSampleType::Default]);
	}



	myContext->Draw(3, 0);
	myPipeline->SetBlendState(BlendStateType::None);
}
