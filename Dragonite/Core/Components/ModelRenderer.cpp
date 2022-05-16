#include "ModelRenderer.h"

void ModelRenderer::OnAwake()
{
	myModelInstance = mySystem->Get<ModelFactory>()->CreateInstanceOf(UNITCUBE, "", "Shaders/AnimatedSinWave_PS.cso");
	myGraphicsEngine = mySystem->GetGraphicsEngine();
}

void ModelRenderer::OnUpdate(float /*aDeltaTime*/)
{
	myModelInstance->myTransform = *myTransform;

	myGraphicsEngine->AddRenderInstruction(myModelInstance);
}
