#include "System.h"
#include "Runtime.h"
#include "Rendering/ModelFactory.h"

#include <chrono>
#include "Utilities/Input.h"

LRESULT CALLBACK WndProc(HWND aHWnd, UINT aMessage, WPARAM aWParam, LPARAM anLParam)
{
	CommonUtilities::Mouse::UpdateEvents(aHWnd, aMessage, aWParam, anLParam);
	CommonUtilities::Keyboard::Update(aMessage, aWParam, anLParam);

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
	myWindowsInfo = InitializeWindow(L"Dragonite", { 1920, 1080 }, anHInstance, WndProc);

	if (!myWindowsInfo.myWindowInstance) return false;
	//myGraphicsEngine = new Graphics::GraphicsEngine();
	if (!myGraphicsEngine->Initialize(myWindowsInfo.myResolution, myWindowsInfo.myWindowInstance, this)) return false;


	ShowWindow(myWindowsInfo.myWindowInstance, nCmdShow);
	UpdateWindow(myWindowsInfo.myWindowInstance);

	return true;
}

void Engine::System::Shutdown()
{
	myRuntimeState = SystemState::Exit;
}

MSG Engine::System::StartRuntime()
{
	AddManager<ModelFactory>()->FetchSystem(this);
	//AddManager<EntityManager>()->FetchSystem(this);


	Runtime runtime = Runtime(this);
	runtime.Awake();
	myRuntimeState = SystemState::Run;
	MSG msg = {};

	

	auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	auto deltaTime = totalTime;

	while (myRuntimeState == SystemState::Run)
	{

		auto curTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		deltaTime = curTime - totalTime;
		totalTime = curTime;
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



		myTimeDelta = deltaTime / 1000.f;
		myTotalTime = totalTime / 1000.f;
		runtime.Update(static_cast<float>(myTimeDelta));
		myGraphicsEngine->DrawElements();
		CommonUtilities::Mouse::EndFrame();
	}

	return msg;
}




