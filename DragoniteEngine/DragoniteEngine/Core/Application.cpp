#include "Application.h"

#include <iostream>
#include "Graphics/GraphicsAPI.h"

const bool Dragonite::Application::CreateInstance(const ApplicationDesc& anApplicationDesc, Application** anOutput)
{
	*anOutput = new Application();



	WNDCLASSEXW wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = anApplicationDesc.myHInstance;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszClassName = anApplicationDesc.myWinClassName;

	RegisterClassExW(&wcex);

	HWND hWnd = CreateWindow(anApplicationDesc.myWinClassName, anApplicationDesc.myApplicationName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, anApplicationDesc.myInitialResolution.myWidth, anApplicationDesc.myInitialResolution.myHeight, nullptr, nullptr, anApplicationDesc.myHInstance, nullptr);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Error"),
			NULL);

		return false;
	}



	ShowWindow(hWnd, anApplicationDesc.myNCmdShow);
	UpdateWindow(hWnd);

	(*anOutput)->myInstance = hWnd;


	SetWindowLongPtr((*anOutput)->myInstance, GWLP_USERDATA, (LONG_PTR)(*anOutput));

	(*anOutput)->myRuntimeState = true;

	(*anOutput)->myPipeline = new GraphicsAPI::GraphicsPipeline();

	if (!(*anOutput)->myPipeline->Initialize(hWnd))
		return false;
	(*anOutput)->myRuntimeHandler.AddHandler((*anOutput)->myPipeline);

	return true;
}

Dragonite::Application::Application() = default;

Dragonite::Application::~Application() = default;

int Dragonite::Application::ExecuteRuntime()
{
	MSG msg = { 0 };
	while (myRuntimeState)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				myRuntimeState = false;
			}

		}
		myPipeline->Render();

	}




	return (int)msg.wParam;
}

LRESULT Dragonite::Application::LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

LRESULT Dragonite::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Application* application = (Application*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (!application) return E_FAIL;

	LRESULT r = application->LocalWndProc(hWnd, message, wParam, lParam);
	if (r)
		return DefWindowProc(hWnd, message, wParam, lParam);

	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}break;

	default:
		break;
	}


	return DefWindowProc(hWnd, message, wParam, lParam);
}

Dragonite::RuntimeHandler::~RuntimeHandler()
{
	for (auto& el : myUniqueDataMembers)
	{
		if (el.second)
			delete el.second;
	}
	myUniqueDataMembers.clear();
}
