#pragma once
#include "Math/Vector3.h"
#include "Math/Matrix4x4.hpp"

#include <vector>
namespace Dragonite
{
	class Transform
	{
	public:
		Vector3f myPosition;
		Vector3f myRotation;
		Vector3f myScale;

	private:
		Transform* myParent;

		std::vector<Transform*> myChildren;

		Matrix4x4f GetMatrix();
		Matrix4x4f GetLocalMatrix();


		void SetParent(Transform* aTransform);

		/// <summary>
		/// Replaces the current transformation with a new one
		/// </summary>
		/// <param name="aMatrix">An incoming matrix</param>
		void SetMatrix(const Matrix4x4f& aMatrix);

		/// <summary>
		/// Applies an incoming transformation to self
		/// </summary>
		/// <param name="aMatrix">An incoming matrix</param>
		void ApplyMatrix(const Matrix4x4f& aMatrix);



		inline Transform* GetParent() noexcept { return myParent; }
		inline std::vector<Transform*> GetChildren() noexcept { return myChildren; }
	};
}


