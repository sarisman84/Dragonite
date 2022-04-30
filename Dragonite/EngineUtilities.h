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
		};


		inline WindowInfo InitializeWindow(const wchar_t* aName, const Resolution& aResolution, HINSTANCE anHInstance, const WNDPROC& aCallback)
		{
			WindowInfo info;

			info.myResolution = aResolution;
			info.myWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

			WNDCLASSEXW wcex = {};
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wcex.lpfnWndProc = aCallback;
			wcex.hInstance = anHInstance;
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
			wcex.lpszClassName = aName;
			RegisterClassExW(&wcex);


			HWND hWnd = CreateWindow(aName, aName,	info.myWindowStyle, CW_USEDEFAULT, CW_USEDEFAULT, aResolution.width, aResolution.height, nullptr, nullptr, anHInstance, nullptr
			);

			info.myWindowInstance = hWnd;

			return info;
		}
	}




}
