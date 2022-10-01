#include <iostream>
#include "Runtime.h"
#include "Scene.h"

#include "ImGui/DragoniteGui.h"
#include "Core/PollingStation.h"
#include "Graphics/GraphicsAPI.h"
#include "Utilities/Input.h"


Dragonite::Runtime Dragonite::Runtime::myRuntime;



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

bool Dragonite::Runtime::Initialize(HWND& anInstance)
{
	myRuntimeHandler = new PollingStation();

	myInstance = anInstance;

	myPipeline = new GraphicsPipeline();

	

	if (!myPipeline->Initialize(this, anInstance)) return false;

	


	myRuntimeHandler->AddHandler(myPipeline);
	myRuntimeHandler->AddHandler(new RenderInterface(*myPipeline));

	myScene = new Scene();
	myScene->myApplication = this;
	myScene->myPollingStation = myRuntimeHandler;
	
	

	auto IM = myRuntimeHandler->AddHandler(new InputManager());

	if (!IM->Init(this)) return false;
	myScene->myInputManager = IM;

	
	myGUIInterface = new DragoniteGui();
	myGUIInterface->Init(this, myPipeline);

	myScene->Awake();
	return true;
}

void Dragonite::Runtime::Update(const float aDeltaTime)
{
	myUpdateCB(aDeltaTime);
	myScene->Update(aDeltaTime);
	myPipeline->Render();

}

DLLEXPORT APIInterface* InitializeRuntime()
{
	using namespace Dragonite;

	Runtime* runtime = new Runtime();
	return runtime;
}
