#pragma once
#include <wtypes.h>
namespace Dragonite
{
	struct Resolution
	{
		int myWidth, myHeight;
	};

	struct ApplicationDesc
	{
		Resolution myResolution;
		const wchar_t* myWindowsName;
		const wchar_t* myInternalName;
		_In_ HINSTANCE hInstance;
		_In_ int nCmdShow;
	};

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	class Application
	{
	public:
		Application(const ApplicationDesc& aDesc);
		int ExecuteRuntime();

		operator bool() const
		{
			return mySelf != nullptr;
		}

	private:
		HWND mySelf;
		HINSTANCE myEngineDLLIns;
	};
}


