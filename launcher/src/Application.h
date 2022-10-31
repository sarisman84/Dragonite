#pragma once
#include <wtypes.h>
#include <chrono>

#include "DLLParser.h"
#include "APIInterface.h"

#ifdef _DEBUG
#define DLLPATH L"engine_Debug.dll"
#elif defined _RELEASE
#define DLLPATH L"engine_Release.dll"
#elif defined _RETAIL
#define DLLPATH L"engine_Retail.dll"
#endif


namespace Dragonite
{

	using Time = std::chrono::time_point<std::chrono::steady_clock>;
	using Clock = std::chrono::high_resolution_clock;
	using DeltaTime = std::chrono::duration<float, std::ratio<1>>;

	struct Resolution
	{
		int myWidth, myHeight;
	};

	//API implementation is based from this: https://blog.benoitblanchon.fr/getprocaddress-like-a-boss/
	//Reads an DLL and parses it to the APIInterface.
	class DragoniteAPI
	{
	private:
		DLLParser myParser{ L"DragoniteEngine.dll" };
		DLLParser myEditorParser{ DLLPATH };

		static APIInterface* InitializeRuntime() {};
	public:
		decltype(InitializeRuntime)* getInterface = myParser["?InitializeRuntime@@YAPEAUAPIInterface@@XZ"];

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
		~Application();
		int ExecuteRuntime();
		inline void EndRuntime() { myRuntimeState = false; }

		inline APIInterface* GetDragoniteAPI() { return myInterface; }

		operator bool() const
		{
			return mySelf != nullptr;
		}

	private:
		bool myRuntimeState;
		APIInterface* myInterface;
		DragoniteAPI myDragoniteAPI;
		HWND mySelf;
	};



}


