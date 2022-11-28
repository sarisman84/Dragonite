#include "ContentFactories.h"
#include "Pipeline/Rendering/GraphicsEngine.h"
#include "Pipeline/Rendering/DX/DXDrawer.h"

#include <d3d11.h>
#include <fstream>
#include <filesystem>


Dragonite::ModelFactory::ModelFactory(GraphicsEngine* anEngine) : myEngine(anEngine)
{
	InitPrimitiveCube();
	InitPrimitivePlane();
	InitPrimitiveSphere();

	myInstance = this;
}

Dragonite::Model Dragonite::ModelFactory::GetModel(uint32_t anID)
{
	return Model
	{
		myModelVertecies[anID],
		std::get<0>(myModelIndicies[anID]),
		std::get<1>(myModelIndicies[anID])
	};
}

Dragonite::Model Dragonite::ModelFactory::GetModel(const wchar_t* aPath)
{
	return Model();
}

void Dragonite::ModelFactory::InitPrimitiveCube()
{
	std::vector<Vertex> vertecies =
	{
		Vertex{Vector3f(-0.5f,-0.5f, -0.5f), Vector2f(0,0)}, //0 - Back
		Vertex{Vector3f(-0.5f, 0.5f, -0.5f), Vector2f(0,1)},
		Vertex{Vector3f(0.5f,  0.5f, -0.5f), Vector2f(1,1)},
		Vertex{Vector3f(0.5f, -0.5f, -0.5f), Vector2f(1,0)},

		Vertex{Vector3f(-0.5f, -0.5f,  0.5f), Vector2f(0,0)}, //4 - Front
		Vertex{Vector3f(-0.5f,  0.5f,  0.5f), Vector2f(0,1)},
		Vertex{Vector3f(0.5f,   0.5f,  0.5f), Vector2f(1,1)},
		Vertex{Vector3f(0.5f,  -0.5f,  0.5f), Vector2f(1,0)},

		Vertex{Vector3f(-0.5f, -0.5f,  -0.5f), Vector2f(0,0)}, //8 - Left
		Vertex{Vector3f(-0.5f,  0.5f,  -0.5f), Vector2f(0,1)},
		Vertex{Vector3f(-0.5f,  0.5f,   0.5f), Vector2f(1,1)},
		Vertex{Vector3f(-0.5f, -0.5f,   0.5f), Vector2f(1,0)},

		Vertex{Vector3f(0.5f,  -0.5f,  -0.5f), Vector2f(0,0)}, //12 - Right
		Vertex{Vector3f(0.5f,   0.5f,  -0.5f), Vector2f(0,1)},
		Vertex{Vector3f(0.5f,   0.5f,   0.5f), Vector2f(1,1)},
		Vertex{Vector3f(0.5f,  -0.5f,   0.5f), Vector2f(1,0)},

		Vertex{Vector3f(-0.5f, -0.5f, -0.5f), Vector2f(0,0)}, //16 - Down
		Vertex{Vector3f(-0.5f, -0.5f,  0.5f), Vector2f(0,1)},
		Vertex{Vector3f(0.5f, -0.5f,  0.5f), Vector2f(1,1)},
		Vertex{Vector3f(0.5f, -0.5f, -0.5f), Vector2f(1,0)},

		Vertex{Vector3f(-0.5f,  0.5f,  -0.5f), Vector2f(0,0)}, //20 - Up
		Vertex{Vector3f(-0.5f,  0.5f,   0.5f), Vector2f(0,1)},
		Vertex{Vector3f(0.5f,  0.5f,  0.5f), Vector2f(1,1)},
		Vertex{Vector3f(0.5f,  0.5f, -0.5f), Vector2f(1,0)},

	};
	std::vector<uint32_t> indicies =
	{
	 0,1,2,
	 0,2,3,

	 4,5,6,
	 4,6,7,

	 8,9,10,
	 8,10,11,

	 12,13,14,
	 12,14,15,

	 16,17,18,
	 16,18,19,

	 20,21,22,
	 20,22,23
	};


	uint32_t id = typeid(Primitive).hash_code();
	uint32_t key = id + (uint32_t)Primitive::Cube;

	myModelIndicies[key] = std::make_pair(CreateIndexBuffer(indicies), indicies.size());
	myModelVertecies[key] = CreateVertexBuffer(vertecies);

}

