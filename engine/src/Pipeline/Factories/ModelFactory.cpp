#include "ModelFactory.h"

void Dragonite::ModelFactory::Init()
{
}

void Dragonite::ModelFactory::AddRecipe(MeshRecipe* const aRecipe)
{
	if (!aRecipe) return;


	std::vector<Mesh> meshes;
	std::string name;

	aRecipe, aRecipe->InitRecipe(meshes, name);

	myModels[name] = Model(name.c_str(), meshes);
}

Dragonite::Model* Dragonite::ModelFactory::FetchModel(const char* aModelName)
{
	return nullptr;
}

Dragonite::ModelFactory::ModelFactory(GraphicsEngine* anEngine)
{
	myGraphicsEngine = anEngine;
}