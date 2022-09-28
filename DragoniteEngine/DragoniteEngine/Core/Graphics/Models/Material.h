#pragma once
#include "../DXIncludes.h"
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
		friend class ModelFactory;
	public:
		Material(const char* aShader);
		Material(const VertexShaderDesc& aVSDesc, const PixelShaderDesc& aPSDesc);

		static Material defaultMaterial;
		std::string myTexture;
		Color myColor;
	private:
		VertexShaderDesc myVSInfo;
		PixelShaderDesc myPSInfo;


	};
}


