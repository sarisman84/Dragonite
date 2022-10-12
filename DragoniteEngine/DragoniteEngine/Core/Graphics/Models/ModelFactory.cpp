#include "ModelFactory.h"
#include <fstream>
#include <string>
#include <iterator>
#include <d3d11.h>


#include "../Textures/TextureFactory.h"


#include "Core/Graphics/DirectX11/DXInterface.h"
#include "Core/Graphics/DirectX11/DXUtilities.h"

#include "Core/Graphics/GraphicsAPI.h"
#include "Core/Runtime.h"
#include "Core/PollingStation.h"




bool Dragonite::ModelFactory::Initialize(GraphicalInterface* aPipeline)
{
	myPipeline = aPipeline;

	auto cube = CreateUnitCube();
	auto screen = CreateScreenMesh();

	myFactoryModelData[cube->myName] = cube;
	myFactoryModelData[screen->myName] = screen;

	Material unlit = Dragonite::Material(
		{
			"Unlit",
			{
				{"POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_INPUT_PER_VERTEX_DATA },
				{"TEXCOORD", DXGI_FORMAT_R32G32_FLOAT, D3D11_INPUT_PER_VERTEX_DATA }
			}
		},
		{
		"Unlit"
		},
		L"",
		Color(1, 1, 1, 1),
		"unlit"
	);
	myFactoryMaterialData[unlit.myName] = unlit;

	return true;
}

std::shared_ptr<Dragonite::ModelInstance> Dragonite::ModelFactory::GetModel(const PrimitiveType aPrimitiveType, const std::string& aMaterial, const std::wstring aTexture)
{
	switch (aPrimitiveType)
	{
	case PrimitiveType::Cube:
		return GetModel("Primitive Cube", aMaterial);
	default:
		return nullptr;
	}
}

std::shared_ptr<Dragonite::ModelInstance> Dragonite::ModelFactory::GetModel(const std::string aModelName, const std::string& aMaterialName, const std::wstring aTexture)
{
	auto device = DXInterface::Device;
	std::string vsData;

	auto ins = std::make_shared<ModelInstance>();
	if (!ins) return nullptr;
	ins->myModel = myFactoryModelData[aModelName];

	if (!ins->myModel) return nullptr;

	Material mat = myFactoryMaterialData[aMaterialName];
	mat.myTexture = aTexture;
	VertexShader vsShader;
	PixelShader psShader;
	InputLayout inputLayout;
	if (FAILED(DXInterface::CreateVSInstance(mat.myVSInfo.myVertexShader, vsShader, vsData)))
	{
		return nullptr;
	}
	if (FAILED(DXInterface::CreatePSInstance(mat.myPSInfo.myPixelShader, psShader)))
	{
		return nullptr;
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout;

	for (auto& IPElement : mat.myVSInfo.myInputLayout)
	{
		layout.push_back(D3D11_INPUT_ELEMENT_DESC{ IPElement.myInputName, 0, (DXGI_FORMAT)IPElement.myFormat, 0, D3D11_APPEND_ALIGNED_ELEMENT,IPElement.myClassification,0 });
	}

	if (FAILED(device->CreateInputLayout(layout.data(), layout.size(), vsData.data(), vsData.size(), &inputLayout)))
	{
		return nullptr;
	}

	ins->myShaderInstructionsID = myPipeline->AddShaderInstructions(mat, vsShader, psShader, inputLayout);



	ins->myTexture = myPipeline->GetPollingStation()->Get<TextureFactory>()->LoadTexture(mat.myTexture.c_str());
	ins->myTexture->myName = mat.myTexture;
	ins->myTextureName = mat.myTexture;
	ins->myMaterialName = mat.myName;
	ins->myModelName = ins->myModel->myName;
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

	BufferDesc desc /*= BufferDesc(vertices, sizeof(vertices), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER)*/;
	//BufferDesc indexDesc/* = BufferDesc(indices, sizeof(indices), D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER)*/;

	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.ByteWidth = sizeof(vertices);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;


	desc.GetSubResourceData().SysMemPitch = 0;
	desc.GetSubResourceData().SysMemSlicePitch = 0;
	desc.GetSubResourceData().pSysMem = vertices;

	if (FAILED(DXInterface::CreateBuffer(model->myVertexBuffer, &desc)))
		return nullptr;

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(indices);
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;


	desc.GetSubResourceData().SysMemPitch = 0;
	desc.GetSubResourceData().SysMemSlicePitch = 0;
	desc.GetSubResourceData().pSysMem = indices;


	if (FAILED(DXInterface::CreateBuffer(model->myIndexBuffer, &desc)))
		return nullptr;

	model->myIndexCount = 36;
	model->myName = "Primitive Cube";

	return model;
}

Dragonite::ModelRef Dragonite::ModelFactory::CreateScreenMesh()
{
	ModelRef model = std::make_shared<Model>();

	unsigned int indices[6] =
	{
		0,1,2,
		0,2,3
	};

	Vertex vertices[4] =
	{
		Vertex::Position(-0.5f, -0.5f, 0, 1.0f).UV(0.0f, 0.0f),
		Vertex::Position(-0.5f,  0.5f, 0, 1.0f).UV(0.0f, 1.0f),
		Vertex::Position(0.5f,  0.5f, 0, 1.0f).UV(1.0f, 1.0f),
		Vertex::Position(0.5f, -0.5f, 0, 1.0f).UV(1.0f, 0.0f)
	};


	BufferDesc desc /*= BufferDesc(vertices, sizeof(vertices), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER)*/;
	//BufferDesc indexDesc/* = BufferDesc(indices, sizeof(indices), D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER)*/;

	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.ByteWidth = sizeof(vertices);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;


	desc.GetSubResourceData().SysMemPitch = 0;
	desc.GetSubResourceData().SysMemSlicePitch = 0;
	desc.GetSubResourceData().pSysMem = vertices;


	if (FAILED(DXInterface::CreateBuffer(model->myVertexBuffer, &desc)))
		return nullptr;

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(indices);
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;


	desc.GetSubResourceData().SysMemPitch = 0;
	desc.GetSubResourceData().SysMemSlicePitch = 0;
	desc.GetSubResourceData().pSysMem = indices;

	if (FAILED(DXInterface::CreateBuffer(model->myIndexBuffer, &desc)))
		return nullptr;

	model->myIndexCount = 6;
	model->myName = "Fullscreen Mesh";

	return model;
}





