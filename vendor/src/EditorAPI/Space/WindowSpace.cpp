#include "WindowSpace.h"
#include "EditorAPI/EmberGUI.h"
void ember::EmberWindow::Render()
{

	PrepareIMGUI();
	OnEarlyGUI();
	OnGUI();
	OnLateGUI();
}
void ember::EmberWindow::PrepareIMGUI()
{
	ImGui::SetCurrentContext(myInterface->GetIMGUIContext());
}
void ember::EmberWindow::RenderChildContexts()
{
	for (size_t i = 0; i < myChildContexts.size(); i++)
	{
		auto& context = myChildContexts[i];
		context->PrepareIMGUI();
		ImGui::Begin(context->myName);
		context->Render();
		ImGui::End();
		context->RenderChildContexts();
	}



}

ember::EmberWindow::~EmberWindow() = default;


ID3D11Device*& ember::EmberWindow::DXDevice()
{
	return myInterface->GetDevice();
}

ID3D11DeviceContext*& ember::EmberWindow::DXContext()
{
	return myInterface->GetContext();
}

ID3D11RenderTargetView*& ember::EmberWindow::BackBuffer()
{
	return myInterface->GetBackBuffer();
}
