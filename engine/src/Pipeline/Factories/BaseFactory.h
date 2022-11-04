#pragma once
namespace Dragonite
{
	class BaseFactory
	{
	public:
		virtual void Init() = 0;
		void SetWorkingDirectory(const char* aDir) { myWorkingDirectory = aDir; }
	protected:
		const char* myWorkingDirectory;
	};
}