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


LRESULT Dragonite::Runtime::LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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





Dragonite::Runtime::Runtime() = default;

Dragonite::Runtime::~Runtime()
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

bool Dragonite::Runtime::Initialize(HWND& anInstance, EmberGUI* anEditorInterface)
{
	myGUIInterface = nullptr;

	myGraphicsEngine = GraphicsEngine::InitializeEngine(anInstance);

	myRuntimeHandler = new PollingStation();

	myInstance = anInstance;
	myEditorInterface = anEditorInterface;

	bool existsEditor = myEditorInterface;

	if (existsEditor)
	{
		myEditorInterface->Init(anInstance);
 		//myEditorInterface->AddSpace(new EmberAPI::Viewport("Scene", 1920, 1080, []()
			//{
			//	//Render your scene here
			//}));

	}
	auto IM = myRuntimeHandler->AddHandler(new InputManager());

	if (!IM->Init(this)) return false;
	myRuntimeHandler->AddHandler(this);
	return true;
}

void Dragonite::Runtime::Update(const float aDeltaTime)
{
	if (myEditorInterface)
		myEditorInterface->Update(aDeltaTime);
}

DLLEXPORT APIInterface* InitializeRuntime()
{
	using namespace Dragonite;

	Runtime* runtime = new Runtime();
	return runtime;
}
