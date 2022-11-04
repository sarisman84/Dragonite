#include "ShaderFactory.h"
#include "DirectX/DXUtilities.h"
#include "Pipeline/Rendering/GraphicsEngine.h"
#include "Pipeline/Rendering/DX/DXDrawer.h"

#include <string>
#include <filesystem>
#include <d3d11.h>
#include <cassert>

Dragonite::ShaderFactory::ShaderFactory(GraphicsEngine* const aGraphicsEngine)
{
	myGraphicsEngine = aGraphicsEngine;
}

void Dragonite::ShaderFactory::Init()
{
}


void Dragonite::ShaderFactory::AddRecipe(const ShaderRecipe& aNewRecipe)
{
	auto dxDrawer = myGraphicsEngine->GetDrawer<DXDrawer>();

	myVertexShaders.emplace_back();
	myPixelShaders.emplace_back();
	myInputLayouts.emplace_back();

	std::string vertexData;
	assert(SUCCEEDED(DirectX::CreateVertexShader(dxDrawer->Device(), aNewRecipe.myVSPath, myVertexShaders.back(), &vertexData)) && "Failed to create vertex shader");
	assert(SUCCEEDED(DirectX::CreatePixelShader(dxDrawer->Device(), aNewRecipe.myPSPath, myPixelShaders.back())) && "Failed to create pixel shader");


	assert(SUCCEEDED(dxDrawer->Device()->CreateInputLayout(aNewRecipe.myInputDescs, aNewRecipe.myInputDescsSize, vertexData.data(), vertexData.size(), &myInputLayouts.back())) && "Failed to create input layout");

}

