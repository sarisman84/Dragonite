#pragma once
#include "Core/Graphics/DirectX11/DXIncludes.h"
#include "Core/CU/Math/Vector2.h"

#include <vector>
#include <memory>
//#include "Core/Editor/EditorSettings.h"



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


	class GraphicalInterface;

	class RenderTarget
	{
	protected:
		RenderView myRenderView;
		ShaderResourceV myResourceView;
		GraphicalInterface* myPipeline;
		void InternalInit(const RenderTargetDesc& aDesc);
		virtual void Render() = 0;
	public:
		RenderTarget();
		RenderTarget(GraphicalInterface* aPipeline, const RenderTargetDesc& aDesc);
	};
}


