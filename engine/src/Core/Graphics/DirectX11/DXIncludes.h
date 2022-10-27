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
struct ID3D11DepthStencilState;
struct ID3D11BlendState;
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
	typedef ComPtr<ID3D11BlendState> BlendState;
	typedef ComPtr<ID3D11DepthStencilState> DepthStencilState;


	typedef ID3D11Texture2D DXTexture2DRaw;
	typedef ID3D11Buffer DataBufferRaw;
	typedef ID3D11DepthStencilView DepthStencilRaw;
	typedef ID3D11RenderTargetView RenderViewRaw;
	typedef ID3D11ShaderResourceView ShaderResourceVRaw;
	typedef ID3D11Resource DXResourceRaw;
}