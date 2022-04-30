#include "System.h"

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

bool Engine::System::Initialize(HINSTANCE anHInstance, int nCmdShow)
{
	myWindowsInfo = InitializeWindow(L"Dragonite", { 1980, 1080 }, anHInstance, WndProc);

	if (!myWindowsInfo.myWindowInstance) return false;
	myGraphicsEngine = Graphics::GraphicsEngine();
	if (!myGraphicsEngine.Initialize(myWindowsInfo.myResolution, myWindowsInfo.myWindowInstance)) return false;


	ShowWindow(myWindowsInfo.myWindowInstance, nCmdShow);
	UpdateWindow(myWindowsInfo.myWindowInstance);

	return true;
}

void Engine::System::Shutdown()
{
}

MSG Engine::System::StartRuntime()
{
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
		EngineRuntime();
	}

	return msg;
}



void Engine::System::EngineRuntime()
{
	myGraphicsEngine.Render();
}
