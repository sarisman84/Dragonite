#pragma once
#include "Core/Graphics/DXIncludes.h"
#include "Core/CU/Math/Vector2.h"

#include <vector>
#include <memory>
#include "Core/Editor/EditorSettings.h"



#pragma warning(disable: 4244)

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
		UINT myCPUAccessFlags;
		UINT myMiscFlags;

		

	};


	class GraphicsPipeline;

	class RenderTarget
	{
	protected:
		RenderView myRenderView;
		ShaderResourceV myResourceView;
		GraphicsPipeline* myPipeline;
		void InternalInit(const RenderTargetDesc& aDesc);

	public:
		RenderTarget();
		RenderTarget(GraphicsPipeline* aPipeline, const RenderTargetDesc& aDesc);
		~RenderTarget();
		
		virtual const bool OnRender() = 0;
		const bool RenderThisTarget(DepthStencil aDepthStenci = nullptr);
	};
}


