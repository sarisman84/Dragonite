#pragma once
namespace Dragonite
{
	class Command
	{
	public:
		Command()
		{
			static unsigned int id = 0;
			myGuiID = ++id;
		}
		virtual ~Command() = default;

		virtual void Execute(void* someData) = 0;
		virtual void Undo() = 0;

		inline unsigned int GetID() { return myGuiID; }



		template<typename TCommand>
		inline TCommand& As()
		{
			return *(TCommand*)this;
		}

	protected:
		unsigned int myGuiID;

	};
}