#pragma once
#include "Core/RuntimeAPI/Component.h"
#include "Core/Graphics/CameraInterface.h"
#include "Core/Utilities/Reflection.h"

namespace Dragonite
{
	class Scene;
	class Camera : public Component
	{
	public:
		Camera();
		// Inherited via Component
		void Awake() override;
		void Update(const float aDt) override;
		void Start() override;
		void LateUpdate(const float aDt) override;
		void* Serialize() override;
		void Deserialize(void* someData) override;
		void OnInspectorGUI() override;

		inline const unsigned int GetID() { return myCameraID; }
		inline CameraInterface& GetInterface() { return myInterface; }


		Vector2f myViewportSize;

		bool myPerspectiveFlag;
		float myFOV;
		float myNearPlane, myFarPlane;

	private:
		CameraInterface* myPreviousCameraInterface = nullptr;
		Scene* myCurrentScene = nullptr;
		PerspectiveProfile myPerspectiveProfile;
		OrthographicProfile myOrthographicProfile;

		CameraInterface myInterface;
		unsigned int myCameraID;




		

	};


	template<>
	inline auto Reflect::RegisterElement<Camera>()
	{
		return Class("Camera",
			Member("FOV", &Camera::myFOV),
			Member("Near Plane", &Camera::myNearPlane),
			Member("Far Plane", &Camera::myFarPlane));
	}
}


