#include "ModelRenderer.h"

void ModelRenderer::OnAwake()
{
	
	myGraphicsEngine = mySystem->GetGraphicsEngine();
}

void ModelRenderer::OnUpdate(float /*aDeltaTime*/)
{
	if (!myModelInstance) return;
	myModelInstance->myTransform = *myTransform;

	myGraphicsEngine->AddRenderInstruction(myModelInstance);
}

void ModelRenderer::LoadModel(const char* aModel, const char* aVertexShader, const char* aPixelShader)
{
	myModelInstance = mySystem->Get<ModelFactory>()->CreateInstanceOf(aModel, aVertexShader, aPixelShader);
}
