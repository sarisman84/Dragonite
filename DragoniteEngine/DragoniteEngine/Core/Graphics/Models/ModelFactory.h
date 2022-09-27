#pragma once

#include "Model.h"
#include <unordered_map>


namespace Dragonite
{
	namespace GraphicsAPI
	{
		class GraphicsPipeline;
	}

	class ModelFactory
	{
	public:
		ModelFactory() = default;
		bool Initialize(GraphicsAPI::GraphicsPipeline* aPipeline);

		std::shared_ptr<ModelInstance> GetModel(const PrimitiveType aPrimitiveType);
	private:
		ModelPtr CreateUnitCube();

		GraphicsAPI::GraphicsPipeline* myPipeline;
		std::unordered_map<PrimitiveType, ModelPtr> myFactoryData;
	};
}


