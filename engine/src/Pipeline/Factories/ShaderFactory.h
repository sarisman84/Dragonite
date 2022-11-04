#pragma once
#include "BaseFactory.h"
#include <vector>

#include "Recipes/ShaderRecipe.h"


struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;



namespace Dragonite
{
	

	class GraphicsEngine;

	class ShaderFactory : public BaseFactory
	{
	public:
		ShaderFactory(GraphicsEngine* const aGraphicsEngine);

		void Init() override;


		void AddRecipe(const ShaderRecipe& aNewRecipe);


		inline ID3D11InputLayout* GetILOfID(const uint32_t anID)  noexcept { return myInputLayouts[anID]; }
		inline ID3D11VertexShader* GetVSOfID(const uint32_t anID) noexcept { return myVertexShaders[anID]; }
		inline ID3D11PixelShader* GetPSOfID(const uint32_t anID) noexcept { return myPixelShaders[anID]; }


	private:
		std::vector<ID3D11VertexShader*> myVertexShaders;
		std::vector<ID3D11PixelShader*> myPixelShaders;
		std::vector<ID3D11InputLayout*> myInputLayouts;

		GraphicsEngine* myGraphicsEngine;

		const char* myShaderDirectory;

	};
}