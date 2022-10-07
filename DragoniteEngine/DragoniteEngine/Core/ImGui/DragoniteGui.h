#pragma once
#include <memory>
#include <vector>
#include "Core/Editor/GUIWindow.h"

namespace Dragonite
{
	class Runtime;
	class GraphicsPipeline;

	class DragoniteGui
	{
	public:
		DragoniteGui();
		~DragoniteGui();
		void Init(Runtime* anAppIns, GraphicsPipeline* aGraphicsPipeline);
		void Render();

		void AddWindow(GUIWindow* aNewWindow);
		std::unique_ptr<GUIWindow>& GetWindow(const char* aName);

		inline const bool IsHoveringOverAWindow() const noexcept { return myIsCurrentlyOnImGuiFlag; }

	private:
		void InitializeImgui();

		void BeginDockingSpace();
		void EndDockingSpace();

		bool myIsCurrentlyOnImGuiFlag;


		std::vector<bool> myCachedGuiWindowsStates;
		std::vector<std::unique_ptr<GUIWindow>> myGuiWindows;
		Runtime* myApplicationIns;
		GraphicsPipeline* myGraphicsPipeline;
	};
}


