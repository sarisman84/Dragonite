#pragma once
#include <wtypes.h>
#include <type_traits>
#include <libloaderapi.h>
#include <filesystem>


namespace Dragonite
{

	class CallbackPtr
	{
	public:
		explicit CallbackPtr(FARPROC ptr) : myPtr(ptr) {}

		template <typename T, typename = std::enable_if_t<std::is_function_v<T>>>
		operator T* () const
		{
			return reinterpret_cast<T*>(myPtr);
		}

	private:
		FARPROC myPtr;
	};


	class DLLParser
	{
	public:
		explicit DLLParser(LPCTSTR  aFilename) : myModule(LoadLibrary(aFilename)), myFileName(aFilename) {}

		~DLLParser() { if (IsValid()) FreeLibrary(myModule); }

		CallbackPtr operator[](LPCSTR aProcName) const
		{
			return CallbackPtr(GetProcAddress(myModule, aProcName));
		}

		operator bool() {
			return IsValid();
		}



	private:
		const bool IsValid()
		{
			return std::filesystem::directory_entry(myFileName).exists() || myModule != nullptr;
		}

		LPCTSTR myFileName;
		HMODULE myModule;
	};
}


