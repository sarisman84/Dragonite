#pragma once
#include <wtypes.h>
namespace Dragonite
{

	struct IContent
	{
		virtual void Init() = 0;
		virtual void* GetContent() = 0;
	};

	struct IDrawer
	{
		virtual ~IDrawer() = default;
		virtual void Init(HWND anInstance) = 0;
		virtual void Present(bool aVSyncState) = 0;
		virtual void SetRenderTarget(IContent* aTargetBuffer, IContent* aDepthBuffer) = 0;
		virtual void ClearRenderTarget(IContent* aTargetBuffer, IContent* aDepthBuffer) = 0;
	};




	
}