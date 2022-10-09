#pragma once
#include "GUIWindow.h"
namespace Dragonite
{
	class EngineDebugger :public GUIWindow
	{
	public:
		EngineDebugger();
		void OnWindowUpdate() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnWindowInit() override;
	};
}


