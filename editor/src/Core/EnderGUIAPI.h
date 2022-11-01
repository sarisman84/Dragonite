#pragma once
#include "EditorAPI/EmberGUI.h"
#include "EditorAPI/Space/WindowSpace.h"

#include <memory>
#include <vector>


struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;






namespace Dragonite
{
	class EmberGUIAPI : public EmberGUI
	{
	public:
		EmberGUIAPI();
		virtual ~EmberGUIAPI() override;
		const bool Init(HWND anInstance, ID3D11Device* aDevice, ID3D11DeviceContext* aContext) override;
		void Update(const float aDt) override;
		void Shutdown() override;
		LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

		void AddSpace(GUISpace* aNewSpace) override;


		inline ID3D11ShaderResourceView* GetData() { return myRenderData; }

	private:
		const bool InitializeBackBuffer();

		HWND myWindowsInstance;
		ID3D11Device* myDevice;
		ID3D11DeviceContext* myDeviceContext;
		GUISpace* myDockingSpace;
		std::vector<std::shared_ptr<GUISpace>> myElements;
		ID3D11RenderTargetView* 
			myLocalBackbuffer;
		ID3D11ShaderResourceView* 
			myRenderData;


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


