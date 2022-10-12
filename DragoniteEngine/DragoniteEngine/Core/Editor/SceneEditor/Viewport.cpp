#include "Viewport.h"
#include "Core/Graphics/GraphicsAPI.h"
#include "Core/RuntimeAPI/Scene.h"
#include "Core/RuntimeAPI/Object.h"
#include "Core/RuntimeAPI/Components/ModelRenderer.h"
#include "Core/RuntimeAPI/SceneManagement/SceneBuilder.h"

#include "Core/Editor/SceneEditor.h"
#include "Core/Editor/AssetBrowser.h"

#include "Core/Utilities/Input.h"

#include "Core/Graphics/Models/ModelFactory.h"
#include "Core/Graphics/Textures/TextureFactory.h"
#include "Core/CU/Math/MathFunctions.h"




#include <d3d11.h>

Dragonite::Viewport::Viewport() : GUIWindow("Scene"),
myEditorCameraInterface(CameraInterface()),
myPerspectiveProfile(PerspectiveProfile(90.0f, 0.1f, 1000.0f))
{
	myEditorCameraInterface.Profile() = &myPerspectiveProfile;
}

Dragonite::Viewport::~Viewport()
{
}

const bool Dragonite::Viewport::TryGetObjectID(Mouse* aMouse, int& anId)
{
	bool r = false;
	if (!myIsManipulatingFlag)
	{

		r = myRenderID.TryGetElement(GetLocalMousePos(aMouse), anId);
		myLastValidElement = anId;
	}
	else
		anId = myLastValidElement;
	myFoundID = anId;

	return r;
}

void Dragonite::Viewport::DisplayDebugInfo(Mouse* aMouse)
{

	//ImGui::OpenPopup("mouse_coordinates");
	if (ImGui::CollapsingHeader("Debug"))
	{
		ImGui::Indent();
		auto pos = GetLocalMousePos(aMouse);
		ImGui::Text("Mouse Position, (%0.0f, %0.0f)", aMouse->position.x, aMouse->position.y);
		ImGui::Text("Viewport Mouse Position, (%0.0f, %0.0f)", pos.x, pos.y);
		ImGui::Text("Min Region (%0.0f, %0.0f)", myMinRegion.x, myMinRegion.y);
		ImGui::Text("Resolution (%0.0f, %0.0f)", myCurrentResolution.x, myCurrentResolution.y);
		ImGui::Text("Found ID: %i", myFoundID);
		ImGui::Checkbox("View RenderID", &myRenderID.ViewRenderID());

		ImGui::Unindent();
	}




}

const Dragonite::Vector2f Dragonite::Viewport::GetLocalMousePos(Mouse* aMouse)
{
	auto viewport = DXInterface::GetViewportResolution();
	auto pos = aMouse->position - myMinRegion;
	return pos;
}

void Dragonite::Viewport::ManipulateObject(Dragonite::Scene* aScene, Dragonite::Object* anObject)
{
	myIsManipulatingFlag = false;
	if (!anObject) return;

	Matrix4x4f view = myEditorCameraInterface.ViewMatrix();
	Matrix4x4f proj = myEditorCameraInterface.Profile()->CalculateProjectionMatrix();
	Matrix4x4f transform = anObject->GetTransform().GetMatrix();
	ImGuizmo::Manipulate(&view, &proj, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, &transform);


	if (ImGuizmo::IsUsing())
	{
		myIsManipulatingFlag = true;
		anObject->GetTransform().SetMatrix(transform);
	}

}

void Dragonite::Viewport::OnWindowInit()
{
	myGraphicsInterface = myPollingStation->Get<GraphicalInterface>();
	myTextureFactory = myPollingStation->Get<TextureFactory>();
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

	myRenderID.ViewRenderID() = false;
}



void Dragonite::Viewport::OnWindowUpdate()
{
	ImGui::ShowDemoWindow();


	myMinRegion.x = ImGui::GetWindowContentRegionMin().x;
	myMinRegion.y = ImGui::GetWindowContentRegionMin().y + 37.0f;

	ImGuizmo::SetRect(myMinRegion.x, myMinRegion.y, myCurrentResolution.x, myCurrentResolution.y);
	ImGuizmo::DrawGrid(&myEditorCameraInterface.ViewMatrix(), &myEditorCameraInterface.Profile()->CalculateProjectionMatrix(), &Matrix4x4f(), 1.0f);

	RenderViewport();
	DetectAssetDrop();

	auto obj = mySceneEditor->GetInspectedObject();

	ManipulateObject(myCurrentScene, obj);



}

void Dragonite::Viewport::OnEnable()
{
	myGraphicsInterface->DrawToBackBuffer(false);
}

void Dragonite::Viewport::OnDisable()
{
	myGraphicsInterface->DrawToBackBuffer(true);
}

void Dragonite::Viewport::RenderViewport()
{
	auto resolution = DXInterface::GetViewportResolution();
	myAspectRatio = (resolution.x / resolution.y);
	Vector2f scaleDif = Vector2f(ImGui::GetWindowWidth() / resolution.x, ImGui::GetWindowHeight() / resolution.y);

	float scaleVal = 0.0f;

	if (resolution.y * scaleDif.x < ImGui::GetWindowHeight())
		scaleVal = scaleDif.x * 0.95f;
	else if (resolution.x * scaleDif.y < ImGui::GetWindowWidth())
		scaleVal = scaleDif.y * 0.95f;
	else
		scaleVal = 1;

	myCurrentResolution = (Vector2f(resolution.x, resolution.y)) * (scaleVal);

	myGraphicsInterface->SetActiveCameraAs(myEditorCameraInterface);
	myRenderID.SetTotalElementCount(static_cast<float>(mySceneEditor->GetCurrentScene()->SceneObjects().size()));
	myRenderID.RefreshRenderView(myCurrentResolution);
	myRenderID.Render();
	DXInterface::SwitchRenderTarget(myViewportTarget, DXInterface::GetDepthBuffer());


	if (myRenderID.ViewRenderID())
		myRenderID.RenderIDTexture();
	else
		myGraphicsInterface->DrawInstructions();
	DXInterface::SwitchRenderTarget(DXInterface::GetBackBuffer(), DXInterface::GetDepthBuffer());
	auto cam = myCurrentScene->GetCamera();
	myGraphicsInterface->SetActiveCameraAs(*cam);
	myRenderID.SetViewport(myCurrentResolution, Vector2f(0, 0));
	ImGui::Image(myViewportResource.Get(), ImVec2(myCurrentResolution.x, myCurrentResolution.y));

}

void Dragonite::Viewport::DetectAssetDrop()
{
	auto assetBrowser = mySceneEditor->GetAssetBrowser();



	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* load = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM", ImGuiDragDropFlags_SourceExtern);



		mySceneEditor->TryGetNewElement();


		auto foundObj = mySceneEditor->GetInspectedObject();
		if (load && load->IsDelivery())
		{
			std::filesystem::directory_entry* file = (std::filesystem::directory_entry*)load->Data;
			auto extension = file->path().filename().extension();

			if (extension == ".json")
			{
				auto path = file->path();
				auto absPath = std::filesystem::absolute(file->path());
				SceneBuilder::LoadScene(path.string().c_str(), *myCurrentScene);
			}
			else if (foundObj)
			{

				if (extension == ".dds" ||
					extension == ".png")
				{
					std::shared_ptr<ModelRenderer> renderer = foundObj->GetComponent<ModelRenderer>();
					renderer->Model()->myTexture = myTextureFactory->LoadTexture(file->path().wstring().c_str());
				}
			}



		}

		ImGui::EndDragDropTarget();

	}
}
