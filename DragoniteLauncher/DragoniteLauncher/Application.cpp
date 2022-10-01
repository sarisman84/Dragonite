#include "Application.h"
#include "DLLParser.h"

Dragonite::Application::Application(const ApplicationDesc& aDesc)
{



	WNDCLASSEXW wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = aDesc.hInstance;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszClassName = aDesc.myInternalName;

	RegisterClassExW(&wcex);

	HWND hWnd = CreateWindow(aDesc.myInternalName, aDesc.myWindowsName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, aDesc.myResolution.myWidth, aDesc.myResolution.myHeight, nullptr, nullptr, aDesc.hInstance, nullptr);

	if (!hWnd)return;

	ShowWindow(hWnd, aDesc.nCmdShow);
	UpdateWindow(hWnd);

	mySelf = hWnd;


	SetWindowLongPtr(mySelf, GWLP_USERDATA, (LONG_PTR)(this));


	//GetProcAddress(myEngineDLLIns,)

	myInterface = myDragoniteAPI.getInterface();

	if (!myInterface) return;

	myInterface->Initialize(mySelf);
	myRuntimeState = true;
}

Dragonite::Application::~Application()
{

}

int Dragonite::Application::ExecuteRuntime()
{
	MSG msg = {};
	float deltaTime = 0;
	while (myRuntimeState)
	{
		Time start = Clock::now();
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				myRuntimeState = false;
				goto exit;
			}
		}
		myInterface->Update(deltaTime);

		Time end = Clock::now();
		deltaTime = DeltaTime(end - start).count();
	}
exit:
	return (int)msg.wParam;
}




LRESULT Dragonite::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	Application* app = (Application*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (app)
	{
		app->GetDragoniteAPI()->LocalWndProc(hWnd, message, wParam, lParam);
	}

	switch (message)
	{
	case WM_DESTROY:
		app->EndRuntime();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
