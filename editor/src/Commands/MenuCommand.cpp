//#include "MenuCommand.h"
//#include "Core/External/imgui/imgui.h"
//#include <filesystem>
//#include <string>
//
//
//Dragonite::ModalWindow::ModalWindow(const char* aWindowName, const std::function<void(void*)>& aCallback) : Command(), myCallback(aCallback), myName(aWindowName)
//{
//}
//
//
//Dragonite::ModalWindow::ModalWindow() : Command() {}
//
//void Dragonite::ModalWindow::Execute(void* someData)
//{
//	if (myActiveState)
//		ImGui::OpenPopup(myName);
//
//
//	if (ImGui::BeginPopupModal(myName, &myActiveState, ImGuiWindowFlags_AlwaysAutoResize))
//	{
//		if (myCallback)
//			myCallback(someData);
//
//		if (ImGui::Button("Close"))
//		{
//			myActiveState = false;
//			ImGui::CloseCurrentPopup();
//		}
//
//		ImGui::EndPopup();
//	}
//}
//
//
//void Dragonite::ModalWindow::Undo()
//{
//}
//
//Dragonite::FolderCommand::FolderCommand(const Type aCommandType) : Command(), myType(aCommandType)
//{
//	myMarkedForExecutionFlag = false;
//}
//
//void Dragonite::FolderCommand::Execute(void* someData)
//{
//	if (!someData) return;
//
//
//
//	myTargetPath = *(std::string*)someData;
//
//
//	myMarkedForExecutionFlag = true;
//
//
//}
//
//void Dragonite::FolderCommand::Undo()
//{
//}
//
//void Dragonite::FolderCommand::ApplyCommand()
//{
//	if (!myMarkedForExecutionFlag) return;
//
//	using namespace std::filesystem;
//	switch (myType)
//	{
//	case Dragonite::FolderCommand::Type::Create:
//	{
//		create_directory(myTargetPath);
//		break;
//	}
//
//	case Dragonite::FolderCommand::Type::Remove:
//		remove_all(myTargetPath);
//		break;
//	default:
//		break;
//	}
//
//
//	myMarkedForExecutionFlag = false;
//}
//
//Dragonite::PopupMenu::PopupMenu() : Command()
//{
//	myLastImGuiID = nullptr;
//}
//
//void Dragonite::PopupMenu::ParentTo(const char* aNewImguiID)
//{
//	myLastImGuiID = aNewImguiID;
//}
//
//void Dragonite::PopupMenu::Execute(void* someData)
//{
//	static bool isSelected = false;
//	if (ImGui::BeginPopupContextItem(myLastImGuiID ? myLastImGuiID : 0))
//	{
//		auto cpy = myMenuElements;
//		for (auto& button : cpy)
//		{
//			std::string id = std::get<0>(button);
//			id += "##";
//			id += std::to_string(myGuiID);
//			ImGui::Selectable(id.c_str(), &isSelected);
//			if (isSelected)
//			{
//				auto callback = std::get<1>(button);
//				if (callback)
//					callback(someData);
//			}
//
//			isSelected = false;
//		}
//		ImGui::EndPopup();
//	}
//
//}
//
//void Dragonite::PopupMenu::Undo()
//{
//}
//
//Dragonite::DragCommand::DragCommand() : Command()
//{
//}
//
//void Dragonite::DragCommand::Execute(void* someData)
//{
//	if (!myData)
//	{
//		myData = someData;
//	}
//
//}
//
//void Dragonite::DragCommand::Undo()
//{
//}
