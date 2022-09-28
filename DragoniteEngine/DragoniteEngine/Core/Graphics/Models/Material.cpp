#include "Material.h"
#include <d3d11.h>

Dragonite::Material Dragonite::Material::defaultMaterial =
Dragonite::Material(
	{
		"Unlit",
		{
			{"POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_INPUT_PER_VERTEX_DATA },
			{"TEXCOORD", DXGI_FORMAT_R32G32_FLOAT, D3D11_INPUT_PER_VERTEX_DATA }
		}
	},
	{
		"Unlit"
	}
);


Dragonite::Material::Material(const char* aShader)
{
	myVSInfo.myVertexShader = aShader;
	myVSInfo.myInputLayout.push_back({ "POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_INPUT_PER_VERTEX_DATA });
	myVSInfo.myInputLayout.push_back({ "TEXCOORD", DXGI_FORMAT_R32G32_FLOAT, D3D11_INPUT_PER_VERTEX_DATA });

	myPSInfo.myPixelShader = aShader;
}

Dragonite::Material::Material(const VertexShaderDesc& aVSDesc, const PixelShaderDesc& aPSDesc)
{
	myVSInfo = aVSDesc;
	myPSInfo = aPSDesc;
}
