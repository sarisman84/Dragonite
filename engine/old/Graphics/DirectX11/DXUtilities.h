#pragma once
#include "Core/CU/CommonData.h"
#include "Core/CU/Math/Vector2.h"
#include "DXIncludes.h"
#include <d3d11.h>
namespace Dragonite
{
#pragma region Custom Data Structures
	struct Data
	{
	public:
		template<typename TVal>
		Data(TVal* someData)
		{
			myData = (void*)someData;
			myDataSize = sizeof(TVal);
		}

		void* GetData() {
			return myData;
		}

		size_t GetDataSize() {
			return myDataSize;
		}

	private:
		void* myData;
		size_t myDataSize;
	};


#pragma endregion
#pragma region Custom Descriptors
	struct BufferDesc : D3D11_BUFFER_DESC
	{
		BufferDesc();

		template<typename TVal>
		BufferDesc(TVal* aValue, size_t someSize, const UINT someUsage, const UINT someBindFlags, const UINT someCPUAccess);
		BufferDesc(const void* aValue, size_t someSize, const UINT someUsage, const UINT someBindFlags, const UINT someCPUAccess);
		BufferDesc(size_t someSize, const UINT someUsage, const UINT someBindFlags, const UINT someCPUAccess);
		inline D3D11_SUBRESOURCE_DATA& GetSubResourceData() noexcept { return mySubResourceData; }
		inline const bool ContainsData() const noexcept { return mySubResourceData.pSysMem; }
	private:
		bool myContainsDataFlag;
		D3D11_SUBRESOURCE_DATA mySubResourceData;
	};


	struct TextureDesc : D3D11_TEXTURE2D_DESC
	{
		TextureDesc(const Vector2f& aResolution, DXGI_FORMAT aFormat, UINT someBindFlags);
	};


	struct Viewport : D3D11_VIEWPORT
	{
		Viewport(Vector2f aTopLeftCoords, Vector2f aResolution, const float aMinDepth = 0.0f, const float aMaxDepth = 1.0f);
	};



	struct NormalBlend : D3D11_BLEND_DESC
	{
		NormalBlend();
	};

	struct AlphaBlend : D3D11_BLEND_DESC
	{
		AlphaBlend();
	};

	struct AdditiveBlend : D3D11_BLEND_DESC
	{
		AdditiveBlend();
	};




	struct SamplerDesc : D3D11_SAMPLER_DESC
	{
		SamplerDesc(const D3D11_FILTER& aFilter, const D3D11_TEXTURE_ADDRESS_MODE& aTileMode, const Color aBorderColor, const float aMipLODBias = 0.0f, const UINT MaxAnisotropy = 1, const UINT aMinLOD = 0, const UINT aMaxLOD = D3D11_FLOAT32_MAX);
	};


#pragma endregion



	template<typename TVal>
	inline BufferDesc::BufferDesc(TVal* aValue, size_t someSize, const UINT someUsage, const UINT someBindFlags, const UINT someCPUAccess)
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

}