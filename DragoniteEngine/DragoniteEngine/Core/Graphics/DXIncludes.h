#pragma once

#include <wrl/client.h>
#include <dxgicommon.h>
#include "Core/CU/Math/Vector2.h"

using Microsoft::WRL::ComPtr;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;
struct ID3D11Resource;
struct ID3D11SamplerState;
struct ID3D11DepthStencilView;
struct ID3D11RasterizerState;

enum D3D11_USAGE;
enum D3D11_INPUT_CLASSIFICATION;
enum D3D11_BIND_FLAG;
enum D3D11_CPU_ACCESS_FLAG;
enum DXGI_FORMAT;

namespace Dragonite
{
	typedef ComPtr<ID3D11Device> Device;
	typedef	ComPtr<ID3D11DeviceContext> DeviceContext;
	typedef ComPtr<IDXGISwapChain> SwapChain;
	typedef ComPtr<ID3D11RenderTargetView> RenderView;
	typedef ComPtr<ID3D11Buffer> DataBuffer;
	typedef ComPtr<ID3D11VertexShader> VertexShader;
	typedef ComPtr<ID3D11PixelShader> PixelShader;
	typedef ComPtr<ID3D11InputLayout> InputLayout;
	typedef ComPtr<ID3D11ShaderResourceView> ShaderResourceV;
	typedef ComPtr<ID3D11Texture2D> DXTexture2D;
	typedef ComPtr<ID3D11SamplerState> TextureSampler;
	typedef ComPtr<ID3D11DepthStencilView> DepthStencil;
	typedef ComPtr<ID3D11Resource> DXResource;
	typedef ComPtr<ID3D11RasterizerState> RasterizerState;

	struct DataBufferDesc
	{
		template<typename T>
		DataBufferDesc(T* someData, size_t aSize, D3D11_USAGE aUsage, D3D11_BIND_FLAG aBindFlag) {
			myData = (void*)someData;
			mySize = aSize;
			myUsage = aUsage;
			myBindFlag = aBindFlag;
			myCPUFlags = 0;
		}


		DataBufferDesc(size_t aSize, D3D11_USAGE aUsage, D3D11_BIND_FLAG aBindFlag) : myCPUFlags(0) {
			myData = nullptr;
			mySize = aSize;
			myUsage = aUsage;
			myBindFlag = aBindFlag;
			myCPUFlags = 0;
		}


		DataBufferDesc(size_t aSize, D3D11_USAGE aUsage, D3D11_BIND_FLAG aBindFlag, D3D11_CPU_ACCESS_FLAG someCPUFlags) {
			myData = nullptr;
			mySize = aSize;
			myUsage = aUsage;
			myBindFlag = aBindFlag;
			myCPUFlags = someCPUFlags;
		}

		void* myData = nullptr;
		size_t mySize;
		D3D11_USAGE myUsage;
		D3D11_BIND_FLAG myBindFlag;
		UINT myCPUFlags;
	};

	struct TextureBufferDesc
	{
		TextureBufferDesc(Vector2f aResolution, UINT aFormat, UINT aBindFlag)
		{
			myTextureRes = Vector2ui(
				static_cast<unsigned int>(aResolution.x),
				static_cast<unsigned int>(aResolution.y)
			);
			myArraySize = 1;
			myFormat = aFormat;
			myBindFlags = aBindFlag;
		}

		TextureBufferDesc(Vector2ui aResolution, UINT aFormat, UINT aBindFlag)
		{
			myTextureRes = aResolution;
			myArraySize = 1;
			myFormat = aFormat;
			myBindFlags = aBindFlag;
		}

		Vector2ui myTextureRes;
		unsigned int myArraySize;
		UINT myFormat;
		UINT myBindFlags;
		DXGI_SAMPLE_DESC mySamplerDesc;

	};
}