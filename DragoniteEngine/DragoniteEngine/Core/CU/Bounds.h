#pragma once

#include "Math/Vector.h"
#include "Transform.h"
namespace Dragonite
{


	struct Bounds2D
	{
		Bounds2D(Transform* aTransform)
		{


			myTransform = aTransform;
		}




		const bool IsIntersectingAgainst(const Bounds2D& someOtherBounds)
		{


			auto otherMin = ToVector2(someOtherBounds.myTransform->myPosition + (someOtherBounds.myTransform->myScale / 2.0f) * -1.0f);
			auto otherMax = ToVector2(someOtherBounds.myTransform->myPosition + someOtherBounds.myTransform->myScale / 2.0f);

			auto min = ToVector2(myTransform->myPosition + (myTransform->myScale / 2.0f) * -1.0f);
			auto max = ToVector2(myTransform->myPosition + myTransform->myScale / 2.0f);



			return
				min.x <= otherMax.x && max.x >= otherMin.x &&
				min.y <= otherMax.y && max.y >= otherMin.y;
		}

		Transform* myTransform;


	};

}