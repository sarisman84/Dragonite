#include "ModelRenderer.h"
#include "Core/Graphics/Models/Model.h"
#include "Core/Graphics/Models/ModelFactory.h"

void Dragonite::ModelRenderer::Awake()
{
	myModelFactory = myPollingStation->Get<ModelFactory>();
}

void Dragonite::ModelRenderer::Update(const float aDt)
{
}
