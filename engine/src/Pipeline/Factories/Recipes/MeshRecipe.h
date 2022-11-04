#pragma once
#include "Pipeline/Rendering/Content/Mesh.h"
#include "IRecipe.h"

#include <vector>
#include <string>

namespace Dragonite
{
	class GraphicsEngine;

	struct MeshRecipe
	{
		MeshRecipe(GraphicsEngine* anEngine) : myGraphicsEngine(anEngine) {}
		virtual ~MeshRecipe() = default;
		virtual void InitRecipe(std::vector<Mesh>& anOutput, std::string& anOutputName) = 0;
	protected:
		GraphicsEngine* myGraphicsEngine;
	};



	struct CubeRecipe : public MeshRecipe
	{

	};

	struct FBXImportRecipe : public MeshRecipe
	{
		//Import the mesh here
	};

}