#include "Model.h"
#include <d3d11.h>


Model::Model()
{
}

Model::Model(const Model& aModel)
{
}

Model::~Model()
{
}

bool Model::Initialize(ComPtr<ID3D11Device> aDevice)
{
	bool result;
	result = InitializeBuffers(aDevice);

	if (!result) return false;


	return true;
}

void Model::Shutdown()
{
	ShutdownBuffers();
	return;
}

void Model::Render(ComPtr<ID3D11DeviceContext> aContext)
{
	RenderBuffers(aContext);
	return;
}

int Model::GetIndexCount()
{
	return myIndexCount;
}

bool Model::InitializeBuffers(ComPtr<ID3D11Device> aDevice)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//Manually create a model

	myVertexCount = 3;
	myIndexCount = 3;

	vertices = new VertexType[myVertexCount];
	if (!vertices) return false;
	indices = new unsigned long[myIndexCount];
	if (!indices) return false;

	// Load the vertex array with data.
	vertices[0].myPosition = Vector3<float>(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].myColor = Vector4<float>(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].myPosition = Vector3<float>(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].myColor = Vector4<float>(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].myPosition = Vector3<float>(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].myColor = Vector4<float>(0.0f, 1.0f, 0.0f, 1.0f);


	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * myVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;


	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = aDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * myIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = aDevice->CreateBuffer(&indexBufferDesc, &indexData, &myIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Model::ShutdownBuffers()
{


	return;
}


void Model::RenderBuffers(ComPtr<ID3D11DeviceContext> aContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	aContext->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	aContext->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	aContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
