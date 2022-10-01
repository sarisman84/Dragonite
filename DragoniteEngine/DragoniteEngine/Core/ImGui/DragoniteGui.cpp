#include "DragoniteGui.h"

#include "Core/Runtime.h"
#include "Core/Graphics/GraphicsAPI.h"

#include "Core/External/imgui/imgui.h"
#include "Core/External/imgui/imgui_impl_dx11.h"
#include "Core/External/imgui/imgui_impl_win32.h"

Dragonite::DragoniteGui::DragoniteGui() = default;

Dragonite::DragoniteGui::~DragoniteGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
};


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void Dragonite::DragoniteGui::Init(Runtime* anAppIns, GraphicsPipeline* aGraphicsPipeline)
{
	myApplicationIns = anAppIns;
	myGraphicsPipeline = aGraphicsPipeline;

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags = ImGuiConfigFlags_ViewportsEnable;
	io.BackendFlags = ImGuiBackendFlags_PlatformHasViewports | ImGuiBackendFlags_RendererHasViewports;

	ImGui_ImplWin32_Init(myApplicationIns->GetClientInstance());
	ImGui_ImplDX11_Init(myGraphicsPipeline->GetDevice().Get(), myGraphicsPipeline->GetContext().Get());
	ImGui::StyleColorsDark();

	myApplicationIns->OnRender() += [this]()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		Render();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		ImGui::UpdatePlatformWindows();
	};


	myApplicationIns->OnWndProc() += [this](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
	};

}

void Dragonite::DragoniteGui::Render()
{
	ImGui::Begin("Demo window");
	ImGui::Button("Hello!");
	ImGui::End();
	ImGui::ShowDemoWindow();
}


