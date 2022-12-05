#pragma once
#include <wtypes.h>
namespace Dragonite
{

	struct IContent
	{
		virtual ~IContent() = default;
	public:
		template<typename T>
		inline T* As()
		{
			return (T*)(this);
		}
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