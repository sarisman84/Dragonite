#include "EnderGUIAPI.h"

#include "imgui/imgui.h"
#include "imguizmo/ImGuizmo.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "imgui/backends/imgui_impl_win32.h"

#include "DirectX/DXUtilities.h"

#include "Core/Editors/Root.h"

#include <dxgi.h>
#include <d3d11.h>

Dragonite::EmberGUIAPI::EmberGUIAPI() : EmberGUI()
{
	myRootContext = nullptr;
}

Dragonite::EmberGUIAPI::~EmberGUIAPI()
{
	if (myRootContext)
		delete myRootContext;
	myRootContext = nullptr;


	myDevice = nullptr;
	myDeviceContext = nullptr;
}

const bool Dragonite::EmberGUIAPI::Init(
	HWND anInstance,
	ID3D11Device* aDevice,
	ID3D11DeviceContext* aContext,
	IDXGISwapChain* aSwapChain,
	ID3D11RenderTargetView* aViewToRenderTo)
{
	mySwapChain = aSwapChain;
	myDevice = aDevice;
	myDeviceContext = aContext;
	myWindowsInstance = anInstance;
	myLocalBackBuffer = aViewToRenderTo;
	if (!anInstance || !aDevice || !aContext || !aSwapChain || !aViewToRenderTo) return false;


	{//IMGUI INIT
		IMGUI_CHECKVERSION();
		myImguiContext = ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports | ImGuiBackendFlags_RendererHasViewports;

		ImGui_ImplWin32_Init(anInstance);
		ImGui_ImplDX11_Init(myDevice, myDeviceContext);
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}


		myRootContext = new DropArea();
		myRootContext->Init(this);
	}




	return true;
}

void Dragonite::EmberGUIAPI::Update()
{
	/*if (myElements.empty()) return;
	myDockingSpace->Invoke(aTargetView);*/

	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	myRootContext->Render();
	myRootContext->RenderChildContexts();

	float* color = new float[4] { 0, 0, 0, 0 };
	ImGui::Render();
	myDeviceContext->OMSetRenderTargets(1, &myLocalBackBuffer, nullptr);
	myDeviceContext->ClearRenderTargetView(myLocalBackBuffer, color);
	delete[] color;

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}


}

void Dragonite::EmberGUIAPI::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT Dragonite::EmberGUIAPI::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
}


ID3D11Device*& Dragonite::EmberGUIAPI::GetDevice()
{
	return myDevice;
}

ID3D11DeviceContext*& Dragonite::EmberGUIAPI::GetContext()
{
	return myDeviceContext;
}

ID3D11RenderTargetView*& Dragonite::EmberGUIAPI::GetBackBuffer()
{
	return myLocalBackBuffer;
}

void Dragonite::EmberGUIAPI::AddEditor(ember::EmberWindow* aNewWindow)
{
	aNewWindow->Init(this);
	myRootContext->AddChildWindow(aNewWindow);
}

EmVec2 Dragonite::EmberGUIAPI::GetViewportResolution()
{
	if (!myDeviceContext) return EmVec2(0, 0);
	UINT amm = 1;
	D3D11_VIEWPORT port = { };
	myDeviceContext->RSGetViewports(&amm, &port);
	return EmVec2(port.Width, port.Height);
}

ImGuiContext* Dragonite::EmberGUIAPI::GetIMGUIContext()
{
	return myImguiContext;
}

