#pragma once

#include "Model.h"
#include "Material.h"
#include <unordered_map>

#define DDLVISIBLE __declspec(dllexport)

namespace Dragonite
{



	class GraphicalInterface;


	class ModelFactory
	{
	public:
		ModelFactory() = default;
		bool Initialize(GraphicalInterface* aPipeline);

		DDLVISIBLE std::shared_ptr<ModelInstance> GetModel(const PrimitiveType aPrimitiveType, const Material& aMaterial);
	private:
		//VVV Model Definitions
		ModelRef CreateUnitCube();
		ModelRef CreateScreenMesh();



		GraphicalInterface* myPipeline;
		std::unordered_map<PrimitiveType, ModelRef> myFactoryData;



	};
}


