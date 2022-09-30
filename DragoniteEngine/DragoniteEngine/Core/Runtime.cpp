#include <iostream>
#include "Runtime.h"
#include "Scene.h"

#include "ImGui/DragoniteGui.h"
#include "Core/PollingStation.h"
#include "Graphics/GraphicsAPI.h"
#include "Utilities/Input.h"


//const bool Dragonite::Runtime::CreateInstance(const ApplicationDesc& anApplicationDesc, Runtime** anOutput)
//{
//	*anOutput = new Runtime();
//
//
//
//	WNDCLASSEXW wcex = {};
//	wcex.cbSize = sizeof(WNDCLASSEX);
//	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
//	wcex.lpfnWndProc = WndProc;
//	wcex.hInstance = anApplicationDesc.myHInstance;
//	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
//	wcex.lpszClassName = anApplicationDesc.myWinClassName;
//
//	RegisterClassExW(&wcex);
//
//	HWND hWnd = CreateWindow(anApplicationDesc.myWinClassName, anApplicationDesc.myApplicationName,
//		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
//		CW_USEDEFAULT, CW_USEDEFAULT, anApplicationDesc.myInitialResolution.myWidth, anApplicationDesc.myInitialResolution.myHeight, nullptr, nullptr, anApplicationDesc.myHInstance, nullptr);
//
//	if (!hWnd)
//	{
//		MessageBox(NULL,
//			_T("Call to CreateWindow failed!"),
//			_T("Error"),
//			NULL);
//
//		return false;
//	}
//
//
//
//	ShowWindow(hWnd, anApplicationDesc.myNCmdShow);
//	UpdateWindow(hWnd);
//
//	(*anOutput)->myInstance = hWnd;
//
//
//	SetWindowLongPtr((*anOutput)->myInstance, GWLP_USERDATA, (LONG_PTR)(*anOutput));
//
//	(*anOutput)->myRuntimeState = true;
//
//	(*anOutput)->myPipeline = new GraphicsPipeline();
//
//	if (!(*anOutput)->myPipeline->Initialize(*anOutput, hWnd))
//		return false;
//
//	return true;
//}

Dragonite::Runtime::Runtime() = default;

Dragonite::Runtime::~Runtime()
{
	if (myGUIInterface)
		delete myGUIInterface;
	myGUIInterface = nullptr;
};

int Dragonite::Runtime::ExecuteRuntime(HWND* aWindowsIns)
{
	myInstance = aWindowsIns;
	using std::chrono::high_resolution_clock;


	RenderInterface myInterface(*myPipeline);

	myRuntimeHandler->AddHandler(&myInterface);

	InputManager input;
	input.Init(this);
	myRuntimeHandler->AddHandler(&input);


	myGUIInterface = new DragoniteGui();
	myGUIInterface->Init(this, myPipeline);


	Scene scene;
	scene.myApplication = this;
	scene.myPollingStation = myRuntimeHandler;
	scene.Awake();
	MSG msg = { 0 };


	float deltaTime = 0;
	while (myRuntimeState)
	{
		Time start = Clock::now();
		auto t_start = high_resolution_clock::now();

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				myRuntimeState = false;
			}

		}
		myUpdateCB(deltaTime);
		scene.Update(deltaTime);
		myPipeline->Render();


		Time end = Clock::now();



		deltaTime = std::chrono::duration<float, std::ratio<1>>(end - start).count();
	}




	return (int)msg.wParam;
}

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



