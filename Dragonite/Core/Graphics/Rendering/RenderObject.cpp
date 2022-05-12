#include "RenderObject.h"
#include <d3d11.h>
#include <fstream>
#include "System.h"

RenderObject::RenderObject(const Shape& aShape, ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext, System* aSystem)
{
	myDevice = aDevice;
	myContext = aDeviceContext;
	myIndicesAmm = 0;
	myVertexShaderPath = "Shaders/ColorShader_VS.cso";
	myPixelShaderPath = "Shaders/ColorShader_PS.cso";
	myShape = aShape;
	mySystem = aSystem;
}

void RenderObject::SetSize(Math::Vector3f aSize)
{
	//HRESULT result;
	myObjectMatrix = Math::Matrix4x4<float>::CreateSizeMatrix(aSize) * myObjectMatrix;
	//TryCreateVertexBuffer(result, myShape.myVertices, myShape.myVerticesAmm);
}

void RenderObject::SetPosition(Math::Vector3f aPosition)
{
	//HRESULT result;
	//myObjectMatrix = Math::Matrix4x4<float>::CreateTransformMatrix(aPosition) * myObjectMatrix;
	myObjectMatrix.SetRow(4, { aPosition.x, aPosition.y, aPosition.z, 1 });
	//TryCreateVertexBuffer(result, myShape.myVertices, myShape.myVerticesAmm);
}

void RenderObject::SetPixelShader(std::string aPixelShader)
{
	myCachedPixelShaderPath = aPixelShader;
	HRESULT result;
	if (myPixelShaderPath != myCachedPixelShaderPath)
	{
		myPixelShaderPath = myCachedPixelShaderPath;
		myCachedPixelShaderPath = "";
		if (!TryLoadPixelShader(result, myPixelShaderPath))
		{
			myPixelShaderPath = "Shaders/ColorShader_PS.cso";
			TryLoadPixelShader(result, myPixelShaderPath);
		}
	}

}

bool RenderObject::UpdateBuffers()
{
	HRESULT result;

	if (!TryCreateVertexBuffer(result, myShape.myVertices, myShape.myVerticesAmm))
		return false;

	if (!TryCreateIndexBuffer(result, myShape.myIndicies, myIndicesAmm))
		return false;
	return true;
}

bool RenderObject::Initialize()
{
	Math::Vector3f defaultSize = { 1.f, 1.f, 1.f };
	defaultSize.x /= mySystem->GetWindowsInfo().GetAspectRatio();
	myObjectMatrix = Math::Matrix4x4f::CreateSizeMatrix(defaultSize);
	HRESULT result;

	myIndicesAmm = myShape.myIndicesAmm;


	if (!UpdateBuffers())
		return false;

	std::string vsData;
	if (!TryLoadVertexShader(result, myVertexShaderPath, vsData))
		return false;


	if (!TryLoadPixelShader(result, myPixelShaderPath))
		return false;



	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	result = myDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &myInputLayout);
	if (FAILED(result))
		return false;


	return true;
}

void RenderObject::Draw()
{

	Vertex someVertices[1000];
	ZeroMemory(&someVertices, sizeof(Vertex) * myShape.myVerticesAmm);

	D3D11_MAPPED_SUBRESOURCE resource;

	myContext->Map(myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	for (size_t i = 0; i < myShape.myVerticesAmm; i++)
	{
		auto pos = myShape.myVertices[i].myPosition;
		pos = pos * myObjectMatrix;
		someVertices[i].myPosition = pos;


		someVertices[i].myColor = myShape.myVertices[i].myColor;
	}
	memcpy(resource.pData, someVertices, sizeof(Vertex) * myShape.myVerticesAmm);
	myContext->Unmap(myVertexBuffer.Get(), 0);


	//MVP

	//object to world
	//world to camera
	//camera to projection
	//perpsective divide //Behövs inte göra




	//UpdateBuffers();

	myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	myContext->IASetInputLayout(myInputLayout.Get());

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	myContext->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), &stride, &offset);
	myContext->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	myContext->VSSetShader(myVertexShader.Get(), nullptr, 0);
	myContext->PSSetShader(myPixelShader.Get(), nullptr, 0);

	myContext->DrawIndexed(myIndicesAmm, 0, 0);
}

bool RenderObject::TryCreateVertexBuffer(long& someResult, Vertex* someVertices, unsigned int aSize)
{
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	//ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * aSize;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = someVertices;

	someResult = myDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer);


	return !(FAILED(someResult));
}

bool RenderObject::TryCreateIndexBuffer(long& someResult, unsigned int* someIndicies, unsigned int anIndeciesAmm)
{
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * anIndeciesAmm;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData = { 0 };
	indexData.pSysMem = someIndicies;

	someResult = myDevice->CreateBuffer(&indexBufferDesc, &indexData, &myIndexBuffer);
	return !(FAILED(someResult));
}

bool RenderObject::TryLoadVertexShader(long& someResult, const std::string& aPath, std::string& someExtraData)
{
	std::ifstream vsFile;
	vsFile.open(aPath.c_str(), std::ios::binary);
	someExtraData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };

	someResult = myDevice->CreateVertexShader(someExtraData.data(), someExtraData.size(), nullptr, &myVertexShader);

	vsFile.close();
	return !(FAILED(someResult));
}

bool RenderObject::TryLoadPixelShader(long& someResult, const std::string& aPath)
{
	std::ifstream psFile;
	psFile.open(aPath.c_str(), std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

	someResult = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &myPixelShader);

	psFile.close();
	return !(FAILED(someResult));
}