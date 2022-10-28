#pragma once
#include <wtypes.h>
#include <wrl/client.h>
#include "GUIWindow.h"

#include <vector>
#include <memory>

#include "imgui.h"
#include "ImGuizmo.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"

#define ITF __declspec(dllimport)

using Microsoft::WRL::ComPtr;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;


/// <summary>
/// Spyro's Editor Interface
///<para>
/// In order to get EmberGUI to work, you need the following calls:
///</para>
///<br> 
///</br> 
///<para> 
///> Init at the start of your application
///</para>
///<para> 
///> A WinProc to register input
///</para>
///<para> 
///> An update loop to update and render the Editor elements. 
///</para>
///<para> 
///> Shutdown to deserialize IMGUI. 
///</para>
/// </summary>
class EmberGUI
{
public:
	/// <summary>
	/// Initializes EmberGUI
	/// </summary>
	/// <param name="anInstance">the current windows instance</param>
	/// <returns>true if it succeded, otherwise it will return false</returns>
	ITF static const bool Init(HWND anInstance, ID3D11Device* aDevice, ID3D11DeviceContext* aContext);
	ITF static void Update(const float aDt);
	ITF static void Render();
	ITF static void Shutdown();

	ITF static void AddWindow(Ember::AbsWindows* aNewWindow);
	/// <summary>
	/// EmberGUI's Win Proc. Used for IMGUI/Dragonite Input.
	/// </summary>
	/// <param name="hWnd"></param>
	/// <param name="message"></param>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	/// <returns>its current state (LRESULT)</returns>
	ITF static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	static EmberGUI ourInstance;

private: //Local Methods
	const bool Init_Impl(HWND anInstance, ID3D11Device* aDevice, ID3D11DeviceContext* aContext);
	void Update_Impl(const float aDt);
	void Render_Impl();

	void BeginDockingSpace();
	void EndDockingSpace();

private://Local Variables
	ID3D11DeviceContext* myContext;
	ID3D11Device* myDevice;
	ComPtr<ID3D11RenderTargetView> myBackBuffer;
	ComPtr<ID3D11DepthStencilView> myDepthBuffer;
	std::vector<std::shared_ptr <Ember::AbsWindows>> myElements;

	unsigned int myCurrentID;

};


inline void EmberGUI::AddWindow(Ember::AbsWindows* aNewWindow)
{
	ourInstance.myElements.push_back(std::shared_ptr<Ember::AbsWindows>(aNewWindow));
}
