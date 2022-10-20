#pragma once

#include "Model.h"
#include "Material.h"
#include <unordered_map>
#include <string>


namespace Dragonite
{

	class GraphicalInterface;


	class ModelFactory
	{
	public:
		ModelFactory() = default;
		bool Initialize(GraphicalInterface* aPipeline);

		std::shared_ptr<ModelInstance> GetModel(const PrimitiveType aPrimitiveType, const std::string& aMaterial, const std::wstring aTexture = L"resources/textures/CanardPolicierAnglais.dds");
		std::shared_ptr<ModelInstance> GetModel(const std::string aModelName, const std::string& aMaterialName, const std::wstring aTexture = L"resources/textures/CanardPolicierAnglais.dds");
	private:
		//VVV Model Definitions
		ModelRef CreateUnitCube();
		ModelRef CreateScreenMesh();
		ModelRef CreateUnitQuad();


		GraphicalInterface* myPipeline;
		std::unordered_map<std::string, ModelRef> myFactoryModelData;
		std::unordered_map<std::string, Material> myFactoryMaterialData;



	};
}


