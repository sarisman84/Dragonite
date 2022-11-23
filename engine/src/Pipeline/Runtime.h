#pragma once
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#include <typeinfo>
#include <functional>
#include <vector>

#include <unordered_map>

#include "EngineAPI/APIInterface.h"
#include "Core/Utilities/Function.h"
#include "Core/CU/CommonData.h"
#include <chrono>


#define DLLEXPORT _declspec(dllexport)
#define DLLIMPORT _declspec(dllimport)


#include "Core/Utilities/Reflection.h"
#include "Pipeline/Utility/Event.h"





namespace Dragonite
{
	class GraphicsEngine;
	class DragoniteGui;
	class PollingStation;
	class Scene;





	class Engine : public EngineAPI
	{
	public:
		Engine();
		~Engine() override;

		inline PollingStation& GetPollingStation() { return *myRuntimeHandler; }


		void RegisterUpdateListener(const std::function<void(const float)>& anEvent);

		void RegisterWinProcListener(const std::function<void(HWND, UINT, WPARAM, LPARAM)>& anEvent);

		inline HWND& GetClientInstance() { return myInstance; }

		bool Initialize(HWND& anInstance, EmberGUI* anEditorInterface = nullptr) override;
		void Update(const float aDeltaTime) override;
		LRESULT LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	private:
		std::vector<std::function<void(const float)>> myUpdateEvents;
		std::vector<std::function<void(HWND, UINT, WPARAM, LPARAM)>> myWinProcEvents;
	private:
		EmberGUI* myEditorInterface;
		GraphicsEngine* myGraphicsEngine;
		bool myRuntimeState;
		HWND myInstance;
		PollingStation* myRuntimeHandler;
		DragoniteGui* myGUIInterface;
	};




}

extern "C"
{
	DLLEXPORT EngineAPI* InitializeRuntime();
};



