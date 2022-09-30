#pragma once
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#include <typeinfo>

#include <unordered_map>

#include "Utilities/Function.h"
#include "CU/CommonData.h"
#include <chrono>




void UpdateEngine();

namespace Dragonite
{

	using Time = std::chrono::time_point<std::chrono::steady_clock>;
	using Clock = std::chrono::high_resolution_clock;

	class GraphicsPipeline;
	class DragoniteGui;
	class PollingStation;


	class Runtime
	{
	public:
		int ExecuteRuntime(HWND* aWindowsIns);
		//inline DDLVISIBLE PollingStation& GetPollingStation() { return myRuntimeHandler; }
		LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		inline Function<void(HWND, UINT, WPARAM, LPARAM)>& OnWndProc() { return myWndProcs; }
		inline Function<void(const float)>& OnUpdate() { return myUpdateCB; }
		inline Function<void()>& OnPreRender() { return myOnPreRenderCB; }
		inline Function<void()>& OnRender() { return myOnRenderCB; }

		inline HWND* GetClientInstance() { return myInstance; }

	private:
		bool myRuntimeState;
		HWND* myInstance;
		PollingStation* myRuntimeHandler;
		GraphicsPipeline* myPipeline;

		Function<void(const float)> myUpdateCB;
		Function<void(HWND, UINT, WPARAM, LPARAM)> myWndProcs;
		Function<void()> myOnPreRenderCB, myOnRenderCB;


		DragoniteGui* myGUIInterface;

	};



}

