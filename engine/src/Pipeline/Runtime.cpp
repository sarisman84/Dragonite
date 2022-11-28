#include <iostream>
#include "Runtime.h"
#include "Core/RuntimeAPI/NEW/Scene.h"
#include "EditorAPI/PresetSpaces/Viewport.h"
//#include "ImGui/DragoniteGui.h"
#include "Core/PollingStation.h"
#include "Pipeline/Rendering/GraphicsEngine.h"
#include "Core/Utilities/Input.h"

//#include "Editor/EngineDebugger.h"
//#include "Editor/SceneEditor.h"
//#include "Editor//AssetBrowser.h"

#include "Core/RuntimeAPI/SceneManagement/SceneBuilder.h"
#include "nlohmann/single_include/nlohmann/json.hpp"
#include "Pipeline/Rendering/DX/DXDrawer.h"
#include "EditorAPI/Space/WindowSpace.h"
#include "EditorAPI/PresetSpaces/Viewport.h"

#include "Pipeline/Rendering/Content/ContentFactories.h"

#include "imgui/imgui.h"


LRESULT Dragonite::Engine::LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	for (size_t i = 0; i < myWinProcEvents.size(); i++)
	{
		myWinProcEvents[i](hWnd, message, wParam, lParam);
	}


	if (message == WM_DESTROY)
	{
		myRuntimeState = false;
		PostQuitMessage(0);
		return 1;
	}

	if (myEditorInterface)
		myEditorInterface->WinProc(hWnd, message, wParam, lParam);



	return 0;
}





Dragonite::Engine::Engine()
{
	myEditorInterface = nullptr;
	myGUIInterface = nullptr;
	myRuntimeHandler = nullptr;
	myGraphicsEngine = nullptr;
}

Dragonite::Engine::~Engine()
{
	if (myRuntimeHandler)
		delete myRuntimeHandler;
	myRuntimeHandler = nullptr;

	if (myGUIInterface)
		delete myGUIInterface;
	myGUIInterface = nullptr;


	if (myGraphicsEngine)
		delete myGraphicsEngine;
	myGraphicsEngine = nullptr;

}

void Dragonite::Engine::RegisterUpdateListener(const std::function<void(const float)>& anEvent)
{
	myUpdateEvents.push_back(anEvent);
}

void Dragonite::Engine::RegisterWinProcListener(const std::function<void(HWND, UINT, WPARAM, LPARAM)>& anEvent)
{
	myWinProcEvents.push_back(anEvent);
}

bool Dragonite::Engine::Initialize(HWND& anInstance, EmberGUI* anEditorInterface)
{
	myGUIInterface = nullptr;

	myGraphicsEngine = GraphicsEngine::InitializeEngine(anInstance);

	myRuntimeHandler = new PollingStation();

	myInstance = anInstance;
	myEditorInterface = anEditorInterface;

	bool existsEditor = myEditorInterface;

	if (existsEditor)
	{
		auto drawer = myGraphicsEngine->GetDrawer<DXDrawer>();
		myEditorInterface->Init(
			anInstance,
			drawer->Device(),
			drawer->Context(),
			drawer->SwapChain(),
			myGraphicsEngine->BackBuffer());

		myEditorInterface->AddEditor(new ember::SceneView(
			[this](ID3D11RenderTargetView* aView) 
			{ 
				myGraphicsEngine->Draw(aView); 
				myGraphicsEngine->Present(); 
			}));

	}
	auto IM = myRuntimeHandler->AddHandler(new InputManager());

	if (!IM->Init(this)) return false;
	myRuntimeHandler->AddHandler(this);

	using namespace Dragonite;



	Dragonite::DrawInstruct data;
	data.myDrawType = 0;
	data.myMaterial = MaterialFactory::API()->GetMaterial(Materials::Unlit, "someTexture.dds");
	data.myModelID = ModelFactory::API()->GetModel(Primitive::Cube);
	myGraphicsEngine->Submit(data);

	return true;
}

void Dragonite::Engine::Update(const float aDeltaTime)
{
	for (size_t i = 0; i < myUpdateEvents.size(); i++)
	{
		myUpdateEvents[i](aDeltaTime);
	}

	if (myEditorInterface)
		myEditorInterface->Update();
	myGraphicsEngine->Present();
}

DLLEXPORT EngineAPI* InitializeRuntime()
{
	using namespace Dragonite;

	Engine* runtime = new Engine();
	return runtime;
}
