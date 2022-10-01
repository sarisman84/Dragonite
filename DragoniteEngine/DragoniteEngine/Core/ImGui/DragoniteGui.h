#pragma once
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
		Runtime* myApplicationIns;
		GraphicsPipeline* myGraphicsPipeline;
	};
}


