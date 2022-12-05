#pragma once
#include "Core/CU/Math/Matrix4x4.hpp"
#include "Core/CU/Transform.h"
#include "Core/CU/Math/Vector2.h"
namespace Dragonite
{
	class Projection {
	public:
		inline Matrix4x4f CalculateProjection(Transform aTransform)
		{
			return CalculateProjection(aTransform.GetMatrix());
		}
		Matrix4x4f CalculateProjection(const Matrix4x4f& aTransform);
	protected:
		Matrix4x4f myProjectionMatrix;
	};



	class Perspective : public Projection
	{
	public:
		Perspective(const float anFOV, const Vector2ui& aResolution, const float aNearPlane = 0.1f, const float aFarPlane = 10000.0f);

	};
}