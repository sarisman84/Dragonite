#pragma once
#include "entt/single_include/entt/entt.hpp"
#include "Core/CU/Math/Matrix4x4.hpp"

#include <cstdint>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

struct ID3D11DeviceContext;

namespace Dragonite
{
	struct DrawData
	{
		uint32_t myDrawType;

		//Shader ids
		uint32_t myInputLayout;
		uint32_t myVertexShader;
		uint32_t myPixelShader;

		//Model ID
		uint32_t myModelID;

		//Entity ref
		Matrix4x4f myModelMatrix;

	};
}