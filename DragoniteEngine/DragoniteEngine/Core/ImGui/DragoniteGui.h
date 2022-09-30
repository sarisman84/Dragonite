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

		void BeginFrame();

		void EndFrame();


		

	private:
		Runtime* myApplicationIns;
		GraphicsPipeline* myGraphicsPipeline;
	};
}


