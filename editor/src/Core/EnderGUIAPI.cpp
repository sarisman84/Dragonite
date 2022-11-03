#include "EnderGUIAPI.h"

#include "imgui/imgui.h"
#include "imguizmo/ImGuizmo.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "imgui/backends/imgui_impl_win32.h"

#include "DirectX/DXUtilities.h"

#include <dxgi.h>
#include <d3d11.h>

Dragonite::EmberGUIAPI::EmberGUIAPI() : EmberGUI()
{
	myDockingSpace = nullptr;
}

Dragonite::EmberGUIAPI::~EmberGUIAPI()
{
	if (myDockingSpace)
		delete myDockingSpace;
	myDockingSpace = nullptr;

	myDevice = nullptr;
	myDeviceContext = nullptr;
}

const bool Dragonite::EmberGUIAPI::Init(HWND anInstance, ID3D11Device* aDevice, ID3D11DeviceContext* aContext, IDXGISwapChain* aSwapChain)
{
	mySwapChain = aSwapChain;
	myDevice = aDevice;
	myDeviceContext = aContext;
	myWindowsInstance = anInstance;
	if (!anInstance || !aDevice || !aContext || !aSwapChain) return false;


	{//IMGUI INIT
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
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


		myDockingSpace = new EmberGUISpace(myDevice, myDeviceContext,[]
		(GUISpace* aCurSpace, std::vector<std::shared_ptr<GUISpace>>& someElements, ID3D11RenderTargetView* aBufferToRenderTo)
			{
				{//Render

					ImGuiIO& io = ImGui::GetIO();
					ImGui_ImplDX11_NewFrame();
					ImGui_ImplWin32_NewFrame();
					ImGui::NewFrame();
					ImGuizmo::BeginFrame();


					{//BeginDockingSpace
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

					ImGui::ShowDemoWindow();




					for (size_t i = 0; i < someElements.size(); i++)
					{
						ImGui::Begin(someElements[i]->myName);
						someElements[i]->myFocusedFlag = ImGui::IsWindowFocused();
						someElements[i]->myHoveredFlag = ImGui::IsWindowHovered();
						someElements[i]->Invoke(someElements, aBufferToRenderTo);
						ImGui::End();
					}

					{//EndDockingSpace
						ImGui::End();
					}



					float* color = new float[4] { 0, 0, 0, 0 };
					ImGui::Render();
					aCurSpace->Context()->OMSetRenderTargets(1, &aBufferToRenderTo, nullptr);
					aCurSpace->Context()->ClearRenderTargetView(aBufferToRenderTo, color);
					//myDeviceContext->ClearDepthStencilView(myDepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
					delete[] color;

					ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
					if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
					{
						ImGui::UpdatePlatformWindows();
						ImGui::RenderPlatformWindowsDefault();
					}
				}

			});
	}




	return true;
}

void Dragonite::EmberGUIAPI::Update(const float aDt, ID3D11RenderTargetView* aTargetView)
{
	myDockingSpace->Invoke(myElements, aTargetView);
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


ID3D11Device* Dragonite::EmberGUIAPI::GetDevice()
{
	return myDevice;
}

ID3D11DeviceContext* Dragonite::EmberGUIAPI::GetContext()
{
	return myDeviceContext;
}

void Dragonite::EmberGUIAPI::AddSpace(GUISpace* aNewSpace)
{
	myElements.push_back(std::shared_ptr<GUISpace>(aNewSpace));
}
