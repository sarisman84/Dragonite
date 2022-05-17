#include "ModelFactory.h"
#include "System.h"
#include <d3d11.h>  
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "Utilities/STB/ImageImport.h"

ModelFactory::ModelFactory(Engine::System* aSystem)
{
	mySystem = aSystem;
	myModelTypes[UNITCUBE] = InitializeModelOfType({ GetUnitCube() });
	myModelTypes[UNITPYRAMID] = InitializeModelOfType({ GetUnitPiramid() });
	myModelTypes[UNITICOSPHERE] = InitializeModelOfType({ GetUnitIcoSphere() });
}

ModelInsPtr ModelFactory::CreateInstanceOf(std::string aKey, const Material aMaterial)
{
	ID3D11Device* device = mySystem->GetGraphicsEngine()->GetDevice();
	ModelInsPtr ins = std::make_shared<ModelInstance>();

	const char* vertexShaderPath = aMaterial.myVertexShader;
	const char* pixelShaderPath = aMaterial.myPixelShader;


		


		

	ins->myModel = myModelTypes[aKey];
	std::string someData;

	if (FAILED(LoadVertexShader(device, ins->myModel->myVertexShader, vertexShaderPath, someData)))
		return nullptr;
	if (FAILED(LoadPixelShader(device, ins->myModel->myPixelShader, pixelShaderPath)))
		return nullptr;

	if (FAILED(LoadInputLayout(device, ins->myModel->myInputLayout, someData)))
		return nullptr;

	if (FAILED(LoadTexture(device, ins->myModel->myTexture, aMaterial.myTexturePath, 0)))
		return nullptr;

	std::cout << "[Log]<ModelFactory>: Created Instance of " << aKey << "!" << std::endl;
	ins->myMaterial = aMaterial;
	myInstances.push_back(ins);
	return myInstances.back();
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
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	return aDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), someVertexData.data(), someVertexData.size(), &aLayout);
}

HRESULT ModelFactory::LoadTexture(ID3D11Device* aDevice, Texture& aTexture, const char* aTexturePath, const int aSlot)
{

	int width, height, channels;
	unsigned char* img = stbi_load(aTexturePath, &width, &height, &channels, 0);

	if (!img) return E_INVALIDARG;

	switch (channels)
	{
	case 3:
	{
		std::vector<unsigned char> imageData(width * height * 4);

		for (size_t i = 0; i < width * height; i++)
		{
			imageData[4 * i + 0] = img[3 * i + 0];
			imageData[4 * i + 1] = img[3 * i + 1];
			imageData[4 * i + 2] = img[3 * i + 2];
			imageData[4 * i + 3] = 255;
		}
		img = imageData.data();
		break;
	}
	case 4:
		break;
	default:
		return E_INVALIDARG;
	}



	ID3D11Texture2D* texturePtr = nullptr;

	D3D11_SUBRESOURCE_DATA subResourceDesc;
	D3D11_TEXTURE2D_DESC desc;
	aTexture.mySlot = aSlot;
	ZeroMemory(&desc, sizeof(desc));
	ZeroMemory(&subResourceDesc, sizeof(subResourceDesc));

	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;




	subResourceDesc.pSysMem = (void*)img;
	subResourceDesc.SysMemPitch = width * 4;
	subResourceDesc.SysMemSlicePitch = width * height * 4;

	if (FAILED(aDevice->CreateTexture2D(&desc, &subResourceDesc, &texturePtr)))
		return E_FAIL;

	if (FAILED(aDevice->CreateShaderResourceView(texturePtr, NULL, &aTexture.myTextureResource)))
		return E_FAIL;

	texturePtr->Release();



	return NOERROR;
}



HRESULT ModelFactory::InitializeBuffer(ID3D11Device* aDevice, const BufferType aType, MeshData& someData, TempMeshData someTempData)
{
	someData.myVertecies = someTempData.myVertecies;
	someData.myIndiciesAmm = static_cast<int>(someTempData.myIndicies.size());


	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	switch (aType)
	{
	case BufferType::Vertex:
		data.pSysMem = &someTempData.myVertecies.front();
		bufferDesc.ByteWidth = sizeof(Vertex) * static_cast<int>(someTempData.myVertecies.size());
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		return aDevice->CreateBuffer(&bufferDesc, &data, &someData.myVertexBuffer);
	case BufferType::Index:
		data.pSysMem = &someTempData.myIndicies.front();
		bufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<int>(someTempData.myIndicies.size());
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		return aDevice->CreateBuffer(&bufferDesc, &data, &someData.myIndexBuffer);
	default:
		break;
	}

	return E_NOTIMPL;
}

