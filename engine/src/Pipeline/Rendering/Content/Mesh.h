#pragma once
#include "MeshData.h"
#include <vector>
namespace Dragonite
{
	struct Mesh
	{
		std::vector<Vertex> myVertecies;
		std::vector<uint32_t> myIndicies;
	};


	struct Model
	{
		Model(const char* aName, const std::vector<Mesh>& someMeshes) : myMeshes(someMeshes), myName(aName) {}

		const char* myName;
		std::vector<Mesh> myMeshes;
	};
}