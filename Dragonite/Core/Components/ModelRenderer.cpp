#include "ModelRenderer.h"
#include <iostream>
void ModelRenderer::OnAwake()
{

	myGraphicsEngine = mySystem->GetGraphicsEngine();
}

void ModelRenderer::OnUpdate(float /*aDeltaTime*/)
{
	if (!myModelInstance) return;
	myModelInstance->myTransform = *myTransform;

	myGraphicsEngine->AddRenderInstruction(myModelInstance.get());
}

void ModelRenderer::LoadModel(const char* aModel, const Material aMaterial)
{
	std::cout << "[Log]<ModelRenderer>: Attempting to create an instance of " << aModel << "!" << std::endl;
	myModelInstance = mySystem->Get<ModelFactory>()->CreateInstanceOf(aModel, aMaterial);
}

Math::Vector4f& ModelRenderer::Color()
{
	if (myModelInstance)
		return myModelInstance->myMaterial.myColor;
	return myPaddingVal;
}
