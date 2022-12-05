#pragma once
#include "Pipeline/Rendering/Content/Projection.h"
#include "Math/Matrix4x4.hpp"
#include "Transform.h"

namespace Dragonite
{
	enum class ProjectionType 
	{
		Perspective, Orthographic
	};

	class Camera
	{
	public:
		Camera();
		~Camera();
		const Matrix4x4f GetWorldToClipSpace();
	public:
		Transform myTransform;
		ProjectionType myProjectionType;
	private:
		Perspective myPerspectiveProj;
		//Orthographic myOrthographicProj;
	};
}


