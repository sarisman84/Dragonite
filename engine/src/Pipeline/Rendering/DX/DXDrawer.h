#pragma once
#include "Pipeline/Rendering/RenderAdapter.h"
#include "DXIncludes.h"


struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Dragonite
{
	struct DirectXDrawer : IDrawer
	{
		DirectXDrawer();
		~DirectXDrawer() override;
		void Present(bool aVSyncState) override;
		void Init(HWND anInstance) override;
		void SetRenderTarget(IContent* aTargetBuffer, IContent* aDepthBuffer) override;
		void ClearRenderTarget(IContent* aTargetBuffer, IContent* aDepthBuffer) override;

		inline IDXGISwapChain* SwapChain() noexcept { return mySwapChain; }
		inline ID3D11Device* Device() noexcept { return myDevice; }
		inline ID3D11DeviceContext* Context() noexcept { return myContext; }

	private:
		IDXGISwapChain* mySwapChain;
		ID3D11Device* myDevice;
		ID3D11DeviceContext* myContext;

		// Inherited via IDrawer
	};
}