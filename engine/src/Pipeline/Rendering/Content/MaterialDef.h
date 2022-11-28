#pragma once
#include "Core/CU/Math/Vector2.h"

#include <memory>


struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;

struct ID3D11ShaderResourceView;
struct ID3D11DeviceContext;

namespace Dragonite
{
	class Texture
	{
		friend class MaterialFactory;
		friend class GraphicsEngine;
	public:
		Texture();
		~Texture();
	private:
		Texture(ID3D11ShaderResourceView* aResourceView);
		void Bind(ID3D11DeviceContext* aContext);
	private:
		const char* myName;
		Vector2f myTextureSize;
		uint32_t mySlot;
		ID3D11ShaderResourceView* myResourceView;

	};


	struct Material
	{
		ID3D11InputLayout* myInputLayout;
		ID3D11VertexShader* myVertexShader;
		ID3D11PixelShader* myPixelShader;

		std::shared_ptr<Texture> myAlbedoTexture;
		std::shared_ptr<Texture> myNormalTexture;
		std::shared_ptr<Texture> myMaterialTexture;
	};




}