ModelPtr ModelFactory::InitializeModelOfType(std::vector<TempMeshData> someTempData)
{
	ID3D11Device* device = mySystem->GetGraphicsEngine()->GetDevice();
	ModelPtr model = std::make_shared<Model>();


	for (size_t i = 0; i < someTempData.size(); ++i)
	{
		if (someTempData[i].myVertecies.size() == 0 || someTempData[i].myIndicies.size() == 0) continue;
		MeshData data = MeshData();
		if (FAILED(InitializeBuffer(device, BufferType::Vertex, data, someTempData[i])))
			continue;
		if (FAILED(InitializeBuffer(device, BufferType::Index, data, someTempData[i])))
			continue;
		data.myIndiciesAmm = static_cast<unsigned int>(someTempData[i].myIndicies.size());

		model->myMesh.push_back(data);
	}


	return model;
}

ModelFactory::TempMeshData ModelFactory::GetUnitCube()
{

	TempMeshData data;
	data.myVertecies = {
			  Vertex{Vector4f{0.5f,  -0.5f,  0.5f,  1},  Vector4f{1, 1, 1, 1}, Vector2f{0, 1}},
			  Vertex{Vector4f{0.5f,   0.5f,  0.5f,  1},  Vector4f{1, 1, 1, 1}, Vector2f{0, 0}},
			  Vertex{Vector4f{-0.5f,  0.5f,  0.5f,  1},  Vector4f{1, 1, 1, 1}, Vector2f{1, 0}},
			  Vertex{Vector4f{-0.5f, -0.5f,  0.5f,  1},  Vector4f{1, 1, 1, 1}, Vector2f{1, 1}},
			  Vertex{Vector4f{-0.5f, -0.5f,  0.5f,  1},  Vector4f{1, 0, 0, 1}, Vector2f{0, 1}},
			  Vertex{Vector4f{-0.5f,  0.5f,  0.5f,  1},  Vector4f{1, 0, 0, 1}, Vector2f{0, 0}},
			  Vertex{Vector4f{-0.5f,  0.5f, -0.5f,  1},  Vector4f{1, 0, 0, 1}, Vector2f{1, 0}},
			  Vertex{Vector4f{-0.5f, -0.5f, -0.5f , 1},  Vector4f{1, 0, 0, 1}, Vector2f{1, 1}},
			  Vertex{Vector4f{-0.5f, -0.5f, -0.5f , 1},  Vector4f{0, 1, 0, 1}, Vector2f{0, 1}},
			  Vertex{Vector4f{-0.5f,  0.5f, -0.5f,  1},  Vector4f{0, 1, 0, 1}, Vector2f{0, 0}},
			  Vertex{Vector4f{0.5f,   0.5f, -0.5f,  1},  Vector4f{0, 1, 0, 1}, Vector2f{1, 0}},
			  Vertex{Vector4f{0.5f,  -0.5f, -0.5f,  1},  Vector4f{0, 1, 0, 1}, Vector2f{1, 1}},
			  Vertex{Vector4f{0.5f,  -0.5f, -0.5f,  1},  Vector4f{0, 0, 1, 1}, Vector2f{0, 1}},
			  Vertex{Vector4f{0.5f,   0.5f, -0.5f,  1},  Vector4f{0, 0, 1, 1}, Vector2f{0, 0}},
			  Vertex{Vector4f{0.5f,   0.5f,  0.5f,  1},  Vector4f{0, 0, 1, 1}, Vector2f{1, 0}},
			  Vertex{Vector4f{0.5f,  -0.5f,  0.5f,  1},  Vector4f{0, 0, 1, 1}, Vector2f{1, 1}},
			  Vertex{Vector4f{0.5f,   0.5f,  0.5f,  1},  Vector4f{1, 1, 0, 1}, Vector2f{0, 1}},
			  Vertex{Vector4f{0.5f,   0.5f, -0.5f,  1},  Vector4f{1, 1, 0, 1}, Vector2f{0, 0}},
			  Vertex{Vector4f{-0.5f,  0.5f, -0.5f,  1},  Vector4f{1, 1, 0, 1}, Vector2f{1, 0}},
			  Vertex{Vector4f{-0.5f,  0.5f,  0.5f,  1},  Vector4f{1, 1, 0, 1}, Vector2f{1, 1}},
			  Vertex{Vector4f{-0.5f, -0.5f,  0.5f,  1},  Vector4f{1, 0, 1, 1}, Vector2f{0, 1}},
			  Vertex{Vector4f{-0.5f, -0.5f, -0.5f , 1},  Vector4f{1, 0, 1, 1}, Vector2f{0, 0}},
			  Vertex{Vector4f{0.5f,  -0.5f, -0.5f,  1},  Vector4f{1, 0, 1, 1}, Vector2f{1, 0}},
			  Vertex{Vector4f{0.5f,  -0.5f,  0.5f,  1},  Vector4f{1, 0, 1, 1}, Vector2f{1, 1}},

	};

	data.myIndicies =
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


	return data;
}

