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
		const bool Init(HWND anInstance, ID3D11Device* aDevice, ID3D11DeviceContext* aContext, IDXGISwapChain* aSwapChain) override;
		void Update(const float aDt, ID3D11RenderTargetView* aTargetView) override;
		void Shutdown() override;
		LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

		void AddSpace(GUISpace* aNewSpace) override;


	private:
		ID3D11Device* GetDevice() override;
		ID3D11DeviceContext* GetContext() override;

		HWND myWindowsInstance;
		ID3D11Device* myDevice;
		ID3D11DeviceContext* myDeviceContext;
		GUISpace* myDockingSpace;
		std::vector<std::shared_ptr<GUISpace>> myElements;

		IDXGISwapChain* mySwapChain;


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


