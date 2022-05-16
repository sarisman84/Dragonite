#pragma once
#include "Rendering/MeshInfo.h"
#include <wrl/client.h>
#include <map>
#include <string>
#include <vector>

#define UNITCUBE "Cube"
#define UNITPIRAMID "Piramid"
#define UNITICOSPHERE "IcoSphere"


namespace Engine
{
	class System;
}

struct ID3D11Device;


enum class BufferType
{
	Vertex, Index
};

class ModelFactory
{
public:
	ModelFactory();
	inline void FetchSystem(Engine::System* aSystem)
	{
		mySystem = aSystem;
		InitializeBuffers();
	}
	ModelInstance* CreateInstanceOf(std::string aKey, std::string aVertexShaderPath = "", std::string aPixelShaderPath = "");
private:
	void InitializeBuffers();
	HRESULT LoadVertexShader(ID3D11Device* aDevice, ComPtr<ID3D11VertexShader>& aShader, const char* aPath, std::string& someExtraData);
	HRESULT LoadPixelShader(ID3D11Device* aDevice, ComPtr<ID3D11PixelShader>& aShader, const char* aPath);
	HRESULT LoadInputLayout(ID3D11Device* aDevice, ComPtr<ID3D11InputLayout>& aLayout, std::string someVertexData);

	HRESULT InitializeBuffer(ID3D11Device* aDevice, const BufferType aType, MeshData& someData);

	ModelPtr GetUnitCube();
	ModelPtr GetUnitPiramid();
	ModelPtr GetUnitIcoSphere();

	std::map<std::string, ModelPtr> myModelTypes;
	std::vector<ModelInstance> myInstances;
	Engine::System* mySystem;
};