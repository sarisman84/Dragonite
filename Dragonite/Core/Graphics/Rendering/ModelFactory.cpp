#include "ModelFactory.h"
#include "System.h"
#include <d3d11.h>  
#include <fstream>
#include <iostream>
#include "Utilities/Math/Noise.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Utilities/STB/ImageImport.h"

ModelFactory::ModelFactory(Engine::System* aSystem)
{
	mySystem = aSystem;
	myModelTypes[UNITCUBE] = InitializeModelOfType({ GetUnitCube() });
	myModelTypes[UNITPYRAMID] = InitializeModelOfType({ GetUnitPiramid() });
	myModelTypes[UNITICOSPHERE] = InitializeModelOfType({ GetUnitIcoSphere() });
	myModelTypes[GEN_TERRAIN] = InitializeModelOfType({ GetProcedualTerrain() });
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
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	return aDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), someVertexData.data(), someVertexData.size(), &aLayout);
}

HRESULT ModelFactory::LoadTexture(ID3D11Device* aDevice, Texture& aTexture, const char* aTexturePath, const int aSlot)
{

	int width, height, channels;
	unsigned char* img = stbi_load(aTexturePath, &width, &height, &channels, 0);

	if (!img)
	{
		std::cout << "[ERROR]<ModelFactory/LoadTexture>: STB Reported '" << stbi_failure_reason() << "'" << std::endl;
		return E_INVALIDARG;
	}

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

std::vector<ModelFactory::TempMeshData> ModelFactory::GetUnitCube()
{
	TempMeshData cube;


	cube.myVertecies = {
		{{-0.5f,  -0.5f,  -0.5f}, {1, 0, 0, 1},	UnitVector3::left<float>,  {1, 1}},
		{{-0.5f,  -0.5f,  0.5f},  {1, 0, 0, 1},  UnitVector3::left<float>, {1, 0}},
		{{-0.5f,  0.5f,   0.5f},  {1, 0, 0, 1},  UnitVector3::left<float>, {0, 0}},
		{{-0.5f,  0.5f,  -0.5f},  {1, 0, 0, 1},  UnitVector3::left<float>, {0, 1}},
		{{0.5f,  -0.5f,  -0.5f}, {1, 0, 0, 1},UnitVector3::right<float>,  {1, 1}},
		{{0.5f,  -0.5f,  0.5f},  {1, 0, 0, 1}, UnitVector3::right<float>, {1, 0}},
		{{0.5f,  0.5f,   0.5f},  {1, 0, 0, 1}, UnitVector3::right<float>, {0, 0}},
		{{0.5f,  0.5f,  -0.5f},  {1, 0, 0, 1}, UnitVector3::right<float>, {0, 1}},
		{{-0.5f,  -0.5f, 0.5f},  {0, 0, 1, 1}, UnitVector3::forward<float>, {1, 1}},
		{{0.5f,  -0.5f,  0.5f},  {0, 0, 1, 1}, UnitVector3::forward<float>, {0, 1}},
		{{0.5f,  0.5f,   0.5f},  {0, 0, 1, 1}, UnitVector3::forward<float>, {0, 0}},
		{{-0.5f,  0.5f,  0.5f},  {0, 0, 1, 1}, UnitVector3::forward<float>, {1, 0}},
		{{-0.5f,  -0.5f,  -0.5f},  {0, 0, 1, 1},UnitVector3::backward<float>,  {1, 1}},
		{{0.5f,  -0.5f,   -0.5f},  {0, 0, 1, 1}, UnitVector3::backward<float>, {0, 1}},
		{{0.5f,  0.5f,    -0.5f},  {0, 0, 1, 1}, UnitVector3::backward<float>, {0, 0}},
		{{-0.5f,  0.5f,   -0.5f},  {0, 0, 1, 1}, UnitVector3::backward<float>, {1, 0}},
		{{-0.5f,  0.5f,  -0.5f},  {0, 1, 0, 1}, UnitVector3::up<float>, {1, 1}},
		{{0.5f,   0.5f,  -0.5f},  {0, 1, 0, 1}, UnitVector3::up<float>, {0, 1}},
		{{0.5f,   0.5f,   0.5f},  {0, 1, 0, 1}, UnitVector3::up<float>, {0, 0}},
		{{-0.5f,  0.5f,   0.5f},  {0, 1, 0, 1}, UnitVector3::up<float>, {1, 0}},
		{{-0.5f, -0.5f,   -0.5f},  {0, 1, 0, 1}, UnitVector3::down<float>, {1, 1}},
		{{0.5f,  -0.5f,    -0.5f}, {0, 1, 0, 1}, UnitVector3::down<float>, {0, 1}},
		{{0.5f,  -0.5f,    0.5f},  {0, 1, 0, 1}, UnitVector3::down<float>, {0, 0}},
		{{-0.5f, -0.5f,    0.5f},  {0, 1, 0, 1}, UnitVector3::down<float>, {1, 0}},
	};

	cube.myIndicies = {
		0,1,3,
		1,2,3,
		7,5,4,
		7,6,5,
		8,9,11,
		9,10,11,
		15,13,12,
		15,14,13,
		19,17,16,
		19,18,17,
		20,21,23,
		21,22,23,
	};









	return { cube };
}

ModelFactory::TempMeshData ModelFactory::GetUnitPiramid()
{
	TempMeshData data;

	auto tip = Vector3f(0.f, 0.5f, 0.f);
	auto rbCorner = Vector3f(0.5f, -0.5f, 0.5f);
	auto lbCorner = Vector3f(-0.5f, -0.5f, 0.5f);
	auto lfCorner = Vector3f(-0.5f, -0.5f, -0.5f);
	auto rfCorner = Vector3f(0.5f, -0.5f, -0.5f);

	data.myVertecies.push_back(Vertex{ tip, Vector4f(1, 1, 1, 1),tip, Vector2f(0, 0) }); //0
	data.myVertecies.push_back(Vertex{ lbCorner, Vector4f(1, 1, 1, 1),lbCorner, Vector2f(0, 1) });
	data.myVertecies.push_back(Vertex{ lfCorner, Vector4f(1, 1, 1, 1),lfCorner, Vector2f(1, 0) });

	data.myVertecies.push_back(Vertex{ tip, Vector4f(1, 1, 1, 1),tip, Vector2f(0, 0) }); //3
	data.myVertecies.push_back(Vertex{ rbCorner, Vector4f(1, 1, 1, 1),rbCorner, Vector2f(0, 1) });
	data.myVertecies.push_back(Vertex{ rfCorner, Vector4f(1, 1, 1, 1),rfCorner, Vector2f(1, 0) });

	data.myVertecies.push_back(Vertex{ tip, Vector4f(1, 1, 1, 1),tip, Vector2f(0, 0) }); //6
	data.myVertecies.push_back(Vertex{ rfCorner, Vector4f(1, 1, 1, 1),rfCorner, Vector2f(0, 1) });
	data.myVertecies.push_back(Vertex{ lfCorner, Vector4f(1, 1, 1, 1),lfCorner, Vector2f(1, 0) });

	data.myVertecies.push_back(Vertex{ tip, Vector4f(1, 1, 1, 1),tip, Vector2f(0, 0) }); //9
	data.myVertecies.push_back(Vertex{ rbCorner, Vector4f(1, 1, 1, 1),rbCorner, Vector2f(0, 1) });
	data.myVertecies.push_back(Vertex{ lbCorner, Vector4f(1, 1, 1, 1),lbCorner, Vector2f(1, 0) });



	data.myVertecies.push_back(Vertex{ rfCorner, Vector4f(1, 1, 1, 1),rfCorner, Vector2f(1, 1) }); //12
	data.myVertecies.push_back(Vertex{ lfCorner, Vector4f(1, 1, 1, 1),lfCorner, Vector2f(1, 0) });
	data.myVertecies.push_back(Vertex{ lbCorner, Vector4f(1, 1, 1, 1),lbCorner, Vector2f(0, 0) });
	data.myVertecies.push_back(Vertex{ rbCorner, Vector4f(1, 1, 1, 1),rbCorner, Vector2f(0, 1) });

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

std::vector<float> ModelFactory::GenerateNoise()
{
	int resolution = 16;
	std::vector<float> val;
	for (size_t i = 0; i < resolution * resolution; i++)
	{
		val.push_back(RandomFloat());
	}


	size_t upsampleAmm = 6;

	for (size_t i = 1; i <= upsampleAmm; i++)
	{
		AddNoise(val, powf(0.25f, i));
		val = Upsample2X(val, resolution);
		resolution *= 2;
	}

	return val;
}

std::vector<ModelFactory::TempMeshData> ModelFactory::GetProcedualTerrain()
{
	auto noiseMap = GenerateNoise();
	//Grid Size (Width, Length)
	TempMeshData data;


	int width = sqrt(static_cast<int>(noiseMap.size()));
	int height = sqrt(static_cast<int>(noiseMap.size()));

	//Generate vertex points
	for (int y = -(height / 2); y < height / 2; ++y)
	{
		for (int x = -(width / 2); x < width / 2; ++x)
		{
			int index = ((x + (width / 2)) + width * (y + (height / 2)));
			float uvX = (static_cast<float>(x) + (static_cast<float>(width) / 2.f)) / static_cast<float>(noiseMap.size());
			float uvY = (static_cast<float>(y) + (static_cast<float>(height) / 2.f)) / static_cast<float>(noiseMap.size());
			Vertex vertex;
			vertex.myPosition = Vector3f{ static_cast<float>(x), noiseMap[index] * 50.f, static_cast<float>(y) };
			vertex.myUV = { uvX ,uvY };
			vertex.myColor = { 1,1,1,1 };
			data.myVertecies.push_back(vertex);
		}
	}

	//Setup Normal and Indicies
	for (int y = 0; y < height - 1; y++)
	{
		for (int x = 0; x < width - 1; x++)
		{
			//Calculate indicies
			int nY = y + 1;
			int nX = x + 1;

			int i_a = x + width * y; // 0
			int i_b = x + width * nY; // 1
			int i_c = nX + width * nY; // 2
			int i_d = nX + width * y; // 3


			data.myIndicies.push_back(i_a);
			data.myIndicies.push_back(i_b);
			data.myIndicies.push_back(i_c);

			data.myIndicies.push_back(i_c);
			data.myIndicies.push_back(i_d);
			data.myIndicies.push_back(i_a);


			//Calculate normals
			auto vectorBetweenAandB = (data.myVertecies[i_b].myPosition - data.myVertecies[i_a].myPosition);
			auto vectorBetweenAandD = (data.myVertecies[i_d].myPosition - data.myVertecies[i_a].myPosition);

			auto normal = vectorBetweenAandB.Cross(vectorBetweenAandD);

			data.myVertecies[i_a].myNormal = normal.GetNormalized();

		}
	}



	return { data };
}
