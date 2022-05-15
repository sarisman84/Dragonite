#include "ModelFactory.h"
#include "System.h"
#include <d3d11.h>  
#include <fstream>
#include <iostream>

ModelFactory::ModelFactory()
{
	myModelTypes[UNITCUBE] = GetUnitCube();
	myModelTypes[UNITPIRAMID] = GetUnitPiramid();
	myModelTypes[UNITICOSPHERE] = GetUnitIcoSphere();




}

ModelInstance* ModelFactory::CreateInstanceOf(std::string aKey)
{
	ID3D11Device* device = mySystem->GetGraphicsEngine()->GetDevice();
	ModelInstance ins;
	ins.myModel = myModelTypes[aKey];
	std::string someData;

	if (FAILED(LoadVertexShader(device, ins.myVertexShader, ins.myVertexShaderPath, someData)))
		return nullptr;
	if (FAILED(LoadPixelShader(device, ins.myPixelShader, ins.myPixelShaderPath)))
		return nullptr;

	if (FAILED(LoadInputLayout(device, ins.myInputLayout, someData)))
		return nullptr;

	std::cout << "[Log]<ModelFactory>: Created Instance of " << aKey << "!" << std::endl;
	myInstances.push_back(ins);
	return &myInstances.back();
}

void ModelFactory::InitializeBuffers()
{
	ID3D11Device* device = mySystem->GetGraphicsEngine()->GetDevice();
	for (auto& model : myModelTypes)
	{
		if (!model.second)
			continue;
		for (auto& subMesh : model.second->myMesh)
		{
			if (FAILED(InitializeBuffer(device, BufferType::Vertex, subMesh)))
				continue;
			if (FAILED(InitializeBuffer(device, BufferType::Index, subMesh)))
				continue;

			std::cout << "[Log]<ModelFactory>: Model (" << model.first << ") initialized! " << std::endl;
		}
	}
}

HRESULT ModelFactory::LoadVertexShader(ID3D11Device* aDevice, ComPtr<ID3D11VertexShader>& aShader, const char* aPath, std::string& anOutVertexData)
{
	std::ifstream vsFile;
	vsFile.open(aPath, std::ios::binary);
	anOutVertexData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };



	vsFile.close();

	return aDevice->CreateVertexShader(anOutVertexData.data(), anOutVertexData.size(), nullptr, &aShader);
}

HRESULT ModelFactory::LoadPixelShader(ID3D11Device* aDevice, ComPtr<ID3D11PixelShader>& aShader, const char* aPath)
{

	std::ifstream psFile;
	psFile.open(aPath, std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	psFile.close();
	return aDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &aShader);
}

HRESULT ModelFactory::LoadInputLayout(ID3D11Device* aDevice, ComPtr<ID3D11InputLayout>& aLayout, std::string someVertexData)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	return aDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), someVertexData.data(), someVertexData.size(), &aLayout);;
}

HRESULT ModelFactory::InitializeBuffer(ID3D11Device* aDevice, const BufferType aType, MeshData& someData)
{
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	switch (aType)
	{
	case BufferType::Vertex:
		data.pSysMem = &someData.myVertecies.front();
		bufferDesc.ByteWidth = sizeof(Vertex) * static_cast<int>(someData.myVertecies.size());
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		return aDevice->CreateBuffer(&bufferDesc, &data, &someData.myVertexBuffer);
	case BufferType::Index:
		data.pSysMem = &someData.myIndicies.front();
		bufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<int>(someData.myIndicies.size());
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		return aDevice->CreateBuffer(&bufferDesc, &data, &someData.myIndexBuffer);
	default:
		break;
	}

	return E_NOTIMPL;
}

ModelPtr ModelFactory::GetUnitCube()
{
	ModelPtr model = std::make_shared<Model>();
	MeshData data;

	for (float xPos = -1; xPos <= 1; xPos++)
	{
		if (xPos == 0) continue;
		for (float yPos = -1; yPos <= 1; yPos++)
		{
			if (yPos == 0) continue;
			for (float zPos = -1; zPos <= 1; zPos++)
			{
				if (zPos == 0) continue;
				data.myVertecies.push_back(Vertex{ Math::Vector4f(xPos, yPos, zPos, 1.f), Math::Vector4f(1.f, 1.f, 1.f, 1.f) });
			}

		}
	}

	data.myIndicies = {
		0,2,3, //Left Side
		1,0,3,

		6,2,0, //Back Side
		4,6,0,

		5,4,7, //Right Side
		6,7,4,

		3,7,1, //Front Side
		5,1,7,

		3,6,7,//Top Side
		2,6,3,

		1,4,0,//Bottom Side
		4,5,1
	};

	model->myMesh.push_back(data);
	return model;
}

ModelPtr ModelFactory::GetUnitPiramid()
{
	return ModelPtr();
}

ModelPtr ModelFactory::GetUnitIcoSphere()
{
	return ModelPtr();
}
