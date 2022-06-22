#pragma once
#include "Component.h"
#include "Rendering/MeshInfo.h"
#include "Rendering/ModelFactory.h"
#include "Rendering/Framework/Texture.h"




class ModelRenderer : public Component
{
public:
	inline static const Material defaultMaterial = Material{ Math::Vector4f(1,1,1,1),
	{
			Material::TextureInfo{"Textures/default.png", (int)Texture::Type::Albedo},
			Material::TextureInfo{"Textures/Grass_n.png", (int)Texture::Type::Normal}
	},
		"Shaders/ColorShader_VS.cso",
		"Shaders/ColorShader_PS.cso"
	};


	inline static const Material terrainMaterial = Material{ Math::Vector4f(1,1,1,1),
	{
			Material::TextureInfo{"Textures/Grass_c.png", (int)Texture::Type::Albedo},
			Material::TextureInfo{"Textures/Rock_c.png", (int)Texture::Type::Albedo},
			Material::TextureInfo{"Textures/Snow_c.png", (int)Texture::Type::Albedo},

			Material::TextureInfo{"Textures/Grass_n.png", (int)Texture::Type::Normal},
			Material::TextureInfo{"Textures/Rock_n.png", (int)Texture::Type::Normal},
			Material::TextureInfo{"Textures/Snow_n.png", (int)Texture::Type::Normal},

			Material::TextureInfo{"Textures/Grass_m.png", (int)Texture::Type::Metallic},
			Material::TextureInfo{"Textures/Rock_m.png", (int)Texture::Type::Metallic},
			Material::TextureInfo{"Textures/Snow_m.png", (int)Texture::Type::Metallic}
	},
	  "Shaders/TerrainShader_VS.cso",
	  "Shaders/TerrainShader_PS.cso"
	};

	void OnAwake() override;
	void OnUpdate(float aDeltaTime) override;
	void LoadModel(const char* aModel, const Material aMaterial = defaultMaterial);
	Math::Vector4f& Color();
private:
	ModelInsPtr myModelInstance;
	Dragonite::GraphicsEngine* myGraphicsEngine;
	Math::Vector4f myPaddingVal;
};

