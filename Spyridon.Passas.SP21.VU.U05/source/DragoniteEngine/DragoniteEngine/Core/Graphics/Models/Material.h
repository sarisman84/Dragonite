#pragma once
#include "Core/Graphics/DirectX11/DXIncludes.h"
#include <string>
#include <vector>
#include "../../CU/CommonData.h"


namespace Dragonite
{
	struct InputLayoutDesc
	{
		const char* myInputName;
		UINT myFormat;
		D3D11_INPUT_CLASSIFICATION myClassification;
	};

	struct VertexShaderDesc
	{
		const char* myVertexShader;
		std::vector<InputLayoutDesc> myInputLayout;
	};

	struct PixelShaderDesc
	{
		const char* myPixelShader;
	};


	struct Material
	{
		friend struct ShaderInstructions;
		friend class ModelFactory;
	public:
		Material(const char* aShader);
		Material(const VertexShaderDesc& aVSDesc, const PixelShaderDesc& aPSDesc, std::wstring aTexture, Color aColor);

		static Material defaultMaterial;
		std::wstring myTexture;
		Color myColor;
	private:
		VertexShaderDesc myVSInfo;
		PixelShaderDesc myPSInfo;


	};
}


