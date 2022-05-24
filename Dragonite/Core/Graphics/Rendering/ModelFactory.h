#pragma once
#include "Rendering/MeshInfo.h"
#include <wrl/client.h>
#include <map>
#include <string>
#include <vector>

#define UNITCUBE "Cube"
#define UNITPYRAMID "Piramid"
#define UNITICOSPHERE "IcoSphere"


namespace Engine
{
	class System;
}

struct ID3D11Device;
struct ID3D11DeviceContext;


enum class BufferType
{
	Vertex, Index
};

class ModelFactory
{
public:
	ModelFactory(Engine::System* aSystem);

	ModelInsPtr CreateInstanceOf(std::string aKey, const Material aMaterial);
private:
	struct TempMeshData
	{
		std::vector<Vertex> myVertecies;
		std::vector<unsigned int> myIndicies;
	};


	HRESULT LoadVertexShader(ID3D11Device* aDevice, ComPtr<ID3D11VertexShader>& aShader, const char* aPath, std::string& someExtraData);
	HRESULT LoadPixelShader(ID3D11Device* aDevice, ComPtr<ID3D11PixelShader>& aShader, const char* aPath);
	HRESULT LoadInputLayout(ID3D11Device* aDevice, ComPtr<ID3D11InputLayout>& aLayout, std::string someVertexData);
	HRESULT LoadTexture(ID3D11Device* aDevice, Texture& aTexture, const char* aTexturePath, const int aSlot = 0);

	HRESULT InitializeBuffer(ID3D11Device* aDevice, const BufferType aType, MeshData& someData, TempMeshData someTempData);


	ModelPtr InitializeModelOfType(std::vector<TempMeshData> someTempData);

	std::vector<TempMeshData> GetUnitCube();
	TempMeshData GetUnitPiramid();
	TempMeshData GetUnitIcoSphere();

	std::map<std::string, ModelPtr> myModelTypes;
	std::vector<ModelInsPtr> myInstances;
	Engine::System* mySystem;
};