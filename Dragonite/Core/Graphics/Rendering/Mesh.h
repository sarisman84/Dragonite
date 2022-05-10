#pragma once
#include <wrl/client.h>
#include <string>
#include "Utilities/Math/Vector.h"
#include "Utilities/Math/Matrix.h"
using Microsoft::WRL::ComPtr;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

struct FrameBufferData;

struct VertexInfo
{
	Math::Vector4f myPosition;
	Math::Vector4f myColor;
};



struct ObjectBufferData
{
	Math::Matrix4x4f myObjectMatrix;
};

enum class BufferType
{
	Vertex, Index
};

enum class ShaderType
{
	Vertex, Pixel
};

struct MeshData
{
	VertexInfo* myVertexData;
	unsigned int myVertexDataAmm;

	unsigned int* myIndexData;
	unsigned int myIndecesAmm;
};



class Mesh
{
public:
	Mesh();
	~Mesh();
	HRESULT TryInitializeInputLayout(ID3D11Device* aDevice, std::string someVertexData);
	HRESULT TryIntializeBuffer(ID3D11Device* aDevice, BufferType aType, void* someData, unsigned int aDataSize);
	HRESULT TryInitializeShader(ID3D11Device* aDevice, ShaderType aType, std::string aPath, std::string& someExtraData);

	void Draw(ID3D11DeviceContext* aContext);
private:
	ID3D11VertexShader* myVertexShader;
	ID3D11PixelShader* myPixelShader;
	ID3D11InputLayout* myInputLayout;

	ID3D11Buffer* myVertexBuffer;
	ID3D11Buffer* myIndexBuffer;
};





