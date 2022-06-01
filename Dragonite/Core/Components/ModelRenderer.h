#pragma once
#include "Component.h"
#include "Rendering/MeshInfo.h"
#include "Rendering/ModelFactory.h"





class ModelRenderer : public Component
{
public:
	inline static const Material defaultMaterial = Material{ Math::Vector4f(1,1,1,1),
	{
			Material::TextureInfo{"Textures/Grass_c.png", Texture::Type::Albedo},
			Material::TextureInfo{"Textures/Grass_n.png", Texture::Type::Normal}
	},
		"Shaders/ColorShader_VS.cso",
		"Shaders/ColorShader_PS.cso"
	};


	inline static const Material terrainMaterial = Material{ Math::Vector4f(1,1,1,1),
	{
			Material::TextureInfo{"Textures/Grass_c.png", Texture::Type::Albedo},
			Material::TextureInfo{"Textures/Rock_c.png", Texture::Type::Albedo},
			Material::TextureInfo{"Textures/Snow_c.png", Texture::Type::Albedo},

			Material::TextureInfo{"Textures/Grass_n.png", Texture::Type::Normal},
			Material::TextureInfo{"Textures/Rock_n.png", Texture::Type::Normal},
			Material::TextureInfo{"Textures/Snow_n.png", Texture::Type::Normal}
	},
	  "Shaders/ColorShader_VS.cso",
	  "Shaders/TerrainShader_PS.cso"
	};

	void OnAwake() override;
	void OnUpdate(float aDeltaTime) override;
	void LoadModel(const char* aModel, const Material aMaterial = defaultMaterial);
	Math::Vector4f& Color();
private:
	ModelInsPtr myModelInstance;
	Engine::Graphics::GraphicsEngine* myGraphicsEngine;
	Math::Vector4f myPaddingVal;
};

