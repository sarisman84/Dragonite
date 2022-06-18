#pragma once
#include "Utilities/Math/Vector.h"
#include "Utilities/Math/Matrix.h"

#include "Utilities/Extensions.h"

using namespace Math;
using namespace CPPExtend;
struct Transform
{
	Transform();
	//Property<Vector3f> Position;
	//Property<Vector3f> Size;
	//Property<Vector4f> Rotation;

	//Property<Vector3f> Up;
	//Property<Vector3f> Right;
	//Property<Vector3f> Forward;

	//Vector3f GetUp();
	//Vector3f GetRight();
	//Vector3f GetForward();





	Matrix4x4f& GetMatrix();


	void SetRotation(Math::Vector3f aRotation, const bool aRotateLocally = false);
	void SetRotationX(const float anXVal, const bool aRotateLocally = false);
	void SetRotationY(const float anYVal, const bool aRotateLocally = false);
	void SetRotationZ(const float anZVal, const bool aRotateLocally = false);
	Math::Vector3f GetRotation();

	void SetPosition(Math::Vector3f aPosition);
	Math::Vector3f GetPosition();

	void SetSize(Math::Vector3f aSize);
	Math::Vector3f GetSize();

	Math::Vector3f GetForward();
	Math::Vector3f GetUp();
	Math::Vector3f GetRight();


private:
	Vector3f myRotation;
	Matrix4x4f myLocalToWorldMatrix;
};