void Dragonite::ModelFactory::InitPrimitiveSphere()
{
}

void Dragonite::ModelFactory::InitPrimitivePlane()
{

}

ID3D11Buffer* Dragonite::ModelFactory::CreateVertexBuffer(std::vector<Vertex> myVertecies)
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = myVertecies.size() * sizeof(Vertex);
	desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = myVertecies.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	auto drawer = myEngine->GetDrawer<DXDrawer>();
	if (!drawer) return nullptr;

	auto device = drawer->Device();

	ID3D11Buffer* result = nullptr;

	HRESULT r = device->CreateBuffer(&desc, &data, &result);
	return result;
}
ID3D11Buffer* Dragonite::ModelFactory::CreateIndexBuffer(std::vector<uint32_t> myIndicies)
{
	D3D11_BUFFER_DESC desc = {};

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(uint32_t) * myIndicies.size();
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = myIndicies.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	auto drawer = myEngine->GetDrawer<DXDrawer>();
	if (!drawer) return nullptr;

	ID3D11Buffer* result = nullptr;
	auto device = drawer->Device();

	HRESULT r = device->CreateBuffer(&desc, &data, &result);

	return result;
}

Dragonite::MaterialFactory::MaterialFactory(GraphicsEngine* anEngine) : myEngine(anEngine)
{
	myDefaultPath = L"../assets/shaders/";

	InitPbr();
	InitUnlit();

	myInstance = this;
}

Dragonite::Material Dragonite::MaterialFactory::GetMaterial(const uint32_t someMaterialType, const wchar_t* anAlbedoTexture, const wchar_t* aNormalTexture, const wchar_t* aMaterialTexture)
{
	return Material
	{
	myInputLayouts[someMaterialType],
	myVertexShaders[someMaterialType],
	myPixelShaders[someMaterialType],
	LoadTexture(anAlbedoTexture),
	LoadTexture(aNormalTexture),
	LoadTexture(aMaterialTexture)
	};
}

Dragonite::Material Dragonite::MaterialFactory::GetMaterial(const Materials someMaterialType, const wchar_t* anAlbedoTexture, const wchar_t* aNormalTexture, const wchar_t* aMaterialTexture)
{
	return GetMaterial((uint32_t)someMaterialType, anAlbedoTexture, aNormalTexture, aMaterialTexture);
}

ID3D11VertexShader* Dragonite::MaterialFactory::CreateVertexShader(const wchar_t* aPath, std::string* someVertexData)
{
	std::ifstream vsFile(myDefaultPath + std::wstring(aPath), std::ios::binary);
	if (!vsFile || vsFile.fail()) return nullptr;

	static std::string data = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	someVertexData = &data;

	auto device = GetDevice();
	if (!device) return nullptr;

	ID3D11VertexShader* result = nullptr;

	HRESULT r = device->CreateVertexShader(data.data(), data.size(), nullptr, &result);

	return result;
}

ID3D11PixelShader* Dragonite::MaterialFactory::CreatePixelShader(const wchar_t* aPath)
{
	std::ifstream vsFile(myDefaultPath + std::wstring(aPath), std::ios::binary);
	if (!vsFile || vsFile.fail()) return nullptr;

	std::string data = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };

	auto device = GetDevice();
	if (!device) return nullptr;

	ID3D11PixelShader* result = nullptr;

	HRESULT r = device->CreatePixelShader(data.data(), data.size(), nullptr, &result);

	return result;
}

ID3D11InputLayout* Dragonite::MaterialFactory::CreateInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> someDescriptions, std::string someVertexData)
{
	ID3D11InputLayout* result = nullptr;

	auto drawer = myEngine->GetDrawer<DXDrawer>();
	if (!drawer) return nullptr;

	auto device = drawer->Device();
	HRESULT r = device->CreateInputLayout(someDescriptions.data(), someDescriptions.size(), someVertexData.data(), someVertexData.size(), &result);

	return result;
}

