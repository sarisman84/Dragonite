#include "DXUtilities.h"
#include <d3d11.h>
#include <fstream>

//ID3D11Device* DirectX::devicePtr;
//ID3D11DeviceContext* DirectX::contextPtr;



HRESULT DirectX::CreateConstantBuffer(ID3D11Device* aDevice, const size_t someDataSize, ID3D11Buffer* aBuffer)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = someDataSize;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;


	return aDevice->CreateBuffer(&desc, nullptr, &aBuffer);
}

typedef std::istreambuf_iterator<char> ShaderIT;

HRESULT DirectX::CreateVertexShader(ID3D11Device* aDevice, const char* aPath, ID3D11VertexShader* aVertexShader, std::string* someExtraData)
{
	std::ifstream file(aPath, std::ios::binary);

	if (!file || file.fail())
		return E_ACCESSDENIED;

	(*someExtraData) = { ShaderIT(file),ShaderIT() };
	file.close();

	return aDevice->CreateVertexShader(someExtraData->data(), someExtraData->size(), nullptr, &aVertexShader);
}

HRESULT DirectX::CreatePixelShader(ID3D11Device* aDevice, const char* aPath, ID3D11PixelShader* aPixelShader)
{
	std::ifstream file(aPath, std::ios::binary);

	if (!file || file.fail())
		return E_ACCESSDENIED;

	std::string data = { ShaderIT(file),ShaderIT() };
	file.close();

	return aDevice->CreatePixelShader(data.data(), data.size(), nullptr, &aPixelShader);
}

HRESULT DirectX::CreateRenderTarget(ID3D11Device* aDevice, const RenderTargetDesc& aDesc, ID3D11RenderTargetView** aRenderView, ID3D11ShaderResourceView** aResourceView)
{
	D3D11_TEXTURE2D_DESC desc = { 0 };

	desc.Width = aDesc.myWidth;
	desc.Height = aDesc.myHeight;

	desc.MipLevels = aDesc.myMipLevels;
	desc.ArraySize = aDesc.myArraySize;

	desc.Format = (DXGI_FORMAT)aDesc.myFormat;
	desc.SampleDesc.Count = aDesc.myCount;
	desc.SampleDesc.Quality = aDesc.myQuality;
	desc.Usage = (D3D11_USAGE)aDesc.myUseage;
	desc.BindFlags = aResourceView ? D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE : D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = aDesc.myCPUAccessFlags;
	desc.MiscFlags = aDesc.myMiscFlags;

	ID3D11Texture2D* texture;

	HRESULT result = aDevice->CreateTexture2D(&desc, nullptr, &texture);
	if (FAILED(result)) return result;

	if (aResourceView)
	{
		result = aDevice->CreateShaderResourceView(texture, nullptr, aResourceView);
		if (FAILED(result)) return result;

	}

	result = aDevice->CreateRenderTargetView(texture, nullptr, aRenderView);
	if (FAILED(result)) return result;


	texture->Release();
	return S_OK;
}

void DirectX::SetViewport(ID3D11DeviceContext* aContext, float* aNewViewport)
{
	D3D11_VIEWPORT newViewport;
	newViewport.TopLeftX = 0;
	newViewport.TopLeftY = 0;
	newViewport.Width = aNewViewport[0];
	newViewport.Height = aNewViewport[1];
	newViewport.MinDepth = 0;
	newViewport.MaxDepth = 1;

	aContext->RSSetViewports(1, &newViewport);
}
