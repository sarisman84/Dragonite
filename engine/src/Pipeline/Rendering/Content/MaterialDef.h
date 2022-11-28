#pragma once
#include "Core/CU/Math/Vector2.h"

#include <memory>




struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;

struct ID3D11ShaderResourceView;
struct ID3D11DeviceContext;
struct ID3D11Device;

struct D3D11_TEXTURE2D_DESC;

namespace Dragonite
{
	namespace _internal
	{
		struct PixelData
		{
			unsigned char* myBuffer;
			Vector2i myResolution;
		};

		PixelData LoadTextureBuffer(const char* aPath);
	}



	class Texture
	{
		friend class MaterialFactory;
		friend class GraphicsEngine;
	public:
		Texture();
		~Texture();
	private:
		void Init(ID3D11Device* aDevice, void* aTextureBuffer);
		void Bind(ID3D11DeviceContext* aContext);
	private:
		const char* myName;
		Vector2i myTextureSize;
		uint32_t mySlot;
		ID3D11ShaderResourceView* myResourceView;
	private:
		bool mySRGBFlag;
		bool myGenerateMipMapFlag;
	};


	struct Material
	{
		ID3D11InputLayout* myInputLayout;
		ID3D11VertexShader* myVertexShader;
		ID3D11PixelShader* myPixelShader;

		std::shared_ptr<Texture> myAlbedoTexture;
		std::shared_ptr<Texture> myNormalTexture;
		std::shared_ptr<Texture> myMaterialTexture;

		inline const bool IsValid() {
			return myInputLayout && myVertexShader && myPixelShader && myAlbedoTexture;
		}
	};




}