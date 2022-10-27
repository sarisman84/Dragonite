#include "DXUtilities.h"

Dragonite::BufferDesc::BufferDesc() : D3D11_BUFFER_DESC()
{
}

Dragonite::BufferDesc::BufferDesc(const void* aValue, size_t someSize, const UINT someUsage, const UINT someBindFlags, const UINT someCPUAccess)
{
	StructureByteStride = 0;
	MiscFlags = 0;
	ByteWidth = someSize;
	Usage = (D3D11_USAGE)someUsage;
	BindFlags = someBindFlags;
	CPUAccessFlags = someCPUAccess;

	mySubResourceData = { 0 };

	mySubResourceData.pSysMem = aValue;

	myContainsDataFlag = aValue;
}

Dragonite::BufferDesc::BufferDesc(size_t someSize, const UINT someUsage, const UINT someBindFlags, const UINT someCPUAccess)
{
	StructureByteStride = 0;
	MiscFlags = 0;
	ByteWidth = someSize;
	Usage = (D3D11_USAGE)someUsage;
	BindFlags = someBindFlags;
	CPUAccessFlags = someCPUAccess;

	myContainsDataFlag = false;
	mySubResourceData.pSysMem = nullptr;
}



Dragonite::TextureDesc::TextureDesc(const Vector2f& aResolution, DXGI_FORMAT aFormat, UINT someBindFlags)
{
	Width = static_cast<UINT>(aResolution.x);
	Height = static_cast<UINT>(aResolution.y);
	Format = aFormat;
	SampleDesc.Count = 1;
	BindFlags = someBindFlags;


}

Dragonite::NormalBlend::NormalBlend()
{
	IndependentBlendEnable = false;
	AlphaToCoverageEnable = false;

	RenderTarget[0].BlendEnable = FALSE;
	RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}

Dragonite::AlphaBlend::AlphaBlend()
{
	IndependentBlendEnable = false;
	AlphaToCoverageEnable = false;


	RenderTarget[0].BlendEnable = TRUE;
	RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}

Dragonite::AdditiveBlend::AdditiveBlend()
{
	IndependentBlendEnable = false;
	AlphaToCoverageEnable = false;

	RenderTarget[0].BlendEnable = TRUE;
	RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}

Dragonite::SamplerDesc::SamplerDesc(const D3D11_FILTER& aFilter, const D3D11_TEXTURE_ADDRESS_MODE& aTileMode, const Color aBorderColor, const float aMipLODBias, const UINT aMaxAnisotropy, const UINT aMinLOD, const UINT aMaxLOD)
{
	Filter = aFilter;
	AddressU = aTileMode;
	AddressV = aTileMode;
	AddressW = aTileMode;
	MipLODBias = aMipLODBias;
	MaxAnisotropy = aMaxAnisotropy;
	ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	BorderColor[0] = aBorderColor.r;
	BorderColor[1] = aBorderColor.g;
	BorderColor[2] = aBorderColor.b;
	BorderColor[3] = aBorderColor.a;
	MinLOD = aMinLOD;
	MaxLOD = aMaxLOD;

}

Dragonite::Viewport::Viewport(Vector2f aTopLeftCoords, Vector2f aResolution, const float aMinDepth, const float aMaxDepth)
{
	TopLeftX = aTopLeftCoords.x;
	TopLeftY = aTopLeftCoords.y;

	Width = aResolution.x;
	Height = aResolution.y;

	MinDepth = aMinDepth;
	MaxDepth = aMaxDepth;
}
