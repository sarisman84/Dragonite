#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"


namespace Dragonite
{
	template<class T> Vector2<T> ToVector2(const Vector3<T>& anLVal)
	{
		return Vector2<T>(anLVal.x, anLVal.y);
	}

	template<class T> Vector3<T> ToVector3(const Vector2<T>& anLVal)
	{
		return Vector3<T>(anLVal.x, anLVal.y, T());
	}


	template<class T> Vector2<T> operator-(const Vector2<T>& anLVal, const Vector3<T>& anRVal)
	{
		auto val = Dragonite::ToVector2<T>(anRVal);

		return anLVal - val;
	}


	template<class T> Vector2<T> operator-(const Vector3<T>& anLVal, const Vector2<T>& anRVal)
	{
		auto val = Dragonite::ToVector2<T>(anLVal);

		return val - anRVal;
	}
}