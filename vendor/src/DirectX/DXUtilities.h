#pragma once
#include <wtypes.h>
#include <string>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
struct ID3D11PixelShader;
struct ID3D11VertexShader;

struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;


namespace DirectX
{
	struct RenderTargetDesc
	{
		float myWidth;
		float myHeight;

		unsigned int myMipLevels;
		unsigned int myArraySize;

		UINT myFormat;
		UINT myQuality;
		UINT myCount;
		UINT myUseage;
		UINT myCPUAccessFlags;
		UINT myMiscFlags;
	};



	template<typename Data>
	HRESULT CreateConstantBuffer(ID3D11Device* aDevice, ID3D11Buffer* aBuffer);
	HRESULT CreateConstantBuffer(ID3D11Device* aDevice, const size_t someDataSize, ID3D11Buffer* aBuffer);

	HRESULT CreateVertexShader(ID3D11Device* aDevice, const char* aPath, ID3D11VertexShader* aVertexShader, std::string* someExtraData);
	HRESULT CreatePixelShader(ID3D11Device* aDevice, const char* aPath, ID3D11PixelShader* aPixelShader);

	HRESULT CreateRenderTarget(ID3D11Device* aDevice, const RenderTargetDesc& aDesc, ID3D11RenderTargetView** aRenderView, ID3D11ShaderResourceView** aResourceView);


	template<typename Data>
	HRESULT CreateConstantBuffer(ID3D11Device* aDevice, ID3D11Buffer* aBuffer)
	{
		return DirectX::CreateConstantBuffer(sizeof(Data), aBuffer);
	}

	void SetViewport(ID3D11DeviceContext* aContext, float* aNewViewport);


}