ModelFactory::TempMeshData ModelFactory::GetUnitPiramid()
{
	TempMeshData data;

	auto tip = Vector4f(0.f, 0.5f, 0.f, 1.f);
	auto rbCorner = Vector4f(0.5f, -0.5f, 0.5f, 1.f);
	auto lbCorner = Vector4f(-0.5f, -0.5f, 0.5f, 1.f);
	auto lfCorner = Vector4f(-0.5f, -0.5f, -0.5f, 1.f);
	auto rfCorner = Vector4f(0.5f, -0.5f, -0.5f, 1.f);

	data.myVertecies.push_back(Vertex{ tip, Vector4f(1, 1, 1, 1), Vector2f(0, 0) }); //0
	data.myVertecies.push_back(Vertex{ lbCorner, Vector4f(1, 1, 1, 1), Vector2f(0, 1) });
	data.myVertecies.push_back(Vertex{ lfCorner, Vector4f(1, 1, 1, 1), Vector2f(1, 0) });

	data.myVertecies.push_back(Vertex{ tip, Vector4f(1, 1, 1, 1), Vector2f(0, 0) }); //3
	data.myVertecies.push_back(Vertex{ rbCorner, Vector4f(1, 1, 1, 1), Vector2f(0, 1) });
	data.myVertecies.push_back(Vertex{ rfCorner, Vector4f(1, 1, 1, 1), Vector2f(1, 0) });

	data.myVertecies.push_back(Vertex{ tip, Vector4f(1, 1, 1, 1), Vector2f(0, 0) }); //6
	data.myVertecies.push_back(Vertex{ rfCorner, Vector4f(1, 1, 1, 1), Vector2f(0, 1) });
	data.myVertecies.push_back(Vertex{ lfCorner, Vector4f(1, 1, 1, 1), Vector2f(1, 0) });

	data.myVertecies.push_back(Vertex{ tip, Vector4f(1, 1, 1, 1), Vector2f(0, 0) }); //9
	data.myVertecies.push_back(Vertex{ rbCorner, Vector4f(1, 1, 1, 1), Vector2f(0, 1) });
	data.myVertecies.push_back(Vertex{ lbCorner, Vector4f(1, 1, 1, 1), Vector2f(1, 0) });



	data.myVertecies.push_back(Vertex{ rfCorner, Vector4f(1, 1, 1, 1), Vector2f(1, 1) }); //12
	data.myVertecies.push_back(Vertex{ lfCorner, Vector4f(1, 1, 1, 1), Vector2f(1, 0) });
	data.myVertecies.push_back(Vertex{ lbCorner, Vector4f(1, 1, 1, 1), Vector2f(0, 0) });
	data.myVertecies.push_back(Vertex{ rbCorner, Vector4f(1, 1, 1, 1), Vector2f(0, 1) });

	data.myIndicies = {
		2,1,0, //R
		3,4,5, //B
		6,7,8, //F
		11,10,9, //L
		15,14,13,
		13,12,15
	};


	return data;
}

ModelFactory::TempMeshData ModelFactory::GetUnitIcoSphere()
{
	return TempMeshData();
}
