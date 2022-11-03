#include <iostream>
#include "Runtime.h"
#include "Core/RuntimeAPI/NEW/Scene.h"
#include "EditorAPI/PresetSpaces/Viewport.h"
//#include "ImGui/DragoniteGui.h"
#include "Core/PollingStation.h"
#include "Pipeline/Rendering/GraphicsEngine.h"
#include "Utilities/Input.h"

//#include "Editor/EngineDebugger.h"
//#include "Editor/SceneEditor.h"
//#include "Editor//AssetBrowser.h"

#include "Core/RuntimeAPI/SceneManagement/SceneBuilder.h"
#include "nlohmann/single_include/nlohmann/json.hpp"
#include "Pipeline/Rendering/DX/DXDrawer.h"
#include "EditorAPI/Space/WindowSpace.h"

#include "imgui/imgui.h"


LRESULT Dragonite::Engine::LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{



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
		auto drawer = myGraphicsEngine->GetDrawer<DirectXDrawer>();
		myEditorInterface->Init(
			anInstance,
			drawer->Device(),
			drawer->Context(),
			drawer->SwapChain());
		myEditorInterface->AddEditor("Test", [this](GUISpace* aSelf, void*)
			{
				ImGui::SetCurrentContext(aSelf->myGUIInterface->GetIMGUIContext());

				ImVec4 test;
				ImGui::ColorButton("Yes", test);
				ImGui::ColorPicker3("More Yes", (float*)&test);
				//ImGui::Text("This is a space!");
				//Render your scene here
			});

	}
	auto IM = myRuntimeHandler->AddHandler(new InputManager());

	if (!IM->Init(this)) return false;
	myRuntimeHandler->AddHandler(this);
	return true;
}

void Dragonite::Engine::Update(const float aDeltaTime)
{
	if (myEditorInterface)
		myEditorInterface->Update(aDeltaTime, myGraphicsEngine->GetBackBuffer());

	myGraphicsEngine->Draw();
}

DLLEXPORT EngineAPI* InitializeRuntime()
{
	using namespace Dragonite;

	Engine* runtime = new Engine();
	return runtime;
}
