#include "ModelFactory.h"
#include <fstream>
#include <string>
#include <iterator>
#include <d3d11.h>


#include "../Textures/TextureFactory.h"

#include "../GraphicsAPI.h"
#include "../../Application.h"




bool Dragonite::ModelFactory::Initialize(GraphicsPipeline* aPipeline)
{
	myPipeline = aPipeline;

	myFactoryData[PrimitiveType::Cube] = CreateUnitCube();

	return true;
}

std::shared_ptr<Dragonite::ModelInstance> Dragonite::ModelFactory::GetModel(const PrimitiveType aPrimitiveType, const Material& aMaterial)
{
	auto device = myPipeline->GetDevice();
	std::string vsData;

	auto ins = std::make_shared<ModelInstance>();
	if (!ins) return nullptr;
	ins->myModel = myFactoryData[aPrimitiveType];
	if (!ins->myModel) return nullptr;

	if (FAILED(CreateVSInstance(device, aMaterial.myVSInfo.myVertexShader, ins->myVertexShader, vsData)))
	{
		return nullptr;
	}
	if (FAILED(CreatePSInstance(device, aMaterial.myPSInfo.myPixelShader, ins->myPixelShader)))
	{
		return nullptr;
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout;

	for (auto& IPElement : aMaterial.myVSInfo.myInputLayout)
	{
		layout.push_back(D3D11_INPUT_ELEMENT_DESC{ IPElement.myInputName, 0, (DXGI_FORMAT)IPElement.myFormat, 0, D3D11_APPEND_ALIGNED_ELEMENT,IPElement.myClassification,0 });
	}

	if (FAILED(device->CreateInputLayout(layout.data(), layout.size(), vsData.data(), vsData.size(), &ins->myInputLayout)))
	{
		return nullptr;
	}


	ins->myTexture = myPipeline->GetApplication()->GetPollingStation().Get<TextureFactory>()->LoadTexture(aMaterial.myTexture.c_str());


	return ins;
}

Dragonite::ModelRef Dragonite::ModelFactory::CreateUnitCube()
{

	ModelRef model = std::make_shared<Model>();

	unsigned int indices[36] =
	{
	  0, 1, 2,
	  0, 2, 3,
	  4, 5, 6,
	  4, 6, 7,
	  8, 9, 10,
	  8, 10, 11,
	  12, 13, 14,
	  12, 14, 15,
	  16, 17, 18,
	  16, 18, 19,
	  20, 21, 22,
	  20, 22, 23
	};

	Vertex vertices[36] = {

		Vertex::Position(0.5f,  -0.5f,  0.5f,  1.0f).UV(0.0f, 1.0f),
		Vertex::Position(0.5f,   0.5f,  0.5f,  1.0f).UV(0.0f, 0.0f),
		Vertex::Position(-0.5f,  0.5f,  0.5f,  1.0f).UV(1.0f, 0.0f),
		Vertex::Position(-0.5f, -0.5f,  0.5f,  1.0f).UV(1.0f, 1.0f),
		Vertex::Position(-0.5f, -0.5f,  0.5f,  1.0f).UV(0.0f, 1.0f),
		Vertex::Position(-0.5f,  0.5f,  0.5f,  1.0f).UV(0.0f, 0.0f),
		Vertex::Position(-0.5f,  0.5f, -0.5f,  1.0f).UV(1.0f, 0.0f),
		Vertex::Position(-0.5f, -0.5f, -0.5f , 1.0f).UV(1.0f, 1.0f),
		Vertex::Position(-0.5f, -0.5f, -0.5f , 1.0f).UV(0.0f, 1.0f),
		Vertex::Position(-0.5f,  0.5f, -0.5f,  1.0f).UV(0.0f, 0.0f),
		Vertex::Position(0.5f,   0.5f, -0.5f,  1.0f).UV(1.0f, 0.0f),
		Vertex::Position(0.5f,  -0.5f, -0.5f,  1.0f).UV(1.0f, 1.0f),
		Vertex::Position(0.5f,  -0.5f, -0.5f,  1.0f).UV(0.0f, 1.0f),
		Vertex::Position(0.5f,   0.5f, -0.5f,  1.0f).UV(0.0f, 0.0f),
		Vertex::Position(0.5f,   0.5f,  0.5f,  1.0f).UV(1.0f, 0.0f),
		Vertex::Position(0.5f,  -0.5f,  0.5f,  1.0f).UV(1.0f, 1.0f),
		Vertex::Position(0.5f,   0.5f,  0.5f,  1.0f).UV(0.0f, 1.0f),
		Vertex::Position(0.5f,   0.5f, -0.5f,  1.0f).UV(0.0f, 0.0f),
		Vertex::Position(-0.5f,  0.5f, -0.5f,  1.0f).UV(1.0f, 0.0f),
		Vertex::Position(-0.5f,  0.5f,  0.5f,  1.0f).UV(1.0f, 1.0f),
		Vertex::Position(-0.5f, -0.5f,  0.5f,  1.0f).UV(0.0f, 1.0f),
		Vertex::Position(-0.5f, -0.5f, -0.5f , 1.0f).UV(0.0f, 0.0f),
		Vertex::Position(0.5f,  -0.5f, -0.5f,  1.0f).UV(1.0f, 0.0f),
		Vertex::Position(0.5f,  -0.5f,  0.5f,  1.0f).UV(1.0f, 1.0f)




	};



	DataBufferDesc vertexBufferDesc(vertices, sizeof(vertices), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER);
	DataBufferDesc indexBufferDesc(indices, sizeof(indices), D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER);

	if (FAILED(GraphicsPipeline::CreateBuffer(myPipeline->GetDevice(), model->myVertexBuffer, vertexBufferDesc)))
		return nullptr;
	if (FAILED(GraphicsPipeline::CreateBuffer(myPipeline->GetDevice(), model->myIndexBuffer, indexBufferDesc)))
		return nullptr;

	model->myIndexCount = 36;

	return model;
}

HRESULT Dragonite::ModelFactory::CreateVSInstance(Device aDevice, const char* aPath, VertexShader& aShader, std::string& someData)
{
	typedef std::istreambuf_iterator<char> ShaderIterator;
	std::string path = "Shaders/" + std::string(aPath) + "_VS.cso";
	std::ifstream file(path, std::ios::binary);

	if (!file || file.fail())
		return E_ACCESSDENIED;

	bool hasFailed = file.is_open();
	someData = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
	file.close();
	hasFailed = file.is_open();

	HRESULT r = aDevice->CreateVertexShader(someData.data(), someData.size(), nullptr, &aShader);
	return r;
}

HRESULT Dragonite::ModelFactory::CreatePSInstance(Device aDevice, const char* aPath, PixelShader& aShader)
{
	typedef std::istreambuf_iterator<char> ShaderIterator;
	std::string data;

	std::ifstream file;
	std::string path = "Shaders/" + std::string(aPath) + "_PS.cso";
	file.open(path, std::ios::binary);
	data = { ShaderIterator(file), ShaderIterator() };

	return aDevice->CreatePixelShader(data.data(), data.size(), nullptr, &aShader);
}



