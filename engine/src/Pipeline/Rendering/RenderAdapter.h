#pragma once
#include <wtypes.h>
namespace Dragonite
{

	struct IContent
	{
		virtual ~IContent() = default;
		virtual void* GetContent() const = 0;
		virtual void** EditContent() = 0;
	};

	struct IDrawer
	{
		virtual ~IDrawer() = default;
		virtual void Init(HWND anInstance) = 0;
		virtual void Present(bool aVSyncState) = 0;
		virtual void SetRenderTarget(void* aTargetBuffer, void* aDepthBuffer) = 0;
		virtual void ClearRenderTarget(void* aTargetBuffer, void* aDepthBuffer) = 0;
	};




	
}