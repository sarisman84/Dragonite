#include "Viewport.h"
#include "Core/Graphics/GraphicsAPI.h"
#include "Core/RuntimeAPI/NEW/Scene.h"
#include "Core/RuntimeAPI/NEW/Object.h"
#include "Core/RuntimeAPI/Components/ModelRenderer.h"
#include "Core/RuntimeAPI/SceneManagement/SceneBuilder.h"
#include "Core/RuntimeAPI/Components/SpriteRenderer.h"

#include "Core/Editor/SceneEditor.h"
#include "Core/Editor/AssetBrowser.h"

#include "Core/Utilities/Input.h"

#include "Core/Graphics/Models/ModelFactory.h"
#include "Core/Graphics/Textures/TextureFactory.h"
#include "Core/CU/Math/MathFunctions.h"

#include "Core/ImGui/DragoniteGui.h"
#include "Core/Runtime.h"




#include <d3d11.h>


Dragonite::Viewport::Viewport() : GUIWindow("Scene"),
myEditorCameraInterface(CameraInterface()),
myPerspectiveProfile(PerspectiveProfile(90.0f, 0.1f, 1000.0f)),
myOrthographicProfile(OrthographicProfile(Vector2f(1920.0f, 1080.0f), 0.1f, 1000.0f))
{
	myEditorCameraInterface.AddLayer(&myPerspectiveProfile);
	myEditorCameraInterface.AddLayer(&myOrthographicProfile);
}

Dragonite::Viewport::~Viewport()
{
	if (myScene)
		delete myScene;
	myScene = nullptr;
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
	aMouse->SetOffset(myMinRegion);
	auto pos = aMouse->position;
	return pos;
}

void Dragonite::Viewport::ManipulateObject(Dragonite::Scene* aScene, Dragonite::Object* anObject)
{
	myIsManipulatingFlag = false;
	if (!anObject) return;

	auto sprite = anObject->GetComponent<SpriteRenderer>();
	bool isSprite = sprite != nullptr;

	auto pProfile = isSprite ? aScene->GetMainCamera().Profiles()[1] : aScene->GetMainCamera().Profiles()[0];
	auto eProfile = isSprite ? myEditorCameraInterface.Profiles()[1] : myEditorCameraInterface.Profiles()[0];


	Matrix4x4f view = myIsInPlayFlag ? aScene->GetMainCamera().ViewMatrix() : myEditorCameraInterface.ViewMatrix();
	Matrix4x4f proj = myIsInPlayFlag ? pProfile->CalculateProjectionMatrix() : eProfile->CalculateProjectionMatrix();
	Matrix4x4f transform = isSprite ? sprite->GetLocal2DMatrix() : anObject->myTransform.GetMatrix();
	ImGuizmo::Manipulate(&view, &proj, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, &transform);


	if (ImGuizmo::IsUsing())
	{
		myIsManipulatingFlag = true;
		anObject->myTransform.SetMatrix(transform);
	}

}

void Dragonite::Viewport::OnWindowInit()
{
	myGraphicsInterface = myPollingStation->Get<GraphicalInterface>();
	myTextureFactory = myPollingStation->Get<TextureFactory>();
	myScene = myDragoniteGuiAPI->GetFocusedScene();
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

	mySaveIcon = myTextureFactory->LoadTexture(L"resources/textures/icons/load.png");
	myPlayIcon = myTextureFactory->LoadTexture(L"resources/textures/icons/play-button.png");
	myStopIcon = myTextureFactory->LoadTexture(L"resources/textures/icons/pause-button.png");


	myGraphicsInterface->SetActiveCameraAs(myEditorCameraInterface);


}



