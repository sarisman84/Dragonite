#pragma once
#include "BaseFactory.h"
#include "Pipeline/Rendering/Content/Mesh.h"
#include "Recipes/MeshRecipe.h"

#include <string>
#include <unordered_map>

namespace Dragonite
{
	

	class GraphicsEngine;
	class ModelFactory : public BaseFactory
	{
	public:
		ModelFactory(GraphicsEngine* anEngine);

		void Init() override;

		void AddRecipe(MeshRecipe* const aRecipe);


		Model* FetchModel(const char* aModelName);

	private:
		std::unordered_map<std::string, Model> myModels;
		GraphicsEngine* myGraphicsEngine;
	};
}