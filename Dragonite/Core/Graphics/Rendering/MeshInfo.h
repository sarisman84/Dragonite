#pragma once
#include <wrl/client.h>
#include "GameObject.h"

using Microsoft::WRL::ComPtr;

struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

namespace Engine
{
	namespace Graphics
	{
		class GraphicsEngine;
	}
}

struct Vertex
{
	Math::Vector4f myPosition;
	Math::Vector4f myColor;
};



struct MeshData
{
	friend class Engine::Graphics::GraphicsEngine;
	friend class ModelFactory;
public:
	std::vector<Vertex> myVertecies;
	std::vector<unsigned int> myIndicies;
private:
	ComPtr<ID3D11Buffer> myVertexBuffer;
	ComPtr<ID3D11Buffer> myIndexBuffer;
	
};


struct Model
{
public:
	std::vector<MeshData> myMesh;


};


using ModelPtr = std::shared_ptr<Model>;




struct ModelInstance
{
	friend class ModelFactory;
	friend class Engine::Graphics::GraphicsEngine;
public:
	ModelPtr myModel;
	const char* myVertexShaderPath = "Shaders/ColorShader_VS.cso";
	const char* myPixelShaderPath = "Shaders/ColorShader_PS.cso";
	Transform myTransform;
private:
	ComPtr<ID3D11VertexShader> myVertexShader;
	ComPtr<ID3D11PixelShader> myPixelShader;
	ComPtr<ID3D11InputLayout> myInputLayout;
};