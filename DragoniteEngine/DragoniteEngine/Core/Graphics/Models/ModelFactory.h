#pragma once

#include "Model.h"
#include "Material.h"
#include <unordered_map>

namespace Dragonite
{



	class GraphicsPipeline;


	class ModelFactory
	{
	public:
		ModelFactory() = default;
		bool Initialize(GraphicsPipeline* aPipeline);

		std::shared_ptr<ModelInstance> GetModel(const PrimitiveType aPrimitiveType, const Material& aMaterial);
	private:
		//VVV Model Definitions
		ModelRef CreateUnitCube();

		//VVV Helper Methods

		HRESULT CreateVSInstance(Device aDevice, const char* aPath, VertexShader& aShader, std::string& someExtraData);
		HRESULT CreatePSInstance(Device aDevice, const char* aPath, PixelShader& aShader);




		GraphicsPipeline* myPipeline;
		std::unordered_map<PrimitiveType, ModelRef> myFactoryData;



	};
}


