#pragma once
#include "Component.h"
#include "Utilities/Math/Matrix.h"
class Camera : public Component
{
public:
	Math::Matrix4x4f GetClipSpaceMatrix();
	// Inherited via Component

	inline void SetFOV(const float aNewValue)
	{
		myFOV = aNewValue;
		UpdateProjectionMatrix();
	}

	inline void SetNearPlane(const float aNewValue)
	{
		myNearPlane = aNewValue;
		UpdateProjectionMatrix();
	}

	inline void SetFarPlane(const float aNewValue)
	{
		myFarPlane = aNewValue;
		UpdateProjectionMatrix();
	}


	void OnAwake() override;
	void OnUpdate(float aDeltaTime) override;
private:
	void UpdateProjectionMatrix();
	Math::Matrix4x4f myProjectionMatrix;
	float myNearPlane, myFarPlane;
	float myFOV;
	float myMovementSpeed;
	Math::Vector2f myInput;
};

