#pragma once
#include "Core/CU/Transform.h"
#include "Core/CU/Math/Vector2.h"
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

		Matrix4x4f CalculateProjectionMatrix() override;
	};


	struct OrthographicProfile : public CameraProfile
	{
		OrthographicProfile(Vector2f& aViewPort, const float aNearPlane, const float aFarPlane);
		Vector2f& myCurrentViewPort;
		float myNearPlane, myFarPlane;

		Matrix4x4f CalculateProjectionMatrix() override;
	};



	class CameraInterface
	{
	public:
		CameraInterface();
		~CameraInterface();
		inline Matrix4x4f ViewMatrix() { return Matrix4x4f::GetFastInverse(myTransform.GetMatrix()); }
		inline Transform& GetTransform() { return myTransform; }
		inline CameraProfile*& Profile() { return myProfile; }
		inline Matrix4x4f WorldToClipSpace() { return ViewMatrix() * myProfile->CalculateProjectionMatrix(); }
	private:
		Transform myTransform;
		CameraProfile* myProfile;
	};
}


