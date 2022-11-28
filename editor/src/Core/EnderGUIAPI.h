#pragma once
#include "EditorAPI/EmberGUI.h"
#include "EditorAPI/Space/WindowSpace.h"

#include <memory>
#include <vector>


struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
struct IDXGISwapChain;





namespace Dragonite
{
	class EmberGUIAPI : public EmberGUI
	{
	public:
		EmberGUIAPI();
		virtual ~EmberGUIAPI() override;
		const bool Init(
			HWND anInstance,
			ID3D11Device* aDevice,
			ID3D11DeviceContext* aContext,
			IDXGISwapChain* aSwapChain,
			ID3D11RenderTargetView* aViewToRenderTo) override;
		void Update() override;
		void Shutdown() override;
		LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

		void AddEditor(ember::EmberWindow* aNewWindow) override;
		ImGuiContext* GetIMGUIContext() override;
		EmVec2 GetViewportResolution() override;
		ID3D11RenderTargetView*& GetBackBuffer() override;
		ID3D11Device*& GetDevice() override;
		ID3D11DeviceContext*& GetContext() override;
	private:


	private:
		ember::EmberWindow* myRootContext;
		HWND myWindowsInstance;
		ID3D11Device* myDevice;
		ID3D11DeviceContext* myDeviceContext;
		ID3D11RenderTargetView* myLocalBackBuffer;

		IDXGISwapChain* mySwapChain;
		ImGuiContext* myImguiContext;
	};
}


#define DLLEXPORT _declspec(dllexport)
extern "C"
{
	DLLEXPORT EmberGUI* ImportEmberGUI()
	{
		return new Dragonite::EmberGUIAPI();
	}
}


