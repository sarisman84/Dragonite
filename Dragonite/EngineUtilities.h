#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>


namespace Engine
{
	namespace Windows
	{
		struct Resolution
		{
			int width, height;
		};

		struct WindowInfo
		{
			Resolution myResolution;
			HWND myWindowInstance;
			DWORD myWindowStyle;

			float GetAspectRatio()
			{
				float x = static_cast<float>(myResolution.width);
				float y = static_cast<float>(myResolution.height);

				return x / y;
			}
		};


		inline WindowInfo InitializeWindow(const wchar_t* aName, const Resolution& aResolution, HINSTANCE anHInstance, const WNDPROC& aCallback)
		{
			WindowInfo info;

			info.myResolution = aResolution;
			info.myWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;


			AllocConsole();

			FILE* newstdin = nullptr;
			FILE* newstdout = nullptr;
			FILE* newstderr = nullptr;

			freopen_s(&newstdin, "CONIN$", "r", stdin);
			freopen_s(&newstdout, "CONOUT$", "w", stdout);
			freopen_s(&newstderr, "CONOUT$", "w", stderr);

			WNDCLASSEXW wcex = {};
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wcex.lpfnWndProc = aCallback;
			wcex.hInstance = anHInstance;
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
			wcex.lpszClassName = aName;
			RegisterClassExW(&wcex);


			HWND hWnd = CreateWindow(aName, aName, info.myWindowStyle, CW_USEDEFAULT, CW_USEDEFAULT, aResolution.width, aResolution.height, nullptr, nullptr, anHInstance, nullptr
			);

			info.myWindowInstance = hWnd;

			return info;
		}
	}




}
