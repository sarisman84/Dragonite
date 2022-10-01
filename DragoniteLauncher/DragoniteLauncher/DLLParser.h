#pragma once
#include <wtypes.h>
#include <type_traits>
#include <libloaderapi.h>


namespace Dragonite {

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
		explicit DLLParser(LPCTSTR  aFilename) : myModule(LoadLibrary(aFilename)) {}

		~DLLParser() { FreeLibrary(myModule); }

		CallbackPtr operator[](LPCSTR aProcName) const
		{
			return CallbackPtr(GetProcAddress(myModule, aProcName));
		}

		operator bool() {
			return myModule != nullptr;
		}



	private:

		HMODULE myModule;
	};
}


