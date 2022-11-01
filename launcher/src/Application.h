#pragma once
#include <wtypes.h>
#include <chrono>

#include "DLLParser/DLLParser.h"
#include "EngineAPI/APIInterface.h"
#include "EditorAPI/EmberGUI.h"

#ifdef _DEBUG
#define ENGINEDLLPATH L"engine_Debug.dll"
#define EDITORDLLPATH L"editor_Debug.dll"
#elif defined _RELEASE
#define ENGINEDLLPATH L"engine_Release.dll"
#define EDITORDLLPATH L"editor_Release.dll"
#elif defined _RETAIL
#define ENGINEDLLPATH L"engine_Retail.dll"
#define EDITORDLLPATH L"editor_Retail.dll"
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
		DLLParser myEngineParser{ ENGINEDLLPATH };
		DLLParser myEditorParser{ EDITORDLLPATH };

		static APIInterface* InitializeRuntime() {};
		static EmberGUI* InitializeEditor() {};
	public:
		decltype(InitializeRuntime)* getInterface = myEngineParser["?InitializeRuntime@@YAPEAUAPIInterface@@XZ"];
		decltype(InitializeEditor)* getEditorInterface = myEditorParser["ImportEmberGUI"];

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

		inline APIInterface* GetDragoniteAPI() { return myEngineInterface; }

		operator bool() const
		{
			return mySelf != nullptr;
		}

	private:
		bool myRuntimeState;
		APIInterface* myEngineInterface;
		DragoniteAPI myEngineAPI;
		HWND mySelf;
	};



}


