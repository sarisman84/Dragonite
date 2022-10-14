#include "GUIWindow.h"
#include "Core/ImGui/DragoniteGui.h"
#include "Core/Runtime.h"
#include "Core/PollingStation.h"

Dragonite::GUIWindow::GUIWindow(const char* aWindowName) : myWindowName(aWindowName), myActiveStateFlag(true)
{
	static unsigned int guid = 0;
	myGUID = guid++;
}

const bool Dragonite::GUIWindow::IsBeingHovered() const noexcept
{
	return myHoveredFlag;
}

const bool Dragonite::GUIWindow::IsBeingFocused() const noexcept
{
	return myFocusFlag;
}

void Dragonite::GUIWindow::UpdateWindow()
{
	

	for (auto& window : mySubWindowElements)
	{
		window->UpdateWindow();
	}


	if (myActiveStateFlag)
	{
		auto name = std::string(myWindowName) + "##";
		name += std::to_string(myGUID);
		if (myIsChildElementFlag)
		{
			ImGui::PushID(name.c_str());
			ImGui::BeginChild(ImGui::GetID(name.c_str()));

			OnWindowUpdate();
			ImGui::EndChild();
			ImGui::PopID();

		}
		else
		{
			

			ImGui::Begin(name.c_str(), &myActiveStateFlag);
			myFocusFlag = ImGui::IsWindowFocused();
			myHoveredFlag = ImGui::IsWindowHovered();
			OnWindowUpdate();
			ImGui::End();
		}

	}


	

}

Dragonite::GUIWindow* Dragonite::GUIWindow::CreateEditorWindow(GUIWindow* aWindowType, const bool anIsChildElementFlag)
{
	mySubWindowElements.push_back(std::unique_ptr<GUIWindow>(aWindowType));
	auto& ins = mySubWindowElements.back();
	ins->Init(&myDragoniteGuiAPI->myApplicationIns->GetPollingStation(), myDragoniteGuiAPI);
	ins->SetActive(true);
	ins->myIsChildElementFlag = anIsChildElementFlag;
	return ins.get();
}


void Dragonite::GUIWindow::UpdateWindowState()
{
	if (myActiveStateFlag)
		OnEnable();
	else
		OnDisable();
}


