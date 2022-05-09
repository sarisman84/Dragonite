#pragma once
#include <wrl/client.h>
#include <string>
#include "../Utilities/Math/Matrix4x4.h"

using Microsoft::WRL::ComPtr;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;



enum class Primitive
{
	Circle, Quad, Triangle
};


struct Vertex
{
	float myX, myY, myZ, myW;
	float myR, myG, myB, myA;
};


struct Shape
{
	Vertex myVertices[1000];
	unsigned int myIndicies[1000];
	const char* myPixelShader = "Shaders/ColorShader_PS.cso";
	const char* myVertexShader = "Shaders/ColorShader_VS.cso";
	unsigned int myVerticesAmm;
	unsigned int myIndicesAmm;
};



namespace Engine
{
	class System;
	namespace Graphics
	{
		class GraphicsEngine;
	}
}
using namespace Engine;
class RenderObject
{
	friend Engine::Graphics::GraphicsEngine;
	
public:
	RenderObject(const Shape& aShape, ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext, class System* aSystem );
	void SetSize(Math::Vector3f aSize);
	void SetPosition(Math::Vector3f aPosition);
	void SetPixelShader(std::string aPixelShader);

private:
	void Draw();
	bool UpdateBuffers();
	bool Initialize();
	
	bool TryCreateVertexBuffer(long& someResult, Vertex* someVertices, unsigned int aSize);
	bool TryCreateIndexBuffer(long& someResult, unsigned int* someIndicies, unsigned int anIndeciesAmm);

	bool TryLoadVertexShader(long& someResult, const std::string& aPath, std::string& someExtraData);
	bool TryLoadPixelShader(long& someResult, const std::string& aPath);
	

	ComPtr<ID3D11Buffer> myVertexBuffer;
	ComPtr<ID3D11Buffer> myIndexBuffer;
	ComPtr<ID3D11VertexShader> myVertexShader;
	ComPtr<ID3D11PixelShader> myPixelShader;
	ComPtr<ID3D11InputLayout> myInputLayout;

	Shape myShape;

	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;
	unsigned int myIndicesAmm;

	std::string myVertexShaderPath;
	std::string myPixelShaderPath;
	std::string myCachedPixelShaderPath;
	Math::Matrix4x4f myObjectMatrix;

	Engine::System* mySystem;


};


using Object = std::shared_ptr<RenderObject>;
