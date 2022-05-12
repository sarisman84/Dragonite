#include "System.h"
#include "Runtime.h"
#include "Rendering/ModelFactory.h"
#include "ComponentManager.h"

LRESULT CALLBACK WndProc(HWND aHWnd, UINT aMessage, WPARAM aWParam, LPARAM anLParam)
{
	switch (aMessage)
	{
	case WM_CLOSE:
		DestroyWindow(aHWnd);
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(aHWnd, aMessage, aWParam, anLParam);
	}
	return 0;
}

//Engine::System::System() = default;

//Engine::System::~System() = default;

Engine::System::~System()
{
	for (auto& pair : myManagers)
	{
		if (pair.second)
			delete pair.second;
	}

	myManagers.clear();
}

bool Engine::System::Initialize(HINSTANCE anHInstance, int nCmdShow)
{
	myGraphicsEngine = AddManager<Graphics::GraphicsEngine>();
	myWindowsInfo = InitializeWindow(L"Dragonite", { 1980, 1080 }, anHInstance, WndProc);

	if (!myWindowsInfo.myWindowInstance) return false;
	//myGraphicsEngine = new Graphics::GraphicsEngine();
	if (!myGraphicsEngine->Initialize(myWindowsInfo.myResolution, myWindowsInfo.myWindowInstance, this)) return false;


	ShowWindow(myWindowsInfo.myWindowInstance, nCmdShow);
	UpdateWindow(myWindowsInfo.myWindowInstance);

	return true;
}

void Engine::System::Shutdown()
{
}

MSG Engine::System::StartRuntime()
{
	AddManager<ModelFactory>();
	AddManager<EntityManager>()->FetchSystem(this);


	Runtime runtime = Runtime(this);
	runtime.Awake();
	myRuntimeState = SystemState::Run;
	MSG msg = {};

	while (myRuntimeState == SystemState::Run)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				myRuntimeState = SystemState::Exit;
				break;
			}
		}

		runtime.Update();
		myGraphicsEngine->DrawElements();

	}

	return msg;
}




