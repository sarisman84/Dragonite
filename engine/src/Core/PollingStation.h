#pragma once
#include <memory>
#include <unordered_map>

namespace Dragonite
{
	class Engine;
	class GraphicalInterface;
	class PollingStation
	{
		friend struct Viewport;
		friend class Scene;
		friend class DragoniteGui;
		friend GraphicalInterface;
		friend Engine;
	public:
		PollingStation() = default;
		~PollingStation();

		PollingStation(const Engine& anOtherIns) = delete;
		void operator=(const Engine& anOtherIns) = delete;


		template<typename Type>
		inline Type* Get()
		{
			auto id = typeid(Type).hash_code();
			if (myUniqueDataMembers.count(id) > 0)
			{
				auto data = myUniqueDataMembers[id];
				if (data.myIsPointer)
					return std::static_pointer_cast<Type>(data.myPtrData).get();
				else
					return (Type*)data.myRefData;
			}
			return nullptr;
		}

	private:
		struct ManagerInfo
		{
			~ManagerInfo();
			bool myIsPointer;
			void* myRefData;
			std::shared_ptr<void> myPtrData;
		};

		template<typename Type>
		Type* AddHandler(Type* anInstance)
		{
			ManagerInfo info;
			info.myIsPointer = false;
			info.myRefData = anInstance;

			myUniqueDataMembers[typeid(Type).hash_code()] = info;
			return anInstance;
		}

		template<typename Type>
		Type* AddHandler(const Type& anInstance)
		{
			ManagerInfo info;
			info.myIsPointer = true;
			info.myPtrData = std::make_shared<Type>(anInstance);

			myUniqueDataMembers[typeid(Type).hash_code()] = info;
			return &anInstance;
		}

		std::unordered_map<size_t, ManagerInfo> myUniqueDataMembers;
	};
}