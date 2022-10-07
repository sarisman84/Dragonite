#pragma once
#include "GUIWindow.h"
namespace Dragonite
{
	class EngineDebugger :public GUIWindow
	{
	public:
		EngineDebugger();
		void OnWindowRender() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnWindowInit() override;
		const bool IsBeingInteracted() override;
	};
}


