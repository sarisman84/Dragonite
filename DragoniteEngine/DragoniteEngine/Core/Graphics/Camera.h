#pragma once
#include "../CU/Transform.h"
namespace Dragonite
{
	struct CameraProfile
	{
		virtual Matrix4x4f CalculateViewMatrix() = 0;
	};


	struct PerspectiveProfile : public CameraProfile
	{
		PerspectiveProfile(const float myFOV, const float myNearPlane, const float myFarPlane);
		float myNearPlane, myFarPlane;
		float myFOV;

		virtual Matrix4x4f CalculateViewMatrix() override;
	};



	class Camera
	{
	public:
		Camera();
		~Camera();
		inline Transform& GetTransform() { return myTransform; }
		inline CameraProfile*& GetProfile() { return myProfile; }
	private:
		Transform myTransform;
		CameraProfile* myProfile;
	};
}


