#pragma once
#include "Command.h"
#include <functional>
#include <string>
namespace Dragonite
{

	class ModalWindow : public Command
	{
	public:
		ModalWindow();
		ModalWindow(const char* aWindowName, const std::function<void(void*)>& aCallback);
		~ModalWindow() = default;

		inline void Open() noexcept { myActiveState = true; };
		inline void Close() noexcept { myActiveState = false; };

		void Execute(void* someData) override;
		void Undo() override;
	private:


		const char* myName;
		std::function<void(void*)> myCallback;
		bool myActiveState;
	};



	class FolderCommand : public Command
	{
		
	public:
		enum class Type
		{
			Create, Remove
		};
		FolderCommand(const Type aCommandType);
		~FolderCommand() = default;
		void Execute(void* someData) override;
		void Undo() override;

		void ApplyCommand();
	private:
		bool myMarkedForExecutionFlag;
		std::string myTargetPath;
		Type myType;
	};



	class PopupMenu : public Command
	{

	public:
		
		PopupMenu();
		~PopupMenu() = default;
		template<typename Func>
		void AddMenuElement(const char* aName, const Func& aCallback);

		void ParentTo(const char* aNewImguiID);
		void Execute(void* someData) override;
		void Undo() override;
	private:
		const char* myLastImGuiID;
		std::vector<std::tuple<const char*,std::function<void(void*)>>> myMenuElements;
	};

	template<typename Func>
	inline void PopupMenu::AddMenuElement(const char* aName, const Func& aCallback)
	{
		auto val = std::make_tuple(aName, aCallback);
		myMenuElements.push_back(val);
	}

}


