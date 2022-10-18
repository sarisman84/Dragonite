#include <iostream>
#include "Runtime.h"
#include "Core/RuntimeAPI/Scene.h"

#include "ImGui/DragoniteGui.h"
#include "Core/PollingStation.h"
#include "Graphics/GraphicsAPI.h"
#include "Utilities/Input.h"

#include "Editor/EngineDebugger.h"
#include "Editor/SceneEditor.h"
#include "Editor//AssetBrowser.h"

#include "Core/RuntimeAPI/SceneManagement/SceneBuilder.h"

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

Dragonite::Runtime::Runtime() = default;

Dragonite::Runtime::~Runtime()
{
	if (myRuntimeHandler)
		delete myRuntimeHandler;
	myRuntimeHandler = nullptr;

	if (myScene)
		delete myScene;
	myScene = nullptr;

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




	myScene = new Scene();
	auto projectS = SceneBuilder::GetProjectSettings();
	if (!Scene::New(*myScene, this, myPipeline, myRuntimeHandler))
	{
		return false;
	}

	myRuntimeHandler->AddHandler(myScene);

	if (!projectS.empty())
	{
		SceneBuilder::LoadScene(projectS[mainScene].get<std::string>().c_str(), *myScene);
	}






	auto IM = myRuntimeHandler->AddHandler(new InputManager());

	if (!IM->Init(this)) return false;
	myScene->myInputManager = IM;

	myEditorFlag = anInitializeAsEditorFlag;
	if (anInitializeAsEditorFlag)
	{
		myGUIInterface = new DragoniteGui();
		myGUIInterface->Init(this, myPipeline);
		myGUIInterface->FocusScene(myScene);
		myRuntimeHandler->AddHandler(myGUIInterface);
		/*myScene->Awake();*/

		myGUIInterface->CreateEditorWindow(new EngineDebugger());
		myGUIInterface->CreateEditorWindow(new SceneEditor());
	}
	else
	{
		myScene->OnSceneInit();
		myUpdateCB += [this](const float aDt)
		{
			myScene->Update(aDt);
			//myScene->LateUpdate();
		};
		myPipeline->DrawToBackBuffer(true);
	}





	myRuntimeHandler->AddHandler(this);




	return true;
}

void Dragonite::Runtime::Update(const float aDeltaTime)
{
	myUpdateCB(aDeltaTime);

	auto scene = myEditorFlag ? myGUIInterface->GetFocusedScene() : myScene;

	if (scene)
	{
		scene->LateUpdate();
	}

	myLateUpdateCB();

	myPipeline->Render();

}

DLLEXPORT APIInterface* InitializeRuntime()
{
	using namespace Dragonite;

	Runtime* runtime = new Runtime();
	return runtime;
}
