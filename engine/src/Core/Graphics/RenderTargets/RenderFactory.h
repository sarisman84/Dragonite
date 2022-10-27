#pragma once
#include <map>
#include <vector>
#include <memory>
#include "Core/Utilities/EnumMap.h"
namespace Dragonite
{
	class RenderTarget;
	class GraphicalInterface;

	class RenderFactory
	{
	public:
		RenderFactory() = default;

		bool Init(GraphicalInterface* aPipeline);

		void RegisterTarget(RenderTarget& aTarget);

	private:
		GraphicalInterface* myPipeline;
	};

}


