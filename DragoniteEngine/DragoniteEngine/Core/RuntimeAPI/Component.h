#pragma once
#include <string>
#include <memory>
namespace Dragonite
{
	class Object;
	class Component
	{
		friend Object;
	public:
		Component() = default;
		virtual ~Component() = default;


		virtual void Awake() = 0;
		virtual void Start() = 0;
		virtual void Update(const float aDt) = 0;
		virtual void LateUpdate(const float aDt) = 0;
		virtual void* Serialize() = 0;
		virtual void Deserialize(void* someData) = 0;

		virtual void OnInspectorGUI() = 0;

		virtual std::shared_ptr<Component> Clone() = 0;

		std::string myName;

	protected:
		Object* myObject;
	};
}