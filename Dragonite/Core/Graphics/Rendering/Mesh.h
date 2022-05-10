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

class Mesh
{
public:
	Mesh(ID3D11Device* aDevice, ID3D11DeviceContext* aContext);
	~Mesh();
	HRESULT TryInitializeInputLayout(std::string someVertexData);
	HRESULT TryIntializeBuffer(BufferType aType, void* someData, unsigned int aDataSize);
	HRESULT TryInitializeShader(ShaderType aType, std::string aPath, std::string& someExtraData);
	inline ObjectBufferData& GetObjectData() { return myObjectBufferData; }
	void Draw();
private:
	VertexInfo* myVertexData;
	unsigned int myVertexDataAmm;

	unsigned int* myIndexData;
	unsigned int myIndecesAmm;

	ObjectBufferData myObjectBufferData;
	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;

	ID3D11VertexShader* myVertexShader;
	ID3D11PixelShader* myPixelShader;
	ID3D11InputLayout* myInputLayout;

	ID3D11Buffer* myVertexBuffer;
	ID3D11Buffer* myIndexBuffer;


};

