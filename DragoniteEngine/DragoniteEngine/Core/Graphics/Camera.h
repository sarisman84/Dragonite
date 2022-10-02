#pragma once
#include "Core/CU/Transform.h"
namespace Dragonite
{
	struct CameraProfile
	{
		virtual Matrix4x4f CalculateProjectionMatrix() = 0;
	};


	struct PerspectiveProfile : public CameraProfile
	{
		PerspectiveProfile(const float myFOV, const float myNearPlane, const float myFarPlane);
		float myNearPlane, myFarPlane;
		float myFOV;

		virtual Matrix4x4f CalculateProjectionMatrix() override;
	};



	class Camera
	{
	public:
		Camera();
		~Camera();
		inline Transform& GetTransform() { return myTransform; }
		inline CameraProfile*& GetProfile() { return myProfile; }
		inline Matrix4x4f WorldToClipSpace() { return Matrix4x4f::GetFastInverse(myTransform.GetMatrix()) * myProfile->CalculateProjectionMatrix(); }
	private:
		Transform myTransform;
		CameraProfile* myProfile;
	};
}


