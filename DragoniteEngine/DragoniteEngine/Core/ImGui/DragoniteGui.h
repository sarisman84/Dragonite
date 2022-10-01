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




		

	private:
		std::vector<std::shared_ptr<GUIWindow>> myGuiWindows;

		Runtime* myApplicationIns;
		GraphicsPipeline* myGraphicsPipeline;
	};
}


