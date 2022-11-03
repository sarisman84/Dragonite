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
#include "Pipeline/Utilities/Events.h"






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

		template<typename Event>
		void RegisterWinProcListener(Event&& anEvent);

		template<typename Event>
		void RegisterUpdateListener(Event&& anEvent);

		inline HWND& GetClientInstance() { return myInstance; }

		bool Initialize(HWND& anInstance, EmberGUI* anEditorInterface = nullptr) override;
		void Update(const float aDeltaTime) override;
		LRESULT LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	private:
		std::vector<std::shared_ptr<IUpdateEvent>> myUpdateEvents;
		std::vector<std::shared_ptr<IWinEvent>> myWinProcEvents;
	private:
		EmberGUI* myEditorInterface;
		GraphicsEngine* myGraphicsEngine;
		bool myRuntimeState;
		HWND myInstance;
		PollingStation* myRuntimeHandler;
		DragoniteGui* myGUIInterface;
	};



	template<typename Event>
	inline void Engine::RegisterWinProcListener(Event&& anEvent)
	{
		myWinProcEvents.push_back(std::make_shared<WindowsEvent<Event>>(std::forward<Event>(anEvent)));
	}

	template<typename Event>
	inline void Engine::RegisterUpdateListener(Event&& anEvent)
	{
		myUpdateEvents.push_back(std::make_shared<UpdateEvent<Event>>(std::forward<Event>(anEvent)));
	}

}

extern "C"
{
	DLLEXPORT EngineAPI* InitializeRuntime();
};



