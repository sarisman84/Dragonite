#include <iostream>
#include "Runtime.h"
#include "Core/RuntimeAPI/NEW/Scene.h"
#include "EditorAPI/PresetSpaces/Viewport.h"
//#include "ImGui/DragoniteGui.h"
#include "Core/PollingStation.h"
#include "Graphics/GraphicsAPI.h"
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

	myWndProcs(hWnd, message, wParam, lParam);

	return 0;
}

void Dragonite::Runtime::FocusScene(Scene* aNewScene)
{
	myScene = aNewScene;
}

Dragonite::Scene* Dragonite::Runtime::GetFocusedScene()
{
	return myScene;
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

	if (myPipeline)
		delete myPipeline;
	myPipeline = nullptr;
}

bool Dragonite::Runtime::Initialize(HWND& anInstance, EmberGUI* anEditorInterface)
{
	myGUIInterface = nullptr;
	myRuntimeHandler = new PollingStation();
	myPipeline = new GraphicalInterface();
	if (!myPipeline->Init(anInstance, myRuntimeHandler)) return false;

	myRuntimeHandler->AddHandler(myPipeline);
	myInstance = anInstance;
	myEditorInterface = anEditorInterface;




	bool existsEditor = myEditorInterface;

	if (existsEditor)
	{
		myEditorInterface->Init(anInstance, DXInterface::Device.Get(), DXInterface::Context.Get(), DXInterface::SwapChain.Get());
 		//myEditorInterface->AddSpace(new EmberAPI::Viewport("Scene", 1920, 1080, []()
			//{
			//	//Render your scene here
			//}));

	}
	auto projectS = *(nlohmann::json*)Scene::GetProjectSettings();


	if (!projectS.empty())
	{
		myScene = new Scene(*myRuntimeHandler, projectS["mainScene"].get<std::string>());
	}
	else
	{
		myScene = new Scene(*myRuntimeHandler);
	}






	auto IM = myRuntimeHandler->AddHandler(new InputManager());

	if (!IM->Init(this)) return false;



	myScene->Start();
	myPipeline->DrawToBackBuffer(true);



	myRuntimeHandler->AddHandler(this);




	return true;
}

void Dragonite::Runtime::Update(const float aDeltaTime)
{

	if (myEditorInterface)
		myEditorInterface->Update(aDeltaTime);

	myPipeline->Render();

}

DLLEXPORT APIInterface* InitializeRuntime()
{
	using namespace Dragonite;

	Runtime* runtime = new Runtime();
	return runtime;
}
