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

	struct DX
	{
		inline static ID3D11Device* devicePtr;
		inline static ID3D11DeviceContext* contextPtr;
	};
	


	HRESULT Begin(ID3D11DeviceContext* aContext);
	HRESULT Begin(ID3D11Device* aDevice, ID3D11DeviceContext* aContext);
	HRESULT Begin(ID3D11Device* aDevice);
	void End();

	template<typename Data>
	HRESULT CreateConstantBuffer(const Data& someData, ID3D11Buffer* aBuffer);
	HRESULT CreateConstantBuffer(void* someData, const size_t someDataSize, ID3D11Buffer* aBuffer);

	HRESULT CreateVertexShader(const char* aPath, ID3D11VertexShader* aVertexShader, std::string* someExtraData);
	HRESULT CreatePixelShader(const char* aPath, ID3D11PixelShader* aPixelShader);

	HRESULT CreateRenderTarget(const RenderTargetDesc& aDesc, ID3D11RenderTargetView** aRenderView, ID3D11ShaderResourceView** aResourceView);

	template<typename Data>
	HRESULT CreateConstantBuffer(const Data& someData, ID3D11Buffer* aBuffer)
	{
		return DirectX::CreateConstantBuffer(&someData, sizeof(Data), aBuffer);
	}

	void SetViewport(float* aNewViewport);


}