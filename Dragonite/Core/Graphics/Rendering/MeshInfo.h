#pragma once
#include <wrl/client.h>
#include "GameObject.h"

using Microsoft::WRL::ComPtr;

using namespace Dragonite;


struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;


namespace Dragonite
{
	class GraphicsEngine;
	class Texture;
}



struct Vertex
{
	Math::Vector3f myPosition;
	Math::Vector4f myColor;
	Math::Vector3f myNormal;
	Math::Vector3f myTangent;
	Math::Vector3f myBiNormal;
	Math::Vector2f myUV;
};


struct Material
{
	struct TextureInfo
	{
		const char* myTexturePath;
		int myType;
	};
	Math::Vector4f myColor;
	std::vector<TextureInfo> myTextureMapInfo;
	const char* myVertexShader;
	const char* myPixelShader;
};


struct MeshData
{
	friend Dragonite::GraphicsEngine;
	friend class ModelFactory;
	friend class RenderTarget;
public:
	std::vector<Vertex> myVertecies;
	unsigned int myIndiciesAmm;
private:
	ComPtr<ID3D11Buffer> myVertexBuffer;
	ComPtr<ID3D11Buffer> myIndexBuffer;
	std::vector <std::shared_ptr< Texture>> myTextureBuffer;
};


struct Model
{
	friend Dragonite::GraphicsEngine;
	friend class ModelFactory;
	friend class RenderTarget;
public:
	std::vector<MeshData> myMesh;
private:
	ComPtr<ID3D11VertexShader> myVertexShader;
	ComPtr<ID3D11PixelShader> myPixelShader;
	ComPtr<ID3D11InputLayout> myInputLayout;


};


using ModelPtr = std::shared_ptr<Model>;




struct ModelInstance
{

public:
	ModelPtr myModel;
	Transform myTransform;
	Material myMaterial;
};


using ModelInsPtr = std::shared_ptr<ModelInstance>;