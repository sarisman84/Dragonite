#pragma once

struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;

namespace Dragonite
{
	struct MaterialData
	{
		ID3D11InputLayout* myInputLayout;
		ID3D11VertexShader* myVertexShader;
		ID3D11PixelShader* myPixelShader;
	};
}