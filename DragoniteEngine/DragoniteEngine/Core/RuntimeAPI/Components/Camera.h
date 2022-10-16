#pragma once
#include "Core/RuntimeAPI/Component.h"
#include "Core/Graphics/CameraInterface.h"

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
		void OnInspectorGUI() override;
		void OnDisable() override;
		void ConstantUpdate() override;
		void OnCreate() override;
		std::string GetName() override;

		inline const unsigned int GetID() { return myCameraID; }
		inline CameraInterface& GetInterface() { return myInterface; }

		void SetCameraAsPrimary();


		float myFOV;
		float myNearPlane, myFarPlane;

	private:
		CameraInterface* myPreviousCameraInterface = nullptr;
		Scene* myCurrentScene = nullptr;
		PerspectiveProfile* myPerspectiveProfile = nullptr;
		CameraInterface myInterface;
		unsigned int myCameraID;











	

	};
}


