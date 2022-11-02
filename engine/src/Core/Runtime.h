#pragma once
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#include <typeinfo>
#include <functional>
#include <vector>

#include <unordered_map>

#include "EngineAPI/APIInterface.h"
#include "Utilities/Function.h"
#include "CU/CommonData.h"
#include <chrono>


#define DLLEXPORT _declspec(dllexport)
#define DLLIMPORT _declspec(dllimport)


#include "Utilities/Reflection.h"






namespace Dragonite
{
	class GraphicsEngine;
	class DragoniteGui;
	class PollingStation;
	class Scene;


	class Runtime : public APIInterface
	{
	public:
		Runtime();
		~Runtime() override;

		inline PollingStation& GetPollingStation() { return *myRuntimeHandler; }

		inline HWND& GetClientInstance() { return myInstance; }

		bool Initialize(HWND& anInstance, EmberGUI* anEditorInterface = nullptr) override;
		void Update(const float aDeltaTime) override;
		LRESULT LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	private:
		EmberGUI* myEditorInterface;
		GraphicsEngine* myGraphicsEngine;
		bool myRuntimeState;
		HWND myInstance;
		PollingStation* myRuntimeHandler;
		DragoniteGui* myGUIInterface;
	};



}
DLLEXPORT APIInterface* InitializeRuntime();