std::shared_ptr<Dragonite::Texture> Dragonite::MaterialFactory::LoadTexture(const wchar_t* aTexture, const bool anUseSRGB, const bool aGenerateMipMaps)
{
	if (aTexture == nullptr) return nullptr;
	std::wstring path(aTexture);

	if (!std::filesystem::directory_entry(path).exists()) return nullptr;

	if (myTextures.contains(path))
	{
		return myTextures[path];
	}

	std::shared_ptr<Texture> newTexture = std::make_shared<Texture>();
	newTexture->mySRGBFlag = anUseSRGB;
	newTexture->myGenerateMipMapFlag = aGenerateMipMaps;

	if (path.find(L".dds") != std::wstring::npos)
	{
		LoadDDS(newTexture, path);
	}
	else
		LoadPNG(newTexture, path);

	return LoadTexture(aTexture);

}

const bool Dragonite::MaterialFactory::LoadDDS(std::shared_ptr<Texture>& aTexture, std::wstring aPath)
{
	auto drawer = myEngine->GetDrawer<DXDrawer>();
	if (!drawer) return false;

	auto device = drawer->Device();


	ID3D11Texture2D* texture2D = {};
	D3D11_TEXTURE2D_DESC texture2DDesc = {};
	ID3D11Resource* textureResource = {};

	HRESULT r = DirectX::CreateDDSTextureFromFile(device, aPath.c_str(), &textureResource, &aTexture->myResourceView);
	if (FAILED(r))
		return false;

	textureResource->QueryInterface(IID_ID3D11Texture2D, (void**)&texture2DDesc);

	aTexture->myTextureSize = Vector2f(texture2DDesc.Width, texture2DDesc.Height);

	return true;
}

const bool Dragonite::MaterialFactory::LoadPNG(std::shared_ptr<Texture>& aTexture, std::wstring aPath)
{
	auto drawer = myEngine->GetDrawer<DXDrawer>();
	if (!drawer) return false;

	auto device = drawer->Device();




	const auto size = wcslen(aPath.c_str());
	char path[1000];
	size_t r;
	wcstombs_s(&r, path, aPath.c_str(), size);


	_internal::PixelData pixelData = _internal::LoadTextureBuffer(path);

	if (pixelData.myBuffer == nullptr) return false;

	aTexture->myTextureSize = pixelData.myResolution;


	aTexture->Init(device, pixelData.myBuffer);


	return true;

}

ID3D11Device* Dragonite::MaterialFactory::GetDevice()
{
	auto drawer = myEngine->GetDrawer<DXDrawer>();
	if (!drawer) return nullptr;
	return drawer->Device();
}

void Dragonite::MaterialFactory::InitPbr()
{
	uint32_t id = typeid(Materials).hash_code();
	uint32_t key = id + (uint32_t)Materials::Pbr;

	std::string vsData;
	myVertexShaders[key] = CreateVertexShader(L"Default_VS.cso", &vsData);
	myPixelShaders[key] = CreatePixelShader(L"PBR_PS.cso");

	std::vector<D3D11_INPUT_ELEMENT_DESC> descs =
	{
		D3D11_INPUT_ELEMENT_DESC{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC{"UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}

	};

	myInputLayouts[key] = CreateInputLayout(descs, vsData);

}

void Dragonite::MaterialFactory::InitUnlit()
{
	uint32_t id = typeid(Materials).hash_code();
	uint32_t key = id + (uint32_t)Materials::Unlit;

	std::string vsData;
	myVertexShaders[key] = CreateVertexShader(L"Default_VS.cso", &vsData);
	myPixelShaders[key] = CreatePixelShader(L"Unlit_PS.cso");

	std::vector<D3D11_INPUT_ELEMENT_DESC> descs =
	{
		D3D11_INPUT_ELEMENT_DESC{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		D3D11_INPUT_ELEMENT_DESC{"UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}

	};

	myInputLayouts[key] = CreateInputLayout(descs, vsData);
}