void Dragonite::Viewport::OnWindowUpdate()
{
	ImGui::ShowDemoWindow();




	myMinRegion.x = ImGui::GetWindowContentRegionMin().x;
	myMinRegion.y = ImGui::GetWindowContentRegionMin().y + 37.0f;

	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, myCurrentResolution.x, myCurrentResolution.y);
	ImGuizmo::DrawGrid(&myEditorCameraInterface.ViewMatrix(), &myEditorCameraInterface.Profile()->CalculateProjectionMatrix(), &Matrix4x4f(), 1.0f);

	if (myScene)
	{
		auto p = dynamic_cast<OrthographicProfile*>(myScene->GetMainCamera().Profiles()[1]);
		p->myCurrentViewPort = myCurrentResolution;
	}
		

	RenderViewport();
	RenderTopBar();
	DetectAssetDrop();


	if (auto scene = myDragoniteGuiAPI->GetFocusedScene())
	{
		auto obj = mySceneEditor->GetInspectedObject();
		ManipulateObject(scene, obj);
	}




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


	myRenderID.SetTotalElementCount(static_cast<float>(mySceneEditor->GetCurrentScene()->SceneObjects().size()));
	myRenderID.RefreshRenderView(myCurrentResolution);
	myRenderID.Render();
	DXInterface::SwitchRenderTarget(myViewportTarget, DXInterface::GetDepthBuffer());

	if (myRenderID.ViewRenderID())
		myRenderID.RenderIDTexture();
	else
		myGraphicsInterface->DrawInstructions();
	DXInterface::SwitchRenderTarget(DXInterface::GetBackBuffer(), DXInterface::GetDepthBuffer());


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
				if (myScene)
				{
					delete myScene;
				}
				myScene = new Scene(*myPollingStation, absPath.string());
				myDragoniteGuiAPI->FocusScene(myScene);
			}
			else if (foundObj)
			{

				if (extension == ".dds" ||
					extension == ".png")
				{
					std::shared_ptr<ModelRenderer> mRenderer = foundObj->GetComponent<ModelRenderer>();
					std::shared_ptr<SpriteRenderer> sRenderer = foundObj->GetComponent<SpriteRenderer>();

					if (mRenderer)
						mRenderer->Model()->myTexture = myTextureFactory->LoadTexture(file->path().wstring().c_str());
					if (sRenderer)
						sRenderer->Sprite()->myTexture = myTextureFactory->LoadTexture(file->path().wstring().c_str());
				}
			}



		}

		ImGui::EndDragDropTarget();

	}
}

void Dragonite::Viewport::RenderTopBar()
{
	static std::string sceneCpy;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

	const float PAD = 10.0f;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
	ImVec2 work_size = viewport->WorkSize;
	ImVec2 window_pos, window_pos_pivot;
	window_pos.x = ImGui::GetWindowPos().x + ImGui::GetWindowWidth() / 2.0f;
	window_pos.y = ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y + PAD;
	window_pos_pivot.x = 0.5f;
	window_pos_pivot.y = 0.0f;
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowViewport(viewport->ID);
	window_flags |= ImGuiWindowFlags_NoMove;

	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	auto runtime = myPollingStation->Get<Runtime>();
	if (ImGui::Begin("Top Bar", NULL, window_flags))
	{
		if (ImGui::ImageButton("play", myPlayIcon->GetData().Get(), ImVec2(16, 16)))
		{


			myIsInPlayFlag = true;
			sceneCpy = Scene::LastSavedPath();
			if (sceneCpy.empty())
				sceneCpy = "temp/tempscene.json";
			myScene->Serialize("temp/tempscene.json");
			if (myScene)
			{
				delete myScene;
			}
			myScene = new Scene(*myPollingStation, "temp/tempscene.json");
			myDragoniteGuiAPI->FocusScene(myScene);
			myScene->Start();
			runtime->OnUpdate() += [this](const float aDt)
			{
				myScene->Update(aDt);
			};



		}
		ImGui::SameLine();
		if (ImGui::ImageButton("stop", myStopIcon->GetData().Get(), ImVec2(16, 16)))
		{
			myIsInPlayFlag = false;


			runtime->OnUpdate().operator--();
			if (myScene)
			{
				delete myScene;
			}
			myScene = new Scene(*myPollingStation, sceneCpy);
			myDragoniteGuiAPI->FocusScene(myScene);
			myGraphicsInterface->SetActiveCameraAs(myEditorCameraInterface);


		}

	}
	ImGui::SameLine();
	if (ImGui::ImageButton("save", mySaveIcon->GetData().Get(), ImVec2(16, 16)))
	{
		if (myIsInPlayFlag)
		{
			runtime->OnUpdate().operator--();

			if (myScene)
			{
				delete myScene;
			}
			myScene = new Scene(*myPollingStation, sceneCpy);
			myDragoniteGuiAPI->FocusScene(myScene);
		}
		myIsInPlayFlag = false;
		myGraphicsInterface->SetActiveCameraAs(myEditorCameraInterface);
		mySceneEditor->SaveScene();
	}

	ImGui::End();

}

