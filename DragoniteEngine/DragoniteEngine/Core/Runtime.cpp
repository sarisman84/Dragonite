#include <iostream>
#include "Runtime.h"
#include "Core/RuntimeAPI/NEW/Scene.h"

#include "ImGui/DragoniteGui.h"
#include "Core/PollingStation.h"
#include "Graphics/GraphicsAPI.h"
#include "Utilities/Input.h"

#include "Editor/EngineDebugger.h"
#include "Editor/SceneEditor.h"
#include "Editor//AssetBrowser.h"

#include "Core/RuntimeAPI/SceneManagement/SceneBuilder.h"
#include "Core/External/nlohmann/json.hpp"

LRESULT Dragonite::Runtime::LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_DESTROY)
	{
		myRuntimeState = false;
		PostQuitMessage(0);
		return 1;
	}

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

bool Dragonite::Runtime::Initialize(HWND& anInstance, const bool anInitializeAsEditorFlag)
{
	myRuntimeHandler = new PollingStation();

	myInstance = anInstance;

	myPipeline = new GraphicalInterface();



	if (!myPipeline->Init(anInstance, myRuntimeHandler)) return false;




	myRuntimeHandler->AddHandler(myPipeline);



	

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

	myEditorFlag = anInitializeAsEditorFlag;
	if (anInitializeAsEditorFlag)
	{
		myGUIInterface = new DragoniteGui();
		myGUIInterface->Init(this, myPipeline);
		myRuntimeHandler->AddHandler(myGUIInterface);
		/*myScene->Awake();*/

		myGUIInterface->CreateEditorWindow(new EngineDebugger());
		myGUIInterface->CreateEditorWindow(new SceneEditor());


	}
	else
	{
		myScene->Start();
		myPipeline->DrawToBackBuffer(true);
	}





	myRuntimeHandler->AddHandler(this);




	return true;
}

void Dragonite::Runtime::Update(const float aDeltaTime)
{
	myUpdateCB(aDeltaTime);
	myLateUpdateCB(aDeltaTime);
	//auto scene = myEditorFlag ? myGUIInterface->GetFocusedScene() : myScene;

	if (myEditorFlag)
	{
		myScene->LateUpdate(aDeltaTime);
	}
	else
	{
		myScene->Update(aDeltaTime);
		myScene->LateUpdate(aDeltaTime);
	}

	//if (scene)
	//{
	//	scene->LateUpdate();
	//}

	//myLateUpdateCB();

	myPipeline->Render();

}

DLLEXPORT APIInterface* InitializeRuntime()
{
	using namespace Dragonite;

	Runtime* runtime = new Runtime();
	return runtime;
}
