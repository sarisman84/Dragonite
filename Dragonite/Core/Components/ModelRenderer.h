#pragma once
#include "Component.h"
#include "Rendering/MeshInfo.h"
#include "Rendering/ModelFactory.h"





class ModelRenderer : public Component
{
public:
	inline static const Material defaultMaterial = Material{ Math::Vector4f(1,1,1,1), "Textures/Grass_c.png", "Textures/Grass_n.png",  "Shaders/ColorShader_VS.cso", "Shaders/ColorShader_PS.cso" };
	void OnAwake() override;
	void OnUpdate(float aDeltaTime) override;
	void LoadModel(const char* aModel, const Material aMaterial = defaultMaterial);
	Math::Vector4f& Color();
private:
	ModelInsPtr myModelInstance;
	Engine::Graphics::GraphicsEngine* myGraphicsEngine;
	Math::Vector4f myPaddingVal;
};

