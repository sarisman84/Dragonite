#include "Render.h"
#include <d3d11.h>
#include <fstream>


using namespace Engine::Graphics;

Render::Render(ID3D11Device* aDevice, ID3D11DeviceContext* aDeviceContext)
{
	myDevice = aDevice;
	myContext = aDeviceContext;
	myIndicesAmm = 0;
	myVertexShaderPath = "Shaders/ColorShader_VS.cso";
	myPixelShaderPath = "Shaders/ColorShader_PS.cso";
}

Render::~Render() = default;

bool Render::Initialize()
{
	HRESULT result;



	auto info = OnInit();

	myIndicesAmm = info.myIndicesAmm;


	if (!TryCreateVertexBuffer(result, info.myVerticies, info.myVerticiesAmm))
		return false;

	if (!TryCreateIndexBuffer(result, info.myIndices, myIndicesAmm))
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

void Render::Draw()
{
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

bool Render::TryCreateVertexBuffer(long& someResult, Vertex * someVertices, unsigned int aSize)
{
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * aSize;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = someVertices;

	someResult = myDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer);


	return !(FAILED(someResult));
}

bool Render::TryCreateIndexBuffer(long& someResult, unsigned int* someIndicies, unsigned int anIndeciesAmm)
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

bool Render::TryLoadVertexShader(long& someResult, const std::string & aPath, std::string & someExtraData)
{
	std::ifstream vsFile;
	vsFile.open(aPath, std::ios::binary);
	someExtraData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };

	someResult = myDevice->CreateVertexShader(someExtraData.data(), someExtraData.size(), nullptr, &myVertexShader);

	vsFile.close();
	return !(FAILED(someResult));
}

bool Render::TryLoadPixelShader(long& someResult, const std::string & aPath)
{
	std::ifstream psFile;
	psFile.open(aPath, std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

	someResult = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &myPixelShader);

	psFile.close();
	return !(FAILED(someResult));
}


