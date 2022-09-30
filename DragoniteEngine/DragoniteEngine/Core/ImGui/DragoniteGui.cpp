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

void Dragonite::DragoniteGui::Init(Runtime* anAppIns, GraphicsPipeline* aGraphicsPipeline)
{
	myApplicationIns = anAppIns;
	myGraphicsPipeline = aGraphicsPipeline;

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_Init(myApplicationIns->GetClientInstance());
	ImGui_ImplDX11_Init(myGraphicsPipeline->GetDevice().Get(), myGraphicsPipeline->GetContext().Get());
	ImGui::StyleColorsDark();

	myApplicationIns->OnRender() += [this]()
	{
		EndFrame();
	};
	myApplicationIns->OnPreRender() += [this]()
	{
		BeginFrame();
	};


}

void Dragonite::DragoniteGui::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Dragonite::DragoniteGui::EndFrame()
{
	ImGui::Begin("Demo window");
	ImGui::Button("Hello!");
	ImGui::End();

	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
