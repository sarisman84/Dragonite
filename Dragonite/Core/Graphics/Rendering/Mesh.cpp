#include "Mesh.h"
#include <d3d11.h>
#include <fstream>


Mesh::Mesh(ID3D11Device* aDevice, ID3D11DeviceContext* aContext)
{
	myDevice = aDevice;
	myContext = aContext;

	

}

Mesh::~Mesh()
{
	myVertexBuffer->Release();
	myIndexBuffer->Release();
	myVertexShader->Release();
	myPixelShader->Release();


	delete[] myVertexData;
}

HRESULT Mesh::TryInitializeInputLayout(std::string someVertexData)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	return myDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), someVertexData.data(), someVertexData.size(), &myInputLayout);
}

HRESULT Mesh::TryIntializeBuffer(BufferType aType, void* someData, unsigned int aDataSize)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = someData;

	switch (aType)
	{
	case BufferType::Vertex:
		if (myVertexBuffer)
			myVertexBuffer->Release();

		bufferDesc.ByteWidth = sizeof(VertexInfo) * aDataSize;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;

		return myDevice->CreateBuffer(&bufferDesc, &data, &myVertexBuffer);
	case BufferType::Index:
		if (myIndexBuffer)
			myIndexBuffer->Release();

		bufferDesc.ByteWidth = sizeof(unsigned int) * aDataSize;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;

		return myDevice->CreateBuffer(&bufferDesc, &data, &myIndexBuffer);
	}


	return E_INVALIDARG;
}

HRESULT Mesh::TryInitializeShader(ShaderType aType, std::string aPath, std::string& someExtraData)
{
	std::ifstream file;
	file.open(aPath.c_str(), std::ios::binary);
	someExtraData = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
	switch (aType)
	{
	case ShaderType::Vertex:
		return myDevice->CreateVertexShader(someExtraData.data(), someExtraData.size(), nullptr, &myVertexShader);
	case ShaderType::Pixel:
		return myDevice->CreatePixelShader(someExtraData.data(), someExtraData.size(), nullptr, &myPixelShader);
	}
	return E_INVALIDARG;
}

void Mesh::Draw()
{
	myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	myContext->IASetInputLayout(myInputLayout);

	unsigned int stride = sizeof(VertexInfo);
	unsigned int offset = 0;
	myContext->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
	myContext->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	myContext->VSSetShader(myVertexShader, nullptr, 0);
	myContext->PSSetShader(myPixelShader, nullptr, 0);

	myContext->DrawIndexed(myIndecesAmm, 0, 0);
}


