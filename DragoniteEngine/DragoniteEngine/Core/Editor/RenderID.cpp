#include "RenderID.h"
#include "Core/Graphics/GraphicsAPI.h"
#include "Core/RuntimeAPI/Scene.h"
#include "Core/Graphics/Models/ModelFactory.h"
#include "Core/Graphics/Utilities/RenderUtils.h"

#include "Core/Runtime.h"
#include "Core/PollingStation.h"

#include <d3d11.h>
Dragonite::RenderID::RenderID() = default;
Dragonite::RenderID::~RenderID() = default;
Dragonite::RenderID::RenderID(GraphicsPipeline* aPipeline) : RenderTarget(aPipeline, RenderTargetDesc())
{
	RenderTargetDesc fullscreenPass = {};
	fullscreenPass.myResolution = aPipeline->GetViewPort();
	fullscreenPass.mySample.Count = 1;
	fullscreenPass.mySample.Quality = 0;
	fullscreenPass.myFormat = DXGI_FORMAT_R32_FLOAT;
	fullscreenPass.myArraySize = 1;
	fullscreenPass.myMipLevels = 1;
	fullscreenPass.myUseage = (UINT)D3D11_USAGE_DEFAULT;
	fullscreenPass.myCPUAccessFlags = 0;
	fullscreenPass.myMiscFlags = 0;

	InternalInit(fullscreenPass);

	std::string vsData;

	//Create VS and PS Instances
	assert(SUCCEEDED(RenderUtils::CreateVSInstance(myPipeline->GetDevice(), "RenderID", myRenderIDVertexShader, vsData)));
	assert(SUCCEEDED(RenderUtils::CreatePSInstance(myPipeline->GetDevice(), "RenderID", myRenderIDPixelShader)));

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//Create InputLayout
	myPipeline->GetDevice()->CreateInputLayout(layout, 1, vsData.data(), vsData.size(), &myRenderIDLayout);


	DataBufferDesc desc = DataBufferDesc(sizeof(RenderIDBuffer), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE);


	assert(SUCCEEDED(myPipeline->CreateBuffer(myPipeline->GetDevice(), myRenderIDBuffer, desc)));


}

void Dragonite::RenderID::SetupRenderID(Scene* aCurrentScene)
{
	if (!aCurrentScene) return;
	myScene = aCurrentScene;
}

const bool Dragonite::RenderID::OnRender()
{
	if (!myScene) return false;

	auto myContext = myPipeline->GetContext();
	auto myTextureSamplers = myPipeline->GetTextureSamplers();
	auto myElementsToDraw = myPipeline->GetInstructions();

	myContext->IASetInputLayout(myRenderIDLayout.Get());

	myPipeline->UpdateFrameBuffer();
	auto cpy = myElementsToDraw;
	int index = 0;
	while (!cpy.empty())
	{
		auto element = cpy.back();
		cpy.pop_back();


		myContext->PSSetSamplers(0, 1, &myTextureSamplers[TextureSampleType::Default]);

		RenderIDBuffer buffer;
		buffer.myID = index++;


		myPipeline->UpdateObjectBufferAt(element);
		myPipeline->UpdateBufferAt(&buffer, sizeof(RenderIDBuffer), element, 2, myRenderIDBuffer, false, true);

		{
			unsigned int stride = sizeof(Vertex);
			unsigned int offset = 0;

			myContext->IASetVertexBuffers(0, 1, element->myModel->myVertexBuffer.GetAddressOf(), &stride, &offset);
			myContext->IASetIndexBuffer(element->myModel->myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			myContext->VSSetShader(element->myVertexShader.Get(), nullptr, 0);
			myContext->PSSetShader(element->myPixelShader.Get(), nullptr, 0);
		}



		myContext->DrawIndexed(element->myModel->myIndexCount, 0, 0);

	}


	return myViewRenderID;
}
