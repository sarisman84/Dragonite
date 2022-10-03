#include "Transform.h"

Dragonite::Transform::Transform() : myPosition({ 0,0,0 }), myScale({ 1,1,1 }), myRotation({ 0,0,0 }), myParent(nullptr)
{
}

Dragonite::Transform::Transform(const Transform& someCopy)
{
	if (someCopy.myChildren.size() > 0)
		myChildren = someCopy.myChildren;
	myParent = someCopy.myParent;
	myPosition = someCopy.myPosition;
	myRotation = someCopy.myRotation;
	myScale = someCopy.myScale;
}

void Dragonite::Transform::operator=(const Transform& someCopy)
{
	if (someCopy.myChildren.size() > 0)
		myChildren = someCopy.myChildren;
	myParent = someCopy.myParent;
	myPosition = someCopy.myPosition;
	myRotation = someCopy.myRotation;
	myScale = someCopy.myScale;
}

Dragonite::Matrix4x4f Dragonite::Transform::GetMatrix()
{
	if (!myParent)
		return GetLocalMatrix();

	return GetLocalMatrix() * myParent->GetMatrix();
}

Dragonite::Matrix4x4f Dragonite::Transform::GetLocalMatrix()
{
	using Dragonite::Matrix4x4f;
	Matrix4x4f r;
	Matrix4x4f translationM = Matrix4x4f::CreateTranslationMatrix(myPosition);

	Matrix4x4f rotationM;
	rotationM *= Matrix4x4f::CreateRotationAroundX(myRotation.x * Math::DegToRad);
	rotationM *= Matrix4x4f::CreateRotationAroundY(myRotation.y * Math::DegToRad);
	rotationM *= Matrix4x4f::CreateRotationAroundZ(myRotation.z * Math::DegToRad);

	Matrix4x4f scaleM = Matrix4x4f::CreateScaleMatrix(myScale);
	r = scaleM * rotationM * translationM;
	return r;
}

void Dragonite::Transform::SetParent(Transform* aTransform)
{
	using Dragonite::Matrix4x4f;
	if (!aTransform || aTransform == this) return;

	if (myParent)
	{
		myParent->myChildren.erase(std::remove(myParent->myChildren.begin(), myParent->myChildren.end(), this), myParent->myChildren.end());

	}

	auto r = GetMatrix() * Matrix4x4f::GetInverse(aTransform->GetMatrix());
	aTransform->myChildren.push_back(this);
	SetMatrix(r);
	myParent = aTransform;

}

void Dragonite::Transform::SetMatrix(const Matrix4x4f& aMatrix)
{
	using Dragonite::Vector3f;

	Vector3f pos, rot, scale;

	aMatrix.DecomposeMatrix(pos, rot, scale);


	myPosition = pos;
	myRotation = rot;
	myScale = scale;
}

void Dragonite::Transform::ApplyMatrix(const Matrix4x4f& aMatrix)
{
	using Dragonite::Matrix4x4f;

	if (!myParent)
	{
		SetMatrix(aMatrix);
		return;
	}

	Matrix4x4f parentToWorld = myParent->GetMatrix();
	Matrix4x4f childToParent = aMatrix * Matrix4x4f::GetInverse(parentToWorld);

	SetMatrix(childToParent);

}
