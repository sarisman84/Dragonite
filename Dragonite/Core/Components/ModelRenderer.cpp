#include "ModelRenderer.h"

void ModelRenderer::OnAwake()
{
	myModelInstance = mySystem->Get<ModelFactory>()->CreateInstanceOf(UNITCUBE);
	myGraphicsEngine = mySystem->GetGraphicsEngine();
}

void ModelRenderer::OnUpdate(float /*aDeltaTime*/)
{
	myModelInstance->myTransform = *myTransform;

	myGraphicsEngine->AddRenderInstruction(myModelInstance);
}
