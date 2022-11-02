#pragma once

//Views
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView; 

//Buffers and/or Shaders
struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;

//Other
struct IDXGISwapChain;
struct ID3D11Texture2D;


namespace View
{
	using RenderTarget = ID3D11RenderTargetView;
	using RenderData = ID3D11ShaderResourceView;
	using DepthStencil = ID3D11DepthStencilView;
}
