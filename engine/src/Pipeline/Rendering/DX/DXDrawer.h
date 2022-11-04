#pragma once
#include "Pipeline/Rendering/RenderAdapter.h"
#include "DXIncludes.h"


struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Dragonite
{
	struct RTContent : IContent
	{
		RTContent();
		~RTContent() override;
		void* GetContent() const override;
		void** EditContent() override;
	private:
		ID3D11RenderTargetView* myRenderTargetView;
	};

	struct DSContent : IContent
	{
		DSContent();
		~DSContent() override;
		void* GetContent() const override;
		void** EditContent() override;
	private:
		ID3D11DepthStencilView* myDepthStencilView;
	};


	struct DXDrawer : IDrawer
	{
		DXDrawer();
		~DXDrawer() override;
		void Present(bool aVSyncState) override;
		void Init(HWND anInstance) override;

		void SetRenderTarget(void* aTargetBuffer, void* aDepthBuffer = nullptr) override;
		void ClearRenderTarget(void* aTargetBuffer, void* aDepthBuffer = nullptr) override;


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