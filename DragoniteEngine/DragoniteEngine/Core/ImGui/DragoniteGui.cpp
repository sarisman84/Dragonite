#include "DragoniteGui.h"

#include "Core/Runtime.h"
#include "Core/Graphics/GraphicsAPI.h"

#include "Core/External/imgui/imgui.h"
#include "Core/External/imgui/backends/imgui_impl_dx11.h"
#include "Core/External/imgui/backends/imgui_impl_win32.h"

#include <d3d11.h>

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

	InitializeImgui();

}

void Dragonite::DragoniteGui::Render()
{
	BeginDockingSpace();
	for (size_t i = 0; i < myGuiWindows.size(); i++)
	{
		auto& window = myGuiWindows[i];
		myCachedGuiWindowsStates[i] = window->IsActive();
		if (!window->IsActive()) continue;
		ImGui::Begin(window->Name(), &window->IsActive());
		window->OnWindowRender();
		ImGui::End();


		if (myCachedGuiWindowsStates[i] != window->IsActive())
		{
			window->UpdateWindowState();
		}
	}

	EndDockingSpace();

}

void Dragonite::DragoniteGui::AddWindow(GUIWindow* aNewWindow)
{
	myCachedGuiWindowsStates.push_back(false);
	myGuiWindows.push_back(std::unique_ptr<GUIWindow>(aNewWindow));
	auto& ins = myGuiWindows.back();
	ins->Init(&myApplicationIns->GetPollingStation());
	ins->SetActive(true);
}

void Dragonite::DragoniteGui::InitializeImgui()
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports | ImGuiBackendFlags_RendererHasViewports;

	ImGui_ImplWin32_Init(myApplicationIns->GetClientInstance());
	ImGui_ImplDX11_Init(myGraphicsPipeline->GetDevice().Get(), myGraphicsPipeline->GetContext().Get());
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}


	myApplicationIns->OnRender() += [this](GraphicsPipeline* aPipeline)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		Render();
		ImGui::Render();

		aPipeline->myContext->OMSetRenderTargets(1, aPipeline->myBackBuffer.GetAddressOf(), aPipeline->myDepthBuffer.Get());
		aPipeline->myContext->ClearRenderTargetView(aPipeline->myBackBuffer.Get(), &aPipeline->myClearColor);
		aPipeline->myContext->ClearDepthStencilView(aPipeline->myDepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	};


	myApplicationIns->OnWndProc() += [this](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
	};
}

void Dragonite::DragoniteGui::BeginDockingSpace()
{
	static bool p_open = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}


}

void Dragonite::DragoniteGui::EndDockingSpace()
{
	ImGui::End();
}


