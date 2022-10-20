#pragma once
#include "Core/CU/Transform.h"
#include "Core/CU/Math/Vector2.h"

#include <vector>

namespace Dragonite
{
	struct CameraProfile
	{
		virtual unsigned int GetID() = 0;
		virtual Matrix4x4f CalculateProjectionMatrix() = 0;
	};


	struct PerspectiveProfile : public CameraProfile
	{
		PerspectiveProfile(const PerspectiveProfile& aCpy);
		void operator=(const PerspectiveProfile& aCpy);

		PerspectiveProfile(const float myFOV, const float myNearPlane, const float myFarPlane);
		float myNearPlane, myFarPlane;
		float myFOV;

		Matrix4x4f CalculateProjectionMatrix() override;
		inline unsigned int GetID() override { return 0; }
	};


	struct OrthographicProfile : public CameraProfile
	{
		OrthographicProfile(const Vector2f& aViewPort, const float aNearPlane, const float aFarPlane);
		Vector2f myCurrentViewPort;
		float myNearPlane, myFarPlane;

		Matrix4x4f CalculateProjectionMatrix() override;
		inline unsigned int GetID() override { return 1; }
	};



	class CameraInterface
	{
	public:
		CameraInterface();
		CameraInterface(const CameraInterface&);
		~CameraInterface();
		inline Matrix4x4f ViewMatrix() { return Matrix4x4f::GetFastInverse(myTransform.GetMatrix()); }
		inline Transform& GetTransform() { return myTransform; }
		inline void AddLayer(CameraProfile* const aProfile)
		{
			myMainProfile = static_cast<unsigned int>(myProfileLayers.size());
			myProfileLayers.push_back(aProfile);
		}

		inline std::vector< CameraProfile*>& Profiles() { return myProfileLayers; }

		inline CameraProfile*& Profile() { return myProfileLayers[myMainProfile]; }
		inline Matrix4x4f WorldToClipSpace() { return ViewMatrix() * Profile()->CalculateProjectionMatrix(); }
	private:
		Transform myTransform;
		unsigned int myMainProfile;
		std::vector< CameraProfile*> myProfileLayers;
	};
}


