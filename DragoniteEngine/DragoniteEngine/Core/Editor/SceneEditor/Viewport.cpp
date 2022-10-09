#include "Viewport.h"
#include "Core/Graphics/GraphicsAPI.h"
#include "Core/RuntimeAPI/Scene.h"
#include "Core/RuntimeAPI/Object.h"

#include "Core/Utilities/Input.h"

#include <d3d11.h>

Dragonite::Viewport::Viewport() : GUIWindow("Scene")
{
}

Dragonite::Viewport::~Viewport()
{
}

const bool Dragonite::Viewport::TryGetObjectID(Mouse* aMouse, int& anId)
{
	auto viewport = DXInterface::GetViewportResolution();
	auto pos = aMouse->position - myMinRegion;
	bool r = myRenderID.TryGetElement(pos, myCurrentResolution, anId);
	myFoundID = anId;

	return r;
}

void Dragonite::Viewport::DisplayMouseCoordinateInViewport(Mouse* aMouse)
{

	//ImGui::OpenPopup("mouse_coordinates");
	if (ImGui::CollapsingHeader("Debug")) {
		ImGui::Indent();
		auto pos = aMouse->position - myMinRegion;
		ImGui::Text("Mouse Position, (%0.0f, %0.0f)", aMouse->position.x, aMouse->position.y);
		ImGui::Text("Viewport Mouse Position, (%0.0f, %0.0f)", pos.x, pos.y);
		ImGui::Text("Min Region (%0.0f, %0.0f)", myMinRegion.x, myMinRegion.y);
		ImGui::Text("Resolution (%0.0f, %0.0f)", myCurrentResolution.x, myCurrentResolution.y);
		ImGui::Text("Found ID: %i", myFoundID);
		ImGui::Unindent();
	}




}

void Dragonite::Viewport::OnWindowInit()
{
	myGraphicsInterface = myPollingStation->Get<GraphicalInterface>();
	myCurrentScene = myPollingStation->Get<Scene>();
	myRenderID = RenderID(myGraphicsInterface);

	D3D11_TEXTURE2D_DESC desc = { 0 };

	auto resolution = DXInterface::GetViewportResolution();

	desc.Width = resolution.x;
	desc.Height = resolution.y;

	desc.MipLevels = 1;
	desc.ArraySize = 1;

	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	DXTexture2D texture;
	HRESULT result;
	result = DXInterface::Device->CreateTexture2D(&desc, nullptr, &texture);
	assert(SUCCEEDED(result));
	result = DXInterface::Device->CreateShaderResourceView(texture.Get(), nullptr, &myViewportResource);
	assert(SUCCEEDED(result));
	result = DXInterface::Device->CreateRenderTargetView(texture.Get(), nullptr, &myViewportTarget);
	assert(SUCCEEDED(result));


}



void Dragonite::Viewport::OnWindowUpdate()
{
	auto resolution = DXInterface::GetViewportResolution();
	myMinRegion.x = ImGui::GetWindowContentRegionMin().x;
	myMinRegion.y = ImGui::GetWindowContentRegionMin().y + 37.0f;


	myMousePos.x;
	myMousePos.y;

	myPreviousResolution = myCurrentResolution;


	myAspectRatio = (resolution.y / resolution.x) * (ImGui::GetWindowHeight() / resolution.y) * 1.75f;
	myCurrentResolution = resolution * myAspectRatio;
	myRenderID.Render();
	DXInterface::SwitchRenderTarget(myViewportTarget, DXInterface::GetDepthBuffer());
	myGraphicsInterface->DrawInstructions();
	DXInterface::SwitchRenderTarget(DXInterface::GetBackBuffer(), DXInterface::GetDepthBuffer());
	ImGui::Image(myViewportResource.Get(), ImVec2(myCurrentResolution.x, myCurrentResolution.y));

	myFocusFlag = ImGui::IsWindowFocused();

}

void Dragonite::Viewport::OnEnable()
{
	myGraphicsInterface->DrawToBackBuffer(false);
}

void Dragonite::Viewport::OnDisable()
{
	myGraphicsInterface->DrawToBackBuffer(true);
}
