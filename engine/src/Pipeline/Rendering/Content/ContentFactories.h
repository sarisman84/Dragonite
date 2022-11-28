#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <typeinfo>

#include "ModelDef.h"
#include "MaterialDef.h"
#include "DirectX/DDSTextureLoader11.h"

#define STB_IMAGE_IMPLEMENTATION
//#include "../../External/STB/stb_image.h"
#include "STB/stb_image.h"

struct ID3D11Device;
struct D3D11_INPUT_ELEMENT_DESC;
namespace Dragonite
{
	class GraphicsEngine;


	enum class Primitive
	{
		Cube, Sphere, Plane
	};



	class ModelFactory
	{
	public:


		ModelFactory(GraphicsEngine* anEngine);

		template<typename Type>
		Model GetModel(const Type aType);
		Model GetModel(uint32_t anID);
		Model GetModel(const wchar_t* aPath);


		inline static ModelFactory* API() { return myInstance; }

	private:
		void InitPrimitiveCube();
		void InitPrimitiveSphere();
		void InitPrimitivePlane();


		ID3D11Buffer* CreateVertexBuffer(std::vector<Vertex> myVertecies);
		ID3D11Buffer* CreateIndexBuffer(std::vector<uint32_t> myIndicies);
	private:
		std::unordered_map<uint32_t, ID3D11Buffer*> myModelVertecies;
		std::unordered_map<uint32_t, std::pair<ID3D11Buffer*, uint32_t>> myModelIndicies;
	private:
		inline static ModelFactory* myInstance;
		GraphicsEngine* myEngine;
	};

	template<typename Type>
	inline Model ModelFactory::GetModel(const Type aType)
	{
		uint32_t typeID = typeid(aType).hash_code();
		uint32_t key = typeID + (uint32_t)aType;
		return GetModel(key);
	}


	enum class Materials : uint32_t
	{
		Unlit,
		Pbr,
		Other
	};


	class MaterialFactory
	{
	public:
		MaterialFactory(GraphicsEngine* anEngine);
		template<typename Type>
		Material GetMaterial(const Type aType, const wchar_t* anAlbedoTexture, const wchar_t* aNormalTexture = nullptr, const wchar_t* aMaterialTexture = nullptr);
		Material GetMaterial(const uint32_t myMaterial, const wchar_t* anAlbedoTexture, const wchar_t* aNormalTexture = nullptr, const wchar_t* aMaterialTexture = nullptr);

		inline static MaterialFactory* API() { return myInstance; }

	private:
		ID3D11VertexShader* CreateVertexShader(const wchar_t* aPath, std::string* someVertexData);
		ID3D11PixelShader* CreatePixelShader(const wchar_t* aPath);
		ID3D11InputLayout* CreateInputLayout(std::vector<D3D11_INPUT_ELEMENT_DESC> someDescriptions, std::string someVertexData);
		std::shared_ptr<Texture> LoadTexture(const wchar_t* aTexture);
	private:
		std::shared_ptr<Texture> LoadDDS(std::wstring aPath);
		std::shared_ptr<Texture> LoadPNG(std::wstring aPath);
	private:
		ID3D11Device* GetDevice();
	private:
		void InitPbr();
		void InitUnlit();
	private:
		std::unordered_map<std::wstring, std::shared_ptr<Texture>> myTextures;
		std::unordered_map<uint32_t, ID3D11InputLayout*> myInputLayouts;
		std::unordered_map<uint32_t, ID3D11VertexShader*> myVertexShaders;
		std::unordered_map<uint32_t, ID3D11PixelShader*> myPixelShaders;
	private:
		inline static MaterialFactory* myInstance;
		std::wstring myDefaultPath;
		GraphicsEngine* myEngine;
	};

	template<typename Type>
	inline Material MaterialFactory::GetMaterial(const Type aType, const wchar_t* anAlbedoTexture, const wchar_t* aNormalTexture, const wchar_t* aMaterialTexture)
	{
		uint32_t id = typeid(aType).hash_code();
		uint32_t key = id + (uint32_t)aType;

		return GetMaterial(key, anAlbedoTexture, aNormalTexture, aMaterialTexture);
	}

}