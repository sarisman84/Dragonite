#pragma once
#include "Utilities/Math/Matrix4x4.h"
#include "Utilities/Math/Vector.h"
#include "ComponentManager.h"
#include "CommonComponents.h"
#include "System.h"
#include "Graphics/GraphicsEngine.h"
#include "Graphics/Rendering/ModelFactory.h"
#include <d3d11.h>

using namespace Engine::Graphics;

struct ModelRenderer
{
	ModelInstance& myModelInstance;
};










void RenderModel(ModelInstance& anInstance, Engine::Graphics::GraphicsEngine* anEngine)
{
	float pi = 3.141592653589793238f;
	Transform& transform = anInstance.myTransform;

	Math::Matrix4x4f posMatrix = Math::Matrix4x4f::CreateTransformMatrix(transform.myPosition);

	Math::Matrix4x4f rotMatrix;
	rotMatrix = Math::Matrix4x4f::CreateRotationAroundX((transform.myRotation.x * pi) / 180.f);
	rotMatrix = Math::Matrix4x4f::CreateRotationAroundY((transform.myRotation.y * pi) / 180.f) * rotMatrix;
	rotMatrix = Math::Matrix4x4f::CreateRotationAroundZ((transform.myRotation.z * pi) / 180.f) * rotMatrix;

	Math::Matrix4x4f size = Math::Matrix4x4f::CreateSizeMatrix(transform.mySize);


	auto objectMatrix = size * rotMatrix * posMatrix;


	auto context = anEngine->GetDeviceContext();
	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(anEngine->ObjectBuffer().Get(), 1, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	ObjectBufferData data;
	data.myObjectMatrix = objectMatrix;
	memcpy(resource.pData, &data, sizeof(ObjectBufferData));
	context->Unmap(anEngine->ObjectBuffer().Get(), 1);
}

void UpdateModel(EntityID anID, ModelRenderer& anInstance, EntityManager* aManager)
{
	Transform transform = aManager->GetComponent<Transform>(anID);
	anInstance.myModelInstance.myTransform = transform;
	aManager->GetSystem()->GetGraphicsEngine()->AddRenderInstruction(RenderModel, anInstance.myModelInstance);
}

