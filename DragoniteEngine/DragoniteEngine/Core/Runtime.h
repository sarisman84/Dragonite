#pragma once
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#include <typeinfo>

#include <unordered_map>

#include "APIInterface.h"
#include "Utilities/Function.h"
#include "CU/CommonData.h"
#include <chrono>


#define DLLEXPORT _declspec(dllexport)
#define DLLIMPORT _declspec(dllimport)





namespace Dragonite
{

	using Time = std::chrono::time_point<std::chrono::steady_clock>;
	using Clock = std::chrono::high_resolution_clock;

	class GraphicalInterface;
	class DragoniteGui;
	class PollingStation;
	class Scene;


	class Runtime : public APIInterface
	{
	public:
		Runtime();
		~Runtime() override;

		inline PollingStation& GetPollingStation() { return *myRuntimeHandler; }


		inline Function<void(HWND, UINT, WPARAM, LPARAM)>& OnWndProc() { return myWndProcs; }
		inline Function<void(const float)>& OnUpdate() { return myUpdateCB; }
		inline Function<void(GraphicalInterface*)>& OnRender() { return myOnRenderCB; }

		inline HWND& GetClientInstance() { return myInstance; }

		bool Initialize(HWND& anInstance, const bool anInitializeAsEditorFlag = false) override;
		void Update(const float aDeltaTime) override;
		LRESULT LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;


	private:
		static Runtime myRuntime;
		bool myRuntimeState;
		HWND myInstance;
		PollingStation* myRuntimeHandler;
		GraphicalInterface* myPipeline;
		Scene* myScene;


		Function<void(const float)> myUpdateCB;
		Function<void(HWND, UINT, WPARAM, LPARAM)> myWndProcs;
		Function<void(GraphicalInterface*)> myOnRenderCB;


		DragoniteGui* myGUIInterface;




	};



}
DLLEXPORT APIInterface* InitializeRuntime();
