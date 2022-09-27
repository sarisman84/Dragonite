#pragma once
#include <wrl/client.h>
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

namespace Dragonite
{
	typedef ComPtr<ID3D11Device> Device;
	typedef	ComPtr<ID3D11DeviceContext> DeviceContext;
	typedef ComPtr<IDXGISwapChain> SwapChain;
	typedef ComPtr<ID3D11RenderTargetView> TargetTexture;
	typedef ComPtr<ID3D11Buffer> DataBuffer;
	typedef ComPtr<ID3D11VertexShader> VertexShader;
	typedef ComPtr<ID3D11PixelShader> PixelShader;
	typedef ComPtr<ID3D11InputLayout> InputLayout;
	typedef ComPtr<ID3D11ShaderResourceView> ShaderResourceV;
}