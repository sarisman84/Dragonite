#pragma once
#include <map>
#include <vector>
#include <memory>
#include "Core/Utilities/EnumMap.h"
namespace Dragonite
{
	class RenderTarget;
	class GraphicsPipeline;

	class RenderFactory
	{
	public:
		RenderFactory() = default;

		bool Init(GraphicsPipeline* aPipeline);

		void RegisterTarget(RenderTarget& aTarget);

	private:
		GraphicsPipeline* myPipeline;
	};

}


