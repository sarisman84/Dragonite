#pragma once
#include "Component.h"
#include "Rendering/MeshInfo.h"
#include "Rendering/ModelFactory.h"
class ModelRenderer : public Component
{
public:
	void OnAwake() override;
	void OnUpdate(float aDeltaTime) override;
	void LoadModel(const char* aModel, const char* aVertexShader = "", const char* aPixelShader = "");
private:
	ModelInstance* myModelInstance;
	Engine::Graphics::GraphicsEngine* myGraphicsEngine;
};

