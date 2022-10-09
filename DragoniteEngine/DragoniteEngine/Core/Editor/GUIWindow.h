#pragma once
#include "EditorSettings.h"
#include "Core/PollingStation.h"
#include "Core/External/imgui/imgui.h"
#include "Core/External/imgui/ImGuizmo.h"
#include "Core/PollingStation.h"
#include <memory>


namespace Dragonite
{
	class DragoniteGui;
	class GUIWindow
	{
	public:




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
		virtual ~GUIWindow() = default;

		inline void Init(PollingStation* aStation, DragoniteGui* aDragoniteGuiAPI) noexcept {
			myPollingStation = aStation;
			myDragoniteGuiAPI = aDragoniteGuiAPI;
			OnWindowInit();
		}


		void UpdateWindow();

		GUIWindow* CreateEditorWindow(GUIWindow* aWindowType, const bool anIsChildElementFlag = false);

	protected:
		virtual void OnWindowInit() = 0;
		virtual void OnWindowUpdate() = 0;
		virtual void OnEnable() = 0;
		virtual void OnDisable() = 0;

		DragoniteGui* myDragoniteGuiAPI;
		PollingStation* myPollingStation;
		const char* myWindowName;
	private:
		std::vector<std::unique_ptr<GUIWindow>> mySubWindowElements;
		bool myActiveStateFlag;
		bool myIsChildElementFlag;
		unsigned int myGUID;



	};

}


