#pragma once
#include "EditorSettings.h"
namespace Dragonite
{
	class GUIWindow
	{
	public:
		GUIWindow(const char* aWindowName);

		virtual void OnWindowRender() = 0;
		virtual void OnEnable() = 0;
		virtual void OnDisable() = 0;

		inline void SetActive(const bool aNewState)
		{
			myActiveStateFlag = aNewState;
			if (myActiveStateFlag)
				OnEnable();
			else
				OnDisable();
		}
		inline const bool IsActive() const noexcept { return myActiveStateFlag; }

	protected:
		const char* myWindowName;
	private:
		bool myActiveStateFlag;

	};
}


