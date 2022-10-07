#pragma once
#include "EditorSettings.h"
#include "Core/PollingStation.h"
#include "Core/External/imgui/imgui.h"
#include "Core/External/imgui/ImGuizmo.h"


namespace Dragonite
{

	class DragoniteGui;
	class GUIWindow
	{
	public:
		virtual void OnWindowInit() = 0;
		virtual void OnWindowRender() = 0;
		virtual void OnEnable() = 0;
		virtual void OnDisable() = 0;



		inline void SetActive(const bool aNewState)
		{
			myActiveStateFlag = aNewState;
			UpdateWindowState();
		}
		inline const bool IsActive() const noexcept { return myActiveStateFlag; }
		inline bool& IsActive() noexcept { return myActiveStateFlag; }
		inline const char* Name() { return myWindowName; }

		void UpdateWindowState();
		GUIWindow(const char* aWindowName);

		inline void Init(PollingStation* aStation, DragoniteGui* aDragoniteGuiAPI) noexcept {
			myPollingStation = aStation;
			myDragoniteGuiAPI = aDragoniteGuiAPI;
			OnWindowInit();
		}

		virtual const bool IsBeingInteracted() = 0;


	protected:
		DragoniteGui* myDragoniteGuiAPI;
		PollingStation* myPollingStation;
		const char* myWindowName;
	private:
		bool myActiveStateFlag;
		


	};
}


