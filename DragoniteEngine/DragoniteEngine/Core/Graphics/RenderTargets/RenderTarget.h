#pragma once
#include "Core/Graphics/DXIncludes.h"
#include "Core/CU/Math/Vector2.h"
namespace Dragonite
{
	struct RenderTargetDesc
	{
		Vector2f myResolution;
		DXGI_FORMAT myFormat;
		DXGI_SAMPLE_DESC mySample;
		UINT myMipLevels;
		UINT myArraySize;
		UINT myUseage;
		UINT myBindFlags;
		UINT myCPUAccessFlags;
		UINT myMiscFlags;
	};



	struct RenderTarget
	{
		RenderView myRenderView;
		ShaderResourceV myResourceView;


		static RenderTarget* CreateTarget(Device aDeivice, const RenderTargetDesc& aDesc);
	};
}


