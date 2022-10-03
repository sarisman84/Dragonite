#pragma once
#include "EditorSettings.h"
#include "Core/PollingStation.h"
#include "Core/External/imgui/imgui.h"



namespace Dragonite
{


	class GUIWindow
	{
	public:
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

		inline void Init(PollingStation* aStation) noexcept { myPollingStation = aStation; }

	protected:
		PollingStation* myPollingStation;
		const char* myWindowName;
	private:
		bool myActiveStateFlag;


	};
}


